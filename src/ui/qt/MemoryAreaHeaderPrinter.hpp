#ifndef __MEMORY_AREA_HEADER_PRINTER_H__
# define __MEMORY_AREA_HEADER_PRINTER_H__

# include "IMedusaPrinter.hpp"

class MemoryAreaHeader;
class MedusaEditor;
class MainWindow;

class MemoryAreaHeaderPrinter : public IMedusaPrinter
{
	Q_OBJECT

public:
	MemoryAreaHeaderPrinter(QWidget * parent, MedusaEditor * medusaEditor);
	~MemoryAreaHeaderPrinter();

public:
	QSize					sizeHint() const;

protected:
	void					paintEvent(QPaintEvent * event);

private:
	void					itemChanged();

private:
	MemoryAreaHeader *			_memoryAreaHeader;
};

#endif // !__MEMORY_AREA_HEADER_PRINTER_H__