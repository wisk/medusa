#include "medusa/view.hpp"

MEDUSA_NAMESPACE_USE;

View::View(u32 Type, Document& rDoc)
  : Document::Subscriber()
  , m_rDoc(rDoc)
{
  rDoc.Connect(Type, this);
}

View::~View(void)
{
}