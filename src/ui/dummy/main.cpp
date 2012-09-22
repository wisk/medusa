#include <ios>
#include <iostream>
#include <iomanip>
#include <string>
#include <exception>
#include <stdexcept>
#include <limits>
#include <boost/foreach.hpp>

#include "boost/graph/graphviz.hpp"

#include <medusa/configuration.hpp>
#include <medusa/address.hpp>
#include <medusa/medusa.hpp>
#include <medusa/database.hpp>
#include <medusa/memory_area.hpp>
#include <medusa/log.hpp>
#include <medusa/event_handler.hpp>

#undef max

MEDUSA_NAMESPACE_USE

std::ostream& operator<<(std::ostream& out, std::pair<u32, std::string> const& p)
{
  out << p.second;
  return out;
}

class DummyEventHandler : public EventHandler
{
public:
  virtual bool OnDatabaseUpdated(void)
  {
    return true;
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

    Medusa m(wfile_path);

    m.GetDatabase().StartsEventHandling(new DummyEventHandler());
    m.LoadModules(wmod_path);

    if (m.GetSupportedLoaders().empty())
    {
      std::cerr << "Not loader available" << std::endl;
      return EXIT_FAILURE;
    }

    std::cout << "Choose a executable format:" << std::endl;
    AskFor<Loader::VectorSharedPtr::value_type, Loader::VectorSharedPtr> AskForLoader;
    Loader::VectorSharedPtr::value_type pLoader = AskForLoader(m.GetSupportedLoaders());
    std::cout << "Interpreting executable format using \"" << pLoader->GetName() << "\"..." << std::endl;
    pLoader->Map();
    std::cout << std::endl;

    std::cout << "Choose an architecture:" << std::endl;
    AskFor<Architecture::VectorSharedPtr::value_type, Architecture::VectorSharedPtr> AskForArch;
    Architecture::VectorSharedPtr::value_type pArch = pLoader->GetMainArchitecture(m.GetAvailableArchitectures());
    if (!pArch)
      pArch = AskForArch(m.GetAvailableArchitectures());

    std::cout << std::endl;

    ConfigurationModel CfgMdl;
    pArch->FillConfigurationModel(CfgMdl);
    pLoader->Configure(CfgMdl.GetConfiguration());

    std::cout << "Configuration:" << std::endl;
    for (ConfigurationModel::ConstIterator It = CfgMdl.Begin(); It != CfgMdl.End(); ++It)
      boost::apply_visitor(AskForConfiguration(CfgMdl.GetConfiguration()), *It);

    pArch->UseConfiguration(CfgMdl.GetConfiguration());

    std::cout << "Disassembling..." << std::endl;
    m.Disassemble(pLoader, pArch);

    for (Database::TConstIterator ma = m.GetDatabase().Begin();
      ma != m.GetDatabase().End(); ++ma)
    {
      u16 Skip = 0;
      for (MemoryArea::TConstIterator cell = (*ma)->Begin();
        cell != (*ma)->End(); ++cell)
      {
        if (cell->second == NULL) continue;

        if (Skip)
        {
          --Skip;
          continue;
        }

        std::string Label = m.GetDatabase().GetLabelFromAddress(cell->first).GetLabel();

        std::string RawByte = "\t";
        TOffset Offset = 0;
        Address::SharedPtr Addr((*ma)->MakeAddress(cell->first));

        if (!Label.empty())
          std::cout
            << "\n"
            << (*ma)->GetName() << ":"
            << Addr->ToString() << ":\t"
            << Label << ":" << std::endl;

        MultiCell* pMc = m.GetDatabase().RetrieveMultiCell(cell->first);
        if (pMc)
        {
          std::cout << pMc->ToString() << std::endl;

          if (!pMc->DisplayCell())
          {
            Skip = pMc->GetSize();
            continue;
          };
        }

        if (cell->second->GetType() == CellInformation::InstructionType)
          for (size_t i = 0; i < 15; ++i)
          {
            if (i < cell->second->GetLength())
            {
              std::ostringstream oss;
              u8 Byte;

              if ((*ma)->Read(cell->first + Offset, &Byte, sizeof(Byte)) == true)
                oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned>(Byte);
              else
                oss << "??";
              RawByte += oss.str();
            }
            else RawByte += "  ";

            RawByte += " ";
            Offset++;
          }

        std::cout
          << (*ma)->GetName() << ":"
          << Addr->ToString() << ":"
          << RawByte
          << " " << cell->second->ToString();

        std::string Comment = cell->second->GetComment();

        if (!Comment.empty())
          std::cout << " ; " << Comment;

        Address::List RefAddrList;
        m.GetDatabase().GetXRefs().From(cell->first, RefAddrList);

        if (Comment.empty() && RefAddrList.size())
          std::cout << " ;";
        for (Address::List::const_iterator It = RefAddrList.begin();
            It != RefAddrList.end(); ++It)
          std::cout << " xref:" << It->ToString();

        std::cout << std::endl;
      }
    }

    std::cout << "To -- From" << std::endl;
    XRefs::TXRefs const& rRefs = m.GetDatabase().GetXRefs().GetAllXRefs();
    for (XRefs::TXRefs::left_const_iterator It = rRefs.left.begin();
        It != rRefs.left.end(); ++It)
    {
      std::cout << It->first << "-->" << It->second << std::endl;
    }

    m.GetDatabase().StopsEventHandling();

    std::cout << "Enter graph filename: " << std::endl;
    std::string GraphFileName;
    std::cin >> GraphFileName;

    auto MultiCells = m.GetDatabase().GetMultiCells();

    {
      ControlFlowGraph Cfg;
      if (m.BuildControlFlowGraph(pLoader->GetEntryPoint(), Cfg))
        Cfg.Dump(std::string("oep_") + GraphFileName, m.GetDatabase());
    }
    for (auto itMultiCell = std::begin(MultiCells); itMultiCell != std::end(MultiCells); ++itMultiCell)
    {
      auto CurLog = Log::Write("core") << "Dumping CFG: " << itMultiCell->first.ToString();
      ControlFlowGraph Cfg;
      if (m.BuildControlFlowGraph(itMultiCell->first, Cfg))
      {
        std::string AddrStr = itMultiCell->first.ToString();
        std::replace(std::begin(AddrStr), std::end(AddrStr), ':', '_');
        Cfg.Dump(AddrStr + std::string("_") + GraphFileName, m.GetDatabase());
        CurLog << " succeed" << LogEnd;
        ControlFlowGraph::PositionMap pm;
        Cfg.Layout(pm);
      }
      else CurLog << " failed" << LogEnd;
    }
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