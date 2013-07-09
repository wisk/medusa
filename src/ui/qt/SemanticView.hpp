#ifndef __SEMANTIC_VIEW_HPP__
#define __SEMANTIC_VIEW_HPP__

# include <QtGui>
# include <QTextEdit>
# include <QAbstractScrollArea>
# include <QPainter>
# include <QResizeEvent>
# include <QPaintEvent>

# include <medusa/medusa.hpp>
# include <medusa/address.hpp>
# include <medusa/database.hpp>
# include <medusa/instruction.hpp>
# include <medusa/screen.hpp>

class SemanticView : public QTextEdit
{
  Q_OBJECT

public:
  SemanticView(QWidget *parent, medusa::Medusa const& core, medusa::Address const& funcAddr);
};

#endif // !__SEMANTIC_VIEW_HPP__