#ifndef __SEMANTIC_WIEW_HPP__
#define __SEMANTIC_WIEW_HPP__

# include <QtGui>
# include <QTextEdit>
# include <QAbstractScrollArea>
# include <QPainter>
# include <QResizeEvent>
# include <QPaintEvent>

# include <medusa/medusa.hpp>
# include <medusa/address.hpp>
# include <medusa/document.hpp>
# include <medusa/instruction.hpp>
# include <medusa/disassembly_view.hpp>

class SemanticView : public QTextEdit
{
  Q_OBJECT

public:
  SemanticView(QWidget *parent, medusa::Medusa const& core, medusa::Address const& funcAddr);
};

#endif // !__SEMANTIC_VIEW_HPP__
