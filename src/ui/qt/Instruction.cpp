#include <QFont>
#include <QDebug>
#include <medusa/value.hpp>
#include "Instruction.hpp"
#include "InstructionPrinter.hpp"
#include "MedusaPrinterPool.hpp"
#include "MedusaFont.hpp"
#include "Settings.hpp"
#include "MedusaHtml.hpp"

Instruction::Instruction(int itemID, MedusaEditor * editor, medusa::Cell * cell, medusa::MemoryArea * memoryArea, medusa::Address const &  address)
	: IMedusaItem(itemID, editor),
	_cell(cell),
	_instruction(0),
	_comment(),
	_memoryArea(memoryArea),
	_address(address)
{
	this->_instruction = dynamic_cast<medusa::Instruction *>(this->_cell);
}

Instruction::~Instruction()
{

}

void					Instruction::setComment(QString const & comment)
{
	this->_comment = comment;
}

QString					Instruction::comment() const
{
	return (this->_comment);
}

medusa::Address const &	Instruction::address() const
{
	return (this->_address);
}

QString					Instruction::toString() const
{
	return (QString::fromStdString(this->_address.ToString()) + "  " + QString::fromStdString(this->_cell->ToString()) + "  " + this->_comment);
}

QString					Instruction::toHtml() const
{
	QString				text;
	QFont				font;

	text = QString::fromStdString(this->_address.ToString()) + "  ";

	font.fromString(Settings::instance().value(EDITOR_FONT_INSTRUCTION, EDITOR_FONT_INSTRUCTION_DEFAULT).toString());
	text += "<span " + MedusaHtml::getStyle(font, Settings::instance().value(EDITOR_INSTRUCTION_INSTRUCTION_PATH, EDITOR_INSTRUCTION_INSTRUCTION_DEFAULT).toString()) + ">"
		+ QString::fromStdString(this->_cell->ToString()) + "</span>";


	font.fromString(Settings::instance().value(EDITOR_FONT_COMMENT, EDITOR_FONT_COMMENT_DEFAULT).toString());
	text += "  <span " + MedusaHtml::getStyle(font, Settings::instance().value(EDITOR_INSTRUCTION_COMMENT_PATH, EDITOR_INSTRUCTION_COMMENT_DEFAULT).toString()) + ">"
		+ this->_comment + "</span>";

	return (text);
}

bool					Instruction::convertDataToString()
{
	if (this->_cell == 0)
		return (false);

	if (this->_cell->GetType() != medusa::Cell::ValueType)
		return (false);

	medusa::Value<medusa::u8> * value = dynamic_cast<medusa::Value<medusa::u8> *>(this->_cell);

	if (value->GetData() == 0)
		this->_comment = QString("\"\\0\"");
	else
		this->_comment = QString("\"%1\"").arg((char) value->GetData());

	if (this->_printer != 0)
		this->_printer->update();

	return (value->GetData() != 0);
}

IMedusaPrinter *		Instruction::retrievePrinter() const
{
	return (MedusaPrinterPool::getPrinter<InstructionPrinter>());
}