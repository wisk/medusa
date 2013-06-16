#include "BasicBlockItem.hpp"
#include <QObject>
#include <sstream>
#include "Settings.hpp"

BasicBlockItem::BasicBlockItem(QObject * parent, medusa::Medusa& core, medusa::Address::List const& addresses)
  : _parent(parent)
  , _width(0.0), _height(0.0)
  , _isPress(false)
  , _core(core), _printer(core), _view(core, _printer, addresses)
  , _z(zValue()), _fx(new QGraphicsDropShadowEffect(this))
  , _needRepaint(true)
{
  setFlags(ItemIsMovable | ItemIsSelectable);
  //_fx->setBlurRadius(25.0);
  //setGraphicsEffect(_fx);
}

QRectF BasicBlockItem::boundingRect(void) const
{
  return QRectF(0, 0, _width, _height);
}

void BasicBlockItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
  if (_needRepaint == true || _width == 0.0 || _height == 0.0)
  {
    auto fm = painter->fontMetrics();
    u32 viewWidth, viewHeight;
    _view.GetDimension(viewWidth, viewHeight);
    _width = viewWidth * fm.width('M');
    _height = viewHeight * fm.height();

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
  QRectF rect = boundingRect();
  QColor clr = QColor(Settings::instance().value(MEDUSA_COLOR_VIEW_BACKGROUND, MEDUSA_COLOR_VIEW_BACKGROUND_DEFAULT).toString());
  qreal opacity = 1.0;

  if (_isPress)
  {
    clr = Qt::darkBlue;
    opacity = 0.7;
  }

  QBrush brush(clr);

  setOpacity(opacity);
  p.fillRect(rect, brush);
  _fx->setColor(clr);
  p.drawRect(rect);
}

void BasicBlockItem::paintText(QPainter& p)
{
  auto fm = p.fontMetrics();
  _printer.SetFontMetrics(&fm);
  _printer.SetPainter(&p);
  _view.Print();
  _printer.SetPainter(nullptr);
  _printer.SetFontMetrics(nullptr);
}