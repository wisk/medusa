#include "UnLoader.hpp"
#include "MedusaEditorLine.hpp"

UnLoader::UnLoader(MedusaEditor * editor, medusa::Medusa & medusa)
	: QThread(),
	_editor(editor),
	_medusa(medusa)
{

}

UnLoader::~UnLoader()
{

}

void		UnLoader::run()
{
	while(this->_editor->removeLine());

	this->_editor->clearItems();

	this->_medusa.Close();
}