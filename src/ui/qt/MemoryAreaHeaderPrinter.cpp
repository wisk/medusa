#include <QPaintEvent>
#include <QPainter>
#include <medusa/medusa.hpp>
#include <QAction>
#include "MemoryAreaHeaderPrinter.hpp"
#include "MemoryAreaHeader.hpp"

MemoryAreaHeaderPrinter::MemoryAreaHeaderPrinter(QWidget * parent, MedusaEditor * editor)
	: IMedusaPrinter(parent, editor),
	_memoryAreaHeader(0)
{
	this->resize(500, 50);

	this->setContextMenuPolicy(Qt::ActionsContextMenu);
	this->addAction(new QAction("Tata", this));
}

MemoryAreaHeaderPrinter::~MemoryAreaHeaderPrinter()
{
}

QSize			MemoryAreaHeaderPrinter::sizeHint() const
{
	return (QSize(500, 50));
}

void			MemoryAreaHeaderPrinter::paintEvent(QPaintEvent * event)
{
	if (this->_memoryAreaHeader == 0)
		return;

	QPainter	painter(this);

  painter.setPen(Qt::darkCyan);
	painter.setFont(QFont("Courier New", 12, QFont::Bold));
  QString const& maStr = this->_memoryAreaHeader->_name;
  painter.drawText(0, 0, maStr.length() * 32, 50, Qt::AlignLeft | Qt::AlignVCenter, maStr);
}

void			MemoryAreaHeaderPrinter::itemChanged()
{
	if (this->_item == 0)
		this->_memoryAreaHeader = 0;
	else
		this->_memoryAreaHeader = dynamic_cast<MemoryAreaHeader *>(this->_item);
}