#ifndef __MEDUSA_DISASSEMBLY_PRINTER_HPP__
#define __MEDUSA_DISASSEMBLY_PRINTER_HPP__

#include <medusa/namespace.hpp>
#include <medusa/printer.hpp>
#include <QPainter>
#include <QFontMetrics>

class DisassemblyPrinter : public medusa::Printer
{
public:
  DisassemblyPrinter(medusa::Medusa const& rCore)
    : medusa::Printer(rCore), _p(nullptr) {}
    
  void SetPainter(QPainter* p)                { _p  = p;  }

protected:
  virtual medusa::u32 PrintAddress   (medusa::Address const& rAddress, medusa::u32 xOffset, medusa::u32 yOffset);
  virtual medusa::u32 PrintCell      (medusa::Address const& rAddress, medusa::u32 xOffset, medusa::u32 yOffset);
  virtual medusa::u32 PrintMultiCell (medusa::Address const& rAddress, medusa::u32 xOffset, medusa::u32 yOffset);
  virtual medusa::u32 PrintLabel     (medusa::Address const& rAddress, medusa::u32 xOffset, medusa::u32 yOffset);
  virtual medusa::u32 PrintXref      (medusa::Address const& rAddress, medusa::u32 xOffset, medusa::u32 yOffset);
  virtual medusa::u32 PrintMemoryArea(medusa::Address const& rAddress, medusa::u32 xOffset, medusa::u32 yOffset);
  virtual medusa::u32 PrintEmpty     (medusa::Address const& rAddress, medusa::u32 xOffset, medusa::u32 yOffset);

private:
  void drawText(int x, int y, QString const & text);

  QPainter* _p;
};

#endif // !__MEDUSA_DISASSEMBLY_PRINTER_HPP__