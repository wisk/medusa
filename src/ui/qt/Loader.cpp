#include <QPair>
#include <QDebug>
#include "Loader.hpp"
#include "MedusaEditor.hpp"
#include "Instruction.hpp"
#include "MemoryAreaHeader.hpp"
#include "Function.hpp"
#include "medusa/function.hpp"

Loader::Loader(MedusaEditor * editor, medusa::Medusa & medusa)
	: QThread(),
	_editor(editor),
	_medusa(medusa)
{
}

Loader::~Loader()
{
}

void		Loader::run()
{
	// Content
	int		index = -1;

	medusa::Database & database = this->_medusa.GetDatabase();
	medusa::Database::TMemoryAreas & memoryAreas = database.GetMemoryAreas();
	medusa::Database::TMemoryAreas::const_iterator end = memoryAreas.end();

	// Loading Cell
	for (medusa::Database::TMemoryAreas::const_iterator memoryArea = memoryAreas.begin();
		memoryArea != end; ++memoryArea)
	{
		medusa::MemoryArea::TCellMap::const_iterator end2 = (*memoryArea)->End();

		this->_editor->addItem(new MemoryAreaHeader(++index, this->_editor, QString::fromStdString((*memoryArea)->GetName())));

		for (medusa::MemoryArea::TCellMap::const_iterator cell = (*memoryArea)->Begin();
			cell != end2; ++cell)
		{
			if (cell->second == NULL)
				continue;

			medusa::Address::SPtr address = (*memoryArea)->MakeAddress(cell->first);

			Instruction * instruction;
			Function *	  function;

			if (cell->second->GetType() == medusa::Cell::InstructionType)
			{
				medusa::Function * multiCell = dynamic_cast<medusa::Function*>(database.RetrieveMultiCell(*address));
				if (multiCell != 0)
				{
					medusa::u16 total = multiCell->GetInstructionCounter();
					if (total > 1)
						total -= 1;

					if (total != 0)
					{
						function = new Function(++index, this->_editor, cell->second, *memoryArea, *(address.get()), total);

						function->setComment(QString::fromStdString(cell->second->GetComment()));
						this->_editor->addItem(QPair<medusa::TBase, medusa::TOffset>(address->GetBase(), address->GetOffset()), function);
					}
					else
					{
						instruction = new Instruction(++index, this->_editor, cell->second, *memoryArea, *(address.get()));

						instruction->setComment(QString::fromStdString(cell->second->GetComment()));
						this->_editor->addItem(QPair<medusa::TBase, medusa::TOffset>(address->GetBase(), address->GetOffset()), instruction);
					}
				}
				else
				{
					instruction = new Instruction(++index, this->_editor, cell->second, *memoryArea, *(address.get()));

					instruction->setComment(QString::fromStdString(cell->second->GetComment()));
					this->_editor->addItem(QPair<medusa::TBase, medusa::TOffset>(address->GetBase(), address->GetOffset()), instruction);
				}
			}
			else
			{
				instruction = new Instruction(++index, this->_editor, cell->second, *memoryArea, *(address.get()));

				instruction->setComment(QString::fromStdString(cell->second->GetComment()));
				this->_editor->addItem(QPair<medusa::TBase, medusa::TOffset>(address->GetBase(), address->GetOffset()), instruction);
			}
		}
	}
}
