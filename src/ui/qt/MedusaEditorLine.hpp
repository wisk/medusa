#ifndef __MEDUSA_EDITOR_LINE_H__
# define __MEDUSA_EDITOR_LINE_H__

# include <QString>

class MedusaEditorLinePrinter;

class MedusaEditorLine
{
public:
	static MedusaEditorLine *	focusedLine;

public:
	MedusaEditorLine(QString const & address, QString const & text);
	~MedusaEditorLine();

public:
	int							height() const;
	QString const &				address() const;
	QString const &				text() const;
	void						setPrinter(MedusaEditorLinePrinter * printer);
	void						setFocus(bool focus);
	bool						focus() const;

private:
	int							_height;
	MedusaEditorLinePrinter *	_printer;

	// Data
	QString						_address;
	QString						_text;
	bool						_focus;
};

#endif // !__MEDUSA_EDITOR_LINE_H__