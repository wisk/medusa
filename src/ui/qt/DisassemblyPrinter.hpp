#ifndef __MEDUSA_DISASSEMBLY_PRINTER_HPP__
#define __MEDUSA_DISASSEMBLY_PRINTER_HPP__

#include <medusa/namespace.hpp>
#include <medusa/printer.hpp>
#include <QPainter>
#include <QFontMetrics>

MEDUSA_NAMESPACE_USE

class DisassemblyPrinter : public Printer
{
public:
  DisassemblyPrinter(Medusa const& rCore)
    : Printer(rCore), _p(nullptr), _fm(nullptr) {}

  void SetPainter(QPainter* p)                { _p  = p;  }
  void SetFontMetrics(QFontMetrics const* fm) { _fm = fm; }

  virtual u32 GetNumberOfLine(Address const& rAddress);

protected:
  virtual u32 PrintAddress   (Address const& rAddress, u32 xOffset, u32 yOffset);
  virtual u32 PrintCell      (Address const& rAddress, u32 xOffset, u32 yOffset);
  virtual u32 PrintMultiCell (Address const& rAddress, u32 xOffset, u32 yOffset);
  virtual u32 PrintLabel     (Address const& rAddress, u32 xOffset, u32 yOffset);
  virtual u32 PrintXref      (Address const& rAddress, u32 xOffset, u32 yOffset);
  virtual u32 PrintMemoryArea(Address const& rAddress, u32 xOffset, u32 yOffset);
  virtual u32 PrintEmpty     (Address const& rAddress, u32 xOffset, u32 yOffset);

private:
  void drawText(int x, int y, QString const & text);

  QPainter* _p;
  QFontMetrics const* _fm;
};

#endif // !__MEDUSA_DISASSEMBLY_PRINTER_HPP__