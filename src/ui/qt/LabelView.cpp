#include "LabelView.hpp"

#include <QStandardItemModel>
#include <QTreeWidgetItem>

Q_DECLARE_METATYPE(medusa::Label);

LabelView::LabelView(QWidget * parent, medusa::Medusa &core)
  : QTreeView(parent), View(medusa::Document::Subscriber::LabelUpdated, core.GetDocument())
  , _core(core)
{
  setUniformRowHeights(false);
  qRegisterMetaType<medusa::Label>("Label");
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  auto model = new QStandardItemModel(this);
  model->setColumnCount(3);
  model->setHeaderData(0, Qt::Horizontal, "Name");
  model->setHeaderData(1, Qt::Horizontal, "Type");
  model->setHeaderData(2, Qt::Horizontal, "Address");
  setModel(model);
  connect(this, SIGNAL(doubleClicked(QModelIndex const&)),  this, SLOT(onLabelDoubleClicked(QModelIndex const&)));
  connect(this, SIGNAL(labelAdded(medusa::Label const&)),   this, SLOT(onAddLabel(medusa::Label const&)));
  connect(this, SIGNAL(labelRemoved(medusa::Label const&)), this, SLOT(onRemoveLabel(medusa::Label const&)));
}

void LabelView::OnLabelUpdated(medusa::Label const& label, bool removed)
{
  if (removed)
    emit labelRemoved(label);
  else
    emit labelAdded(label);
}

void LabelView::onAddLabel(medusa::Label const& label)
{
  if (label.GetType() & medusa::Label::Local)
    return;

  setUpdatesEnabled(false);
  QString labelType = "";
  switch (label.GetType() & medusa::Label::SourceMask)
  {
  case medusa::Label::Exported: labelType += "exported "; break;
  case medusa::Label::Imported: labelType += "imported "; break;
  default:                                                break;
  }
  switch (label.GetType() & medusa::Label::CellMask)
  {
  case medusa::Label::Code:   labelType += "code";    break;
  case medusa::Label::Data:   labelType += "data";    break;
  case medusa::Label::String: labelType += "string";  break;
  default:                    labelType += "unknown"; break;
  }

  auto model = this->model();
  _mutex.lock();
  const int row = model->rowCount();
  medusa::Address const& addr = _core.GetDocument().GetAddressFromLabelName(label.GetName());
  model->insertRow(row);
  model->setData(model->index(row, 0), QString::fromStdString(label.GetLabel()));
  model->setData(model->index(row, 1), labelType);
  model->setData(model->index(row, 2), QString::fromStdString(addr.ToString()));

  // This method can assert
  //resizeColumnToContents(0);
  //resizeColumnToContents(1);
  //resizeColumnToContents(2);
  _mutex.unlock();
  setUpdatesEnabled(true);
}

void LabelView::onRemoveLabel(medusa::Label const& label)
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

void LabelView::onLabelDoubleClicked(QModelIndex const& idx)
{
  auto model = this->model();
  const QString strAddr = model->data(model->index(idx.row(), 2)).toString();
  medusa::Address addr(strAddr.toStdString());
  emit goTo(addr);
}