#include "medusa/analyzer.hpp"

#include "medusa/function.hpp"
#include "medusa/character.hpp"
#include "medusa/string.hpp"
#include "medusa/label.hpp"
#include "medusa/log.hpp"
#include "medusa/module.hpp"
#include "medusa/symbolic.hpp"
#include "medusa/expression.hpp"
#include "medusa/expression_visitor.hpp"

#include <list>
#include <stack>

MEDUSA_NAMESPACE_BEGIN

// TODO(wisk): implement switch on string with operator ""

Task* Analyzer::CreateTask(std::string const& rTaskName, Document& rDoc)
{
  if (rTaskName == "disassemble all functions")
  {
    return new AnalyzerTask(rTaskName, rDoc, [](Document& rDoc)
    {
      rDoc.ForEachLabel([&](Address const& rAddr, Label const& rLabel)
      {
        u16 LblType     = rLabel.GetType() & Label::CellMask;
        bool IsExported = ((rLabel.GetType() & Label::AccessMask) == Label::Exported) ? true : false;
        bool IsGlobal   = ((rLabel.GetType() & Label::AccessMask) == Label::Global) ? true : false;

        if (!(LblType == Label::Function || ((LblType == Label::Code) && (IsExported || IsGlobal))))
          return;

        AnalyzerDisassemble AnlzDisasm(rDoc, rAddr);
        AnlzDisasm.Disassemble();
        AnalyzerFunction AnlzFunc(rDoc, rAddr);
        AnlzFunc.CreateFunction();
      });
    });
  }


  // TODO(wisk): use AnalyzerString::DetermineStringType instead
  if (rTaskName == "find all strings")
  {
    return new AnalyzerTask(rTaskName, rDoc, [](Document& rDoc)
    {
      rDoc.ForEachLabel([&](Address const& rAddress, Label const& rLabel)
      {
        if ((rLabel.GetType() & Label::AccessMask) == Label::Imported)
          return;
        if ((rLabel.GetType() & Label::CellMask) != Label::Data)
          return;

        BinaryStream const& rBinStrm = rDoc.GetBinaryStream();
        TOffset StrOff;

        if (!rDoc.ConvertAddressToFileOffset(rAddress, StrOff))
          return;

        /* UTF-16 */
        static Utf16StringTrait Utf16Str;
        Utf16StringTrait::CharType Utf16Char;
        u16 RawLen = 0;

        while (true)
        {
          if (!rBinStrm.Read(StrOff + RawLen, Utf16Char))
          {
            Log::Write("core") << "Unable to read utf-16 string at " << rAddress << LogEnd;
            return;
          }

          if (!Utf16Str.IsValidCharacter(Utf16Char))
            break;

          // FIXME(wisk): variable length character...
          RawLen += sizeof(Utf16Char);
        }

        if (Utf16Str.IsFinalCharacter(Utf16Char) && RawLen != 0x0)
        {
          RawLen += sizeof(Utf16Char);
          auto pStrBuf = new u8[RawLen];
          if (!rBinStrm.Read(StrOff, pStrBuf, RawLen))
          {
            Log::Write("core") << "Unable to read utf-16 string at " << rAddress << LogEnd;
            delete[] pStrBuf;
            return;
          }
          std::string CvtStr = Utf16Str.ConvertToUtf8(pStrBuf, RawLen);
          delete[] pStrBuf;
          if (CvtStr.empty())
          {
            Log::Write("core") << "Unable to convert utf-16 string at " << rAddress << LogEnd;
            return;
          }
          auto spString = std::make_shared<String>(String::Utf16Type, RawLen);
          rDoc.SetCellWithLabel(rAddress, spString, Label(CvtStr, Label::String | Label::Global), true);
          return;
        }

        /* UTF-8 */
        static Utf8StringTrait Utf8Str;
        Utf8StringTrait::CharType Utf8Char;
        RawLen = 0;
        std::string CurStr;

        while (true)
        {
          if (!rBinStrm.Read(StrOff + RawLen, Utf8Char))
          {
            Log::Write("core") << "Unable to read utf-8 string at " << rAddress << LogEnd;
            return;
          }

          if (!Utf8Str.IsValidCharacter(Utf8Char))
            break;

          // FIXME(wisk): variable length character...
          RawLen += sizeof(Utf8Char);
          CurStr += Utf8Char;
        }

        if (Utf8Str.IsFinalCharacter(Utf8Char) && RawLen != 0x0)
        {
          RawLen += sizeof(Utf8Char);
          auto spString = std::make_shared<String>(String::Utf8Type, RawLen);
          rDoc.SetCellWithLabel(rAddress, spString, Label(CurStr, Label::String | Label::Global), true);
        }
      });
    });
  }

  return nullptr;
}

