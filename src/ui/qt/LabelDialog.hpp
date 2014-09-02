#ifndef QMEDUSA_LABEL_DIALOG_HPP
#define QMEDUSA_LABEL_DIALOG_HPP

#include <QDialog>
#include "ui_Label.h"

#include <medusa/medusa.hpp>

class LabelDialog : public QDialog, public Ui::LabelDialog
{
  Q_OBJECT

public:
  LabelDialog(QWidget* pParent, medusa::Medusa& rCore, medusa::Address const& rAddress);
  ~LabelDialog(void);

protected slots:
  void SetLabel(void);

private:
  medusa::Medusa& m_rCore;
  medusa::Address const& m_rAddress;
};

#endif // !QMEDUSA_LABEL_DIALOG_HPP
