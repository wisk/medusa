#ifndef __JUMP_ACTION_H__
# define __JUMP_ACTION_H__

# include <QUndoCommand>
# include <QString>
# include <QPair>
# include <medusa/medusa.hpp>
# include "MedusaEditor.hpp"

class JumpAction : public QUndoCommand
{
public:
	JumpAction(QString const & text, int prev, int next, MedusaEditor * editor);
	~JumpAction();

public:
	void		redo();
	void		undo();

private:
	int				_prev;
	int				_next;
	MedusaEditor *	_editor;
};

#endif // !__JUMP_ACTION_H__
