#ifndef __FUNCTION_PRINTER_H__
# define __FUNCTION_PRINTER_H__

# include <QMoveEvent>
# include "InstructionPrinter.hpp"
# include "MedusaCollapse.hpp"

class Function;
class QShowEvent;

class FunctionPrinter : public InstructionPrinter
{
	Q_OBJECT

public:
	FunctionPrinter(QWidget * parent, MedusaEditor * editor);
	~FunctionPrinter();

public:
	int					skipNumber() const;
	virtual void		setY(int y);
	void				release(bool force = false);
	void				awake();

public slots:
	void				on_medusaCollapse_collapsedChanged(bool collapsed);

protected:
	void				paintEvent(QPaintEvent * event);

private:
	void				itemChanged();

private:
	Function *			_function;
	MedusaCollapse		_medusaCollapse;
};

#endif // !__FUNCTION_PRINTER_H__
