#include "Function.hpp"
#include "FunctionPrinter.hpp"
#include "MedusaPrinterPool.hpp"

Function::Function(int itemID,
	MedusaEditor * editor,
	medusa::Cell * cell,
	medusa::MemoryArea * memoryArea,
	medusa::Address const & address,
	int skipNumber)
	: Instruction(itemID, editor, cell, memoryArea, address),
	_skipNumber(skipNumber),
	_collapsed(false)
{

}

Function::~Function()
{

}

int			Function::skipNumber() const
{
	return (this->_skipNumber);
}

void		Function::setCollapsed(bool collapsed)
{
	this->_collapsed = collapsed;
}

IMedusaPrinter *		Function::retrievePrinter() const
{
	return (MedusaPrinterPool::getPrinter<FunctionPrinter>());
}