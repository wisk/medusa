#ifndef __MEMORY_AREA_VIEW_HPP__
#define __MEMORY_AREA_VIEW_HPP__

#include <QTreeView>
#include <QMutex>

#include <medusa/medusa.hpp>
#include <medusa/view.hpp>

class MemoryAreaView : public QTreeView, public medusa::View
{
  Q_OBJECT

public:
    MemoryAreaView(QWidget * parent, medusa::Medusa& core);
    virtual ~MemoryAreaView(void) {}

    virtual void OnMemoryAreaUpdated(medusa::MemoryArea const& memArea, bool removed);

signals:
  void goTo(medusa::Address const& addr);
  void memoryAreaAdded(medusa::MemoryArea const& memArea);

private slots:
  void onAddMemoryArea(medusa::MemoryArea const& memArea);
  void onDoubleClickMemoryArea(QModelIndex const& idx);

private:
  medusa::Medusa& _core;
  QMutex _mutex;
};

#endif // !__MEMORY_AREA_VIEW_HPP__
