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
  u16 operator()(Address const& rAddress, u16 xOffset, u16 yOffset);

  virtual u16 GetNumberOfLine(Address const& rAddress) = 0;

protected:
  virtual u16 PrintCell      (Address const& rAddress, u16 xOffset, u16 yOffset) = 0;
  virtual u16 PrintMultiCell (Address const& rAddress, u16 xOffset, u16 yOffset) = 0;
  virtual u16 PrintLabel     (Address const& rAddress, u16 xOffset, u16 yOffset) = 0;
  virtual u16 PrintXref      (Address const& rAddress, u16 xOffset, u16 yOffset) = 0;
  virtual u16 PrintMemoryArea(Address const& rAddress, u16 xOffset, u16 yOffset) = 0;
  virtual u16 PrintEmpty     (Address const& rAddress, u16 xOffset, u16 yOffset) = 0;

  Medusa const& m_rCore;
};

class Medusa_EXPORT StreamPrinter : public Printer
{
public:
  StreamPrinter(Medusa const& rCore, std::ostream& rStream) : Printer(rCore), m_rStream(rStream) {}

  virtual u16 GetNumberOfLine(Address const& rAddress);

protected:
  virtual u16 PrintCell      (Address const& rAddress, u16 xOffset, u16 yOffset);
  virtual u16 PrintMultiCell (Address const& rAddress, u16 xOffset, u16 yOffset);
  virtual u16 PrintLabel     (Address const& rAddress, u16 xOffset, u16 yOffset);
  virtual u16 PrintXref      (Address const& rAddress, u16 xOffset, u16 yOffset);
  virtual u16 PrintMemoryArea(Address const& rAddress, u16 xOffset, u16 yOffset);
  virtual u16 PrintEmpty     (Address const& rAddress, u16 xOffset, u16 yOffset);

  std::ostream& m_rStream;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_PRINTER_HPP__