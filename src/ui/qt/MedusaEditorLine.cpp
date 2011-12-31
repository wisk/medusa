#include <QPainter>
#include "MedusaEditorLine.hpp"
#include "MedusaEditorLinePrinter.hpp"

MedusaEditorLine * MedusaEditorLine::focusedLine = 0;

MedusaEditorLine::MedusaEditorLine(QString const & address, QString const & text)
	: _height(20),
	_printer(0),
	_address(address),
	_text(text),
	_focus(false)
{
}

MedusaEditorLine::~MedusaEditorLine()
{
	if (this->_printer != 0)
		this->_printer->setLine(0);

	if (MedusaEditorLine::focusedLine == this)
		MedusaEditorLine::focusedLine = 0;
}

QString const &		MedusaEditorLine::address() const
{
	return (this->_address);
}

QString const &		MedusaEditorLine::text() const
{
	return (this->_text);
}

void				MedusaEditorLine::setPrinter(MedusaEditorLinePrinter * printer)
{
	this->_printer = printer;
}

int					MedusaEditorLine::height() const
{
	return (this->_height);
}

void				MedusaEditorLine::setFocus(bool focus)
{
	this->_focus = focus;

	if (MedusaEditorLine::focusedLine != this && this->_focus)
	{
		if (MedusaEditorLine::focusedLine != 0)
			MedusaEditorLine::focusedLine->setFocus(false);
		MedusaEditorLine::focusedLine = this;
	}

	if (MedusaEditorLine::focusedLine == this && !this->_focus)
		MedusaEditorLine::focusedLine = 0;
}

bool				MedusaEditorLine::focus() const
{
	return (this->_focus);
}