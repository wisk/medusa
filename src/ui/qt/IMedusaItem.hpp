#ifndef __IMEDUSA_ITEM_H__
# define __IMEDUSA_ITEM_H__

# include <QString>

class IMedusaPrinter;
class MedusaEditor;

class IMedusaItem
{
	friend class IMedusaPrinter;

public:
	IMedusaItem(int itemID, MedusaEditor * editor);
	virtual ~IMedusaItem();

public:
	IMedusaPrinter *			getPrinter(); // Final
	void						setPrinter(IMedusaPrinter * printer = 0); // Final
	void						releasePrinter(); // Final
	bool						hasFocus() const;
	void						setFocus(bool focus, bool keepPrevious = false);
	int							itemID() const;
	bool						hasCollapsedParent() const;
	void						setHasCollapsedParent(bool has);

public:
	virtual QString				toString() const;
	virtual QString				toHtml() const;
	virtual bool				convertDataToString();

private:
	virtual IMedusaPrinter *	retrievePrinter() const = 0;

protected:
	int							_itemID;
	IMedusaPrinter *			_printer;
	bool						_hasFocus;
	MedusaEditor *				_editor;
	bool						_hasCollapsedParent;
};

#endif // !__IMEDUSA_ITEM_H__