#include "BasicBlockItem.hpp"
#include <QObject>
#include <sstream>
#include "Settings.hpp"

BasicBlockItem::BasicBlockItem(QObject * parent, medusa::Medusa& core, medusa::Address::List const& addresses)
  : _parent(parent)
  , _width(0.0), _height(0.0), _adLen(0.0)
  , _isPress(false)
  , _core(core), _printer(core), _disasmView(core, _printer, medusa::Printer::ShowAddress, addresses)
  , _z(zValue()), _fx(new QGraphicsDropShadowEffect(this))
  , _needRepaint(true)
{
  setFlags(ItemIsMovable | ItemIsSelectable);
  _fx->setBlurRadius(25.0);
  setGraphicsEffect(_fx);
  setZValue(10.0);

  QString fontInfo = Settings::instance().value(MEDUSA_FONT_TEXT, MEDUSA_FONT_TEXT_DEFAULT).toString();
  QFont font;
  font.fromString(fontInfo);
  QFontMetrics fm(font);
  medusa::u32 viewWidth, viewHeight;
  _disasmView.GetDimension(viewWidth, viewHeight);
  _width  = viewWidth  * fm.width('M');
  _height = viewHeight * fm.height();
  _adLen  = (addresses.front().ToString().length() + 1) * fm.width('M');
}

QRectF BasicBlockItem::boundingRect(void) const
{
  return QRectF(0, 0, _width, _height);
}

void BasicBlockItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
  if (_needRepaint == true || _width == 0.0 || _height == 0.0)
  {
    _cache = QPixmap(QSize(_width, _height));
    QPainter cachedPainter(&_cache);
    paintBackground(cachedPainter);
    cachedPainter.setRenderHint(QPainter::TextAntialiasing);
    paintText(cachedPainter);
    _needRepaint = false;
  }

  painter->drawPixmap(0, 0, _cache);
}

void BasicBlockItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  _isPress = true;
  setZValue(1.0);
  update();
  QGraphicsItem::mousePressEvent(event);
}

void BasicBlockItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  QGraphicsItem::mouseReleaseEvent(event);
  _isPress = false;
  setZValue(_z);
  update();
}

void BasicBlockItem::paintBackground(QPainter& p)
{
  QRectF bgRct = boundingRect();
  QRectF adRct = boundingRect();
  QColor bgClr = QColor(Settings::instance().value(MEDUSA_COLOR_VIEW_BACKGROUND, MEDUSA_COLOR_VIEW_BACKGROUND_DEFAULT).toString());
  QColor adClr = QColor(Settings::instance().value(MEDUSA_COLOR_ADDRESS_BACKGROUND, MEDUSA_COLOR_ADDRESS_BACKGROUND_DEFAULT).toString());
  qreal opacity = 1.0;

  if (_isPress)
  {
    bgClr = Qt::darkBlue;
    opacity = 0.7;
  }

  bgRct.setX(bgRct.x() + _adLen);
  adRct.setWidth(_adLen);

  QBrush bgBrsh(bgClr);
  QBrush adBrsh(adClr);

  setOpacity(opacity);
  p.fillRect(bgRct, bgBrsh);
  p.fillRect(adRct, adBrsh);
  _fx->setColor(bgClr);
  p.drawRect(bgRct);
  p.drawRect(adRct);
}

void BasicBlockItem::paintText(QPainter& p)
{
  auto fm = p.fontMetrics();
  _printer.SetPainter(&p);
  _disasmView.Print();
  _printer.SetPainter(nullptr);
}