#include "IMedusaPrinter.hpp"
#include "IMedusaItem.hpp"
#include "Settings.hpp"

IMedusaPrinter::IMedusaPrinter(QWidget * parent, MedusaEditor * editor)
	: QWidget(parent),
	_item(0),
	_used(false),
	_locked(false),
	_focusMode(IMedusaPrinter::None),
	_mutex(QMutex::Recursive),
	_editor(editor)
{
	this->connect(&Settings::instance(), SIGNAL(settingsChanged()), SLOT(settingsEvent()));
}

IMedusaPrinter::~IMedusaPrinter()
{
	QMutexLocker	locker(&this->_mutex);

	if (this->_item != 0)
		this->_item->setPrinter();
}

void		IMedusaPrinter::setItem(IMedusaItem * item)
{
	QMutexLocker	locker(&this->_mutex);

	if (this->_item != 0)
		this->_item->setPrinter();

	this->_item = item;
	this->itemChanged();

	if (item != 0)
	{
		this->_used = true;
		if (this->_item->_hasFocus)
		{
			this->setFocus(Qt::OtherFocusReason);
			this->focusChanged(true);
		}
	}
}

bool		IMedusaPrinter::used() const
{
	return (this->_used);
}

void		IMedusaPrinter::release(bool force)
{
	if (this->_locked && !force)
		return;

	this->_locked = false;
	this->_used = false;
	this->setItem(0);
}

void		IMedusaPrinter::awake()
{
	this->show();
}

void		IMedusaPrinter::settingsEvent()
{

}

int			IMedusaPrinter::getHeight() const
{
	return (this->height());
}

int			IMedusaPrinter::skipNumber() const
{
	return (0);
}

void		IMedusaPrinter::setY(int y)
{
	this->move(50, y);
}

void		IMedusaPrinter::mouseReleaseEvent(QMouseEvent * event)
{
	QMutexLocker	locker(&this->_mutex);

	if (this->_item == 0)
		return;

	switch (this->_focusMode)
	{
	case IMedusaPrinter::LeftClick:
		this->_item->setFocus(true, event->modifiers().testFlag(Qt::ControlModifier));
		this->setFocus(Qt::MouseFocusReason);
		break;
	}
}

void		IMedusaPrinter::itemChanged()
{

}

void		IMedusaPrinter::focusChanged(bool focus)
{

}