#include <ios>
#include <iostream>
#include <iomanip>
#include <string>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <limits>
#include <filesystem>
#include <CLI/CLI.hpp>

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
#include <medusa/user_configuration.hpp>

MEDUSA_NAMESPACE_USE

class TextFullDisassemblyView : public FullDisassemblyView
{
public:
  TextFullDisassemblyView(Medusa& rCore, u32 FormatFlags, u32 Width, u32 Height, Address const& rAddress)
    : FullDisassemblyView(rCore, FormatFlags, Width, Height, rAddress)
  {}

  void Print()
  {
    std::cout << m_PrintData.GetTexts() << '\n';
  }

};

std::ostream& operator<<(std::ostream& out, std::pair<u32, std::string> const& p)
{
  out << p.second;
  return out;
}

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
        std::cout << Count << " " << (*i)->GetName() << '\n';
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

struct AskForConfiguration
{
  AskForConfiguration(ConfigurationModel& rCfgMdl) : m_rCfgMdl(rCfgMdl) {}

  ConfigurationModel& m_rCfgMdl;

  void operator()(ConfigurationModel::NamedBool const& rBool) const
  {
    std::cout << rBool.GetName() << " " << rBool.GetValue() << '\n';
    std::cout << "true/false" << '\n';

    while (true)
    {
      u32 Choose;
      std::string Result;

      std::getline(std::cin, Result, '\n');

      if (Result.empty()) return;

      if (Result == "false" || Result == "true")
      {
        m_rCfgMdl.SetEnum(rBool.GetName(), !!(Result == "true"));
        return;
      }

      std::istringstream iss(Result);
      if (!(iss >> Choose)) continue;

      if (Choose == 0 || Choose == 1)
      {
        m_rCfgMdl.SetEnum(rBool.GetName(), Choose);
        return;
      }
    }
  }

  void operator()(ConfigurationModel::NamedUint8 const& rUint8) const
  {
    std::cout << rUint8.GetName() << " " << static_cast<int>(rUint8.GetValue()) << '\n';

    while (true)
    {
      u64 Integer;

      std::cin >> Integer;

      if (Integer <= std::pow(2, sizeof(decltype(rUint8.GetValue())) * 8) - 1)
      {
        m_rCfgMdl.SetUint8(rUint8.GetName(), static_cast<u8>(Integer));
        return;
      }
    }
  }

  void operator()(ConfigurationModel::NamedUint16 const& rUint16) const
  {
    std::cout << rUint16.GetName() << " " << static_cast<int>(rUint16.GetValue()) << '\n';

    while (true)
    {
      u64 Integer;

      std::cin >> Integer;

      if (Integer <= std::pow(2, sizeof(decltype(rUint16.GetValue())) * 8) - 1)
      {
        m_rCfgMdl.SetUint16(rUint16.GetName(), static_cast<u16>(Integer));
        return;
      }
    }
  }

  void operator()(ConfigurationModel::NamedUint32 const& rUint32) const
  {
    std::cout << rUint32.GetName() << " " << static_cast<int>(rUint32.GetValue()) << '\n';

    while (true)
    {
      u64 Integer;

      std::cin >> Integer;

      if (Integer <= std::pow(2, sizeof(decltype(rUint32.GetValue())) * 8) - 1)
      {
        m_rCfgMdl.SetUint32(rUint32.GetName(), static_cast<u32>(Integer));
        return;
      }
    }
  }

  void operator()(ConfigurationModel::NamedUint64 const& rUint64) const
  {
    std::cout << rUint64.GetName() << " " << static_cast<int>(rUint64.GetValue()) << '\n';

    while (true)
    {
      u64 Integer;

      std::cin >> Integer;

      if (Integer <= std::pow(2, sizeof(decltype(rUint64.GetValue())) * 8) - 1)
      {
        m_rCfgMdl.SetUint64(rUint64.GetName(), static_cast<u64>(Integer));
        return;
      }
    }
  }

  void operator()(ConfigurationModel::NamedEnum const& rEnum) const
  {
    std::cout << std::dec;
    std::cout << "ENUM TYPE: " << rEnum.GetName() << '\n';
    for (Configuration::Enum::const_iterator It = rEnum.GetConfigurationValue().begin();
      It != rEnum.GetConfigurationValue().end(); ++It)
    {
      if (It->second == m_rCfgMdl.GetEnum(rEnum.GetName()))
        std::cout << "* ";
      else
        std::cout << "  ";
      std::cout << It->first << ": " << It->second << '\n';
    }

    while (true)
    {
      u32 Choose;
      std::string Result;

      std::getline(std::cin, Result, '\n');

      if (Result.empty()) return;

      std::istringstream iss(Result);
      if (!(iss >> Choose)) continue;

      for (Configuration::Enum::const_iterator It = rEnum.GetConfigurationValue().begin();
        It != rEnum.GetConfigurationValue().end(); ++It)
        if (It->second == Choose)
        {
          m_rCfgMdl.SetEnum(rEnum.GetName(), Choose);
          return;
        }
    }
  }

  void operator()(ConfigurationModel::NamedString const& rString) const
  {
    std::cout << rString.GetName() << " " << rString.GetValue() << '\n';
    std::string Result;

    std::getline(std::cin, Result, '\n');
    if (Result.empty())
      return;
    m_rCfgMdl.SetString(rString.GetName(), Result);
  }

