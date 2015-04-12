#ifndef QMEDUSA_ABOUT_HPP
#define QMEDUSA_ABOUT_HPP

# include <QDialog>
# include "ui_About.h"

class QWidget;

class About: public QDialog, public Ui::About
{
public:
  About(QWidget * parent = 0);
  ~About();
};

#endif // !QMEDUSA_ABOUT_HPP