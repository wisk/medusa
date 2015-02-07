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

  void Std_Write(bp::str s)
  {
    std::string Msg = bp::extract<char const*>(s.encode("utf-8"))();
    medusa::Log::Write("pydusa") << Msg;
    if (Msg.find("\n") != std::string::npos)
      medusa::Log::Write("pydusa") << medusa::LogFlush;
  }

  void Std_Flush(void)
  {
    medusa::Log::Write("pydusa") << medusa::LogFlush;
  }
}

MEDUSA_NAMESPACE_USE

void PydusaLogger(void)
{
  bp::def("log_to_stdout", pydusa::SetLogToStdOut);
  bp::def("std_write", pydusa::Std_Write);
  bp::def("std_flush", pydusa::Std_Flush);
}
