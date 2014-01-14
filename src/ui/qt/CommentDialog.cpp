#include "CommentDialog.hpp"

CommentDialog::CommentDialog(QWidget* pParent, medusa::Medusa& rCore, medusa::Address const& rAddress)
  : QDialog(pParent), m_rCore(rCore), m_rAddress(rAddress)
{
  this->setupUi(this);
  connect(ButtonBox, SIGNAL(accepted()), SLOT(SetComment()));
  connect(ButtonBox, SIGNAL(rejected()), SLOT(close()));

  std::string Cmt;
  if (rCore.GetDocument().GetComment(rAddress, Cmt))
    CommentEdit->setPlainText(QString::fromStdString(Cmt));
}

CommentDialog::~CommentDialog(void)
{
}

void CommentDialog::SetComment(void)
{
  // TODO Notify the user in case of error.
  m_rCore.GetDocument().SetComment(m_rAddress, CommentEdit->toPlainText().toStdString());
  close();
}