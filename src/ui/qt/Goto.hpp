#ifndef __GOTO_H__
# define __GOTO_H__

# include <QDialog>
# include <QWidget>
# include "ui_Goto.h"

class Goto : public QDialog, public Ui::Goto
{
public:
	Goto(QWidget * parent = 0);
	~Goto();

public:
	int		value() const;
	int		type() const;
};

#endif // !__GOTO_H__