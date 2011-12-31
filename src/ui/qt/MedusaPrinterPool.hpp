#ifndef __MEDUSA_PRINTER_POOL_H__
# define __MEDUSA_PRINTER_POOL_H__

# include <QMetaObject>
# include <QHash>
# include <QList>

# include "IMedusaPrinter.hpp"

class QWidget;
class IMedusaPrinter;
class MedusaEditor;
class MainWindow;

class MedusaPrinterPool
{
private:
	MedusaPrinterPool();
	~MedusaPrinterPool();

private:
	static MedusaPrinterPool *		instance();

public:
	static void						setDefaultParent(QWidget * parent = 0);
	static void						setEditor(MedusaEditor * editor = 0);
	static void						setMainWindow(MainWindow * mainWindow = 0);

	template <class T>
	static IMedusaPrinter *			getPrinter()
	{
		static MedusaPrinterPool *	instance = MedusaPrinterPool::instance();
		static QMetaObject const *	metaObject = &T::staticMetaObject;

		if (instance->_printers.contains(metaObject))
		{
			foreach (IMedusaPrinter * printer, instance->_printers[metaObject])
			{
				if (!printer->used())
					return (printer);
			}
		}

		IMedusaPrinter * printer = new T(instance->_defaultParent, instance->_medusaEditor);

		instance->_printers[metaObject].append(printer);
		printer->show();
		return (printer);
	}

private:
	QWidget *											_defaultParent;
	MedusaEditor *										_medusaEditor;
	MainWindow *										_mainWindow;
	QHash<QMetaObject const *, QList<IMedusaPrinter *> >	_printers;
};

#endif // !__MEDUSA_PRINTER_POOL_H__
