#ifndef QMEDUSA_GOTO_HPP
#define QMEDUSA_GOTO_HPP

#include <QDialog>
#include <QWidget>
#include "ui_Goto.h"
#include <medusa/address.hpp>

namespace Ui
{
  class Goto;
}

class Goto : public QDialog, public Ui::Goto
{
public:
  Goto(QWidget * parent = 0);
  ~Goto();

public:
  Ui::Goto *ui;
  medusa::Address address(void) const;
};

#endif // !QMEDUSA_GOTO_HPP