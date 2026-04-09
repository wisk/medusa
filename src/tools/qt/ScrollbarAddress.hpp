#ifndef QMEDUSA_SCROLLBAR_ADDRESS_HPP
#define QMEDUSA_SCROLLBAR_ADDRESS_HPP

# include <QtGui>
# include <QWidget>

# include <medusa/address.hpp>
# include <medusa/medusa.hpp>
# include <medusa/view.hpp>

class ScrollbarAddress : public QWidget, public medusa::View
{
  Q_OBJECT

public:
  ScrollbarAddress(QWidget * parent, medusa::Medusa & core);
  virtual ~ScrollbarAddress();

  virtual void OnAddressUpdated(medusa::Address::Vector const& rAddresses);

  void Refresh();

public slots:
  void setCurrentAddress(medusa::Address const& addr);

protected:
  virtual void paintEvent(QPaintEvent* evt);
  virtual void resizeEvent(QResizeEvent* evt);
  virtual void mousePressEvent(QMouseEvent * evt);
  virtual void mouseMoveEvent(QMouseEvent * evt);

signals:
  void goTo(medusa::Address const& addr);
  void updated();

private:
  QColor const& _CellTypeToColor(medusa::Cell::Type CellType) const;

  medusa::Medusa&       _core;
  QPixmap               _img;
  medusa::u32           _lastPos;
  medusa::u32           _currPos;
  medusa::u32           _maxPos;
  QMutex                _mutex;
  static int            _width;
};

#endif // !QMEDUSA_SCROLLBAR_ADDRESS_HPP
