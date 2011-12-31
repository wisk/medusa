#include "MemoryAreaHeader.hpp"
#include "MemoryAreaHeaderPrinter.hpp"
#include "MedusaPrinterPool.hpp"

MemoryAreaHeader::MemoryAreaHeader(int itemID, MedusaEditor * editor, QString const & name)
	: IMedusaItem(itemID, editor),
	_name(name)
{

}

MemoryAreaHeader::~MemoryAreaHeader()
{

}

IMedusaPrinter *		MemoryAreaHeader::retrievePrinter() const
{
	return (MedusaPrinterPool::getPrinter<MemoryAreaHeaderPrinter>());
}