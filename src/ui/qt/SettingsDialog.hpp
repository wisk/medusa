#ifndef __SETTINGS_DIALOG_H__
# define __SETTINGS_DIALOG_H__

# include <QDialog>
# include "ui_SettingsDialog.h"
# include "medusa/medusa.hpp"

class QWidget;
class QShowEvent;
class QCloseEvent;

class SettingsDialog : public QDialog, public Ui::SettingsDialog
{
  Q_OBJECT

public:
  SettingsDialog(QWidget* pParent, medusa::Medusa& rCore);
  ~SettingsDialog();

protected slots:
  void    SaveSettings(void);

private:
  medusa::Medusa& m_rCore;
};

#endif // !__SETTINGS_DIALOG_H__