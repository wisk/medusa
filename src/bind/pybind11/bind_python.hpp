#ifndef BIND_PYTHON_HPP
#define BIND_PYTHON_HPP

#include "medusa/types.hpp"
#include "medusa/binding.hpp"

#if defined(_WIN32) || defined(WIN32)
# ifdef bind_python_EXPORTS
#  define BIND_PYTHON_EXPORT __declspec(dllexport)
# else
#  define BIND_PYTHON_EXPORT __declspec(dllimport)
# endif
#else
# define BIND_PYTHON_EXPORT
#endif

MEDUSA_NAMESPACE_USE

extern "C" BIND_PYTHON_EXPORT Binding* GetBinding(void);

class BindingPython : public Binding
{
public:
  BindingPython(void);
  virtual ~BindingPython(void);

  virtual std::string GetName(void) const;
  virtual bool Bind(Medusa& rCore);
  virtual bool Unbind(Medusa& rCore);
  virtual bool Execute(std::string const& rCode);
};

#endif // !BIND_PYTHON_HPP