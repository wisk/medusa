#include "pydusa.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include <medusa/compilation.hpp>

namespace nb = nanobind;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  bool Compiler_CompileToFile(Compiler* pCompiler, std::string const& rOutputFile)
  {
    return pCompiler->Compile(rOutputFile);
  }

  nb::object Compiler_CompileToBuffer(Compiler* pCompiler)
  {
    std::vector<u8> Buffer;
    if (!pCompiler->Compile(Buffer))
      return nb::none();
    return nb::bytes(Buffer.data(), Buffer.size());
  }

  bool Compiler_AddCode(Compiler* pCompiler, std::string const& rCodeName, nb::list const& rCode)
  {
    Expression::VSPType rExprs;
    for (auto spExpr : rCode)
      rExprs.push_back(nb::cast<Expression::SPType>(spExpr));
    return pCompiler->AddCode(rCodeName, rExprs);
  }
}

void PydusaCompiler(nb::module_& rMod)
{
  nb::class_<Compiler>(rMod, "Compiler")
    .def_prop_rw("format", &Compiler::GetFormat, &Compiler::SetFormat)
    .def_prop_rw("entry_point", &Compiler::GetEntryPoint, &Compiler::SetEntryPoint)

    .def("add_code", pydusa::Compiler_AddCode)
    .def("add_data", &Compiler::AddData)

    .def("compile_to_file", pydusa::Compiler_CompileToFile)
    .def("compile_to_buffer", pydusa::Compiler_CompileToBuffer)
    ;
}
