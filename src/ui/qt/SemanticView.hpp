#ifndef QMEDUSA_SEMANTIC_WIEW_HPP
#define QMEDUSA_SEMANTIC_WIEW_HPP

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

#endif // !QMEDUSA_SEMANTIC_VIEW_HPP
