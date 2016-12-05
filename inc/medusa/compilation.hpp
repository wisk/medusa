#ifndef MEDUSA_COMPILATION_HPP
#define MEDUSA_COMPILATION_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/expression.hpp"
#include "medusa/document.hpp"
#include "medusa/function.hpp"

MEDUSA_NAMESPACE_BEGIN

class MEDUSA_EXPORT Compiler
{
public:
  typedef std::shared_ptr<Compiler> SPType;
  typedef std::vector<SPType> VSPType;

  virtual ~Compiler(void) {}

  virtual std::string GetName(void) const = 0;

  //virtual bool AddLabel(Label const& rLabel, Address const& rAddress) = 0;
  //virtual bool AddFunction(FunctionDetail const& rFunction, Address const& rAddress) = 0;

  void SetEntryPoint(std::string const& rLabelName = "start");

  virtual bool Compile(std::string const& rFormatName, Path const& rOutputFile) = 0;
  virtual bool Compile(std::string const& rFormatName, std::vector<u8>& rBuffer) = 0;

protected:
  std::string m_EntryPoint;
};

typedef Compiler* (*TGetCompiler)(void);

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_COMPILATION_HPP
