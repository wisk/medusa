#ifndef _MEDUSA_LOADER_
#define _MEDUSA_LOADER_

#include "medusa/export.hpp"
#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/endian.hpp"
#include "medusa/database.hpp"
#include "medusa/configuration.hpp"

#include "medusa/export.hpp"

#include "medusa/architecture.hpp"

#include <vector>
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>

MEDUSA_NAMESPACE_BEGIN

// Don't forget to export a Loader* GetLoader(Database& rDatabase); function
class Medusa_EXPORT         Loader
{
public:
  typedef boost::shared_ptr<Loader> Ptr;
  typedef std::vector<Ptr>          VectorPtr;

                            Loader(Database&) {}
  virtual                  ~Loader(void) {}

  virtual char const*       GetName(void) = 0;
  virtual bool              IsSupported(void) = 0;
  virtual void              Map(void) = 0;
  virtual void              Translate(Address const& rVirtAddr, TOffset& rRawOff) = 0;
  virtual Address           GetEntryPoint(void) = 0;
  virtual Architecture::Ptr GetMainArchitecture(Architecture::VectorPtr const& Architectures) = 0;
  virtual void              Configure(Configuration& rCfg) {};
};

typedef Loader* (*TGetLoader)(Database&);

MEDUSA_NAMESPACE_END

#endif // _MEDUSA_LOADER_