#include "LabelView.hpp"

#include <QStandardItemModel>
#include <QTreeWidgetItem>
#include <QFileInfo>
#include <QDir>

Q_DECLARE_METATYPE(medusa::Address);
Q_DECLARE_METATYPE(medusa::Label);

LabelView::LabelView(QWidget * parent, medusa::Medusa &core, QString const &fileName)
  : QTreeView(parent), View(medusa::Document::Subscriber::LabelUpdated, core.GetDocument())
  , _core(core)
{
  // Look for a map file with the same base name as the binary file, and if such
  // is present, record the listed symbols, indexed by address for later lookup.
  // For now, ignorantly assume Microsoft Visual Studio linker map file format.
  QFileInfo const fi(fileName);
  QFile file(QFileInfo(fi.dir(), fi.completeBaseName() + ".map").filePath());
  if (file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    while (!file.atEnd())
    {
      auto line = file.readLine();
      int64_t addr;
      char name[256];
      if (sscanf(line, "%llx:%llx 0%llx", &addr, &addr, &addr) == 3)
      {
        // The line seems to belong to the section directory. Just ignore it.
      }
      else if (sscanf(line, "%llx:%llx %255s %llx", &addr, &addr, name, &addr) == 4)
      {
        // Now this seems worth recording.
        _symbolMap[addr] = name;
      }
    }
  }
  setSortingEnabled(true);
  setUniformRowHeights(false);
  qRegisterMetaType<medusa::Address>("Address");
  qRegisterMetaType<medusa::Label>("Label");
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  auto model = new QStandardItemModel(this);
  model->setColumnCount(4);
  model->setHeaderData(0, Qt::Horizontal, "Name");
  model->setHeaderData(1, Qt::Horizontal, "Type");
  model->setHeaderData(2, Qt::Horizontal, "Address");
  model->setHeaderData(3, Qt::Horizontal, "Symbol");
  setModel(model);
  connect(this, SIGNAL(doubleClicked(QModelIndex const&)),  this, SLOT(onDoubleClickLabel(QModelIndex const&)));
  connect(this, SIGNAL(labelAdded(medusa::Address const&, medusa::Label const&)),   this, SLOT(onAddLabel(medusa::Address const&, medusa::Label const&)));
  connect(this, SIGNAL(labelRemoved(medusa::Address const&, medusa::Label const&)), this, SLOT(onRemoveLabel(medusa::Address const&, medusa::Label const&)));
}

void LabelView::OnLabelUpdated(medusa::Address const& address, medusa::Label const& label, bool removed)
{
  if (removed)
    emit labelRemoved(address, label);
  else
    emit labelAdded(address, label);
}

void LabelView::Refresh(void)
{
  _mutex.lock();
  auto pModel = model();
  pModel->removeRows(0, pModel->rowCount());
  _mutex.unlock();

  m_rDoc.ForEachLabel([&](medusa::Address const& rAddr, medusa::Label const& rLbl)
  {
    emit labelAdded(rAddr, rLbl);
  });
}

void LabelView::onAddLabel(medusa::Address const& address, medusa::Label const& label)
{
  if (label.GetType() & medusa::Label::Local)
    return;

  setUpdatesEnabled(false);
  QString labelType = "";
  switch (label.GetType() & medusa::Label::AccessMask)
  {
  case medusa::Label::Exported: labelType += "exported "; break;
  case medusa::Label::Imported: labelType += "imported "; break;
  default:                                                break;
  }
  switch (label.GetType() & medusa::Label::CellMask)
  {
  case medusa::Label::Code:     labelType += "code";    break;
  case medusa::Label::Function: labelType += "function"; break;
  case medusa::Label::Data:     labelType += "data";    break;
  case medusa::Label::String:   labelType += "string";  break;
  default:                      labelType += "unknown"; break;
  }

  auto model = this->model();
  _mutex.lock();
  const int row = model->rowCount();
  model->insertRow(row);
  model->setData(model->index(row, 0), QString::fromStdString(label.GetLabel()));
  model->setData(model->index(row, 1), labelType);
  model->setData(model->index(row, 2), QString::fromStdString(address.ToString()));
  auto it = _symbolMap.find(address.GetOffset());
  if (it != _symbolMap.end())
    model->setData(model->index(row, 3), QString::fromStdString(it->second));

  // This method can assert
  //resizeColumnToContents(0);
  //resizeColumnToContents(1);
  //resizeColumnToContents(2);
  _mutex.unlock();
  setUpdatesEnabled(true);
}

void LabelView::onRemoveLabel(medusa::Address const& address, medusa::Label const& label)
{
  auto model = this->model();
  _mutex.lock();
  auto const& res = model->match(model->index(0, 0), 0, QString::fromStdString(label.GetLabel()), 1, Qt::MatchExactly);
  if (res.isEmpty())
  {
    _mutex.unlock();
    return;
  }
  model->removeRow(res.at(0).row());
  _mutex.unlock();
}

void LabelView::onDoubleClickLabel(QModelIndex const& idx)
{
  auto model = this->model();
  const QString strAddr = model->data(model->index(idx.row(), 2)).toString();
  medusa::Address addr(strAddr.toStdString());
  emit goTo(addr);
}