#include "MedusaPrinterPool.hpp"

MedusaPrinterPool::MedusaPrinterPool()
	: _defaultParent(0),
	_printers()
{

}

MedusaPrinterPool::~MedusaPrinterPool()
{

}

MedusaPrinterPool *		MedusaPrinterPool::instance()
{
	static MedusaPrinterPool	pool;

	return (&pool);
}

void		MedusaPrinterPool::setDefaultParent(QWidget * parent)
{
	MedusaPrinterPool::instance()->_defaultParent = parent;
}

void		MedusaPrinterPool::setEditor(MedusaEditor * editor)
{
	MedusaPrinterPool::instance()->_medusaEditor = editor;
}

void		MedusaPrinterPool::setMainWindow(MainWindow * mainWindow)
{
	MedusaPrinterPool::instance()->_mainWindow = mainWindow;
}
