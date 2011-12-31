#include <QScrollBar>
#include <QPainter>
#include <QDebug>
#include <QTime>
#include <QPalette>
#include <QColor>
#include <QPair>
#include <QMessageBox>
#include <QKeySequence>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include "MedusaEditor.hpp"
#include "MedusaPrinterPool.hpp"
#include "IMedusaItem.hpp"
#include "IMedusaPrinter.hpp"
#include "Settings.hpp"
#include "Instruction.hpp"
#include "JumpAction.hpp"
#include "FunctionPrinter.hpp"

MedusaEditor::MedusaEditor(QWidget * parent)
	: QAbstractScrollArea(parent),
	_lineCounter(this),
	_items(),
	_addressedItems(),
	_focusedItems(),
	_mutex(QMutex::Recursive),
	_updateScroll(false),
	_whiteScreen(false),
	_prevStartIndex(0),
	_prevEndIndex(0),
	_undoJump(this),
	_gotoScroll(false)
{
	MedusaPrinterPool::setDefaultParent(this->viewport());
	MedusaPrinterPool::setEditor(this);

	this->setViewportMargins(this->_lineCounter.width(), 0, 0, 0);
	this->connect(&Settings::instance(), SIGNAL(settingsChanged()), SLOT(on_SettingsChanged()));
	this->on_SettingsChanged();
	this->setFocusPolicy(Qt::ClickFocus);
}

MedusaEditor::~MedusaEditor()
{

}

void							MedusaEditor::goTo(MedusaEditor::Address const & address, QString description)
{
	IMedusaItem *				item;

	if (this->_addressedItems.contains(address))
	{
		item = this->_addressedItems.value(address);
		this->_undoJump.push(new JumpAction(description, this->verticalScrollBar()->value(), item->itemID(), this));
		this->_gotoScroll = true;
		this->verticalScrollBar()->setValue(item->itemID());
	}
	else
		QMessageBox::warning(this, tr("Go To error"), tr("Invalide destination address"));
}

void							MedusaEditor::goTo(int index)
{
	if (index >= 0 && index < this->verticalScrollBar()->maximum()) {

		this->_gotoScroll = true;
		this->verticalScrollBar()->setValue(index);
	}
}

void							MedusaEditor::addFocusedItem(IMedusaItem * item, bool keepPrevious)
{
	if (!keepPrevious)
	{
		foreach (IMedusaItem * item, this->_focusedItems)
			item->setFocus(false);
		this->_focusedItems.clear();
	}

	this->_focusedItems.insert(item->itemID(), item);
}

void							MedusaEditor::delFocusedItem(IMedusaItem * item)
{
	this->_focusedItems.remove(item->itemID());
}

void							MedusaEditor::setCollapsedItem(int startIndex, int count, bool collapsed)
{
	int							max = startIndex + count;

	this->_lineCounter.update();
	while (startIndex < max)
		this->_items[startIndex++]->setHasCollapsedParent(collapsed);
}

void							MedusaEditor::releasePrinter()
{
	if (this->_prevStartIndex < this->_items.count())
		for (int i = this->_prevStartIndex; i < this->_prevEndIndex && i < this->_items.count(); ++i)
			this->_items[i]->releasePrinter();
}

bool							MedusaEditor::removeLine()
{
	QMutexLocker				locker(&this->_mutex);
	IMedusaItem *				item;
	Instruction *				instruction;

	if (this->_items.isEmpty())
		return (false);

	item = this->_items.takeLast();

	instruction = dynamic_cast<Instruction *>(item);

	if (instruction != 0)
		this->_addressedItems.remove(MedusaEditor::Address(instruction->address().GetBase(), instruction->address().GetOffset()));
	delete item;

	return (true);
}

void							MedusaEditor::clearItems()
{
	QMutexLocker				locker(&this->_mutex);

	this->_items.clear();
	this->_addressedItems.clear();
	this->_undoJump.clear();
	this->verticalScrollBar()->setValue(0);

	InstructionPrinter::MaxInstructionWidth = -1;
	for (int i = 0; i < InstructionPrinter::MaxOperandSize.size(); ++i)
		InstructionPrinter::MaxOperandSize[i] = 0;
}

void							MedusaEditor::setWhiteScreen(bool white)
{
	this->_whiteScreen = white;
}

void							MedusaEditor::addItem(IMedusaItem * item)
{
	QMutexLocker				locker(&this->_mutex);

	this->_items.append(item);
	this->_updateScroll = true;
}

