#include <QPaintEvent>
#include <QPainter>
#include <QIcon>
#include <QString>
#include <QDebug>
#include <QFont>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QFontInfo>
#include <QColor>
#include <QPair>
#include <medusa/medusa.hpp>
#include "InstructionPrinter.hpp"
#include "Instruction.hpp"
#include "MedusaFont.hpp"
#include "Settings.hpp"
#include "MedusaEditor.hpp"
#include "MainWindow.hpp"

int InstructionPrinter::MaxInstructionWidth = -1;
int InstructionPrinter::MaxInstructionHeight = -1;
MedusaVector<int>	InstructionPrinter::MaxOperandSize = MedusaVector<int>();

InstructionPrinter::InstructionPrinter(QWidget * parent, MedusaEditor * editor)
	: IMedusaPrinter(parent, editor),
	_instruction(0),
	_menu(this),
	_title("", &_menu),
	_editComment("Edit comment", &_menu),
	_convertToString("Convert to string", &_menu),
	_updateWidget(false),
	_labels(this),
	_commentInput(this),
	_commentWidth(MinCommentWidth)
{
	this->_focusMode = IMedusaPrinter::LeftClick;

	this->setContextMenuPolicy(Qt::DefaultContextMenu);

	this->updateMenu();

	this->_editComment.setObjectName("editComment");
	this->_convertToString.setObjectName("convertToString");
	this->_commentInput.setObjectName("commentInput");

	this->_commentInput.hide();

	this->updateFontSize();

	QMetaObject::connectSlotsByName(this);
}

InstructionPrinter::~InstructionPrinter()
{

}

QSize			InstructionPrinter::sizeHint() const
{
	return (QSize(0, 0));
}

void			InstructionPrinter::settingsEvent()
{
	this->updateLabels();
	this->updateFontSize();
}

void			InstructionPrinter::paintEvent(QPaintEvent * event)
{
	QMutexLocker	locker(&this->_mutex);

	if (this->_instruction == 0)
		return;

	QFont	font;
	QPainter	painter(this);
	QString		text;

	if (this->_instruction->_hasFocus) {

		painter.setBrush(QColor(Settings::instance().value(EDITOR_INSTRUCTION_SELECTION_PATH, EDITOR_INSTRUCTION_SELECTION_DEFAULT).toString()));
		painter.setPen(QColor(Settings::instance().value(EDITOR_INSTRUCTION_SELECTION_PATH, EDITOR_INSTRUCTION_SELECTION_DEFAULT).toString()));
		painter.drawRect(this->rect());
	}
	
	text = "; " + this->_instruction->_comment;
	if (text == "; ")
		text = "";
	font.fromString(Settings::instance().value(EDITOR_FONT_COMMENT, EDITOR_FONT_COMMENT_DEFAULT).toString());

	painter.setPen(QColor(Settings::instance().value(EDITOR_INSTRUCTION_COMMENT_PATH, EDITOR_INSTRUCTION_COMMENT_DEFAULT).toString()));
	painter.setFont(font);
	painter.drawText(MaxInstructionWidth + Separator + (50 - this->x()), 0, this->_commentWidth, InstructionHeight, Qt::AlignLeft | Qt::AlignVCenter, text);
}

void			InstructionPrinter::contextMenuEvent(QContextMenuEvent * event)
{
	this->_menu.move(event->globalPos());
	this->_menu.show();
}

void			InstructionPrinter::keyReleaseEvent(QKeyEvent * event)
{
	if (this->_commentInput.isVisible())
		return;

	switch (event->key()) {

	case Qt::Key_Semicolon:
		this->on_editComment_triggered(true);
		break;
	}
}

void			InstructionPrinter::mouseDoubleClickEvent(QMouseEvent * event)
{
	QMutexLocker	locker(&this->_mutex);

	if (this->_instruction != 0 && this->_instruction->_instruction != 0)
	{
		switch (this->_instruction->_instruction->GetType())
		{
		case medusa::Instruction::OpCall:
		case medusa::Instruction::OpJump:
			{

				medusa::Address destination;

				if (this->_instruction->_instruction->GetOperandReference(0, this->_instruction->_address, destination))
					this->_editor->goTo(MedusaEditor::Address(destination.GetBase(), destination.GetOffset()), QString("Go to %1").arg(QString::fromStdString(destination.ToString())));
			}
			break;
		}
	}
}

