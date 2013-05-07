#include <QPushButton>
#include <QLabel>
#include <QLayoutItem>
#include <QLayoutIterator>
#include "LoaderChooser.hpp"

LoaderChooser::LoaderChooser(QWidget * parent, medusa::Medusa & medusa)
  : QDialog(parent),
  _medusa(medusa),
  _cfgModel(),
  _cfg(),
  _widgets()
{
  this->setupUi(this);
}

LoaderChooser::~LoaderChooser()
{
}

bool LoaderChooser::getSelection(medusa::Loader::SharedPtr & loader, medusa::Architecture::SharedPtr & architecture, medusa::OperatingSystem::SharedPtr & os)
{
  this->architecture->hide();
  this->labelArchitecture->hide();
  this->operatingsystem->hide();
  this->labelOperatingSystem->hide();

  this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

  // Let the user choose the loader he/she wants to use
  medusa::Loader::VectorSharedPtr const & loaders = this->_medusa.GetSupportedLoaders();

  this->loader->clear();
  for (unsigned int i = 0; i < loaders.size(); i++)
    this->loader->addItem(QIcon(":/images/ram.png"), QString::fromStdString(loaders[i]->GetName()));

  this->loader->setCurrentIndex(0);

  // Execution
  int result = this->exec();

  // Getting result
  if (result == QDialog::Accepted)
  {
    medusa::Architecture::VectorSharedPtr const & architectures = this->_medusa.GetAvailableArchitectures();

    loader = loaders[this->loader->currentIndex()];
    architecture = architectures[this->architecture->currentIndex()];

    for (medusa::ConfigurationModel::ConstIterator It = this->_cfgModel.Begin(); It != this->_cfgModel.End(); ++It)
      boost::apply_visitor(ConfigGetter(this->_cfg, this->_widgets), *It);

    architecture->UseConfiguration(this->_cfg);

    medusa::OperatingSystem::VectorSharedPtr const & oses = this->_medusa.GetCompatibleOperatingSystems(loader, architecture);
    if (this->operatingsystem->currentIndex() == -1 || oses.empty())
      os = medusa::OperatingSystem::SharedPtr();
    else
      os = oses[this->operatingsystem->currentIndex()];
  }

  return (result == QDialog::Accepted);
}

void LoaderChooser::operator()(medusa::ConfigurationModel::NamedBool const & rBool)
{
  QLabel * label = new QLabel(rBool.GetName());
  QCheckBox * checkbox = new QCheckBox();

  checkbox->setChecked(rBool.GetValue());
  this->formLayout->addRow(label, checkbox);

  this->_widgets[rBool.GetName()] = WidgetPair(label, checkbox);
}

void LoaderChooser::operator()(medusa::ConfigurationModel::NamedEnum const & rEnum)
{
  QLabel *  label = new QLabel(rEnum.GetName());
  QComboBox * combo = new QComboBox();

  for (medusa::ConfigurationModel::Enum::const_iterator It = rEnum.GetValue().begin();
        It != rEnum.GetValue().end(); ++It)
    {
    combo->addItem(It->first, It->second);
    if (It->second == this->_cfg.Get(rEnum.GetName()))
      combo->setCurrentIndex(combo->count() - 1);
    }
  this->formLayout->addRow(label, combo);

  this->_widgets[rEnum.GetName()] = WidgetPair(label, combo);
}

void LoaderChooser::on_loader_currentIndexChanged(int index)
{
  if (index == -1)
  {
    this->architecture->hide();
    this->labelArchitecture->hide();
    this->operatingsystem->hide();
    this->labelOperatingSystem->hide();

    this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  }
  else
  {
    // Select arch
    medusa::Architecture::VectorSharedPtr & architectures = this->_medusa.GetAvailableArchitectures();
    medusa::Loader::VectorSharedPtr const & loaders = this->_medusa.GetSupportedLoaders();

    auto idx = this->loader->currentIndex();
    auto ldr = loaders[idx];
    medusa::Architecture::SharedPtr defaultArchi = ldr->GetMainArchitecture(architectures);

    // Let the user choose the arch he/she wants to use
    this->architecture->clear();
    int defaultIndex = -1;

    for (unsigned int i = 0; i < architectures.size(); i++)
    {
      if (defaultArchi != 0 && defaultArchi->GetName() == architectures[i]->GetName())
        defaultIndex = i;
      this->architecture->addItem(QIcon(":/images/ram.png"), architectures[i]->GetName().c_str());
    }

    this->architecture->setCurrentIndex(defaultIndex);

    this->architecture->show();
    this->labelArchitecture->show();

    if (defaultIndex != -1)
      this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
  }
}

void LoaderChooser::on_architecture_currentIndexChanged(int index)
{
  foreach (WidgetPair pair, this->_widgets.values())
  {
    delete pair.first;
    delete pair.second;
  }
  this->_widgets.clear();
  this->_cfgModel = medusa::ConfigurationModel();

  if (index == -1)
  {
    // Clean the option

    this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  }
  else
  {
    medusa::Loader::VectorSharedPtr const & loaders             = this->_medusa.GetSupportedLoaders();
    medusa::Architecture::VectorSharedPtr const & architectures = this->_medusa.GetAvailableArchitectures();
    medusa::Loader::SharedPtr loader                            = loaders[this->loader->currentIndex()];
    medusa::Architecture::SharedPtr archi                       = architectures[this->architecture->currentIndex()];

    this->_cfg.Clear();
    archi->FillConfigurationModel(this->_cfgModel);
    this->_cfg = this->_cfgModel.GetConfiguration();
    loaders[this->loader->currentIndex()]->Configure(this->_cfg);

    for (medusa::ConfigurationModel::ConstIterator It = this->_cfgModel.Begin(); It != this->_cfgModel.End(); ++It)
      boost::apply_visitor(*this, *It);

    medusa::OperatingSystem::VectorSharedPtr oses               = this->_medusa.GetCompatibleOperatingSystems(loader, archi);
    for (auto it = std::begin(oses); it != std::end(oses); ++it)
      this->operatingsystem->addItem(QIcon(":/images/ram.png"), (*it)->GetName().c_str());

    this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
  }

  this->operatingsystem->show();
  this->labelOperatingSystem->show();
}

void LoaderChooser::on_operatingsystem_currentIndexChanged(int index)
{
  //this->operatingsystem->clear();

  //medusa::Loader::VectorSharedPtr const & loaders             = this->_medusa.GetSupportedLoaders();
  //medusa::Architecture::VectorSharedPtr const & architectures = this->_medusa.GetAvailableArchitectures();
  //medusa::Loader::SharedPtr loader                            = loaders[this->loader->currentIndex()];
  //medusa::Architecture::SharedPtr archi                       = architectures[this->architecture->currentIndex()];

  //medusa::OperatingSystem::VectorSharedPtr oses               = this->_medusa.GetCompatibleOperatingSystems(loader, archi);
  //for (auto it = std::begin(oses); it != std::end(oses); ++it)
  //  this->operatingsystem->addItem(QIcon(":/images/ram.png"), (*it)->GetName().c_str());
}