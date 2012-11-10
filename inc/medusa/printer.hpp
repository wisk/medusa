#ifndef __MEDUSA_PRINTER_HPP__
#define __MEDUSA_PRINTER_HPP__

#include "medusa/export.hpp"
#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/view.hpp"
#include "medusa/medusa.hpp"

#include <sstream>

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT Printer
{
public:
  Printer(Medusa const& rCore) : m_rCore(rCore) {}
  void operator()(View::LineInformation const& rLineInfo);

protected:
  virtual void PrintCell      (View::LineInformation const& rLineInfo) = 0;
  virtual void PrintMultiCell (View::LineInformation const& rLineInfo) = 0;
  virtual void PrintLabel     (View::LineInformation const& rLineInfo) = 0;
  virtual void PrintXref      (View::LineInformation const& rLineInfo) = 0;
  virtual void PrintMemoryArea(View::LineInformation const& rLineInfo) = 0;
  virtual void PrintEmpty     (View::LineInformation const& rLineInfo) = 0;

  Medusa const& m_rCore;
};

class Medusa_EXPORT StreamPrinter : public Printer
{
public:
  StreamPrinter(Medusa const& rCore, std::ostringstream& rStream) : Printer(rCore), m_rStream(rStream) {}

protected:
  virtual void PrintCell      (View::LineInformation const& rLineInfo);
  virtual void PrintMultiCell (View::LineInformation const& rLineInfo);
  virtual void PrintLabel     (View::LineInformation const& rLineInfo);
  virtual void PrintXref      (View::LineInformation const& rLineInfo);
  virtual void PrintMemoryArea(View::LineInformation const& rLineInfo);
  virtual void PrintEmpty     (View::LineInformation const& rLineInfo);

  std::ostringstream& m_rStream;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_PRINTER_HPP__