#ifndef __CONFIGURE_DIALOG_HPP__
#define __CONFIGURE_DIALOG_HPP__

#include <QDialog>
#include "ui_ConfigureDialog.h"

#include <medusa/medusa.hpp>

class ConfigureDialog : public QDialog, public Ui::ConfigureDialog
{
  Q_OBJECT

public:
  ConfigureDialog(QWidget* pParent);
  ~ConfigureDialog(void);

protected slots:

private:
};

#endif // !__CONFIGURE_DIALOG_HPP__