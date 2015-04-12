#ifndef QMEDUSA_MEMORY_AREA_VIEW_HPP
#define QMEDUSA_MEMORY_AREA_VIEW_HPP

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
    void Refresh(void);

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

#endif // !QMEDUSA_MEMORY_AREA_VIEW_HPP
