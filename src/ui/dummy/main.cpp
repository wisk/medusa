#include <ios>
#include <iostream>
#include <iomanip>
#include <string>
#include <exception>
#include <stdexcept>
#include <limits>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/join.hpp>

#include "boost/graph/graphviz.hpp"

#include <medusa/configuration.hpp>
#include <medusa/address.hpp>
#include <medusa/medusa.hpp>
#include <medusa/document.hpp>
#include <medusa/memory_area.hpp>
#include <medusa/log.hpp>
#include <medusa/event_handler.hpp>
#include <medusa/disassembly_view.hpp>
#include <medusa/view.hpp>
#include <medusa/module.hpp>

MEDUSA_NAMESPACE_USE

  std::ostream& operator<<(std::ostream& out, std::pair<u32, std::string> const& p)
{
  out << p.second;
  return out;
}

class DummyEventHandler : public EventHandler
{
public:
  virtual bool OnDocumentUpdated(void)
  {
    return true;
  }
};

class DummyView : public View
{
public:
  DummyView(Document& rDoc) : View(Document::Subscriber::LabelUpdated | Document::Subscriber::Quit | Document::Subscriber::DocumentUpdated, rDoc) {}
  virtual u32 GetType(void) const
  {
    return Document::Subscriber::LabelUpdated;
  }

  virtual void OnQuit(void) { std::cout << "Quitting!" << std::endl; }
  virtual void OnDocumentUpdated(void) { std::cout << "Document updated!" << std::endl; }
  virtual void OnLabelUpdated(Label const& rLabel, bool Removed)
  {
    std::cout
      << "Label updated: " << rLabel.GetLabel()
      << ", removed? " << (Removed ? "yes" : "no") << std::endl;
  }
};

template<typename Type, typename Container>
class AskFor
{
public:
  Type operator()(Container const& c)
  {
    if (c.empty())
      throw std::runtime_error("Nothing to ask!");

    while (true)
    {
      size_t Count = 0;
      for (typename Container::const_iterator i = c.begin(); i != c.end(); ++i)
      {
        std::cout << Count << " " << (*i)->GetName() << std::endl;
        Count++;
      }
      size_t Input;
      std::cin >> Input;

      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

      if (Input < c.size())
        return c[Input];
    }
  }
};

struct AskForConfiguration : public boost::static_visitor<>
{
  AskForConfiguration(Configuration& rCfg) : m_rCfg(rCfg) {}

  Configuration& m_rCfg;

  void operator()(ConfigurationModel::NamedBool const& rBool) const
  {
    std::cout << rBool.GetName() << " " << rBool.GetValue() << std::endl;
    std::cout << "true/false" << std::endl;

    while (true)
    {
      u32 Choose;
      std::string Result;

      std::getline(std::cin, Result, '\n');

      if (Result.empty()) return;

      if (Result == "false" || Result == "true")
      {
        m_rCfg.Set(rBool.GetName(), !!(Result == "true"));
        return;
      }

      std::istringstream iss(Result);
      if (!(iss >> Choose)) continue;

      if (Choose == 0 || Choose == 1)
      {
        m_rCfg.Set(rBool.GetName(), Choose);
        return;
      }
    }
  }

  void operator()(ConfigurationModel::NamedEnum const& rEnum) const
  {
    std::cout << std::dec;
    std::cout << "ENUM TYPE: " << rEnum.GetName() << std::endl;
    for (ConfigurationModel::Enum::const_iterator It = rEnum.GetValue().begin();
      It != rEnum.GetValue().end(); ++It)
    {
      if (It->second == m_rCfg.Get(rEnum.GetName()))
        std::cout << "* ";
      else
        std::cout << "  ";
      std::cout << It->first << ": " << It->second << std::endl;
    }

    while (true)
    {
      u32 Choose;
      std::string Result;

      std::getline(std::cin, Result, '\n');

      if (Result.empty()) return;

      std::istringstream iss(Result);
      if (!(iss >> Choose)) continue;

      for (ConfigurationModel::Enum::const_iterator It = rEnum.GetValue().begin();
        It != rEnum.GetValue().end(); ++It)
        if (It->second == Choose)
        {
          m_rCfg.Set(rEnum.GetName(), Choose);
          return;
        }
    }
  }
};

std::wstring mbstr2wcstr(std::string const& s)
{
  wchar_t *wcs = new wchar_t[s.length() + 1];
  std::wstring result;

  if (mbstowcs(wcs, s.c_str(), s.length()) == -1)
    throw std::invalid_argument("convertion failed");

  wcs[s.length()] = L'\0';

  result = wcs;

  delete[] wcs;

  return result;
}

void DummyLog(std::wstring const & rMsg)
{
  std::wcout << rMsg << std::flush;
}

class PrintSemanticTracker : public Analyzer::Tracker
{
public:
  virtual bool Track(Analyzer& rAnlz, Document& rDoc, Address const& rAddr)
  {
    auto spInsn = std::dynamic_pointer_cast<Instruction const>(rDoc.GetCell(rAddr));
    if (spInsn == nullptr)
      return false;
    if (spInsn->GetSubType() == Instruction::ReturnType)
      return false;
    auto& rSem = spInsn->GetSemantic();
    std::for_each(std::begin(rSem), std::end(rSem), [&rAddr](Expression const* pExpr)
    {
      std::cout << rAddr.ToString() << ": " << pExpr->ToString() << std::endl;
    });
    return true;
  }
};

