#ifndef __MEDUSA_COLLECTION_H__
# define __MEDUSA_COLLECTION_H__
#include<qdebug.h>
# include <QVector>

template <class T, class P>
class MedusaCollection : public QVector<T *>
{
public:
	MedusaCollection(P * parent)
		: QVector<T *>(),
		_parent(0),
		_index(-1)
	{
		this->_parent = parent;
	}


public:
	T *		getNextItem()
	{
		++this->_index;
		if (this->_index + 1 > this->size())
			this->append(new T(this->_parent));
		return (this->at(this->_index));
	}
	void	reset()
	{
		this->_index = -1;
	}

private:
	P *					_parent;
	int					_index;
};

template <class T>
class MedusaVector : public QVector<T>
{
public:
	T &		operator[](int i)
	{
		Q_ASSERT_X(i >= 0 && i <= this->size(), "MedusaVector<T>::operator[]", "index out of range");

		if (i == this->size())
			this->append(T());
		return (QVector<T>::operator[](i));
	}
};

template <>
class MedusaVector<int> : public QVector<int>
{
public:
	int &		operator[](int i)
	{
		Q_ASSERT_X(i >= 0 && i <= this->size(), "MedusaVector<T>::operator[]", "index out of range");

		if (i == this->size())
			this->append(0);
		return (QVector::operator[](i));
	}
};

#endif // !__MEDUSA_COLLECTION_H__
