#ifndef __BASIC_BLOCK_ITEM_HPP__
#define __BASIC_BLOCK_ITEM_HPP__

#include <QObject>
#include <QPainter>
#include <QPaintEvent>
#include <QGraphicsItem>
#include <QGraphicsDropShadowEffect>
#include <QStyleOptionGraphicsItem>

#include <medusa/medusa.hpp>
#include <medusa/disassembly_view.hpp>

class BasicBlockItem : public QObject, public QGraphicsItem, public medusa::DisassemblyView
{
  Q_OBJECT
public:
  BasicBlockItem(QObject * parent, medusa::Medusa& core, medusa::Address::List const& addresses);

  virtual void OnDocumentUpdated(void);

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
  qreal                      _width, _height, _adLen;
  bool                       _isPress;
  qreal                      _z;
  QGraphicsDropShadowEffect *_fx;
  medusa::Medusa&            _core;
  bool                       _needRepaint;
  QPixmap                    _cache;
};

#endif // __BASIC_BLOCK_ITEM_HPP__