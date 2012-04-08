#ifndef __ABOUT_H__
# define __ABOUT_H__

# include <QDialog>
# include "ui_About.h"

class QWidget;

class About: public QDialog, public Ui::About
{
public:
  About(QWidget * parent = 0);
  ~About();
};

#endif // !__ABOUT_H__