  void operator()(ConfigurationModel::NamedPath const& rPath) const
  {
    std::cout << rPath.GetName() << " " << rPath.GetValue() << '\n';
    std::string Result;

    std::getline(std::cin, Result, '\n');
    if (Result.empty())
      return;
    m_rCfgMdl.SetPath(rPath.GetName(), Result);
  }
};

void TextLog(std::string const & rMsg)
{
  std::cout << rMsg << std::flush;
}

int main(int argc, char **argv)
{
  namespace fs = std::filesystem;
  fs::path file_path;
  fs::path db_path;
  fs::path mod_path;

  bool auto_cfg = false;

  CLI::App app{"Medusa text disassembler"};
  std::string file_str, db_str;
  app.add_option("--exec", file_str, "executable path")->required();
  app.add_option("--db", db_str, "database path")->required();
  app.add_flag("--auto", auto_cfg, "configure module automatically");

  CLI11_PARSE(app, argc, argv);

  file_path = file_str;
  db_path = db_str;

  Log::SetLog(TextLog);

  UserConfiguration usr_cfg;
  std::string mod_path_opt;
  if (!usr_cfg.GetOption("core.modules_path", mod_path_opt))
    mod_path = ".";
  else
    mod_path = mod_path_opt;

  try
  {

    Log::Write("ui_text") << "Analyzing the following file: \"" << file_path.string() << "\"" << LogEnd;
    Log::Write("ui_text") << "Database will be saved to the file: \"" << db_path.string() << "\"" << LogEnd;
    Log::Write("ui_text") << "Using the following path for modules: \"" << mod_path.string() << "\"" << LogEnd;

    Medusa m;
    if (!m.NewDocument(
      std::make_shared<FileBinaryStream>(file_path), true,
      [&](std::filesystem::path& rDatabasePath, std::list<Medusa::Filter> const& rExtensionFilter)
    {
      rDatabasePath = db_path;
      return true;
    },
      [&](
      BinaryStream::SPType& rspBinStrm,
      Database::SPType& rspDatabase,
      Loader::SPType& rspLoader,
      Architecture::VSPType& rspArchitectures,
      OperatingSystem::SPType& rspOperatingSystem
      )
    {
      auto const& mod_mgr = ModuleManager::Instance();

      auto all_ldrs = mod_mgr.MakeAllLoaders();
      all_ldrs.erase(std::remove_if(all_ldrs.begin(), all_ldrs.end(),
        [&rspBinStrm](Loader::SPType spLdr) { return !spLdr->IsCompatible(*rspBinStrm); }),
        all_ldrs.end());

      if (all_ldrs.empty())
      {
        Log::Write("ui_text") << "no loader module available" << LogEnd;
        return false;
      }

      if (auto_cfg)
      {
        rspLoader = all_ldrs.front();
      }
      else
      {
        std::cout << "Choose a executable format:" << '\n';
        AskFor<Loader::VSPType::value_type, Loader::VSPType> AskForLoader;
        rspLoader = AskForLoader(all_ldrs);
      }

      Log::Write("ui_text").Level(LogInfo) << "Interpreting executable format using \"" << rspLoader->GetName() << "\"...\n" << LogEnd;

      auto ArchNames = rspLoader->GetUsedArchitectures(*rspBinStrm);
      for (auto const& ArchName : ArchNames)
      {
        TagType ArchTag;
        u8 ArchMode;
        if (!mod_mgr.ConvertArchitectureNameToTagAndMode(ArchName, ArchTag, ArchMode))
        {
          Log::Write("ui_text").Level(LogError) << "unable to find architecture: " << ArchName << LogEnd;
          return false;
        }
        rspArchitectures.push_back(mod_mgr.GetArchitecture(ArchTag));
      }
      if (rspArchitectures.empty())
        throw std::runtime_error("no architecture module available");

      auto const& rSysName = rspLoader->GetSystemName(*rspBinStrm);
      if (!rSysName.empty())
        rspOperatingSystem = mod_mgr.GetOperatingSystem(rSysName);

      std::cout << '\n';

      ConfigurationModel CfgMdl;

      if (!auto_cfg)
      {
        std::cout << "Configuration:" << '\n';
        for (ConfigurationModel::ConstIterator itCfg = CfgMdl.Begin(), itEnd = CfgMdl.End(); itCfg != CfgMdl.End(); ++itCfg)
          std::visit(AskForConfiguration(CfgMdl), itCfg->second);
      }

      rspDatabase = mod_mgr.MakeDatabase("Memory");
      if (rspDatabase == nullptr)
      {
        Log::Write("ui_text") << "no database module available" << LogEnd;
        return false;
      }

      return true;
    }))
      throw std::runtime_error("failed to create new document");

    m.WaitForTasks();

    int step = 100;
    TextFullDisassemblyView tfdv(m, FormatDisassembly::ShowAddress | FormatDisassembly::AddNewLineBeforeCrossReference, 80, step, m.GetDocument().GetStartAddress());
    do tfdv.Print();
    while (tfdv.MoveView(0, step));
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }
  catch (Exception& e)
  {
    std::cerr << e.What() << '\n';
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
