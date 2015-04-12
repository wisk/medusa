#include "Goto.hpp"

Goto::Goto(QWidget * parent)
  : QDialog(parent)
  , ui(new Ui::Goto)
{
  ui->setupUi(this);
}

Goto::~Goto()
{
}

medusa::Address Goto::address(void) const
{
  auto addrStr = this->ui->address->text().toStdString();
  return medusa::Address(addrStr);
}