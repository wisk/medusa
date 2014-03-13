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

private:
  void GetDefaultModules(void);
  void SetupTree(void);

  void AddTreeChild(QTreeWidgetItem* pParent, QString const& rName, QString const& rDescription);

  medusa::BinaryStream::SharedPtr       m_spBinaryStream;
  medusa::Database::SharedPtr           m_spDatabase;
  medusa::Loader::SharedPtr             m_spLoader;
  medusa::Architecture::VectorSharedPtr m_spArchitectures;
  medusa::OperatingSystem::SharedPtr    m_spOpratingSystem;
};

#endif // !__CONFIGURE_DIALOG_HPP__