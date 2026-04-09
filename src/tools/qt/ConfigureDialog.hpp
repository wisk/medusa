#ifndef QMEDUSA_CONFIGURE_DIALOG_HPP
#define QMEDUSA_CONFIGURE_DIALOG_HPP

#include <QDialog>
#include "ui_ConfigureDialog.h"

#include <medusa/medusa.hpp>
#include <medusa/binary_stream.hpp>
#include <medusa/database.hpp>
#include <medusa/loader.hpp>
#include <medusa/architecture.hpp>
#include <medusa/os.hpp>

class ConfigureDialog : public QDialog, public Ui::ConfigureDialog
{
  Q_OBJECT

public:
  ConfigureDialog(QWidget* pParent, medusa::BinaryStream::SPType spBinaryStream);

  ~ConfigureDialog();

  medusa::Database::SPType           GetSelectedDatabase() const;
  medusa::Loader::SPType             GetSelectedLoader() const;
  medusa::Architecture::VSPType      GetSelectedArchitectures() const;
  medusa::OperatingSystem::SPType    GetSelectedOperatingSystem() const;

protected slots:
  void OnItemClicked(QTreeWidgetItem * pItem, int Column);
  void OnAccepted();

private:
  void _GetModulesByLoader();
  void _GetDefaultModules();
  void _CreateTree();
  void _UpdateTree();
  void _DestroyTree();

  void _AddTreeChild(QTreeWidgetItem* pParent, QString const& rIconPath, QString const& rName, QString const& rDescription);

  void _DisplayDocumentOptions();
  void _DisplayConfigurationOptions(medusa::ConfigurationModel& rConfigurationModel);
  void _ClearOptions();

  medusa::BinaryStream::SPType          m_spBinaryStream;
  medusa::Database::SPType              m_spDatabase;
  medusa::Loader::SPType                m_spLoader;
  medusa::Architecture::VSPType         m_spArchitectures;
  medusa::OperatingSystem::SPType       m_spOpratingSystem;
  QLineEdit*                            m_pModulePathWidget;

  class ConfigurationVisitor
  {
  public:
    ConfigurationVisitor(QVBoxLayout *pLayout, medusa::ConfigurationModel& rCfgMdl);
    void operator()(medusa::ConfigurationModel::NamedBool const& rBool);
    void operator()(medusa::ConfigurationModel::NamedUint8 const& rUint8);
    void operator()(medusa::ConfigurationModel::NamedUint16 const& rUint16);
    void operator()(medusa::ConfigurationModel::NamedUint32 const& rUint32);
    void operator()(medusa::ConfigurationModel::NamedUint64 const& rUint64);
    void operator()(medusa::ConfigurationModel::NamedEnum const& rEnum);
    void operator()(medusa::ConfigurationModel::NamedString const& rString);
    void operator()(medusa::ConfigurationModel::NamedPath const& rPath);

  private:
    QVBoxLayout*                m_pLayout;
    QWidget*                    m_pWidget;
    medusa::ConfigurationModel& m_rCfgMdl;
  };
  QVector<ConfigurationVisitor*> m_Visitors;
};

#endif // !QMEDUSA_CONFIGURE_DIALOG_HPP