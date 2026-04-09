#include "pydusa.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include <medusa/log.hpp>

namespace nb = nanobind;

namespace pydusa
{
  void PythonLogger(std::string const& rMsg)
  {
    std::cout << rMsg << std::flush;
  }

  void SetLogToStdOut()
  {
    medusa::Log::SetLog(PythonLogger);
  }

  void Std_Write(std::string const& rMsg)
  {
    medusa::Log::Write("pydusa") << rMsg;
    if (rMsg.find("\n") != std::string::npos)
      medusa::Log::Write("pydusa") << medusa::LogFlush;
  }

  void Std_Flush()
  {
    medusa::Log::Write("pydusa") << medusa::LogFlush;
  }
}

MEDUSA_NAMESPACE_USE

void PydusaLogger(nb::module_& rMod)
{
  rMod.def("log_to_stdout", pydusa::SetLogToStdOut);
  rMod.def("std_write", pydusa::Std_Write);
  rMod.def("std_flush", pydusa::Std_Flush);
}
