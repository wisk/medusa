#include "medusa/view.hpp"

MEDUSA_NAMESPACE_USE;

View::View(Document& rDoc)
  : Document::Subscriber()
  , m_rDoc(rDoc)
{
}

View::~View(void)
{
}