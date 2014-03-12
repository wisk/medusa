#include "ConfigureDialog.hpp"
#include <QTreeWidgetItem>
#include <medusa/module.hpp>

ConfigureDialog::ConfigureDialog(QWidget* pParent, medusa::BinaryStream::SharedPtr spBinaryStream)
  : QDialog(pParent)
  , m_spBinaryStream(spBinaryStream)
  , m_spDatabase()
  , m_spLoader()
  , m_spArchitectures()
  , m_spOpratingSystem()
{
  setupUi(this);
  connect(ButtonBox, SIGNAL(rejected()), SLOT(close()));
  ConfigurationTree->setColumnCount(2);
  GetDefaultModules();
  SetupTree();
}

ConfigureDialog::~ConfigureDialog(void)
{
}

void ConfigureDialog::GetDefaultModules(void)
{
  medusa::ModuleManager& rModMgr = medusa::ModuleManager::Instance();

  rModMgr.UnloadModules();
  rModMgr.LoadModules(L".", *m_spBinaryStream);

  // Database
  auto const& rDbs = rModMgr.GetDatabases();
  if (rDbs.empty())
    return;
  m_spDatabase = rDbs.front();

  // Loader
  auto const& rLdrs = rModMgr.GetLoaders();
  if (rLdrs.empty())
    return;
  for (auto itLdr = std::begin(rLdrs), itEnd = std::end(rLdrs); itLdr != itEnd; ++itLdr)
    if ((*itLdr)->IsCompatible(*m_spBinaryStream))
    {
      m_spLoader = *itLdr;
      break;
    }
  if (m_spLoader == nullptr)
    return;

  auto const& rArchs = rModMgr.GetArchitectures();
  if (rArchs.empty())
    return;
  auto spArch = m_spLoader->GetMainArchitecture(rArchs);
  m_spArchitectures.push_back(spArch);

  m_spOpratingSystem = rModMgr.GetOperatingSystem(m_spLoader, spArch);
}

void ConfigureDialog::SetupTree(void)
{
  auto pDocTree = new QTreeWidgetItem(ConfigurationTree);
  pDocTree->setText(0, "document");
  ConfigurationTree->insertTopLevelItem(0, pDocTree);

  if (m_spDatabase)
    AddTreeChild(pDocTree, QString::fromStdString(m_spDatabase->GetName()), "database");
  if (m_spLoader)
    AddTreeChild(pDocTree, QString::fromStdString(m_spLoader->GetName()), "loader");
  for (auto itArch = std::begin(m_spArchitectures), itEnd = std::end(m_spArchitectures); itArch != itEnd; ++itArch)
    AddTreeChild(pDocTree, QString::fromStdString((*itArch)->GetName()), "architecture");
  if (m_spOpratingSystem)
    AddTreeChild(pDocTree, QString::fromStdString(m_spOpratingSystem->GetName()), "operating system");

}

void ConfigureDialog::AddTreeChild(QTreeWidgetItem* pParent, QString const& rName, QString const& rDescription)
{
  auto pTreeItem = new QTreeWidgetItem;
  pTreeItem->setText(0, rName);
  pTreeItem->setText(1, rDescription);
  pParent->addChild(pTreeItem);
}

medusa::Database::SharedPtr ConfigureDialog::GetSelectedDatabase(void) const
{
  return m_spDatabase;
}

medusa::Loader::SharedPtr ConfigureDialog::GetSelectedLoader(void) const
{
  return m_spLoader;
}

medusa::Architecture::VectorSharedPtr ConfigureDialog::GetSelectedArchitectures(void) const
{
  return m_spArchitectures;
}

medusa::OperatingSystem::SharedPtr ConfigureDialog::GetSelectedOperatingSystem(void) const
{
  return m_spOpratingSystem;
}