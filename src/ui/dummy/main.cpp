#include <ios>
#include <iostream>
#include <iomanip>
#include <string>
#include <exception>
#include <stdexcept>
#include <limits>

#include <medusa/configuration.hpp>
#include <medusa/address.hpp>
#include <medusa/medusa.hpp>
#include <medusa/database.hpp>
#include <medusa/memory_area.hpp>
#include <medusa/log.hpp>

#undef max

MEDUSA_NAMESPACE_USE

std::ostream& operator<<(std::ostream& out, std::pair<u32, std::string> const& p)
{
  out << p.second;
  return out;
}

void DummyNotify(Address::List const& rModifiedAddresses)
{
  std::cout << "NotifiedAddress:" << std::endl;
  for (Address::List::const_iterator It = rModifiedAddresses.begin();
      It != rModifiedAddresses.end(); ++It)
    std::cout << It->ToString() << std::endl;
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

void dummy_log(wchar_t const* pMsg)
{
  std::wcout << pMsg << std::flush;
}

int main(int argc, char **argv)
{
  std::cout.sync_with_stdio(false);
  std::wcout.sync_with_stdio(false);
  std::string file_path;
  std::string mod_path;
  Log::SetLog(dummy_log);

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
    m.LoadModules(wmod_path);

    if (m.GetSupportedLoaders().empty())
    {
      std::cerr << "Not loader available" << std::endl;
      return EXIT_FAILURE;
    }

    std::cout << "Choose a executable format:" << std::endl;
    AskFor<Loader::VectorPtr::value_type, Loader::VectorPtr> AskForLoader;
    Loader::VectorPtr::value_type pLoader = AskForLoader(m.GetSupportedLoaders());
    std::cout << "Interpreting executable format using \"" << pLoader->GetName() << "\"..." << std::endl;
    pLoader->Map();
    std::cout << std::endl;

    std::cout << "Choose an architecture:" << std::endl;
    AskFor<Architecture::VectorPtr::value_type, Architecture::VectorPtr> AskForArch;
    Architecture::VectorPtr::value_type pArch = pLoader->GetMainArchitecture(m.GetArchitectures());
    if (!pArch)
      pArch = AskForArch(m.GetArchitectures());

    std::cout << std::endl;

    ConfigurationModel CfgMdl;
    pArch->FillConfigurationModel(CfgMdl);
    pLoader->Configure(CfgMdl.GetConfiguration());

    std::cout << "Configuration:" << std::endl;
    for (ConfigurationModel::ConstIterator It = CfgMdl.Begin(); It != CfgMdl.End(); ++It)
      boost::apply_visitor(AskForConfiguration(CfgMdl.GetConfiguration()), *It);

    pArch->UseConfiguration(CfgMdl.GetConfiguration());

    // For testing purpose, it makes ui_dummy a bit too much verbose
    //m.GetDatabase().SetNotifyCallback(DummyNotify);

    std::cout << "Disassembling..." << std::endl;
    m.Disassemble(pLoader, pArch);

    for (Database::TConstIterator ma = m.GetDatabase().Begin();
      ma != m.GetDatabase().End(); ++ma)
    {
      for (MemoryArea::TConstIterator cell = (*ma)->Begin();
        cell != (*ma)->End(); ++cell)
      {
        if (cell->second == NULL) continue;

        if (cell->second->GetType() == Cell::ValueType) continue;

        std::string Label = m.GetDatabase().GetLabelFromAddress(cell->first).GetName();

        std::string RawByte = "\t";
        TOffset Offset = 0;
        TAddressPtr Addr((*ma)->MakeAddress(cell->first));

        if (!Label.empty())
          std::cout
            << "\n"
            << (*ma)->GetName() << ":"
            << Addr->ToString() << ":\t"
            << Label << ":" << std::endl;

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
          << "\t" << cell->second->ToString();

        std::string Comment = cell->second->GetComment();

        if (!Comment.empty())
          std::cout << " ; " << Comment;

        XRefs::TAddressList RefAddrList;
        m.GetDatabase().GetXRefs().From(cell->first, RefAddrList);

        if (Comment.empty() && RefAddrList.size())
          std::cout << " ;";
        for (XRefs::TAddressList::const_iterator It = RefAddrList.begin();
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

    std::cout << "Select the name of the database file:" << std::endl;
    std::string dbName;
    std::cin >> dbName;
    std::cout << std::endl;

    std::cout << "Saving database..." << std::endl;
    Serialize::SPtr s = (*m.GetSerializes().begin());
    s->Open(dbName);
    m.Save(*s);

    std::cout << "Closing database..." << std::endl;
    m.Close();
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
