#include "medusa/view.hpp"

MEDUSA_NAMESPACE_USE;

View::View(Document& rDoc)
  : m_rDoc(rDoc), m_Connect(rDoc.Connect(boost::bind(&View::Update, this, _1)))
{
}

View::~View(void)
{
  m_Connect.disconnect();
}