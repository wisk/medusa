#include "py_database.hpp"

#include <boost/python.hpp>
#include <boost/python/iterator.hpp>

#include "medusa/database.hpp"

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
}

void PydusaDatabase(void)
{
  bp::class_<Database, boost::noncopyable>("Database",  bp::no_init)
  ;
}
