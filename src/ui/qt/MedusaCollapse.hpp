#ifndef __MEDUSA_COLLAPSE_H__
# define __MEDUSA_COLLAPSE_H__

# include <QWidget>
# include <QSize>
# include <QMouseEvent>

class MedusaCollapse : public QWidget
{
	Q_OBJECT

public:
	static const int		height = 20;
	static const int		width = 20;

public:
	MedusaCollapse(QWidget * parent, bool collapsed = false);
	~MedusaCollapse();

public:
	QSize					sizeHint() const;

public:
	bool					collapsed() const;
	void					setCollapsed(bool collapsed);

protected:
	void					paintEvent(QPaintEvent * event);
	void					mousePressEvent(QMouseEvent * event);

signals:
	void					collapseChanged(bool collapsed);

private:
	bool					_collapsed;
};

#endif // !__MEDUSA_COLLAPSE_H__
