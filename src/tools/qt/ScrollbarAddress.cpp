#include "ScrollbarAddress.hpp"

#include <medusa/log.hpp>

int ScrollbarAddress::_width = 40;

ScrollbarAddress::ScrollbarAddress(QWidget * parent, medusa::Medusa& core)
  : QWidget(parent), View(medusa::Document::Subscriber::AddressUpdated, core.GetDocument())
  , _core(core)
  , _img(size())
  , _lastPos(0)
  , _currPos(0)
  , _maxPos(core.GetDocument().GetNumberOfAddress())
{
  setFixedWidth(_width);
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  _img.fill(_CellTypeToColor(medusa::Cell::CellType));

  connect(this, SIGNAL(updated()), this, SLOT(update()));
}

ScrollbarAddress::~ScrollbarAddress(void)
{
}

void ScrollbarAddress::OnAddressUpdated(medusa::Address::Vector const& rAddresses)
{
  auto const& doc = _core.GetDocument();

  if (!rAddresses.empty())
  {
    doc.ConvertAddressToPosition(*rAddresses.crbegin(), _lastPos);
    emit updated();
  }

  _mutex.lock();
  QPainter p(&_img);
  std::for_each(std::begin(rAddresses), std::end(rAddresses), [&](medusa::Address const& addr)
  {
    medusa::u32 pos;
    if (!doc.ConvertAddressToPosition(addr, pos))
      return;
    auto cell = doc.GetCell(addr);
    size_t cellLen = cell->GetSize();
    auto y = static_cast<int>(static_cast<medusa::u64>(pos) * _img.height() / _maxPos);
    QColor CurClr(_CellTypeToColor(doc.GetCellType(addr)));
    QColor LastClr = _img.toImage().pixel(QPoint(0, y));
    CurClr.setRgbF(
      (CurClr.redF() + LastClr.redF()) / 2,
      (CurClr.greenF() + LastClr.greenF()) / 2,
      (CurClr.blueF() + LastClr.blueF()) / 2);
    p.setPen(CurClr);
    p.setBrush(CurClr);
    auto h = static_cast<int>(static_cast<medusa::u64>(cellLen) * _img.height() / _maxPos);
    p.drawRect(0, y, _width * 3, h); // LATER: figure out why we've to * 3 ?!?
  });

  _mutex.unlock();
}

void ScrollbarAddress::Refresh(void)
{
  // FIXME(wisk):
  //auto const& rDoc = _core.GetDocument();
  //_maxPos = rDoc.GetNumberOfAddress();
  //_mutex.lock();
  //QPainter p(&_img);
  //rDoc.ForEachMemoryArea([&](medusa::MemoryArea const& rMemArea)
  //{
  //  rMemArea.ForEachCellData([&](medusa::OffsetType Offset, medusa::CellData::SPType spCellData)
  //  {
  //    medusa::Address CurAddr = rMemArea.GetBaseAddress() + rMemArea.MakeAddress(Offset);
  //    medusa::u32 Pos;
  //    if (!rDoc.ConvertAddressToPosition(CurAddr, Pos))
  //      return;

  //    size_t CellLen = spCellData->GetSize();
  //    auto h = static_cast<int>(static_cast<medusa::u64>(CellLen) * _img.height() / _maxPos);
  //    if (h == 0)
  //      h = 1;

  //    auto y = static_cast<int>(static_cast<medusa::u64>(Pos) * _img.height() / _maxPos);

  //    QColor CurClr(_CellTypeToColor(spCellData->GetType()));
  //    QColor LastClr = _img.toImage().pixel(QPoint(0, y));
  //      CurClr.setRgbF(
  //        (CurClr.redF() + LastClr.redF()) / 2,
  //        (CurClr.greenF() + LastClr.greenF()) / 2,
  //        (CurClr.blueF() + LastClr.blueF()) / 2);

  //    p.setPen(CurClr);
  //    p.setBrush(CurClr);

  //    p.drawRect(0, y, _width * 3, h); // LATER: see above about * 3
  //  });
  //});
  //_mutex.unlock();
}

void ScrollbarAddress::paintEvent(QPaintEvent * evt)
{
  if (_img.isNull())
    return;
  if (_maxPos == 0)
    return;

  _mutex.lock();
  QPainter painter(this);
  painter.drawPixmap(0, 0, _img);
  painter.setPen(Qt::yellow);
  int lastPosY = static_cast<int>(static_cast<medusa::u64>(_lastPos) * _img.height() / _maxPos);
  painter.drawLine(0, lastPosY, _width / 2, lastPosY);
  painter.setPen(Qt::cyan);
  int currPosY = static_cast<int>(static_cast<medusa::u64>(_currPos) * _img.height() / _maxPos);
  painter.drawLine(_width / 2, currPosY, _width, currPosY);
  _mutex.unlock();
}

void ScrollbarAddress::resizeEvent(QResizeEvent *evt)
{
  _mutex.lock();
  _img = _img.scaled(evt->size());
  _mutex.unlock();
}

void ScrollbarAddress::mousePressEvent(QMouseEvent * evt)
{
  mouseMoveEvent(evt);
}

void ScrollbarAddress::mouseMoveEvent(QMouseEvent * evt)
{
  if (evt->buttons() & Qt::LeftButton)
  {
    auto pos = static_cast<int>(static_cast<medusa::u64>(evt->y()) * _maxPos / _img.height());
    medusa::Address addr;
    if (!_core.GetDocument().ConvertPositionToAddress(pos, addr))
      return;
    _currPos = pos;
    if (!m_rDoc.GetNearestAddress(addr, addr))
      return;
    emit goTo(addr);
    emit updated();
  }
}

void ScrollbarAddress::setCurrentAddress(medusa::Address const& addr)
{
  if (_core.GetDocument().ConvertAddressToPosition(addr, _currPos))
    emit updated();
}

QColor const& ScrollbarAddress::_CellTypeToColor(medusa::u8 CellType) const
{
  static const QColor s_InsClr(0x01, 0xa9, 0xdb);
  static const QColor s_ValClr(0x8a, 0x08, 0x4b);
  static const QColor s_StrClr(0x0b, 0x61, 0x38);
  static const QColor s_UnkClr(0x00, 0x00, 0x00);
  switch (CellType)
  {
  case medusa::Cell::InstructionType: return s_InsClr;
  case medusa::Cell::ValueType:       return s_ValClr;
  case medusa::Cell::StringType:      return s_StrClr;
  default:                            return s_UnkClr;
  }
}
