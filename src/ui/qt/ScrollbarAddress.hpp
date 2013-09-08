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

protected:
  void paintEvent(QPaintEvent* p);

private:
  medusa::Medusa& _core;
};

#endif // !__SCROLLBAR_ADDRESS_HPP__