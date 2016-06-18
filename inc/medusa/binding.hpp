#ifndef MEDUSA_BINDING_HPP
#define MEDUSA_BINDING_HPP

#include "medusa/export.hpp"
#include "medusa/namespace.hpp"
#include "medusa/endian.hpp"
#include "medusa/medusa.hpp"

#include <string>

MEDUSA_NAMESPACE_BEGIN

class MEDUSA_EXPORT Binding
{
public:
  Binding(void);
  virtual ~Binding(void);

  virtual std::string GetName(void) const = 0;
  virtual bool Bind(Medusa& rCore) = 0;
  virtual bool Unbind(Medusa& rCore) = 0;
  virtual bool Execute(std::string const& rCode) = 0;
};

typedef Binding* (*TGetBinding)(void);

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_BINDING_HPP
