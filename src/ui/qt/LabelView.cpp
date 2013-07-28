#include "LabelView.hpp"

#include <QStandardItemModel>
#include <QTreeWidgetItem>

LabelView::LabelView(QWidget * parent, medusa::Medusa &core)
  : QTreeView(parent), View(medusa::Document::Subscriber::LabelUpdated, core.GetDocument())
  , _core(core)
{
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  auto model = new QStandardItemModel(this);
  model->setColumnCount(3);
  model->setHeaderData(0, Qt::Horizontal, "Name");
  model->setHeaderData(1, Qt::Horizontal, "Type");
  model->setHeaderData(2, Qt::Horizontal, "Address");
  setModel(model);
  connect(this, SIGNAL(doubleClicked(QModelIndex const&)), this, SLOT(onLabelDoubleClicked(QModelIndex const&)));
}

void LabelView::OnLabelUpdated(medusa::Label const& label, bool removed)
{
  QString labelType = "";
  switch (label.GetType())
  {
  case medusa::Label::LabelExported: labelType += "exported "; break;
  case medusa::Label::LabelImported: labelType += "imported "; break;
  default:                                                     break;
  }
  switch (label.GetType())
  {
  case medusa::Label::LabelCode:   labelType += "code";    break;
  case medusa::Label::LabelData:   labelType += "data";    break;
  case medusa::Label::LabelString: labelType += "string";  break;
  default:                         labelType += "unknown"; break;
  }

  auto model = this->model();
  if (removed)
  {
    auto const& res = model->match(model->index(0, 0), 0, QString::fromStdString(label.GetLabel()), 1, Qt::MatchExactly);
    if (res.isEmpty())
      return;
    model->removeRows(res.at(0).row(), model->rowCount());
  }
  else
  {
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
  }
}

void LabelView::onLabelDoubleClicked(QModelIndex const& idx)
{
  auto model = this->model();
  const QString strAddr = model->data(model->index(idx.row(), 2)).toString();
  medusa::Address addr(strAddr.toStdString());
  emit goTo(addr);
}