#include <QPainter>
#include <QChar>
#include <QScrollBar>
#include "MedusaEditorLineCounter.hpp"
#include "MedusaEditor.hpp"
#include "IMedusaItem.hpp"
#include "IMedusaPrinter.hpp"
#include "Instruction.hpp"

MedusaEditorLineCounter::MedusaEditorLineCounter(MedusaEditor * parent)
	: QWidget(parent),
	_editor(parent)
{

}

MedusaEditorLineCounter::~MedusaEditorLineCounter()
{

}

QSize			MedusaEditorLineCounter::sizeHint() const
{
	return (QSize(this->width(), 0));
}

int				MedusaEditorLineCounter::width() const
{
	return (180);
	/*
	static int	prevLineCount = -1;
	static int	width = 0;

	if (prevLineCount != this->_editor->itemCount())
	{
		int		count = this->_editor->itemCount();
		int		digits = 1;

		while (count >= 10)
		{
			++digits;
			count /= 10;
		}

		width = 25 + this->fontMetrics().width(QChar('9')) * digits;
	}

	return (width);*/
}

void			MedusaEditorLineCounter::paintEvent(QPaintEvent * event)
{
	QPainter			painter(this);
	int					startLine = this->_editor->verticalScrollBar()->value();
	int					currentPosition = 0;
	QRect				rect = this->_editor->contentsRect();
	int					lineCount = this->_editor->itemCount();
	IMedusaItem *		item;
	IMedusaPrinter *	printer;
	Instruction	*		instruction;

	rect.setWidth(this->width());
	painter.fillRect(rect, Qt::lightGray);
	rect.setWidth(20);
	painter.fillRect(rect, Qt::darkGray);

	if (lineCount == 0)
		return;
	
	painter.setPen(Qt::black);
	while (currentPosition < rect.height() && startLine < lineCount)
	{
		item = this->_editor->itemAt(startLine++);
		printer = item->getPrinter();

		instruction = dynamic_cast<Instruction *>(item);

		if (instruction != 0)
			painter.drawText(30, currentPosition, this->width(), printer->height(),
			Qt::AlignLeft | Qt::AlignVCenter, QString::fromStdString(instruction->address().ToString()));

		currentPosition += printer->getHeight();
		startLine += printer->skipNumber();
	}
}