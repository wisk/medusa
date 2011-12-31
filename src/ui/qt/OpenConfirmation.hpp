#ifndef __OPEN_CONFIRMATION_H__
# define __OPEN_CONFIRMATION_H__

# include <QDialog>
# include "ui_OpenConfirmation.h"

class QWidget;

class OpenConfirmation : public QDialog, public Ui::OpenConfirmation
{
public:
	OpenConfirmation(QWidget * parent = 0);
	~OpenConfirmation();
};

#endif // !__OPEN_CONFIRMATION_H__