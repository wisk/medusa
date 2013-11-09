#include "ScrollbarAddress.hpp"

#include <medusa/log.hpp>

int ScrollbarAddress::_width = 40;

ScrollbarAddress::ScrollbarAddress(QWidget * parent, medusa::Medusa& core)
  : QWidget(parent), View(medusa::Document::Subscriber::AddressUpdated, core.GetDocument())
  , _core(core)
  , _fullImg(1, static_cast<int>(core.GetDocument().GetNumberOfAddress())), _img()
  , _lastPos(0)
  , _currPos(0)
{
  setFixedWidth(_width);
  setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
  _fullImg.fill(Qt::black);
  _img = _fullImg;

  connect(this, SIGNAL(flushed(QSize const&)), this, SLOT(flush(QSize const&)));
  connect(this, SIGNAL(updated()), this, SLOT(update()));
}

ScrollbarAddress::~ScrollbarAddress(void)
{
}

void ScrollbarAddress::OnAddressUpdated(medusa::Address::List const& rAddressList)
{
  auto const& doc = _core.GetDocument();

  if (!rAddressList.empty())
  {
    doc.ConvertAddressToPosition(*rAddressList.crbegin(), _lastPos);
    emit updated();
  }

  QPainter p(&_fullImg);
  const QColor insnColor(Qt::blue);
  const QColor dataColor(Qt::darkRed);
  const QColor strnColor(Qt::magenta);
  const QColor unknColor(Qt::black);

  _mutex.lock();
  std::for_each(std::begin(rAddressList), std::end(rAddressList), [&](medusa::Address const& addr)
  {
    switch (doc.GetCellType(addr))
    {
    case medusa::Cell::InstructionType: p.setPen(insnColor); p.setBrush(insnColor); break;
    case medusa::Cell::ValueType:       p.setPen(dataColor); p.setBrush(dataColor); break;
    case medusa::Cell::StringType:      p.setPen(strnColor); p.setBrush(strnColor); break;
    default:                            p.setPen(unknColor); p.setBrush(unknColor); break;
    }

    medusa::u32 pos;
    if (!doc.ConvertAddressToPosition(addr, pos))
      return;
    auto cell = doc.GetCell(addr);
    size_t cellLen = cell->GetLength();
    p.drawRect(0, pos, 1, static_cast<int>(cellLen));
  });

  _img = _fullImg.scaled(this->size());
  _mutex.unlock();
}

void ScrollbarAddress::paintEvent(QPaintEvent * evt)
{
  if (_img.isNull())
    return;

  QPainter painter(this);
  _mutex.lock();
  painter.drawPixmap(0, 0, _img);
  painter.setPen(Qt::yellow);
  int lastPosY = static_cast<int>(_lastPos * _img.height() / _fullImg.height());
  painter.drawLine(0, lastPosY, _width / 2, lastPosY);
  painter.setPen(Qt::cyan);
  int currPosY = static_cast<int>(_currPos * _img.height() / _fullImg.height());
  painter.drawLine(_width / 2, currPosY, _width, currPosY);
  _mutex.unlock();
}

void ScrollbarAddress::resizeEvent(QResizeEvent *evt)
{
  _mutex.lock();
  _img = _fullImg.scaled(evt->size());
  _mutex.unlock();
}

void ScrollbarAddress::mouseMoveEvent(QMouseEvent * evt)
{
  if (evt->buttons() & Qt::LeftButton)
  {
    auto pos = static_cast<medusa::u32>(evt->y() * _fullImg.height() / _img.height());
    medusa::Address addr;
    if (!_core.GetDocument().ConvertPositionToAddress(pos, addr))
      return;
    _currPos = pos;
    emit goTo(addr);
    emit updated();
  }
}

void ScrollbarAddress::flush(QSize const& size)
{

}

void ScrollbarAddress::setCurrentAddress(medusa::Address const& addr)
{
  if (_core.GetDocument().ConvertAddressToPosition(addr, _currPos))
    emit updated();
}
