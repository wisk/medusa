#ifndef __COMMENT_DIALOG_HPP__
#define __COMMENT_DIALOG_HPP__

#include <QDialog>
#include "ui_Comment.h"

#include <medusa/medusa.hpp>

class CommentDialog : public QDialog, public Ui::CommentDialog
{
  Q_OBJECT

public:
    CommentDialog(QWidget* pParent, medusa::Medusa& rCore, medusa::Address const& rAddress);
    ~CommentDialog(void);

protected slots:
  void SetComment(void);

private:
  medusa::Medusa& m_rCore;
  medusa::Address const& m_rAddress;
};

#endif // !__COMMENT_DIALOG_HPP__
