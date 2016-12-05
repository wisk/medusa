#include "pydusa.hpp"

#include <pybind11/pybind11.h>

#include <medusa/compilation.hpp>

namespace py = pybind11;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  bool Compiler_CompileToFile(Compiler* pCompiler, std::string const& rFormatName, std::string const& rOutputFile)
  {
    return pCompiler->Compile(rFormatName, rOutputFile);
  }

  py::bytes Compiler_CompileToBuffer(Compiler* pCompiler, std::string const& rFormatName)
  {
    std::vector<u8> Buffer;
    if (!pCompiler->Compile(rFormatName, Buffer))
      return py::none();
    return py::bytes(reinterpret_cast<char const*>(Buffer.data()), Buffer.size());
  }
}

void PydusaCompiler(py::module& rMod)
{
  py::class_<Compiler, Compiler::SPType>(rMod, "Compiler")
    .def("compile_to_file", pydusa::Compiler_CompileToFile)
    .def("compile_to_buffer", pydusa::Compiler_CompileToBuffer)
    ;
}
