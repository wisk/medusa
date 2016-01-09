#ifndef QMEDUSA_GRAPH_SCENE_HPP
#define QMEDUSA_GRAPH_SCENE_HPP

#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>

#include <medusa/medusa.hpp>
#include <medusa/function.hpp>

class GraphScene : public QGraphicsScene
{
  Q_OBJECT
public:
  explicit GraphScene(QObject* pParent, medusa::Medusa& rCore, medusa::Address const& rGraphAddr);

private:
  bool _Update(void);

  medusa::Medusa& m_rCore;
  medusa::Address m_GraphAddr;
};

#endif // QMEDUSA_GRAPH_SCENE_HPP