#ifndef __LOADER_CHOOSER_H__
# define __LOADER_CHOOSER_H__

# include <QDialog>
# include <QStringList>
# include <QIcon>
# include <QMap>
# include <QPair>
# include <QCheckBox>
# include <QComboBox>
# include <medusa/medusa.hpp>
# include <medusa/configuration.hpp>
# include "ui_LoaderChooser.h"

class QWidget;

struct ConfigGetter : public boost::static_visitor<>
{
  typedef QPair<QWidget *, QWidget *>      WidgetPair;

  ConfigGetter(medusa::Configuration& rCfg, QMap<QString, WidgetPair> &_widgets) : m_rCfg(rCfg), _widgets(_widgets) {}

  medusa::Configuration & m_rCfg;
  QMap<QString, WidgetPair> &    _widgets;

  void operator()(medusa::ConfigurationModel::NamedBool const& rBool) const
  {
    QCheckBox* box = qobject_cast<QCheckBox *>(_widgets[rBool.GetName()].second);
    m_rCfg.Set(rBool.GetName(), box->isChecked());
  }

  void operator()(medusa::ConfigurationModel::NamedEnum const& rEnum) const
  {
    QComboBox* box = qobject_cast<QComboBox *>(_widgets[rEnum.GetName()].second);
    m_rCfg.Set(rEnum.GetName(), box->itemData(box->currentIndex()).toUInt());
  }
};

class LoaderChooser: public QDialog, public Ui::LoaderChooser, public boost::static_visitor<>
{
  Q_OBJECT

public:
  LoaderChooser(QWidget * parent, medusa::Medusa & medusa);
  ~LoaderChooser();

public:
  bool        getSelection(medusa::Loader::SharedPtr& loader, medusa::Architecture::SharedPtr& architecture, medusa::OperatingSystem::SharedPtr& os, medusa::Database::SharedPtr& database);

public:
  void        operator()(medusa::ConfigurationModel::NamedBool const & rBool);
  void        operator()(medusa::ConfigurationModel::NamedEnum const & rEnum);

  private slots:
    void        on_loader_currentIndexChanged(int index);
    void        on_architecture_currentIndexChanged(int index);
    void        on_operatingsystem_currentIndexChanged(int index);

private:
  typedef QPair<QWidget *, QWidget *>      WidgetPair;

  medusa::Medusa &             _medusa;
  medusa::ConfigurationManager _cfgMgr;
  QMap<QString, WidgetPair>    _widgets;
};

#endif // !__LOADER_CHOOSER_H__
