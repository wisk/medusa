#include <QPainter>
#include <QImage>
#include "MedusaCollapse.hpp"

MedusaCollapse::MedusaCollapse(QWidget * parent, bool collapsed)
	: QWidget(parent),
	_collapsed(collapsed),
	_visible(false)
{

}

MedusaCollapse::~MedusaCollapse()
{

}

QSize			MedusaCollapse::sizeHint() const
{
	return (QSize(MedusaCollapse::width, MedusaCollapse::height));
}

bool			MedusaCollapse::collapsed() const
{
	return (this->_collapsed);
}

void			MedusaCollapse::setCollapsed(bool collapsed)
{
	this->_collapsed = collapsed;
}

void			MedusaCollapse::paintEvent(QPaintEvent *)
{
	QPainter	painter(this);

	if (!this->_visible)
		this->hide();

	if (this->_collapsed)
		painter.drawImage(0, 2, QImage(":/images/plus.png"));
	else
		painter.drawImage(0, 2, QImage(":/images/minus.png"));
}

void			MedusaCollapse::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {

		this->_collapsed = !this->_collapsed;
		emit collapseChanged(this->_collapsed);
	}
}