#ifndef __Medusa_PRINTER_H__
# define __Medusa_PRINTER_H__

# include <QWidget>
# include <QMouseEvent>
# include <QMutex>
# include <QMutexLocker>

class IMedusaItem;
class QPaintEvent;
class MedusaEditor;

class IMedusaPrinter : public QWidget
{
	Q_OBJECT

	friend class IMedusaItem;

public:
	enum FocusMode
	{
		None,
		LeftClick
	};

public:
	IMedusaPrinter(QWidget * parent, MedusaEditor * editor);
	virtual ~IMedusaPrinter();

public:
	void				setItem(IMedusaItem * item = 0); // Final
	bool				used() const; // Final

public:
	virtual int			getHeight() const;
	virtual int			skipNumber() const;
	virtual void		setY(int y);
	virtual void		release(bool force = false);
	virtual void		awake();

protected slots:
	virtual void		settingsEvent();

protected:
	virtual	void		paintEvent(QPaintEvent * event) = 0;
	void				mouseReleaseEvent(QMouseEvent * event);

private:
	virtual void		itemChanged();
	virtual void		focusChanged(bool focus);

protected:
	IMedusaItem *			_item;
	bool					_used;
	bool					_locked;
	FocusMode				_focusMode;
	mutable QMutex			_mutex;
	MedusaEditor *			_editor;
};

#endif // !__Medusa_PRINTER_H__	
