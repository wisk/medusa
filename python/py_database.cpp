#include "py_database.hpp"

#include <boost/python.hpp>
#include <boost/python/iterator.hpp>

#include "medusa/database.hpp"

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  Database::TIterator (Database::*pDatabase_Begin)(void)                    = &Database::Begin;
  Database::TIterator (Database::*pDatabase_End)  (void)                    = &Database::End;
  bool                (Database::*pDatabase_IsPresent)(Address::SPtr) const = &Database::IsPresent;
}

void PydusaDatabase(void)
{
  bp::class_<Database>("Database",  bp::init<FileBinaryStream const&>())
    .def("MakeAddress",             &Database::MakeAddress)
    .def("__iter__",                bp::range(pydusa::pDatabase_Begin, pydusa::pDatabase_End))
    .def("__contains__",            pydusa::pDatabase_IsPresent)
  ;
}