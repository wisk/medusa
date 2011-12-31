#ifndef __INSTRUCTION_PRINTER_H__
# define __INSTRUCTION_PRINTER_H__

# include <QMenu>
# include <QAction>
# include <QLineEdit>
# include <QFont>
# include <QVector>
# include <QList>
# include <QLabel>
# include "IMedusaPrinter.hpp"
# include "MedusaCollection.hpp"

class Instruction;
class MedusaEditor;
class MainWindow;

class InstructionPrinter : public IMedusaPrinter
{
	Q_OBJECT

public:
	static const int			Separator = 20;
	static const int			InstructionHeight = 20;
	static const int			MinCommentWidth = 200;

	static int					MaxInstructionWidth;
	static int					MaxInstructionHeight;

	static MedusaVector<int>	MaxOperandSize;

public:
	InstructionPrinter(QWidget * parent, MedusaEditor * editor);
	~InstructionPrinter();

public:
	QSize					sizeHint() const;

protected:
	void					settingsEvent();

protected:
	void					paintEvent(QPaintEvent * event);
	void					contextMenuEvent(QContextMenuEvent * event);
	void					keyReleaseEvent(QKeyEvent * event);
	void					mouseDoubleClickEvent(QMouseEvent * event);

protected slots:
	void					on_editComment_triggered(bool checked);
	void					on_convertToString_triggered(bool checked);
	void					on_commentInput_returnPressed();

protected:
	void					itemChanged();
	void					focusChanged(bool focus);
	void					updateFontSize();
	void					updateLabels();
	void					updateMenu();

private:
	Instruction *						_instruction;
	QMenu								_menu;
	QAction								_title;
	QAction								_editComment;
	QAction								_convertToString;
	bool								_updateWidget;
	MedusaCollection<QLabel, QWidget>	_labels;

	// Inputs
	QLineEdit				_commentInput;

	// Sizes
	int						_commentWidth;
};

#endif // !__INSTRUCTION_PRINTER_H__