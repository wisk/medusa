#include <QColorDialog>
#include <QPalette>
#include "ColorPicker.h"

ColorPicker::ColorPicker(QWidget * parent)
	: QLineEdit(parent),
	_color()
{
	this->setReadOnly(true);
}

ColorPicker::~ColorPicker()
{

}


void			ColorPicker::setColor(QColor const & color)
{
	if (color.isValid())
	{
		this->_color = color;

		QPalette	palette(this->palette());

		palette.setColor(QPalette::Base, color);
		this->setPalette(palette);
	}

	this->setText(color.name().toUpper());
}

QColor			ColorPicker::color() const
{
	return (this->_color);
}

void			ColorPicker::mousePressEvent(QMouseEvent * event)
{
	QColor		color = QColorDialog::getColor(this->text(), this, QObject::tr("Select color"), QColorDialog::DontUseNativeDialog);

	this->setColor(color);
}
