#include <QPainter>
#include "MedusaEditorLinePrinter.hpp"
#include "MedusaEditorLine.hpp"

MedusaEditorLinePrinter::MedusaEditorLinePrinter(QWidget * parent)
	: QWidget(parent),
	_line(0),
	_mutex()
{

}

MedusaEditorLinePrinter::~MedusaEditorLinePrinter()
{
	if (this->_line != 0)
		this->_line->setPrinter(0);
}

void				MedusaEditorLinePrinter::setLine(MedusaEditorLine * line)
{
	QMutexLocker	locker(&this->_mutex);

	if (this->_line != 0)
		this->_line->setPrinter(0);

	this->_line = line;

	if (this->_line != 0)
	{
		this->_line->setPrinter(this);
		this->resize(MedusaEditorLinePrinter::width, this->_line->height());
	}
	else
		this->resize(MedusaEditorLinePrinter::width, 20);
}

QSize				MedusaEditorLinePrinter::sizeHint() const
{
	return (QSize(MedusaEditorLinePrinter::width, 20));
}

void				MedusaEditorLinePrinter::paintEvent(QPaintEvent * event)
{
	QMutexLocker	locker(&this->_mutex);
	QPainter		painter(this);

	if (this->_line == 0)
		return;

	if (this->_line->focus())
		painter.fillRect(this->rect(), Qt::yellow);

	painter.setPen(Qt::black);
	painter.setFont(QFont("Courier New", 10));
	painter.drawText(20, 0, 150, 20, Qt::AlignRight | Qt::AlignVCenter, this->_line->address());

	painter.setPen(Qt::blue);
	painter.setFont(QFont("Courier New", 10, QFont::Bold));
	painter.drawText(172, 0, 300, 20, Qt::AlignLeft | Qt::AlignVCenter, this->_line->text());
}

void				MedusaEditorLinePrinter::mouseReleaseEvent(QMouseEvent * event)
{
	QMutexLocker	locker(&this->_mutex);

	if (this->_line != 0)
		this->_line->setFocus(true);

	this->update();
}