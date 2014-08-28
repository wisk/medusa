#include <ios>
#include <iostream>
#include <iomanip>
#include <string>
#include <exception>
#include <stdexcept>
#include <limits>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/program_options.hpp>

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

  void Print(void)
  {
    std::cout << m_PrintData.GetTexts() << std::endl;
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
  AskForConfiguration(ConfigurationModel& rCfgMdl) : m_rCfgMdl(rCfgMdl) {}

  ConfigurationModel& m_rCfgMdl;

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
        m_rCfgMdl.SetEnum(rBool.GetName(), !!(Result == "true"));
        return;
      }

      std::istringstream iss(Result);
      if (!(iss >> Choose)) continue;

      if (Choose == 0 || Choose == 1)
      {
        m_rCfgMdl.SetEnum(rBool.GetName(), Choose ? true : false);
        return;
      }
    }
  }

  void operator()(ConfigurationModel::NamedEnum const& rEnum) const
  {
    std::cout << std::dec;
    std::cout << "ENUM TYPE: " << rEnum.GetName() << std::endl;
    for (Configuration::Enum::const_iterator It = rEnum.GetConfigurationValue().begin();
      It != rEnum.GetConfigurationValue().end(); ++It)
    {
      if (It->second == m_rCfgMdl.GetEnum(rEnum.GetName()))
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

      for (Configuration::Enum::const_iterator It = rEnum.GetConfigurationValue().begin();
        It != rEnum.GetConfigurationValue().end(); ++It)
        if (It->second == Choose)
        {
          m_rCfgMdl.SetEnum(rEnum.GetName(), Choose ? true : false);
          return;
        }
    }
  }

  void operator()(ConfigurationModel::NamedString const& rString) const
  {
    std::cout << rString.GetName() << " " << rString.GetValue() << std::endl;
    std::string Result;

    std::getline(std::cin, Result, '\n');
    if (Result.empty())
      return;
    m_rCfgMdl.SetString(rString.GetName(), Result);
  }

  void operator()(ConfigurationModel::NamedPath const& rPath) const
  {
    std::cout << rPath.GetName() << " " << rPath.GetValue() << std::endl;
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
  namespace fs = boost::filesystem;
  fs::path file_path;
  fs::path db_path;
  fs::path mod_path;

  bool auto_cfg = false;

  // TODO: implement database loading...
  namespace po = boost::program_options;
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "produce help message")
    ("exec", po::value<fs::path>(&file_path)->required(), "executable path")
    ("db", po::value<fs::path>(&db_path)->required(), "database path")
    ("auto", "configure module automatically")
    ;
  po::variables_map var_map;

  Log::SetLog(TextLog);

  UserConfiguration usr_cfg;
  std::string mod_path_opt;
  if (!usr_cfg.GetOption("core.modules_path", mod_path_opt))
    mod_path = ".";
  else
    mod_path = mod_path_opt;

  try
  {
    po::store(po::command_line_parser(argc, argv).options(desc).run(), var_map);

    if (var_map.count("help"))
    {
      std::cout << desc << std::endl;
      return EXIT_SUCCESS;
    }

    // notify function must be called AFTER we checked the argument ``help''
    po::notify(var_map);

    if (var_map.count("auto"))
      auto_cfg = true;

    Log::Write("ui_text") << "Analyzing the following file: \"" << file_path.string() << "\"" << LogEnd;
    Log::Write("ui_text") << "Database will be saved to the file: \"" << db_path.string() << "\"" << LogEnd;
    Log::Write("ui_text") << "Using the following path for modules: \"" << mod_path.string() << "\"" << LogEnd;

    Medusa m;
    if (!m.NewDocument(
      file_path,
      [&](boost::filesystem::path& rDatabasePath, std::list<Medusa::Filter> const& rExtensionFilter)
    {
      rDatabasePath = db_path;
      return true;
    },
      [&](
      BinaryStream::SharedPtr spBinStrm,
      Database::SharedPtr& rspDatabase,
      Loader::SharedPtr& rspLoader,
      Architecture::VectorSharedPtr& rspArchitectures,
      OperatingSystem::SharedPtr& rspOperatingSystem
      )
    {
      auto& mod_mgr = ModuleManager::Instance();

      mod_mgr.UnloadModules();
      mod_mgr.LoadModules(mod_path, *spBinStrm);

      auto const& all_ldrs = mod_mgr.GetLoaders();

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
        std::cout << "Choose a executable format:" << std::endl;
        AskFor<Loader::VectorSharedPtr::value_type, Loader::VectorSharedPtr> AskForLoader;
        rspLoader = AskForLoader(mod_mgr.GetLoaders());
      }

      std::cout << "Interpreting executable format using \"" << rspLoader->GetName() << "\"..." << std::endl;
      std::cout << std::endl;

      rspArchitectures = mod_mgr.GetArchitectures();
      rspLoader->FilterAndConfigureArchitectures(rspArchitectures);
      if (rspArchitectures.empty())
        throw std::runtime_error("no architecture module available");

      rspOperatingSystem = mod_mgr.GetOperatingSystem(rspLoader, rspArchitectures.front());

      std::cout << std::endl;

      ConfigurationModel CfgMdl;

      if (!auto_cfg)
      {
        std::cout << "Configuration:" << std::endl;
        for (ConfigurationModel::ConstIterator itCfg = CfgMdl.Begin(), itEnd = CfgMdl.End(); itCfg != CfgMdl.End(); ++itCfg)
          boost::apply_visitor(AskForConfiguration(CfgMdl), itCfg->second);
      }

      auto const& all_dbs = mod_mgr.GetDatabases();

      if (all_dbs.empty())
      {
        Log::Write("ui_text") << "no database module available" << LogEnd;
        return false;
      }

      if (auto_cfg)
      {
        rspDatabase = all_dbs.front();
      }
      else
      {
        AskFor<Database::VectorSharedPtr::value_type, Database::VectorSharedPtr> AskForDb;
        rspDatabase = AskForDb(mod_mgr.GetDatabases());
      }

      return true;
    },
      [](){ std::cout << "Analyzing..." << std::endl; return true; },
      [](){ return true; }))
      throw std::runtime_error("failed to create new document");

    m.WaitForTasks();

    int step = 100;
    TextFullDisassemblyView tfdv(m, FormatDisassembly::ShowAddress | FormatDisassembly::AddSpaceBeforeXref, 80, step, m.GetDocument().GetStartAddress());
    do tfdv.Print();
    while (tfdv.MoveView(0, step));
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  catch (Exception& e)
  {
    std::cerr << e.What() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
