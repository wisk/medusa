#include "ScrollbarAddress.hpp"

#include <medusa/log.hpp>

ScrollbarAddress::ScrollbarAddress(QWidget * parent, medusa::Medusa& core)
  : QWidget(parent), View(medusa::Document::Subscriber::AddressUpdated, core.GetDocument())
  , _core(core), _fullImg(1, static_cast<int>(core.GetDocument().GetNumberOfAddress())), _img()
{
  setFixedWidth(20);
  setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
  _fullImg.fill(Qt::black);
  _img = _fullImg;

  connect(this, SIGNAL(flushed(QSize const&)), this, SLOT(flush(QSize const&)));
}

ScrollbarAddress::~ScrollbarAddress(void)
{
}

void ScrollbarAddress::OnAddressUpdated(medusa::Address::List const& rAddressList)
{
  auto const& doc = _core.GetDocument();

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
  _addrList.clear();

  _mutex.unlock();
}

void ScrollbarAddress::paintEvent(QPaintEvent * p)
{
  if (_img.isNull())
    return;

  QPainter painter(this);
  _mutex.lock();
  painter.drawPixmap(0, 0, _img);
  _mutex.unlock();
}

void ScrollbarAddress::resizeEvent(QResizeEvent *r)
{
  _mutex.lock();
  _img = _fullImg.scaled(r->size());
  _addrList.clear();

  _mutex.unlock();
}

void ScrollbarAddress::flush(QSize const& size)
{

}