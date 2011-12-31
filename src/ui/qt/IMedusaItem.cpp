#include "IMedusaItem.hpp"
#include "IMedusaPrinter.hpp"
#include "MedusaEditor.hpp"

IMedusaItem::IMedusaItem(int itemID, MedusaEditor * editor)
	: _itemID(itemID),
	_printer(0),
	_hasFocus(false),
	_editor(editor),
	_hasCollapsedParent(false)
{

}

IMedusaItem::~IMedusaItem()
{
	if (this->_hasFocus)
		this->_editor->delFocusedItem(this);

	if (this->_printer != 0)
		this->_printer->release(true);
}

IMedusaPrinter *		IMedusaItem::getPrinter()
{
	if (this->_printer == 0)
	{
		this->_printer = this->retrievePrinter();
		this->_printer->setItem(this);
	}

	this->_printer->awake();

	if (this->_hasFocus)
		this->_printer->focusChanged(true);

	return (this->_printer);
}

void					IMedusaItem::setPrinter(IMedusaPrinter * printer)
{
	this->_printer = printer;
}

void					IMedusaItem::releasePrinter()
{
	if (this->_printer != 0)
	{
		this->_printer->hide();
		this->_printer->release();
	}
}

bool					IMedusaItem::hasFocus() const
{
	return (this->_hasFocus);
}

void					IMedusaItem::setFocus(bool focus, bool keepPrevious)
{
	if (!this->_hasFocus && focus)
		this->_editor->addFocusedItem(this, keepPrevious);
	else if (this->_hasFocus)
	{
		if (!focus || keepPrevious)
		{
			this->_editor->delFocusedItem(this);
			focus = false;
		}
	}

	this->_hasFocus = focus;

	if (this->_printer != 0)
	{
		this->_printer->update();
		this->_printer->focusChanged(this->_hasFocus);
	}
}

int						IMedusaItem::itemID() const
{
	return (this->_itemID);
}

bool					IMedusaItem::hasCollapsedParent() const
{
	return (this->_hasCollapsedParent);
}

void					IMedusaItem::setHasCollapsedParent(bool has)
{
	this->_hasCollapsedParent = has;
}

QString					IMedusaItem::toString() const
{
	return ("");
}

QString					IMedusaItem::toHtml() const
{
	return ("");
}

bool					IMedusaItem::convertDataToString()
{
	return (false);
}

