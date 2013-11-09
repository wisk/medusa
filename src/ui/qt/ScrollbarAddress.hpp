#ifndef __SCROLLBAR_ADDRESS_HPP__
#define __SCROLLBAR_ADDRESS_HPP__

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
  virtual ~ScrollbarAddress(void);

  virtual void OnAddressUpdated(medusa::Address::List const& rAddressList);

public slots:
  void setCurrentAddress(medusa::Address const& addr);

protected:
  virtual void paintEvent(QPaintEvent* evt);
  virtual void resizeEvent(QResizeEvent* evt);
  virtual void mouseMoveEvent(QMouseEvent * evt);

private slots:
  void flush(QSize const& size);

signals:
  void goTo(medusa::Address const& addr);
  void flushed(QSize const& size);
  void updated(void);

private:
  medusa::Medusa&       _core;
  QPixmap               _fullImg;
  QPixmap               _img;
  medusa::u32           _lastPos;
  medusa::u32           _currPos;
  QMutex                _mutex;
  static int            _width;
};

#endif // !__SCROLLBAR_ADDRESS_HPP__
