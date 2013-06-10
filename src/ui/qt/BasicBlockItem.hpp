#ifndef BasicBlockItem_h__
#define BasicBlockItem_h__

#include <QObject>
#include <QPainter>
#include <QPaintEvent>
#include <QGraphicsItem>
#include <QGraphicsDropShadowEffect>
#include <QStyleOptionGraphicsItem>

#include "DisassemblyPrinter.hpp"

#include <medusa/medusa.hpp>
#include <medusa/view.hpp>

class BasicBlockItem : public QObject, public QGraphicsItem
{
  Q_OBJECT
public:
  BasicBlockItem(QObject * parent, medusa::Medusa& core, medusa::Address::List const& addresses);

  QRectF boundingRect(void) const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
  void paintBackground(QPainter& p);
  void paintText(QPainter& p);

signals:
  void moved(void);

private:
  QObject *                  _parent;
  qreal                      _width, _height;
  bool                       _isPress;
  qreal                      _z;
  QGraphicsDropShadowEffect *_fx;
  medusa::Medusa&            _core;
  DisassemblyPrinter         _printer;
  medusa::View               _view;
  bool                       _needRepaint;
  QPixmap                    _cache;
};

#endif // BasicBlockItem_h__