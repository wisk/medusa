#ifndef __MEDUSA_EDITOR_LINE_PRINTER_H__
# define __MEDUSA_EDITOR_LINE_PRINTER_H__

# include <QWidget>
# include <QPaintEvent>
# include <QSize>
# include <QMutex>
# include <QMutexLocker>
# include <QFocusEvent>
# include <QMouseEvent>

class MedusaEditorLine;

class MedusaEditorLinePrinter : public QWidget
{
private:
	static const int		width = 800;

public:
	MedusaEditorLinePrinter(QWidget * parent = 0);
	~MedusaEditorLinePrinter();

public:
	void					setLine(MedusaEditorLine * line);
	QSize					sizeHint() const;

protected:
	void					paintEvent(QPaintEvent * event);
	void					mouseReleaseEvent(QMouseEvent * event);

private:
	MedusaEditorLine *		_line;
	QMutex					_mutex;
};

#endif // !__MEDUSA_EDITOR_LINE_PRINTER_H__