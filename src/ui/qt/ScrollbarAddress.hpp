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

protected:
  void paintEvent(QPaintEvent* p);
  void resizeEvent(QResizeEvent* r);

private:
  medusa::Medusa& _core;
  QPixmap         _img;
  QMutex          _mutex;
};

#endif // !__SCROLLBAR_ADDRESS_HPP__