#include "JumpAction.hpp"

JumpAction::JumpAction(QString const & text, int prev, int next, MedusaEditor * editor)
	: QUndoCommand(text),
	_prev(prev),
	_next(next),
	_editor(editor)
{

}

JumpAction::~JumpAction()
{

}

void		JumpAction::redo()
{
	this->_editor->goTo(this->_next);
}

void		JumpAction::undo()
{
	this->_editor->goTo(this->_prev);
}

