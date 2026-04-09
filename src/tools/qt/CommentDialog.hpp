#ifndef QMEDUSA_COMMENT_DIALOG_HPP
#define QMEDUSA_COMMENT_DIALOG_HPP

#include <QDialog>
#include "ui_Comment.h"

#include <medusa/medusa.hpp>

class CommentDialog : public QDialog, public Ui::CommentDialog
{
  Q_OBJECT

public:
  CommentDialog(QWidget* pParent, medusa::Medusa& rCore, medusa::Address const& rAddress);
  ~CommentDialog();

protected slots:
  void SetComment();

private:
  medusa::Medusa& m_rCore;
  medusa::Address const& m_rAddress;
};

#endif // !QMEDUSA_COMMENT_DIALOG_HPP
