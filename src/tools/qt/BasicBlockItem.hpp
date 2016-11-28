#ifndef QMEDUSA_BASIC_BLOCK_ITEM_HPP
#define QMEDUSA_BASIC_BLOCK_ITEM_HPP

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
  Q_INTERFACES(QGraphicsItem)

public:
  BasicBlockItem(QObject * parent, medusa::Medusa& core, medusa::Address::Vector const& addresses);

  virtual void OnDocumentUpdated(void);

  QRectF boundingRect(void) const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

  void SetBackgroundColor(QColor const& rBgClr);

protected:
  virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
  void paintBackground(QPainter& p);
  void paintText(QPainter& p);

  void _Update(void);

signals:
  void moved(void);

private:
  QObject *                  m_pParent;
  medusa::Address::Vector      m_Addresses;
  qreal                      m_Width, m_Height, m_AddrLen;
  bool                       m_IsSelected;
  qreal                      m_Z;
  QGraphicsDropShadowEffect *m_Fx;
  medusa::Medusa&            m_rCore;
  bool                       m_NeedRepaint;
  QPixmap                    m_Cache;
  QFont                      m_Font;
  QColor                     m_BackgroundColor;
};

#endif // QMEDUSA_BASIC_BLOCK_ITEM_HPP