class PrintMemTracker : public Analyzer::Tracker
{
public:
  virtual bool Track(Analyzer& rAnlz, Document& rDoc, Address const& rAddr)
  {
    auto spInsn = std::dynamic_pointer_cast<Instruction const>(rDoc.GetCell(rAddr));
    if (spInsn == nullptr)
      return false;
    if (spInsn->GetSubType() == Instruction::ReturnType)
      return false;
    for (u8 i = 0; i < OPERAND_NO; ++i)
      if (spInsn->Operand(i)->GetType() & O_MEM)
      {
        std::string CellStr;
        Cell::Mark::List Marks;
        auto pMemArea = rDoc.GetMemoryArea(rAddr);
        if (pMemArea == nullptr)
          return false;
        if (rAnlz.FormatCell(rDoc, rDoc.GetFileBinaryStream(), rAddr, *spInsn, CellStr, Marks) == false)
          return false;
        std::cout << rAddr.ToString() << ": " << CellStr << std::endl;
        return true;
      }
      return true;
  }
};

class ParameterTracker : public Analyzer::Tracker
{
  u32 m_InsnNo;
public:
  ParameterTracker(void) : m_InsnNo(5) {}
  virtual bool Track(Analyzer& rAnlz, Document& rDoc, Address const& rAddr)
  {
    if (m_InsnNo == 0)
      return false;
    --m_InsnNo;
    auto spInsn = std::dynamic_pointer_cast<Instruction>(rDoc.GetCell(rAddr));
    if (spInsn == nullptr)
      return false;
    spInsn->SetComment((boost::format("param l.: %d") % m_InsnNo).str());
    rDoc.SetCell(rAddr, spInsn, true);
    std::string CellStr;
    Cell::Mark::List Marks;
    auto pMemArea = rDoc.GetMemoryArea(rAddr);
    if (pMemArea == nullptr)
      return false;
    if (rAnlz.FormatCell(rDoc, rDoc.GetFileBinaryStream(), rAddr, *spInsn, CellStr, Marks) == false)
      return false;
    std::cout << CellStr << std::endl;

    return true;
  }
};

int main(int argc, char **argv)
{
  std::cout.sync_with_stdio(false);
  std::wcout.sync_with_stdio(false);
  std::string file_path;
  std::string mod_path;
  Log::SetLog(DummyLog);

  try
  {
    if (argc != 3)
    {
      do
      {
        std::cout << "Please type the file path:" << std::endl;
        std::cin >> file_path;
        std::cout << "Please type the modules path:" << std::endl;
        std::cin >> mod_path;
      }
      while (file_path.empty());
    }
    else
    {
      file_path = argv[1];
      mod_path = argv[2];
    }

    std::wstring wfile_path = mbstr2wcstr(file_path);
    std::wstring wmod_path  = mbstr2wcstr(mod_path );

    std::wcout << L"Analyzing the following file: \""         << wfile_path << "\"" << std::endl;
    std::wcout << L"Using the following path for modules: \"" << wmod_path  << "\"" << std::endl;

    Medusa m;
    m.Open(wfile_path);

    //DummyView dv(m.GetDocument());
    m.LoadModules(wmod_path);

    ModuleManager& mod_mgr = ModuleManager::Instance();

    if (mod_mgr.GetLoaders().empty())
    {
      std::cerr << "No loader available" << std::endl;
      return EXIT_FAILURE;
    }

    std::cout << "Choose a executable format:" << std::endl;
    AskFor<Loader::VectorSharedPtr::value_type, Loader::VectorSharedPtr> AskForLoader;
    Loader::VectorSharedPtr::value_type spLdr = AskForLoader(mod_mgr.GetLoaders());
    std::cout << "Interpreting executable format using \"" << spLdr->GetName() << "\"..." << std::endl;
    spLdr->Map(m.GetDocument());
    std::cout << std::endl;

    std::cout << "Choose an architecture:" << std::endl;
    AskFor<Architecture::VectorSharedPtr::value_type, Architecture::VectorSharedPtr> AskForArch;
    Architecture::VectorSharedPtr::value_type spArch = spLdr->GetMainArchitecture(mod_mgr.GetArchitectures());
    if (!spArch)
      spArch = AskForArch(mod_mgr.GetArchitectures());

    std::cout << std::endl;

    ConfigurationModel CfgMdl;
    spArch->FillConfigurationModel(CfgMdl);
    spLdr->Configure(CfgMdl.GetConfiguration());

    std::cout << "Configuration:" << std::endl;
    for (ConfigurationModel::ConstIterator It = CfgMdl.Begin(); It != CfgMdl.End(); ++It)
      boost::apply_visitor(AskForConfiguration(CfgMdl.GetConfiguration()), *It);

    spArch->UseConfiguration(CfgMdl.GetConfiguration());
    mod_mgr.RegisterArchitecture(spArch);

    auto spOs = mod_mgr.GetOperatingSystem(spLdr, spArch);

    AskFor<Database::VectorSharedPtr::value_type, Database::VectorSharedPtr> AskForDb;
    Database::SharedPtr spDb = AskForDb(mod_mgr.GetDatabases());

    std::cout << "Disassembling..." << std::endl;
    m.Start(spLdr, spArch, spOs, spDb);

    int step = 100;
    FullDisassemblyView fdv(m, new StreamPrinter(m, std::cout), Printer::ShowAddress | Printer::AddSpaceBeforeXref, 80, step, (*m.GetDocument().Begin())->GetBaseAddress());
    do fdv.Print();
    while (fdv.Scroll(0, step));

    auto db = mod_mgr.GetDatabase("Text");
    db->Create(wfile_path + mbstr2wcstr(db->GetExtension()));
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  catch (Exception& e)
  {
    std::wcerr << e.What() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
