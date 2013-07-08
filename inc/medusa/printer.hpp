#ifndef __MEDUSA_PRINTER_HPP__
#define __MEDUSA_PRINTER_HPP__

#include "medusa/export.hpp"
#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/medusa.hpp"

#include <sstream>

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT Printer
{
public:

  enum Flags
  {
    ShowAddress        = 1 << 0,
    AddSpaceBeforeXref = 1 << 1
  };

  Printer(Medusa const& rCore) : m_rCore(rCore) {}
  u32 operator()(Address const& rAddress, u32 xOffset, u32 yOffset, u32 Flags);

  virtual u16 GetLineHeight(Address const& rAddress, u32 Flags) const;
  virtual u16 GetLineWidth (Address const& rAddress, u32 Flags) const;

protected:
  virtual u32 PrintAddress   (Address const& rAddress, u32 xOffset, u32 yOffset) { return 0; }
  virtual u32 PrintCell      (Address const& rAddress, u32 xOffset, u32 yOffset) { return 0; }
  virtual u32 PrintMultiCell (Address const& rAddress, u32 xOffset, u32 yOffset) { return 0; }
  virtual u32 PrintLabel     (Address const& rAddress, u32 xOffset, u32 yOffset) { return 0; }
  virtual u32 PrintXref      (Address const& rAddress, u32 xOffset, u32 yOffset) { return 0; }
  virtual u32 PrintMemoryArea(Address const& rAddress, u32 xOffset, u32 yOffset) { return 0; }
  virtual u32 PrintEmpty     (Address const& rAddress, u32 xOffset, u32 yOffset) { return 0; }

  Medusa const& m_rCore;
};

class Medusa_EXPORT StreamPrinter : public Printer
{
public:
  StreamPrinter(Medusa const& rCore, std::ostream& rStream) : Printer(rCore), m_rStream(rStream) {}

protected:
  virtual u32 PrintAddress   (Address const& rAddress, u32 xOffset, u32 yOffset);
  virtual u32 PrintCell      (Address const& rAddress, u32 xOffset, u32 yOffset);
  virtual u32 PrintMultiCell (Address const& rAddress, u32 xOffset, u32 yOffset);
  virtual u32 PrintLabel     (Address const& rAddress, u32 xOffset, u32 yOffset);
  virtual u32 PrintXref      (Address const& rAddress, u32 xOffset, u32 yOffset);
  virtual u32 PrintMemoryArea(Address const& rAddress, u32 xOffset, u32 yOffset);
  virtual u32 PrintEmpty     (Address const& rAddress, u32 xOffset, u32 yOffset);

  std::ostream& m_rStream;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_PRINTER_HPP__