#ifndef __MEDUSA_VIEW_HPP__
#define __MEDUSA_VIEW_HPP__

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/document.hpp"
#include "medusa/event_handler.hpp"

#include <boost/signals2.hpp>

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT View : public Document::Subscriber
{
public:
  View(Document& rDoc);
  virtual ~View(void);

protected:
  Document& m_rDoc;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_VIEW_HPP__