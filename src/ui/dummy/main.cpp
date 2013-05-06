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
#include <medusa/database.hpp>
#include <medusa/memory_area.hpp>
#include <medusa/log.hpp>
#include <medusa/event_handler.hpp>
#include <medusa/screen.hpp>

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

class Tracker
{
public:
  virtual bool Track(Medusa& rCore, Address const& rAddr)
  {
    return false;
  }

  bool operator()(Medusa& rCore, Address const& rAddr)
  {
    return Track(rCore, rAddr);
  }
};

class PrintSemanticTracker : public Tracker
{
public:
  virtual bool Track(Medusa& rCore, Address const& rAddr)
  {
    auto pInsn = dynamic_cast<Instruction const*>(rCore.GetCell(rAddr));
    if (pInsn == nullptr)
      return false;
    if (pInsn->GetOperationType() == Instruction::OpRet)
      return false;
    auto& rSem = pInsn->GetSemantic();
    std::for_each(std::begin(rSem), std::end(rSem), [&rAddr](Expression const* pExpr)
    {
      std::cout << rAddr.ToString() << ": " << pExpr->ToString() << std::endl;
    });
    return true;
  }
};

class PrintMemTracker : public Tracker
{
public:
  virtual bool Track(Medusa& rCore, Address const& rAddr)
  {
    auto pInsn = dynamic_cast<Instruction const*>(rCore.GetCell(rAddr));
    if (pInsn == nullptr)
      return false;
    if (pInsn->GetOperationType() == Instruction::OpRet)
      return false;
    for (u8 i = 0; i < OPERAND_NO; ++i)
      if (pInsn->Operand(i)->GetType() & O_MEM)
      {
        std::cout << rAddr.ToString() << ": " << pInsn->ToString() << std::endl;
        return true;
      }
      return true;
  }
};

class ParameterTracker : public Tracker
{
  u32 m_InsnNo;
public:
  ParameterTracker(void) : m_InsnNo(5) {}
  virtual bool Track(Medusa& rCore, Address const& rAddr)
  {
    if (m_InsnNo == 0)
      return false;
    --m_InsnNo;
    auto pInsn = dynamic_cast<Instruction*>(rCore.GetCell(rAddr));
    if (pInsn == nullptr)
      return false;
    pInsn->SetComment((boost::format("param l.: %d") % m_InsnNo).str());
    rCore.GetDatabase().InsertCell(rAddr, pInsn, true, false);
    std::cout << pInsn->ToString() << std::endl;

    return true;
  }
};

void TrackOperand(Medusa& rCore, Address const& rStartAddress, Tracker& rTracker)
{
  std::map<Address, bool> TrackedAddresses;

  Address::List FuncAddrs;
  rCore.FindFunctionAddressFromAddress(FuncAddrs, rStartAddress);

  if (!FuncAddrs.empty()) std::for_each(std::begin(FuncAddrs), std::end(FuncAddrs), [&rCore, &rTracker, &TrackedAddresses, &rStartAddress](Address const& rFuncAddr)
  {
    auto pFunc = dynamic_cast<Function const*>(rCore.GetMultiCell(rFuncAddr));
    if (pFunc == nullptr)
      return;

    auto rCfg = pFunc->GetControlFlowGraph();
    Address::List AllAddrs;
    AllAddrs.push_back(rStartAddress);

    while (!AllAddrs.empty())
    {
      auto Addr = AllAddrs.front();
      AllAddrs.pop_front();
      if (TrackedAddresses[Addr])
        continue;
      TrackedAddresses[Addr] = true;
      if (rTracker(rCore, Addr) && !rCfg.GetNextAddress(Addr, AllAddrs))
        return;
    }
  });

  else
  {
    Address CurAddr = rStartAddress;
    while (rCore.GetDatabase().MoveAddress(CurAddr, CurAddr, 1))
    {
      if (!rTracker(rCore, CurAddr))
        break;
    }
  }
}

