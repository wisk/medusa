#include "MemoryAreaView.hpp"

#include <QStandardItemModel>
#include <QTreeWidgetItem>

//Q_DECLARE_METATYPE(medusa::MemoryArea);

MemoryAreaView::MemoryAreaView(QWidget * parent, medusa::Medusa &core)
  : QTreeView(parent)
  , View(medusa::Document::Subscriber::MemoryAreaUpdated, core.GetDocument())
  , _core(core)
{
  qRegisterMetaType<medusa::MemoryArea*>("MemoryArea");
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  auto model = new QStandardItemModel(this);
  model->setColumnCount(4);
  model->setHeaderData(0, Qt::Horizontal, "Name");
  model->setHeaderData(1, Qt::Horizontal, "Address");
  model->setHeaderData(2, Qt::Horizontal, "Size");
  model->setHeaderData(3, Qt::Horizontal, "Access");
  setModel(model);
  connect(this, SIGNAL(doubleClicked(QModelIndex const&)), this, SLOT(onDoubleClickMemoryArea(QModelIndex const&)));
  connect(this, SIGNAL(memoryAreaAdded(medusa::MemoryArea const&)), this, SLOT(onAddMemoryArea(medusa::MemoryArea const&)));
}

void MemoryAreaView::OnMemoryAreaUpdated(medusa::MemoryArea const& memArea, bool removed)
{
  if (removed)
  {
    // TODO
  }
  else
    emit memoryAreaAdded(memArea);
}


void MemoryAreaView::Refresh(void)
{
  _core.GetDocument().ForEachMemoryArea([&](medusa::MemoryArea const& rMemArea)
  {
    OnMemoryAreaUpdated(rMemArea, false);
  });
}

void MemoryAreaView::onAddMemoryArea(medusa::MemoryArea const& memArea)
{
  auto acc2Str = [](medusa::u32 access) -> std::string
  {
    char buf[4];
    char *p = buf;

    if (access & medusa::MemoryArea::Read)
      *p++ = 'R';
    if (access & medusa::MemoryArea::Write)
      *p++ = 'W';
    if (access & medusa::MemoryArea::Execute)
      *p++ = 'X';
    *p = '\0';
    return std::string(buf);
  };

  const QString memAreaName = QString::fromStdString(memArea.GetName());
  const QString memAreaAddr = QString::fromStdString(memArea.GetBaseAddress().ToString());
  const QString memAreaSize = QString("%1").arg(memArea.GetSize(), 8, 16, QChar('0'));
  const QString memAreaAccs = QString::fromStdString(acc2Str(memArea.GetAccess()));

  auto model = this->model();
  _mutex.lock();
  const int row = model->rowCount();
  model->insertRow(row);
  model->setData(model->index(row, 0), memAreaName);
  model->setData(model->index(row, 1), memAreaAddr);
  model->setData(model->index(row, 2), memAreaSize);
  model->setData(model->index(row, 3), memAreaAccs);
  _mutex.unlock();
}

void MemoryAreaView::onDoubleClickMemoryArea(QModelIndex const& idx)
{
  auto model = this->model();
  const QString strAddr = model->data(model->index(idx.row(), 1)).toString();
  medusa::Address addr(strAddr.toStdString());
  emit goTo(addr);
}