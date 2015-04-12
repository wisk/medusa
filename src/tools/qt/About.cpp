#include "About.hpp"
#include "medusa/medusa.hpp"

About::About(QWidget * parent)
  : QDialog(parent)
{
  this->setupUi(this);
  Version->setText(QString::fromStdString(medusa::Medusa::GetVersion()));
}

About::~About()
{
}