void BacktrackOperand(Medusa& rCore, Address const& rStartAddress, Tracker& rTracker)
{
  std::map<Address, bool> TrackedAddresses;

  Address::List FuncAddrs;
  rCore.FindFunctionAddressFromAddress(FuncAddrs, rStartAddress);

  if (!FuncAddrs.empty()) std::for_each(std::begin(FuncAddrs), std::end(FuncAddrs), [&rCore, &rTracker, &TrackedAddresses, &rStartAddress](Address const& rFuncAddr)
  {
    auto pFunc = dynamic_cast<Function const*>(rCore.GetMultiCell(rFuncAddr));
    if (pFunc == nullptr)
      return;

    auto rCfg = pFunc->GetControlFlowGraph();
    Address::List AllAddrs;
    AllAddrs.push_back(rStartAddress);

    while (!AllAddrs.empty())
    {
      auto Addr = AllAddrs.front();
      AllAddrs.pop_front();
      if (TrackedAddresses[Addr])
        continue;
      TrackedAddresses[Addr] = true;
      if (rTracker(rCore, Addr) == false || rCfg.GetPreviousAddress(Addr, AllAddrs) == false)
        return;
    }
  });

  else
  {
    Address CurAddr = rStartAddress;
    while (rCore.GetDatabase().MoveAddress(CurAddr, CurAddr, -1))
    {
      if (!rTracker(rCore, CurAddr))
        break;
    }
  }
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
    m.RegisterArchitecture(pArch);

    std::cout << "Disassembling..." << std::endl;
    m.Start(pLoader, pArch);

    //StreamPrinter sp(m, std::cout);
    //int step = 100;
    //Screen Scr(m, sp, 80, step, (*m.GetDatabase().Begin())->GetVirtualBase());
    //do Scr.Print();
    //while (Scr.Scroll(0, step));

    auto mcells = m.GetDatabase().GetMultiCells();
    for (auto mc = std::begin(mcells); mc != std::end(mcells); ++mc)
    {
      if (mc->second->GetType() == MultiCell::FunctionType)
      {

        Address::List xref_frm;
        if (m.GetDatabase().GetXRefs().From(mc->first, xref_frm))
        {
          std::for_each(std::begin(xref_frm), std::end(xref_frm), [&m](Address const& ad)
          {
            Address cur_ad;
            if (!m.GetDatabase().GetNearestAddress(ad, cur_ad))
              return;
            ParameterTracker pt;
            BacktrackOperand(m, cur_ad, pt);
            std::cout << std::setfill('#') << std::setw(80) << '#' << std::endl;
          });
        }


        //PrintMemTracker trckr;
        //TrackOperand(m, mc->first, trckr);
        //std::cout << std::endl << std::endl;




        auto func = static_cast<Function const*>(mc->second);
        auto lbl = m.GetDatabase().GetLabelFromAddress(mc->first);
        if (lbl.GetType() == Label::LabelUnknown)
          continue;
        auto const& cfg = func->GetControlFlowGraph();
        m.DumpControlFlowGraph(*func, (boost::format("%s.gv") % lbl.GetLabel()).str());
        //cfg.ForEachBasicBlock([&m, &cfg](BasicBlockVertexProperties const& rBB)
        //{
        //  auto Addrs = rBB.GetAddresses();
        //  Address::List FuncAddr;
        //  m.FindFunctionAddressFromAddress(FuncAddr, rBB.GetFirstAddress());
        //  if (FuncAddr.empty() == false)
        //  {
        //    auto pFunc = m.GetMultiCell(*FuncAddr.begin());
        //    if (pFunc != nullptr)
        //      std::cout << pFunc->ToString() << std::endl;
        //  }
        //  for (auto itAddr = std::begin(Addrs); itAddr != std::end(Addrs); ++itAddr)
        //  {
        //    auto cell = m.GetCell(*itAddr);
        //    if (cell != nullptr)
        //      std::cout << itAddr->ToString() << ": " << cell->ToString() << std::endl;

        //    //Address::List PrevAddrs, NextAddrs;
        //    //if (cfg.GetPreviousAddress(*itAddr, PrevAddrs))
        //    //{
        //    //  std::cout << "prev_addrs: ";
        //    //  std::for_each(std::begin(PrevAddrs), std::end(PrevAddrs), [](Address const& rAddr) { std::cout << rAddr.ToString() << " "; });
        //    //  std::cout << std::endl;
        //    //}
        //    //if (cfg.GetNextAddress(*itAddr, NextAddrs))
        //    //{
        //    //  std::cout << "next_addrs: ";
        //    //  std::for_each(std::begin(NextAddrs), std::end(NextAddrs), [](Address const& rAddr) { std::cout << rAddr.ToString() << " "; });
        //    //  std::cout << std::endl;
        //    //}
        //  }
        //  std::cout << std::endl;
        //});
      }
    }

    StreamPrinter sp(m, std::cout);
    int step = 100;
    Screen Scr(m, sp, 80, step, (*m.GetDatabase().Begin())->GetVirtualBase());
    do Scr.Print();
    while (Scr.Scroll(0, step));
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