void			InstructionPrinter::on_editComment_triggered(bool)
{
	QMutexLocker	locker(&this->_mutex);

	if (this->_instruction == 0)
		return;

	this->_locked = true;

	this->_commentInput.setText(this->_instruction->_comment);
	this->_commentInput.show();
	this->_commentInput.setFocus(Qt::OtherFocusReason);
}

void			InstructionPrinter::on_convertToString_triggered(bool)
{
	QMutexLocker	locker(&this->_mutex);

	if (this->_instruction == 0)
		return;

	if (this->_instruction->_cell->GetType() != medusa::Cell::ValueType)
		return;

	this->_editor->convertDataToString(this->_instruction->_itemID);
}

void			InstructionPrinter::on_commentInput_returnPressed()
{
	QMutexLocker	locker(&this->_mutex);

	if (this->_instruction == 0)
		return;

	this->_commentInput.hide();
	this->_instruction->_comment = this->_commentInput.text();
	this->setFocus(Qt::MouseFocusReason);

	this->updateFontSize();
	this->update();

	this->_locked = false;
}

void			InstructionPrinter::itemChanged()
{
	if (this->_item == 0)
		this->_instruction = 0;
	else
	{
		this->_instruction = dynamic_cast<Instruction *>(this->_item);
		this->_title.setText(QString::fromStdString(this->_instruction->_cell->ToString()));

		this->updateFontSize();
		this->updateLabels();
		this->updateMenu();
	}
}

void			InstructionPrinter::focusChanged(bool focus)
{
	QMutexLocker	locker(&this->_mutex);

	if (focus == false)
	{
		if (this->_commentInput.isVisible())
		{
			this->_commentInput.hide();
			this->_instruction->_comment = this->_commentInput.text();
			this->updateFontSize();
			this->update();
			this->_locked = false;
		}
	}
	else
	{
		if (this->_commentInput.isVisible())
			this->_commentInput.setFocus(Qt::OtherFocusReason);
	}
}

void			InstructionPrinter::updateFontSize()
{
	QMutexLocker	locker(&this->_mutex);

	if (this->_instruction == 0)
		return;

	QFont		font;

	// Set the comment size
	font.fromString(Settings::instance().value(EDITOR_FONT_COMMENT, EDITOR_FONT_COMMENT_DEFAULT).toString());
	if (this->_instruction != 0)
		this->_commentWidth = MedusaFont::getSize(font, "; " + this->_instruction->_comment).width();

	if (this->_commentWidth < MinCommentWidth)
		this->_commentWidth = MinCommentWidth;

	this->resize(MaxInstructionWidth + Separator + this->_commentWidth, InstructionHeight);
	this->_commentInput.resize(this->_commentWidth, InstructionHeight);
	this->_commentInput.move(MaxInstructionWidth + Separator, 0);
}

