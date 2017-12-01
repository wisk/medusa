#include "pydusa.hpp"

#include <pybind11/pybind11.h>

#include <medusa/compilation.hpp>

namespace py = pybind11;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  bool Compiler_CompileToFile(Compiler* pCompiler, std::string const& rOutputFile)
  {
    return pCompiler->Compile(rOutputFile);
  }

  py::bytes Compiler_CompileToBuffer(Compiler* pCompiler)
  {
    std::vector<u8> Buffer;
    if (!pCompiler->Compile(Buffer))
      return py::none();
    return py::bytes(reinterpret_cast<char const*>(Buffer.data()), Buffer.size());
  }

  bool Compiler_AddCode(Compiler* pCompiler, std::string const& rCodeName, py::list const& rCode)
  {
    Expression::VSPType rExprs;
    for (auto spExpr : rCode)
      rExprs.push_back(py::cast<Expression::SPType>(spExpr));
    return pCompiler->AddCode(rCodeName, rExprs);
  }
}

void PydusaCompiler(py::module& rMod)
{
  py::class_<Compiler, Compiler::SPType>(rMod, "Compiler")
    .def_property("format", &Compiler::GetFormat, &Compiler::SetFormat)
    .def_property("entry_point", &Compiler::GetEntryPoint, &Compiler::SetEntryPoint)

    .def("add_code", pydusa::Compiler_AddCode)
    .def("add_data", &Compiler::AddData)

    .def("compile_to_file", pydusa::Compiler_CompileToFile)
    .def("compile_to_buffer", pydusa::Compiler_CompileToBuffer)
    ;
}
