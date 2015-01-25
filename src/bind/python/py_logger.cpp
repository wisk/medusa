#include "py_logger.hpp"

#include <boost/python.hpp>

#include <medusa/log.hpp>

namespace bp = boost::python;

namespace pydusa
{
  void PythonLogger(std::string const& rMsg)
  {
    std::cout << rMsg << std::flush;
  }

  void SetLogToStdOut(void)
  {
    medusa::Log::SetLog(PythonLogger);
  }
}

MEDUSA_NAMESPACE_USE

void PydusaLogger(void)
{
  bp::def("log_to_stdout", pydusa::SetLogToStdOut);
}