void							MedusaEditor::addItem(QPair<medusa::TBase, medusa::TOffset> const & address, IMedusaItem * item)
{
	QMutexLocker				locker(&this->_mutex);

	this->_items.append(item);
	this->_addressedItems.insert(address, item);
}

int								MedusaEditor::itemCount() const
{
	QMutexLocker				locker(&this->_mutex);

	return (this->_items.count());
}

IMedusaItem * const				MedusaEditor::itemAt(int index) const
{
	QMutexLocker				locker(&this->_mutex);

	return (this->_items.at(index));
}

QUndoStack &					MedusaEditor::undoJumpStack()
{
	return (this->_undoJump);
}

void							MedusaEditor::convertDataToString(int startIndex)
{
	QMutexLocker				locker(&this->_mutex);

	while (this->_items.at(startIndex)->convertDataToString())
		++startIndex;
}

void							MedusaEditor::on_SettingsChanged()
{
	QPalette					palette(this->viewport()->palette());

	palette.setColor(QPalette::Base, QColor(Settings::instance().value(EDITOR_EDITOR_BACKGROUND_PATH, EDITOR_EDITOR_BACKGROUND_DEFAULT).toString()));

	this->viewport()->setPalette(palette);
	this->viewport()->update();
}

void							MedusaEditor::resizeEvent(QResizeEvent * event)
{
	QRect cr = this->contentsRect();
	this->_lineCounter.setGeometry(QRect(cr.left(), cr.top(), this->_lineCounter.width(), cr.height()));

	if (this->_prevStartIndex < this->_items.count())
		for (int i = this->_prevStartIndex; i < this->_prevEndIndex && i < this->_items.count(); ++i)
			this->_items[i]->releasePrinter();
}

void							MedusaEditor::paintEvent(QPaintEvent * event)
{
	QMutexLocker				locker(&this->_mutex);

	if (this->_updateScroll)
		this->updateScroll();

	if (this->_items.count() == 0)
		return;

	int							index = this->verticalScrollBar()->value();
	int							nextIndex;
	int							y = 0;
	int							maxY = this->viewport()->height();
	IMedusaItem *				item;
	IMedusaPrinter *			printer;

	this->_prevStartIndex = index;
	while (index < this->_items.count() && y < maxY)
	{
		item = this->_items[index++];

		printer = item->getPrinter();
		printer->setY(y);

		y += printer->getHeight();
		nextIndex = index + printer->skipNumber();

		while (index < nextIndex) {

			item = this->_items[index];
			item->releasePrinter();
			++index;
		}
	}
	this->_prevEndIndex = index;
}

void							MedusaEditor::scrollContentsBy(int dx, int dy)
{
	QMutexLocker				locker(&this->_mutex);

	this->releasePrinter();

	if (this->_items.count() == 0)
		return;

	int	index = this->verticalScrollBar()->value();
	int	inc = dy < 0 ? 1 : -1;

	if (this->_items[index]->hasCollapsedParent()) {

		if (this->_gotoScroll)
			inc = -1;

		while (index > 0 && index < this->_items.count() && this->_items[index]->hasCollapsedParent())
			index += inc;

		if (!this->_gotoScroll)
			this->verticalScrollBar()->setValue(index);
		else {

			FunctionPrinter * printer = dynamic_cast<FunctionPrinter *>(this->_items[index]->getPrinter());
			printer->on_medusaCollapse_collapsedChanged(false);
			this->_items[index]->releasePrinter();
		}
	}

	this->_gotoScroll = false;

	this->_lineCounter.update();
	this->viewport()->update();
}

void							MedusaEditor::keyPressEvent(QKeyEvent * event)
{
	if (event->matches(QKeySequence::Copy))
	{
		if (!this->_focusedItems.empty())
		{
			QClipboard *		clipboard = QApplication::clipboard();
			QMimeData *			data = new QMimeData();
			QString				text;
			QString				html;

			foreach (IMedusaItem * item, this->_focusedItems)
			{
				text += item->toString() + "\n";
				html += item->toHtml() + "<br />";

			}

			data->setHtml(html);
			data->setText(text);

			clipboard->setMimeData(data);
			return;
		}
	}
	
	QWidget::keyPressEvent(event);
}

void							MedusaEditor::updateScroll()
{
	this->verticalScrollBar()->setRange(0, this->_items.count() - 1);

	if (this->verticalScrollBar()->value() > this->_items.count() - 1)
		this->verticalScrollBar()->setValue(this->_items.count() - 1);

	this->setViewportMargins(this->_lineCounter.width(), 0, 0, 0);

	this->_updateScroll = false;
}