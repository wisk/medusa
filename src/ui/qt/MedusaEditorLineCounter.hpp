#ifndef __MEDUSA_EDITOR_LINE_COUNTER_H__
# define __MEDUSA_EDITOR_LINE_COUNTER_H__

# include <QWidget>
# include <QSize>
# include <QPaintEvent>

class MedusaEditor;

class MedusaEditorLineCounter : public QWidget
{
public:
	MedusaEditorLineCounter(MedusaEditor * parent = 0);
	~MedusaEditorLineCounter();

public:
	QSize			sizeHint() const;
	int				width() const;

protected:
	void			paintEvent(QPaintEvent * event);

private:
	MedusaEditor *	_editor;
};

#endif // !__MEDUSA_EDITOR_LINE_COUNTER_H__