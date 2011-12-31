#ifndef __FUNCTION_H__
# define __FUNCTION_H__

# include "Instruction.hpp"

class Function : public Instruction
{
	friend class FunctionPrinter;

public:
	Function(int itemID,
		MedusaEditor * editor,
		medusa::Cell * cell,
		medusa::MemoryArea * memoryArea,
		medusa::Address const & address,
		int skipNumber = 0);
	~Function();

public:
	int					skipNumber() const;
	void				setCollapsed(bool collapsed);

private:
	IMedusaPrinter *	retrievePrinter() const;

private:
	int					_skipNumber;
	bool				_collapsed;
};

#endif // !__FUNCTION_H__