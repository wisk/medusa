#ifndef __UNLOADER_H__
# define __UNLOADER_H__

# include <QThread>
# include <medusa/medusa.hpp>
# include "MedusaEditor.hpp"

class QObject;

class UnLoader : public QThread
{

public:
	UnLoader(MedusaEditor * editor, medusa::Medusa & medusa);
	~UnLoader();

protected:
	void			run();

private:
	MedusaEditor *	_editor;
	medusa::Medusa &		_medusa;
};

#endif // !__UNLOADER_H__