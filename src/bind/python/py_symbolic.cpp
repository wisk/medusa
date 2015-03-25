#include "py_symbolic.hpp"

#include <boost/python.hpp>

#include <medusa/symbolic.hpp>

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  bool Symbolic_Execute(Symbolic* pSymbolic, Address const& rAddr, bp::object Cb)
  {
    return pSymbolic->Execute(rAddr,
      [&](Symbolic::Context const& rCtxt, Address const& rAddr, Address::List& rNextAddrs) -> bool
    {
      return Cb(boost::ref(rCtxt), rAddr, Address::Vector(std::begin(rNextAddrs), std::end(rNextAddrs)));
    });
  }
}

void PydusaSymbolic(void)
{
  bp::class_<Symbolic, boost::noncopyable>("Symbolic", bp::init<Document&>())
    .def("follow_fn", &Symbolic::FollowFunction)
    .def("execute", pydusa::Symbolic_Execute)
    ;

  bp::class_<Symbolic::Context, boost::noncopyable>("SymbolicContext", bp::no_init)
    .def("__str__", &Symbolic::Context::ToString)
    ;
}