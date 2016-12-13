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

  std::string GetFormat(void) const;
  std::string GetEntryPoint(void) const;

  void SetFormat(std::string const& rFormat = "native");
  void SetEntryPoint(std::string const& rLabelName = "start");

  virtual bool AddCode(std::string const& rCodeName, Expression::VSPType const& rCode);
  virtual bool AddData(std::string const& rDataName, std::vector<u8> const& rData);

  //virtual bool AddFunction(FunctionDetail const& rFunction, Address const& rAddress) = 0;

  virtual bool Compile(Path const& rOutputFile);
  virtual bool Compile(std::vector<u8>& rBuffer) = 0;

protected:
  std::string m_Format = "native";
  std::string m_EntryPoint = "start";

  std::unordered_map<std::string, Expression::VSPType> m_CodeMap;
  std::unordered_map<std::string, std::vector<u8>> m_DataMap;
};

typedef Compiler* (*TGetCompiler)(void);

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_COMPILATION_HPP
