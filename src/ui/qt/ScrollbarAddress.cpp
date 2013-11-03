#include "ScrollbarAddress.hpp"

#include <medusa/log.hpp>

ScrollbarAddress::ScrollbarAddress(QWidget * parent, medusa::Medusa& core)
  : QWidget(parent), View(medusa::Document::Subscriber::AddressUpdated, core.GetDocument())
  , _core(core), _img()
{
  setFixedWidth(40);
  setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
}

ScrollbarAddress::~ScrollbarAddress(void)
{
}

void ScrollbarAddress::OnAddressUpdated(medusa::Address::List const& rAddressList)
{
  auto const& doc = _core.GetDocument();
  medusa::u32 h = static_cast<medusa::u32>(size().height());
  medusa::u32 s = doc.GetNumberOfAddress();

  QPainter p(&_img);
  QPen insnPen(Qt::blue, 1.0);
  QPen dataPen(Qt::darkRed, 1.0);
  QPen strnPen(Qt::magenta, 1.0);
  QPen unknPen(Qt::black, 1.0);

  std::for_each(std::begin(rAddressList), std::end(rAddressList), [&](medusa::Address const& addr)
  {
    switch (doc.GetCellType(addr))
    {
    case medusa::Cell::InstructionType: p.setPen(insnPen); break;
    case medusa::Cell::ValueType:       p.setPen(dataPen); break;
    case medusa::Cell::StringType:      p.setPen(strnPen); break;
    default:                            p.setPen(unknPen); break;
    }

    medusa::u32 pos;
    if (!doc.ConvertAddressToPosition(addr, pos))
      return;
    int off = static_cast<int>(pos * h / s);
    _mutex.lock();
    auto cell = doc.GetCell(addr);
    size_t cellLen = cell->GetLength();
    while (cellLen--)
    {
      p.drawLine(0, off, size().height(), off);
      ++off;
    }
    _mutex.unlock();
  });
}

void ScrollbarAddress::paintEvent(QPaintEvent * p)
{
  QPainter painter(this);
  _mutex.lock();
  painter.drawPixmap(0, 0, _img);
  _mutex.unlock();
}

void ScrollbarAddress::resizeEvent(QResizeEvent * r)
{
  QPixmap newImg(size());

  auto const& doc = _core.GetDocument();

  medusa::u32 h = static_cast<medusa::u32>(size().height());
  medusa::u32 s = doc.GetNumberOfAddress();

  medusa::u32 idx = 0;
  medusa::u32 step = s / h;
  if (step == 0)
    step = 1;

  QPainter p(&newImg);
  QPen insnPen(Qt::blue, 1.0);
  QPen dataPen(Qt::darkRed, 1.0);
  QPen strnPen(Qt::magenta, 1.0);
  QPen unknPen(Qt::black, 1.0);

  medusa::Address curAddr = (*doc.Begin())->GetBaseAddress(); // HACK: Need to provide a better API
  do
  {
    if (idx >= h)
      break;

    switch (doc.GetCellType(curAddr))
    {
    case medusa::Cell::InstructionType: p.setPen(insnPen); break;
    case medusa::Cell::ValueType:       p.setPen(dataPen); break;
    case medusa::Cell::StringType:      p.setPen(strnPen); break;
    default:                            p.setPen(unknPen); break;
    }

    auto cell = doc.GetCell(curAddr);
    size_t cellLen = cell->GetLength();
    while (cellLen--)
    {
      p.drawLine(0, idx, size().height(), idx);
      ++idx;
    }
  }
  while (_core.GetDocument().MoveAddress(curAddr, curAddr, step));

  _mutex.lock();
  _img = newImg;
  _mutex.unlock();
}