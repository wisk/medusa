#include "ConfigureDialog.hpp"
#include <QTreeWidgetItem>
#include <medusa/module.hpp>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QToolButton>
#include <QFileDialog>

ConfigureDialog::ConfigureDialog(QWidget* pParent, medusa::BinaryStream::SharedPtr spBinaryStream)
  : QDialog(pParent)
  , m_spBinaryStream(spBinaryStream)
  , m_spDatabase()
  , m_spLoader()
  , m_spArchitectures()
  , m_spOpratingSystem()
  , m_ModulePath(".")
{
  setupUi(this);

  connect(ButtonBox, SIGNAL(rejected()), SLOT(close()));
  connect(ConfigurationTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), SLOT(OnItemClicked(QTreeWidgetItem*, int)));

  ConfigurationTree->setColumnCount(2);
  ConfigurationLayout->setAlignment(Qt::AlignTop);

  _GetDefaultModules();
  _CreateTree();
  _DisplayDocumentOptions();
}

ConfigureDialog::~ConfigureDialog(void)
{
  _DestroyTree();
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

void ConfigureDialog::OnItemClicked(QTreeWidgetItem* pItem, int Column)
{
  _ClearOptions();
  auto ItemText = pItem->text(1);

  // TODO: Find a better way to achieve this
  if (ItemText.isEmpty())
    _DisplayDocumentOptions();
  else if (ItemText == "architecture")
  {
    for (auto itArch = std::begin(m_spArchitectures), itEnd = std::end(m_spArchitectures); itArch != itEnd; ++itArch)
    {
      if ((*itArch)->GetName() == pItem->text(0).toStdString())
      {
        _DisplayConfigurationOptions((*itArch)->GetConfigurationModel());
        break;
      }
    }
  }

  ConfigurationLayout->setAlignment(Qt::AlignTop);
  if (ConfigurationLayout->count() == 0)
  {
    ConfigurationLayout->setAlignment(Qt::AlignCenter);
    ConfigurationLayout->addWidget(new QLabel(QString("No options are available for %2/%1").arg(pItem->text(0), pItem->text(1))));
  }
}

void ConfigureDialog::_GetModulesByLoader(void)
{
  if (m_spLoader == nullptr)
    return;

  medusa::ModuleManager& rModMgr = medusa::ModuleManager::Instance();
  auto const& rArchs = rModMgr.GetArchitectures();
  if (rArchs.empty())
    return;
  auto spArchs = rModMgr.GetArchitectures();
  m_spArchitectures = rModMgr.GetArchitectures();
  m_spLoader->FilterAndConfigureArchitectures(m_spArchitectures);

  m_spOpratingSystem = rModMgr.GetOperatingSystem(m_spLoader, m_spArchitectures.front());
}


void ConfigureDialog::_GetDefaultModules(void)
{
  medusa::ModuleManager& rModMgr = medusa::ModuleManager::Instance();

  rModMgr.UnloadModules();
  rModMgr.LoadModules(m_ModulePath.toStdWString(), *m_spBinaryStream);

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
  {
    if ((*itLdr)->IsCompatible(*m_spBinaryStream))
    {
      m_spLoader = *itLdr;
      break;
    }
  }

  _GetModulesByLoader();
}

void ConfigureDialog::_CreateTree(void)
{
  auto pDocTree = new QTreeWidgetItem(ConfigurationTree);
  pDocTree->setText(0, "Document");
  ConfigurationTree->insertTopLevelItem(0, pDocTree);

  if (m_spDatabase)
    _AddTreeChild(pDocTree, QString::fromStdString(m_spDatabase->GetName()), "database");
  if (m_spLoader)
    _AddTreeChild(pDocTree, QString::fromStdString(m_spLoader->GetName()), "loader");
  for (auto itArch = std::begin(m_spArchitectures), itEnd = std::end(m_spArchitectures); itArch != itEnd; ++itArch)
    _AddTreeChild(pDocTree, QString::fromStdString((*itArch)->GetName()), "architecture");
  if (m_spOpratingSystem)
    _AddTreeChild(pDocTree, QString::fromStdString(m_spOpratingSystem->GetName()), "operating system");
  ConfigurationTree->expandAll();
}

void ConfigureDialog::_DestroyTree(void)
{
  if (ConfigurationTree->topLevelItemCount() == 0)
    return;
  assert(ConfigurationTree->topLevelItemCount() == 1);
  ConfigurationTree->clear();
}

void ConfigureDialog::_AddTreeChild(QTreeWidgetItem* pParent, QString const& rName, QString const& rDescription)
{
  auto pTreeItem = new QTreeWidgetItem;
  pTreeItem->setText(0, rName);
  pTreeItem->setText(1, rDescription);
  pParent->addChild(pTreeItem);
}

void ConfigureDialog::_DisplayDocumentOptions(void)
{
  auto& rModMgr = medusa::ModuleManager::Instance();

  auto pModPathLayout = new QHBoxLayout;
  pModPathLayout->addWidget(new QLabel("Module path"));
  auto pLineEdit = new QLineEdit(m_ModulePath);
  pModPathLayout->addWidget(pLineEdit);
  auto pToolBtn = new QToolButton;
  connect(pToolBtn, &QToolButton::clicked, [&](bool Clicked)
  {
    m_ModulePath = QFileDialog::getExistingDirectory(this, "Select module directory");
    if (m_ModulePath.isEmpty())
      m_ModulePath = ".";
  });
  pModPathLayout->addWidget(pToolBtn);
  ConfigurationLayout->addLayout(pModPathLayout);

  auto const& rDbs = rModMgr.GetDatabases();
  if (!rDbs.empty())
  {
    auto pDbCmbBox = new QComboBox;
    for (auto itDb = std::begin(rDbs), itEnd = std::end(rDbs); itDb != itEnd; ++itDb)
      pDbCmbBox->addItem(QString::fromStdString((*itDb)->GetName()));
    ConfigurationLayout->addWidget(pDbCmbBox);
  }

  auto const& rLdrs = rModMgr.GetLoaders();
  if (!rLdrs.empty())
  {
    auto pLdrCmbBox = new QComboBox;
    for (auto itLdr = std::begin(rLdrs), itEnd = std::end(rLdrs); itLdr != itEnd; ++itLdr)
      pLdrCmbBox->addItem(QString::fromStdString((*itLdr)->GetName()));
    ConfigurationLayout->addWidget(pLdrCmbBox);
  }
  ConfigurationLayout->addSpacerItem(new QSpacerItem(400, 0, QSizePolicy::Minimum, QSizePolicy::Minimum));
}

void ConfigureDialog::_DisplayConfigurationOptions(medusa::ConfigurationModel const& rConfigurationModel)
{
  class CfgVst : public boost::static_visitor<>
  {
  public:
    CfgVst(QVBoxLayout *pLayout, medusa::ConfigurationModel const& rCfgMdl)
      : m_pLayout(pLayout), m_rCfgMdl(rCfgMdl) {}
    void operator()(medusa::ConfigurationModel::NamedBool const& rBool) const
    {
      auto pItemLayout = new QHBoxLayout;

      pItemLayout->addWidget(new QLabel(QString::fromStdString(rBool.GetName())));

      auto pChkBox = new QCheckBox;
      pChkBox->setChecked(rBool.GetValue() ? true : false);
      pItemLayout->addWidget(pChkBox);

      m_pLayout->addLayout(pItemLayout);
    }
    void operator()(medusa::ConfigurationModel::NamedEnum const& rEnum) const
    {
      auto pItemLayout = new QHBoxLayout;

      pItemLayout->addWidget(new QLabel(QString::fromStdString(rEnum.GetName())));

      auto pCmbBox = new QComboBox;
      auto const& rValues = rEnum.GetValue();
      for (auto itVal = std::begin(rValues), itEnd = std::end(rValues); itVal != itEnd; ++itVal)
      {
        pCmbBox->addItem(itVal->first, itVal->second);
        if (itVal->second == m_rCfgMdl.GetConfiguration().Get(rEnum.GetName()))
          pCmbBox->setCurrentIndex(pCmbBox->count() - 1);
      }
      pItemLayout->addWidget(pCmbBox);

      m_pLayout->addLayout(pItemLayout);
    }

  private:
    QVBoxLayout *m_pLayout;
    medusa::ConfigurationModel const& m_rCfgMdl;
  };

  for (auto itCfg = rConfigurationModel.Begin(), itEnd = rConfigurationModel.End(); itCfg != itEnd; ++itCfg)
    boost::apply_visitor(CfgVst(ConfigurationLayout, rConfigurationModel), *itCfg);
  ConfigurationLayout->addSpacerItem(new QSpacerItem(400, 0, QSizePolicy::Minimum, QSizePolicy::Minimum));
}

void ConfigureDialog::_ClearOptions(void)
{
  std::function<void (QLayout*)> ClearLayout = [&](QLayout *pLayout)
  {
    QLayoutItem* pItem = nullptr;
    while ((pItem = pLayout->takeAt(0)))
    {
      if (pItem->layout())
      {
        ClearLayout(pItem->layout());
        delete pItem->layout();
      }
      else if (pItem->spacerItem())
        delete pItem->spacerItem();
      else
        delete pItem->widget();
    }
  };
  ClearLayout(ConfigurationLayout);
}