#ifndef __MEDUSA_EDITOR_H__
# define __MEDUSA_EDITOR_H__

# include <QAbstractScrollArea>
# include <QResizeEvent>
# include <QPaintEvent>
# include <QList>
# include <QMutex>
# include <QString>
# include <QMutexLocker>
# include <QHash>
# include <QPair>
# include <QUndoStack>
# include <QKeyEvent>
# include <medusa/medusa.hpp>
# include "MedusaEditorLineCounter.hpp"

class IMedusaItem;

/**
 * @brief Main Class of the Medusa Editor
 *
 * The MedusaEditor class is the base of the Medusa Editor.
 * It contain the list of all the item to display, handle the scroll
 * and the painting of the main area of the editor.
 */
class MedusaEditor : public QAbstractScrollArea
{
	Q_OBJECT

public:
	typedef QPair<medusa::TBase, medusa::TOffset>	Address;

public:
	MedusaEditor(QWidget * parent = 0);
	~MedusaEditor();

public:
	void								goTo(MedusaEditor::Address const & address, QString description = "");
	void								goTo(int index);
	void								addFocusedItem(IMedusaItem * item, bool keepPrevious = false);
	void								delFocusedItem(IMedusaItem * item);
	void								setCollapsedItem(int startIndex, int count, bool collapsed);
	void								releasePrinter();

	// Thread safe	functions
	bool								removeLine();
	void								clearItems();
	void								setWhiteScreen(bool white);
	void								addItem(IMedusaItem * item);
	void								addItem(MedusaEditor::Address const & address, IMedusaItem * item);
	int									itemCount() const;
	IMedusaItem * const					itemAt(int index) const;
	QUndoStack  &						undoJumpStack();
	void								convertDataToString(int startIndex);

public slots:
	void								on_SettingsChanged();

protected:
	void								resizeEvent(QResizeEvent * event);
	void								paintEvent(QPaintEvent * event);
	void								scrollContentsBy(int dx, int dy);
	void								keyPressEvent(QKeyEvent * event);

private:
	void								updateScroll();

private:
	MedusaEditorLineCounter				_lineCounter;

	// Data
	QList<IMedusaItem *>				_items;
	QHash<MedusaEditor::Address, IMedusaItem *>		_addressedItems;
	QMap<int, IMedusaItem *>			_focusedItems;

	//Other
	mutable QMutex						_mutex;
	bool								_updateScroll;
	bool								_whiteScreen;
	int									_prevStartIndex;
	int									_prevEndIndex;
	QUndoStack							_undoJump;
	bool								_gotoScroll;
};

#endif // !__MEDUSA_EDITOR_H__