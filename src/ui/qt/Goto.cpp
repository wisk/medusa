#include "Goto.hpp"

Goto::Goto(QWidget * parent)
  : QDialog(parent)
{
  this->setupUi(this);
}

Goto::~Goto()
{
}

int      Goto::value() const
{
  int    tmp = -1;
  bool  ok = false;

  tmp = this->valeur->text().toInt(&ok, 0);

  if (!ok)
    tmp = -1;
  return (tmp);
}

int      Goto::type() const
{
  return (0);
}