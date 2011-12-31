#ifndef __INSTRUCTION_H__
# define __INSTRUCTION_H__

# include <QString>
# include "IMedusaItem.hpp"
# include <medusa/medusa.hpp>
# include <medusa/memory_area.hpp>

class Cell;
class MedusaEditor;

class Instruction : public IMedusaItem
{
	friend class InstructionPrinter;

public:
	Instruction(int itemID,
		MedusaEditor * editor,
		medusa::Cell * cell,
		medusa::MemoryArea * memoryArea,
		medusa::Address const & address);
	~Instruction();

public:
	void					setComment(QString const & comment);
	QString					comment() const;
	medusa::Address const &	address() const;

public:
	QString					toString() const;
	QString					toHtml() const;
	bool					convertDataToString();

private:
	IMedusaPrinter *		retrievePrinter() const;

private:
	medusa::Cell *			_cell;
	medusa::Instruction *	_instruction;
	QString					_comment;
	medusa::MemoryArea *	_memoryArea;
	medusa::Address			_address;
};

#endif // !__INSTRUCTION_H__