#include "FunctionPrinter.hpp"
#include "Function.hpp"
#include "MedusaEditor.hpp"

FunctionPrinter::FunctionPrinter(QWidget * parent, MedusaEditor * editor)
	: InstructionPrinter(parent, editor),
	_function(0),
	_medusaCollapse(parent)
{
	this->_medusaCollapse.show();

	this->connect(&this->_medusaCollapse, SIGNAL(collapseChanged(bool)), SLOT(on_medusaCollapse_collapsedChanged(bool)));
}

FunctionPrinter::~FunctionPrinter()
{

}

int					FunctionPrinter::skipNumber() const
{
	QMutexLocker	locker(&this->_mutex);

	if (this->_medusaCollapse.collapsed() && this->_function != 0)
		return (this->_function->_skipNumber);

	return (0);
}

void				FunctionPrinter::setY(int y)
{
	this->move(30, y);
	this->_medusaCollapse.move(5, y);
}

void				FunctionPrinter::release(bool force)
{
	IMedusaPrinter::release(force);

	if (!force)
		this->_medusaCollapse.hide();
	this->_medusaCollapse._visible = false;
}

void				FunctionPrinter::awake()
{
	IMedusaPrinter::awake();

	this->_medusaCollapse._visible = true;
	this->_medusaCollapse.show();
}

void				FunctionPrinter::on_medusaCollapse_collapsedChanged(bool collapsed)
{
  // XXX: Quick fix for ghost collapse icon
	if (this->_function == NULL)
		return;
	this->_locked = collapsed;
	this->_function->_collapsed = collapsed;
	this->_editor->setCollapsedItem(this->_function->_itemID + 1, this->_function->_skipNumber, collapsed);
	if (!collapsed)
		this->_editor->releasePrinter();
	this->_editor->viewport()->update();
}

void				FunctionPrinter::paintEvent(QPaintEvent * event)
{
	if (this->_item == 0 && this->isVisible())
	{
		this->hide();
		this->_medusaCollapse.hide();
	}
	else
		InstructionPrinter::paintEvent(event);
}

void				FunctionPrinter::itemChanged()
{
	InstructionPrinter::itemChanged();

	if (this->_item == 0)
		this->_function = 0;
	else {

		this->_function = dynamic_cast<Function *>(this->_item);
		this->_medusaCollapse.setCollapsed(this->_function->_collapsed);
		this->_locked = this->_function->_collapsed;
		this->_medusaCollapse.show();
	}
}