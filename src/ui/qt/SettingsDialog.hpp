#ifndef __SETTINGS_DIALOG_H__
# define __SETTINGS_DIALOG_H__

# include <QDialog>
# include "ui_SettingsDialog.h"

class QWidget;
class QShowEvent;
class QCloseEvent;

class SettingsDialog : public QDialog, public Ui::SettingsDialog
{
	Q_OBJECT

public:
	SettingsDialog(QWidget * parent = 0);
	~SettingsDialog();

protected slots:
	void		saveSettings();

protected:
	void		showEvent(QShowEvent * e);
};

#endif // !__SETTINGS_DIALOG_H__