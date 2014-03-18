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
  , m_pModulePathWidget(nullptr)
  , m_ModulePath(".")
{
  setupUi(this);

  connect(ButtonBox, &QDialogButtonBox::accepted, this, &ConfigureDialog::OnAccepted);
  connect(ButtonBox, &QDialogButtonBox::rejected, this, &QDialog::close);
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
  _ClearOptions();
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

void ConfigureDialog::OnAccepted(void)
{
  close();
}

void ConfigureDialog::_GetModulesByLoader(void)
{
  if (m_spLoader == nullptr)
    return;

  medusa::ModuleManager& rModMgr = medusa::ModuleManager::Instance();
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

void ConfigureDialog::_UpdateTree(void)
{
  _DestroyTree();
  _CreateTree();
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
  m_pModulePathWidget = new QLineEdit(m_ModulePath);
  pModPathLayout->addWidget(m_pModulePathWidget);
  auto pToolBtn = new QToolButton;
  connect(pToolBtn, &QToolButton::clicked, [&](bool Clicked)
  {
    m_ModulePath = QFileDialog::getExistingDirectory(this, "Select module directory");
    if (m_ModulePath.isEmpty())
      m_ModulePath = ".";
    m_pModulePathWidget->setText(m_ModulePath);
  });
  pModPathLayout->addWidget(pToolBtn);
  ConfigurationLayout->addLayout(pModPathLayout);

  auto const& rDbs = rModMgr.GetDatabases();
  if (!rDbs.empty())
  {
    auto pDbCmbBox = new QComboBox;
    for (auto itDb = std::begin(rDbs), itEnd = std::end(rDbs); itDb != itEnd; ++itDb)
    {
      pDbCmbBox->addItem(QString::fromStdString((*itDb)->GetName()));
      if (m_spDatabase != nullptr && (*itDb)->GetName() == m_spDatabase->GetName())
        pDbCmbBox->setCurrentIndex(pDbCmbBox->count() - 1);
    }
    connect(pDbCmbBox, static_cast<void (QComboBox::*)(QString const&)>(&QComboBox::currentIndexChanged), [&](QString const& rDbName)
    {
      auto& rModMgr = medusa::ModuleManager::Instance();
      auto AllDbs = rModMgr.GetDatabases();
      auto itDb = std::find_if(std::begin(AllDbs), std::end(AllDbs), [&rDbName](medusa::Database::SharedPtr spDb)
      { return spDb->GetName() == rDbName.toStdString(); });
      if (itDb == std::end(AllDbs))
        return;
      m_spDatabase = *itDb;
      _UpdateTree();
    });
    ConfigurationLayout->addWidget(pDbCmbBox);
  }

  auto const& rLdrs = rModMgr.GetLoaders();
  if (!rLdrs.empty())
  {
    auto pLdrCmbBox = new QComboBox;
    for (auto itLdr = std::begin(rLdrs), itEnd = std::end(rLdrs); itLdr != itEnd; ++itLdr)
    {
      pLdrCmbBox->addItem(QString::fromStdString((*itLdr)->GetName()));
      if (m_spLoader != nullptr && (*itLdr)->GetName() == m_spLoader->GetName())
        pLdrCmbBox->setCurrentIndex(pLdrCmbBox->count() - 1);
    }
    connect(pLdrCmbBox, static_cast<void (QComboBox::*)(QString const&)>(&QComboBox::currentIndexChanged), [&](QString const& rLdrName)
    {
      auto& rModMgr = medusa::ModuleManager::Instance();
      auto AllLdrs = rModMgr.GetLoaders();
      auto itLdr = std::find_if(std::begin(AllLdrs), std::end(AllLdrs), [&rLdrName](medusa::Loader::SharedPtr spLdr)
      { return spLdr->GetName() == rLdrName.toStdString(); });
      if (itLdr == std::end(AllLdrs))
        return;
      m_spLoader = *itLdr;
      _GetModulesByLoader();
      _UpdateTree();
    });
    ConfigurationLayout->addWidget(pLdrCmbBox);
  }
  ConfigurationLayout->addSpacerItem(new QSpacerItem(400, 0, QSizePolicy::Minimum, QSizePolicy::Minimum));
}

void ConfigureDialog::_DisplayConfigurationOptions(medusa::ConfigurationModel& rConfigurationModel)
{
  for (auto itCfg = rConfigurationModel.Begin(), itEnd = rConfigurationModel.End(); itCfg != itEnd; ++itCfg)
  {
    auto pVst = new ConfigurationVisitor(ConfigurationLayout, rConfigurationModel);
    m_Visitors.push_back(pVst);
    boost::apply_visitor(*pVst, itCfg->second);
  }
  ConfigurationLayout->addSpacerItem(new QSpacerItem(400, 0, QSizePolicy::Minimum, QSizePolicy::Minimum));
}

void ConfigureDialog::_ClearOptions(void)
{
  delete m_pModulePathWidget;
  m_pModulePathWidget = nullptr;
  qDeleteAll(m_Visitors);
  m_Visitors.clear();
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

ConfigureDialog::ConfigurationVisitor::ConfigurationVisitor(QVBoxLayout *pLayout, medusa::ConfigurationModel& rCfgMdl)
  : m_pLayout(pLayout)
  , m_pWidget(nullptr)
  , m_rCfgMdl(rCfgMdl)
{
}

void ConfigureDialog::ConfigurationVisitor::operator()(medusa::ConfigurationModel::NamedBool const& rBool)
{
  auto pItemLayout = new QHBoxLayout;

  pItemLayout->addWidget(new QLabel(QString::fromStdString(rBool.GetName())));

  auto pChkBox = new QCheckBox;
  m_pWidget = pChkBox;
  pChkBox->setChecked(rBool.GetValue() ? true : false);
  connect(pChkBox, &QCheckBox::toggled, [&](bool Toggled)
  {
    m_rCfgMdl.SetBoolean(rBool.GetName(), Toggled ? 1 : 0);
  });
  pItemLayout->addWidget(pChkBox);
  m_pLayout->addLayout(pItemLayout);
}

void ConfigureDialog::ConfigurationVisitor::operator()(medusa::ConfigurationModel::NamedEnum const& rEnum)
{
  auto pItemLayout = new QHBoxLayout;

  pItemLayout->addWidget(new QLabel(QString::fromStdString(rEnum.GetName())));

  auto pCmbBox = new QComboBox;
  m_pWidget = pCmbBox;
  auto const& rValues = rEnum.GetConfigurationValue();
  auto DefVal = rEnum.GetValue();
  for (auto itVal = std::begin(rValues), itEnd = std::end(rValues); itVal != itEnd; ++itVal)
  {
    if (itVal->first == "")
      continue;
    pCmbBox->addItem(QString::fromStdString(itVal->first), itVal->second);
    if (itVal->second == DefVal)
      pCmbBox->setCurrentIndex(pCmbBox->count() - 1);
  }
  connect(pCmbBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [&](int Index) -> void
  {
    // NOTE: We have to do this because Qt can't give us the items data from slots
    medusa::u32 Value = static_cast<QComboBox*>(m_pWidget)->itemData(Index).toUInt();
    m_rCfgMdl.SetEnum(rEnum.GetName(), Value);
  });
  pItemLayout->addWidget(pCmbBox);

  m_pLayout->addLayout(pItemLayout);
}