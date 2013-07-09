#include "SemanticView.hpp"

SemanticView::SemanticView(QWidget *parent, medusa::Medusa const& core, medusa::Address const& funcAddr)
  : QTextEdit(parent)
{
  setReadOnly(true);
}