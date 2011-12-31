#include "py_exception.hpp"

#include <boost/python.hpp>

#include "medusa/exception.hpp"

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  /*
   * XXX:
   * At this time, pydusa throws 'RuntimeError' exception,
   * it could be better to throw our custom exceptions
   * i.e. MedusaException*
   */
  static void MedusaException_Translate(Exception const& e)
  {
    std::wstring const& WideExcpMsg = e.What();
    std::string ExcpMsg(WideExcpMsg.begin(), WideExcpMsg.end());
    PyErr_SetString(PyExc_RuntimeError, ExcpMsg.c_str());
  }
}

void PydusaException(void)
{
  /* Exception */
  bp::register_exception_translator<Exception>(&pydusa::MedusaException_Translate);

  /* XXX: At this time, these classes cannot be used straight in python

  bp::class_<MedusaException>("MedusaException", bp::init<std::wstring>())
    .def("What", &MedusaException::What,
        bp::return_value_policy<bp::reference_existing_object>())
    ;

  bp::class_<MedusaException_System>("MedusaException_System", bp::init<std::wstring>())
    .def("What", &MedusaException_System::What,
        bp::return_value_policy<bp::reference_existing_object>())
    ;

  bp::class_<MedusaException_NotImplemented>("MedusaException_NotImplemented", bp::init<std::wstring>())
    .def("What", &MedusaException_NotImplemented::What,
        bp::return_value_policy<bp::reference_existing_object>())
    ;
  */
}