void			InstructionPrinter::updateLabels()
{
	QMutexLocker	locker(&this->_mutex);

	foreach (QLabel * label, this->_labels)
		label->hide();
	this->_labels.reset();

	if (this->_instruction == 0)
		return;

	QFont		font;
	QPalette	palette;
	int			i = 0;
	int			width = 0;

	// Dans le cas d'une cellule basic (data, pas d'instruction)
	if (this->_instruction->_cell->GetType() != medusa::Cell::InstructionType)
	{
		QLabel * label = this->_labels.getNextItem();
		label->show();
		label->move(0, 0);
		label->setText(QString::fromStdString(this->_instruction->_cell->ToString()));
		palette = label->palette();

		font.fromString(Settings::instance().value(EDITOR_FONT_INSTRUCTION, EDITOR_FONT_INSTRUCTION_DEFAULT).toString());
		label->setFont(font);
		palette.setColor(QPalette::Text, QColor(Settings::instance().value(EDITOR_INSTRUCTION_INSTRUCTION_PATH, EDITOR_INSTRUCTION_INSTRUCTION_DEFAULT).toString()));
		label->setPalette(palette);
		label->resize(MedusaFont::getSize(font, label->text()));

		if (label->width() > InstructionPrinter::MaxOperandSize[0])
			InstructionPrinter::MaxOperandSize[0] = label->width();
		else
		{
			label->resize(InstructionPrinter::MaxOperandSize[0], label->height());
			this->parentWidget()->update();
		}

		width = label->width();
	}
	else
	{
		QLabel * label = this->_labels.getNextItem();
		label->show();
		label->move(0, 0);
		label->setText(QString::fromStdString(this->_instruction->_instruction->GetName()));
		palette = label->palette();

		font.fromString(Settings::instance().value(EDITOR_FONT_INSTRUCTION, EDITOR_FONT_INSTRUCTION_DEFAULT).toString());
		label->setFont(font);
		palette.setColor(QPalette::Text, QColor(Settings::instance().value(EDITOR_INSTRUCTION_INSTRUCTION_PATH, EDITOR_INSTRUCTION_INSTRUCTION_DEFAULT).toString()));
		label->setPalette(palette);
		label->resize(MedusaFont::getSize(font, label->text()));
		label->resize(label->width() + 30, label->height());

		if (label->width() > InstructionPrinter::MaxOperandSize[0])
			InstructionPrinter::MaxOperandSize[0] = label->width();
		else
		{
			label->resize(InstructionPrinter::MaxOperandSize[0], label->height());
			this->parentWidget()->update();
		}
		
		width = label->width();

		for (i = 0; i < OPERAND_NO; ++i)
		{
			medusa::Operand * operand = this->_instruction->_instruction->Operand(i);
			if (operand != 0)
			{
				QLabel * label = this->_labels.getNextItem();
				label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
				label->setLineWidth(1);
				label->show();
				label->setText(QString::fromStdString(operand->GetName()));
				medusa::u32 type = operand->GetType();
				palette = label->palette();

				if (type & O_REG)
				{
					font.fromString(Settings::instance().value(EDITOR_FONT_REGISTER, EDITOR_FONT_REGISTER_DEFAULT).toString());
					label->setFont(font);
					palette.setColor(QPalette::Text, QColor(Settings::instance().value(EDITOR_INSTRUCTION_REGISTER_PATH, EDITOR_INSTRUCTION_REGISTER_DEFAULT).toString()));
					label->setPalette(palette);
				}
				else if (type & O_IMM)
				{
					font.fromString(Settings::instance().value(EDITOR_FONT_IMMEDIATE, EDITOR_FONT_IMMEDIATE_DEFAULT).toString());
					label->setFont(font);
					palette.setColor(QPalette::Text, QColor(Settings::instance().value(EDITOR_INSTRUCTION_IMMEDIATE_PATH, EDITOR_INSTRUCTION_IMMEDIATE_DEFAULT).toString()));
					label->setPalette(palette);
				}
				else if (type != O_NONE)
				{
					font.fromString(Settings::instance().value(EDITOR_FONT_INSTRUCTION, EDITOR_FONT_INSTRUCTION_DEFAULT).toString());
					label->setFont(font);
					palette.setColor(QPalette::Text, QColor(Settings::instance().value(EDITOR_INSTRUCTION_INSTRUCTION_PATH, EDITOR_INSTRUCTION_INSTRUCTION_DEFAULT).toString()));
					label->setPalette(palette);
				}

				label->resize(MedusaFont::getSize(font, label->text()));
				//label->resize(label->width() + 15, label->height());
				label->move(width, 0);

				/*if (label->width() > InstructionPrinter::MaxOperandSize[i + 1])
					InstructionPrinter::MaxOperandSize[i + 1] = label->width();
				else
				{
					label->resize(InstructionPrinter::MaxOperandSize[i + 1], label->height());
					this->parentWidget()->update();
				}*/

				width += label->width();
			}
			if (i + 1 < OPERAND_NO && this->_instruction->_instruction->Operand(i + 1) != 0
				&& this->_instruction->_instruction->Operand(i + 1)->GetType() != O_NONE)
			{
				QLabel * label = this->_labels.getNextItem();
				label->show();
				font = label->font();
				font.setPointSize(16);
				label->setText(" , ");
				label->resize(MedusaFont::getSize(font, label->text()));
				label->move(width, 0);
				width += label->width();
			}
		}
	}

	if (InstructionPrinter::MaxInstructionWidth < width)
	{
		InstructionPrinter::MaxInstructionWidth = width;
		this->parentWidget()->update();
	}
}

void			InstructionPrinter::updateMenu()
{
	foreach (QAction * action, this->_menu.actions())
		this->_menu.removeAction(action);

	if (this->_instruction == 0)
		return;

	this->_menu.addAction(&this->_title);
	this->_menu.addAction(&this->_editComment);

	this->_title.setEnabled(false);

	if (this->_instruction->_cell->GetType() == medusa::Cell::ValueType)
	{
		this->_menu.addAction(&this->_convertToString);
	}
}


