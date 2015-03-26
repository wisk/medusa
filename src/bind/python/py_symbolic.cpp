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

  bp::object Symbolic2_GetBlock(Symbolic2* pSymbolic, Address const& rAddr)
  {
    Expression::VSPType Block;
    if (!pSymbolic->GetBlock(rAddr, Block))
      return bp::object();
    bp::list Res;
    for (auto& rspExpr : Block)
      Res.append(rspExpr);
    return Res;
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

  bp::class_<Symbolic2, boost::noncopyable>("Symbolic2", bp::init<Document&>())
    .def("add_block", &Symbolic2::AddBlock)
    .def("get_block", pydusa::Symbolic2_GetBlock)
    ;
}