Task* Analyzer::CreateTask(std::string const& rTaskName, Document& rDoc, Address const& rAddr)
{
  if (rTaskName == "disassemble")
  {
    return new AnalyzerTaskAddress(rTaskName, rDoc, rAddr, [](Document& rDoc, Address const& rAddr)
    {
      AnalyzerDisassemble AnlzDisasm(rDoc, rAddr);
      AnlzDisasm.Disassemble();
    });
  }

  if (rTaskName == "symbolic disassemble")
  {
    return new AnalyzerTaskAddress(rTaskName, rDoc, rAddr, [](Document& rDoc, Address const& rAddr)
    {
      AnalyzerDisassemble AnlzDisasm(rDoc, rAddr);
      AnlzDisasm.DisassembleUsingSymbolicExecution();
    });
  }

  if (rTaskName == "create function")
  {
    return new AnalyzerTaskAddress(rTaskName, rDoc, rAddr, [](Document& rDoc, Address const& rAddr)
    {
      AnalyzerFunction AnlzFunc(rDoc, rAddr);
      AnlzFunc.CreateFunction();
    });
  }

  if (rTaskName == "create utf-8 string")
  {
    return new AnalyzerTaskAddress(rTaskName, rDoc, rAddr, [](Document& rDoc, Address const& rAddr)
    {
      AnalyzerString AnlzStr(rDoc, rAddr);
      AnlzStr.CreateUtf8String();
    });
  }

  if (rTaskName == "create utf-16 string")
  {
    return new AnalyzerTaskAddress(rTaskName, rDoc, rAddr, [](Document& rDoc, Address const& rAddr)
    {
      AnalyzerString AnlzStr(rDoc, rAddr);
      AnlzStr.CreateUtf16String();
    });
  }

  return nullptr;
}

Task* Analyzer::CreateTask(std::string const& rTaskName, Document& rDoc, Address const& rAddr, Architecture& rArch, u8 Mode)
{
  if (rTaskName == "disassemble with")
  {
    return new AnalyzerTaskAddress(rTaskName, rDoc, rAddr, [&rArch, Mode](Document& rDoc, Address const& rAddr)
    {
      // LATER(wisk): rArch could be invalided, it'd be wiser to use architecture tag instead
      AnalyzerDisassemble AnlzDisasm(rDoc, rAddr);
      AnlzDisasm.DisassembleWith(rArch, Mode);
    });
  }

  return nullptr;
}

bool Analyzer::BuildControlFlowGraph(Document& rDoc, std::string const& rLblName, ControlFlowGraph& rCfg) const
{
  auto rAddr = rDoc.GetAddressFromLabelName(rLblName);
  return BuildControlFlowGraph(rDoc, rAddr, rCfg);
}

bool Analyzer::BuildControlFlowGraph(Document& rDoc, Address const& rAddr, ControlFlowGraph& rCfg) const
{
  AnalyzerDisassemble AnlzDisasm(rDoc, rAddr);
  return AnlzDisasm.BuildControlFlowGraph(rCfg);
}

bool Analyzer::FormatCell(Document const& rDoc, Address const& rAddress, Cell const& rCell, PrintData &rPrintData) const
{
  auto spArch = ModuleManager::Instance().GetArchitecture(rCell.GetArchitectureTag());
  if (spArch == nullptr)
    return false;
  return spArch->FormatCell(rDoc, rAddress, rCell, rPrintData);
}

bool Analyzer::FormatMultiCell(Document const& rDoc, Address const& rAddress, MultiCell const& rMultiCell, PrintData& rPrintData) const
{
  auto spCell = rDoc.GetCell(rAddress);
  if (spCell == nullptr)
    return false;
  auto spArch = ModuleManager::Instance().GetArchitecture(spCell->GetArchitectureTag());
  if (spArch == nullptr)
    return false;
  return spArch->FormatMultiCell(rDoc, rAddress, rMultiCell, rPrintData);
}

MEDUSA_NAMESPACE_END
