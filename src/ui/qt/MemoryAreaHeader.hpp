#ifndef __MEMORY_AREA_HEADER_H__
# define __MEMORY_AREA_HEADER_H__

# include <QString>
# include "IMedusaItem.hpp"

class MemoryAreaHeader : public IMedusaItem
{
	friend class MemoryAreaHeaderPrinter;

public:
	MemoryAreaHeader(int itemID, MedusaEditor * editor, QString const & name);
	~MemoryAreaHeader();

private:
	IMedusaPrinter *		retrievePrinter() const;

private:
	QString					_name;
};

#endif // !__MEMORY_AREA_HEADER_H__