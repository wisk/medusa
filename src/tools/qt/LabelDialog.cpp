#include "LabelDialog.hpp"

LabelDialog::LabelDialog(QWidget * pParent, medusa::Medusa& rCore, medusa::Address const& rAddress)
  : QDialog(pParent), m_rCore(rCore), m_rAddress(rAddress)
{
  this->setupUi(this);
  connect(ButtonBox, SIGNAL(accepted()), SLOT(SetLabel()));
  connect(ButtonBox, SIGNAL(rejected()), SLOT(close()));

  medusa::Label const& rCurLbl = rCore.GetDocument().GetLabelFromAddress(rAddress);
  LabelName->setText(QString::fromStdString(rCurLbl.GetName()));

  switch (rCurLbl.GetType() & medusa::Label::CellMask)
  {
  default:
  case medusa::Label::Data:     DataButton->setChecked(true);   break;
  case medusa::Label::Code:     CodeButton->setChecked(true);   break;
  case medusa::Label::String:   StringButton->setChecked(true); break;
  case medusa::Label::Function: FunctionButton->setChecked(true); break;
  }

  switch (rCurLbl.GetType() & medusa::Label::AccessMask)
  {
  default:
  case medusa::Label::Local:  LocalButton->setChecked(true);  break;
  case medusa::Label::Global: GlobalButton->setChecked(true); break;
  }
}

LabelDialog::~LabelDialog(void)
{
}

void LabelDialog::SetLabel(void)
{
  medusa::u16 LblType = 0x0;

  if (DataButton->isChecked())
    LblType |= medusa::Label::Data;
  else if (StringButton->isChecked())
    LblType |= medusa::Label::String;
  else if (CodeButton->isChecked())
    LblType |= medusa::Label::Code;
  else if (CodeButton->isChecked())
    LblType |= medusa::Label::Function;

  if (LocalButton->isChecked())
    LblType |= medusa::Label::Local;
  else if (GlobalButton->isChecked())
    LblType |= medusa::Label::Global;

  medusa::Label NewLbl(LabelName->text().toStdString(), LblType);
  m_rCore.GetDocument().SetLabelToAddress(m_rAddress, NewLbl);
  done(QDialog::Accepted);
}