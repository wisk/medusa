#ifndef __CONFIGURE_DIALOG_HPP__
#define __CONFIGURE_DIALOG_HPP__

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
  ConfigureDialog(QWidget* pParent, medusa::BinaryStream::SharedPtr spBinaryStream);

  ~ConfigureDialog(void);

  medusa::Database::SharedPtr           GetSelectedDatabase(void) const;
  medusa::Loader::SharedPtr             GetSelectedLoader(void) const;
  medusa::Architecture::VectorSharedPtr GetSelectedArchitectures(void) const;
  medusa::OperatingSystem::SharedPtr    GetSelectedOperatingSystem(void) const;

protected slots:
  void OnItemClicked(QTreeWidgetItem * pItem, int Column);
  void OnAccepted(void);

private:
  void _GetModulesByLoader(void);
  void _GetDefaultModules(void);
  void _CreateTree(void);
  void _UpdateTree(void);
  void _DestroyTree(void);

  void _AddTreeChild(QTreeWidgetItem* pParent, QString const& rIconPath, QString const& rName, QString const& rDescription);

  void _DisplayDocumentOptions(void);
  void _DisplayConfigurationOptions(medusa::ConfigurationModel& rConfigurationModel);
  void _ClearOptions(void);

  medusa::BinaryStream::SharedPtr       m_spBinaryStream;
  medusa::Database::SharedPtr           m_spDatabase;
  medusa::Loader::SharedPtr             m_spLoader;
  medusa::Architecture::VectorSharedPtr m_spArchitectures;
  medusa::OperatingSystem::SharedPtr    m_spOpratingSystem;
  QLineEdit*                            m_pModulePathWidget;
  QString                               m_ModulePath;


  class ConfigurationVisitor : public boost::static_visitor<>
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

#endif // !__CONFIGURE_DIALOG_HPP__