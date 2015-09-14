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
#include <medusa/execution.hpp>

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

  void operator()(ConfigurationModel::NamedUint8 const& rUint8) const
  {
    std::cout << rUint8.GetName() << " " << static_cast<int>(rUint8.GetValue()) << std::endl;

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
    std::cout << rUint16.GetName() << " " << static_cast<int>(rUint16.GetValue()) << std::endl;

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
    std::cout << rUint32.GetName() << " " << static_cast<int>(rUint32.GetValue()) << std::endl;

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
    std::cout << rUint64.GetName() << " " << static_cast<int>(rUint64.GetValue()) << std::endl;

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
  fs::path script_path;
  fs::path dump_path;
  std::string start_addr = "start";

  bool auto_cfg = false;
  bool analyze = false;
  bool hook_imported_symbols = false;
  std::string emulator_type = "interpreter";

  // TODO: implement database loading...
  namespace po = boost::program_options;
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "produce help message")
    ("exec", po::value<fs::path>(&file_path)->required(), "executable path")
    ("db", po::value<fs::path>(&db_path)->required(), "database path")
    ("script", po::value<fs::path>(&script_path), "script path")
    ("ep", po::value<std::string>(&start_addr), "entrypoint (could be either label or an address)")
    ("dump_path", po::value<fs::path>(&dump_path), "dump path")
    ("auto", "configure module automatically")
    ("analyze", "analyze the executable")
    ("hook", "hook all imported symbols")
    ("emulator", po::value<std::string>(&emulator_type), "set the emulator type")
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
    if (var_map.count("analyze"))
      analyze = true;
    if (var_map.count("hook"))
      hook_imported_symbols = true;

    Log::Write("ui_text") << "Analyzing the following file: \"" << file_path.string() << "\"" << LogEnd;
    Log::Write("ui_text") << "Database will be saved to the file: \"" << db_path.string() << "\"" << LogEnd;
    Log::Write("ui_text") << "Using the following path for modules: \"" << mod_path.string() << "\"" << LogEnd;
    if (!script_path.empty())
      Log::Write("ui_text") << "Using script: \"" << script_path.string() << "\"" << LogEnd;
    Log::Write("ui_text") << "Using the emulator: \"" << emulator_type << "\"" << LogEnd;

    Medusa m;
    if (!m.NewDocument(
      std::make_shared<FileBinaryStream>(file_path), analyze,
      [&](boost::filesystem::path& rDatabasePath, std::list<Medusa::Filter> const& rExtensionFilter)
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
      auto& mod_mgr = ModuleManager::Instance();

      mod_mgr.UnloadModules();
      mod_mgr.LoadModules(mod_path, *rspBinStrm);

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
        AskFor<Loader::VSPType::value_type, Loader::VSPType> AskForLoader;
        rspLoader = AskForLoader(mod_mgr.GetLoaders());
      }

      Log::Write("emulator").Level(LogInfo) << "Interpreting executable format using \"" << rspLoader->GetName() << "\"...\n" << LogEnd;

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
        AskFor<Database::VSPType::value_type, Database::VSPType> AskForDb;
        rspDatabase = AskForDb(mod_mgr.GetDatabases());
      }

      return true;
    },
      [](){ Log::Write("emulator").Level(LogInfo) << "Analyzing..." << LogEnd; return true; },
      [](){ return true; }))
      throw std::runtime_error("failed to create new document");

    m.WaitForTasks();

    auto& rDoc = m.GetDocument();
    auto ep = rDoc.GetStartAddress();

    auto pCell = rDoc.GetCell(ep);
    auto spArch = ModuleManager::Instance().GetArchitecture(pCell->GetArchitectureTag());
    if (spArch == nullptr)
      throw std::runtime_error("unable to find architecture");

    std::vector<std::string> Args;
    std::vector<std::string> Env;
    Args.push_back(file_path.string());

    auto spOs = ModuleManager::Instance().GetOperatingSystem(rDoc.GetOperatingSystemName());

    Execution exec(m.GetDocument());
    if (!exec.Initialize(Args, Env, ""))
    {
      std::cerr << "Unable to initialize emulator" << std::endl;
      return 0;
    }
    if (!exec.SetEmulator(emulator_type))
    {
      std::cerr << "Unable to set the emulator" << std::endl;
      return 0;
    }


    std::ofstream df(dump_path.string());
    auto& dump = (dump_path == "-") ? std::cout : df;

    if (!dump_path.empty())
      exec.HookInstruction([&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& cur_addr)
    {
      static auto pSep("--------------------------------------------------------------------------------\n");
      dump << pSep << pCpuCtxt->ToString() << "\n";
      auto cur_insn = std::dynamic_pointer_cast<Instruction>(m.GetCell(cur_addr));
      if (cur_insn == nullptr)
      {
        dump << "failed to get instruction" << std::endl;
        return;
      }
      auto arch = ModuleManager::Instance().GetArchitecture(cur_insn->GetArchitectureTag());
      if (arch == nullptr)
      {
        dump << "failed to get architecture" << std::endl;
        return;
      }
      PrintData PD;
      PD.PrependAddress(false);
      if (!arch->FormatCell(rDoc, cur_addr, *cur_insn, PD))
      {
        dump << "failed to format instruction" << std::endl;
        return;
      }
      dump << PD.GetTexts() << "\n" << std::flush;
    });

    auto stub_ret = [&](CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const&)
    {
      auto Msg = "[stub] function " + exec.GetHookName() + ", returning ...";
      Log::Write("emulator") << Msg << LogEnd;
      dump << Msg << std::endl;


      // We need to retrieve these registers each time since the current mode could have changed
      auto RegPc = pCpuCtxt->GetCpuInformation().GetRegisterByType(CpuInformation::ProgramPointerRegister, pCpuCtxt->GetMode());
      auto RegSp = pCpuCtxt->GetCpuInformation().GetRegisterByType(CpuInformation::StackPointerRegister, pCpuCtxt->GetMode());
      auto RegSz = pCpuCtxt->GetCpuInformation().GetSizeOfRegisterInBit(RegPc);

      u64 RegSpValue = 0x0, RetAddr = 0x0;
      pCpuCtxt->ReadRegister(RegSp, &RegSpValue, RegSz);
      pMemCtxt->ReadMemory(RegSpValue, &RetAddr, RegSz / 8);
      RegSpValue += (RegSz / 8);
      pCpuCtxt->WriteRegister(RegSp, &RegSpValue, RegSz);
      pCpuCtxt->WriteRegister(RegPc, &RetAddr, RegSz);
    };

    if (hook_imported_symbols) m.GetDocument().ForEachLabel([&](Address const& rAddress, Label const& rLabel)
    {
      if (!(rLabel.GetType() & Label::Imported))
        return;
      if (!(rLabel.GetType() & (Label::Function | Label::Code)))
        return;
      if (!exec.HookFunction(rLabel.GetName(), stub_ret))
        Log::Write("emulator") << "unable to hook function: \"" << rLabel.GetName() << "\"" << LogEnd;
      Log::Write("emulator") << "add stub for function \"" << rLabel.GetName() << "\"" << LogEnd;
    });


    auto ep_addr = m.GetDocument().GetAddressFromLabelName(start_addr);
    if (ep_addr.GetAddressingType() == Address::UnknownType && ep_addr.GetOffset() == 0)
      ep_addr = start_addr;

    Log::Write("emulator") << "using entrypoint: " << ep_addr << LogEnd;

    exec.Execute(ep_addr);

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
