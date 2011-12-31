#ifndef __LOADER_H__
# define __LOADER_H__

# include <QThread>
# include <medusa/medusa.hpp>

class QObject;
class MedusaEditor;

class Loader : public QThread
{

public:
	Loader(MedusaEditor * editor, medusa::Medusa & medusa);
	~Loader();

protected:
	void			run();

private:
	MedusaEditor *	_editor;
	medusa::Medusa &		_medusa;
};

#endif // !__LOADER_H__