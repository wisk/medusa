#include "medusa/expression.hpp"

#include <pegtl.hh>
#include <pegtl/trace.hh>

MEDUSA_NAMESPACE_BEGIN

namespace grammar
{

  /// Prefixes
  struct pref_bv   : pegtl::string< 'b', 'v' > {};
  struct pref_id   : pegtl::string< 'I', 'd'> {};
  struct pref_sym  : pegtl::string< 'S', 'y', 'm' > {};
  struct pref_var  : pegtl::string< 'V', 'a', 'r'> {};
  struct pref_mem  : pegtl::string< 'M', 'e', 'n'> {};
  struct pref_addr : pegtl::string< 'A', 'd', 'd', 'r' > {};

  /// Literals
  struct lit_dec : pegtl::plus<pegtl::digit> {};
  struct lit_hex : pegtl::seq< pegtl::string< '0', 'x' >, pegtl::plus< pegtl::xdigit > > {};

  /// Variable type
  struct var_alloc : pegtl::string< 'a', 'l', 'l', 'o', 'c' > {};
  struct var_use   : pegtl::string< 'u', 's', 'e' > {};
  struct var_free  : pegtl::string< 'f', 'r', 'e', 'e' > {};
  struct var_type  : pegtl::sor< var_alloc, var_use, var_free > {};

  /// Operator

  //// unary
  struct op_un_not : pegtl::one< '~' > {};
  struct op_un_neg : pegtl::one< '-' > {};

  struct op_un_sym : pegtl::sor< op_un_not, op_un_neg > {};

  //// binary
  struct op_bin_lls : pegtl::string< '<', '<' > {};
  struct op_bin_lrs : pegtl::string< '>', '>' > {};

  struct op_bin_and : pegtl::one< '&' > {};
  struct op_bin_or  : pegtl::one< '|' > {};
  struct op_bin_xor : pegtl::one< '^' > {};

  struct op_bin_add : pegtl::one< '+' > {};
  struct op_bin_sub : pegtl::one< '-' > {};
  struct op_bin_mul : pegtl::one< '*' > {};
  struct op_bin_div : pegtl::one< '/' > {};
  struct op_bin_mod : pegtl::one< '%' > {};

  struct op_bin_sym : pegtl::sor< op_bin_lls, op_bin_lrs, op_bin_and, op_bin_or, op_bin_xor, op_bin_add, op_bin_sub, op_bin_mul, op_bin_div, op_bin_mod > {};

  struct op_bin_bcast : pegtl::string< 'b', 'c', 'a', 's', 't' > {};

  struct op_bin_fn : pegtl::sor< op_bin_bcast > {};


  //// contional operator
  // TODO(wisk):

  /// Expressions
  struct expr;

  struct expr_bv     : pegtl::seq< pref_bv, lit_dec, pegtl::one< '(' >, lit_hex, pegtl::one< ')' > > {};
  struct expr_id     : pegtl::seq< pref_id, lit_dec, pegtl::one< '(' >, pegtl::identifier, pegtl::one< ')' > > {};
  struct expr_var    : pegtl::seq< pref_var, lit_dec, pegtl::one< '[' >, var_type, pegtl::one< ']' >, pegtl::space, pegtl::identifier > {};

  struct expr_mem    : pegtl::seq< pref_mem, lit_dec, pegtl::one< '(' >, pegtl::opt< expr, pegtl::one< ','> >, expr, pegtl::one< ')' > > {};

  struct expr_un     : pegtl::seq< op_un_sym, expr > {};
  struct expr_bin_op : pegtl::seq< pegtl::one< '(' >, expr, op_bin_sym, expr, pegtl::one< ')' > > {};
  struct expr_bin_fn : pegtl::seq< op_bin_fn, pegtl::one< '(' >, expr, pegtl::one< ',' >, expr, pegtl::one< ')' > > {};
  struct expr_bin    : pegtl::sor< expr_bin_op, expr_bin_fn > {};

  struct expr_assign : pegtl::seq< pegtl::one< '(' >, expr, pegtl::space, pegtl::one< '=' >, pegtl::space, expr, pegtl::one< ')' > > {};

  struct expr        : pegtl::sor< expr_bv, expr_id, expr_var, expr_un, expr_bin > {};
  struct start       : pegtl::sor< expr_assign > {};
}

// Actions

/// state
class ExpressionState
{
public:
  ExpressionState(Expression::VSPType& rParsedExpressions, CpuInformation const& rCpuInfo, u8 Mode)
    : m_rResExpr(rParsedExpressions)
    , m_rCpuInfo(rCpuInfo), m_Mode(Mode)
  {

  }

  CpuInformation const& GetCpuInformation(void) const
  {
    return m_rCpuInfo;
  }

  u8 GetMode(void) const
  {
    return m_Mode;
  }

  void Push(Expression::SPType spExpression)
  {
    m_ExprStk.push(spExpression);
  }

  Expression::SPType Pop(void)
  {
    if (m_ExprStk.empty())
      return nullptr;
    auto PoppedExpr = m_ExprStk.top();
    m_ExprStk.pop();
    return PoppedExpr;
  }

  void AddParseExpression(Expression::SPType spParsedExpression)
  {
    m_rResExpr.push_back(spParsedExpression);
  }

  bool Finalize(void)
  {
    if (m_ExprStk.empty())
      return true;
    if (m_ExprStk.size() >= 2)
      return false;
    m_rResExpr.push_back(m_ExprStk.top());
    m_ExprStk.pop();
    return true;
  }

  OperationExpression::Type GetOperationType(void) const { return m_OpType; }
  void SetOperationType(OperationExpression::Type OpType) { m_OpType = OpType; }

  VariableExpression::Type GetVariableType(void) const { return m_VarType; }
  void SetVariableType(VariableExpression::Type VarType) { m_VarType = VarType; }

  u16 GetBitSize(void) const { return m_BitSize; }
  void SetBitSize(u16 BitSize) { m_BitSize = BitSize; }

  BitVector GetBitVector(void) const { return m_BitVector; }
  void SetBitVector(BitVector const& rBitVector) { m_BitVector = rBitVector; }

  std::string GetIdentifier(void) const { return m_Identifier; }
  void SetIdentifier(std::string const& rIdentifier) { m_Identifier = rIdentifier; }

private:
  Expression::VSPType& m_rResExpr;
  CpuInformation const& m_rCpuInfo;
  u8 m_Mode;

  std::stack<Expression::SPType> m_ExprStk;
  OperationExpression::Type m_OpType = OperationExpression::OpUnk;
  VariableExpression::Type m_VarType = VariableExpression::Unknown;
  u16 m_BitSize = 0;
  BitVector m_BitVector;
  std::string m_Identifier;
};

/// Default (do nothing)

namespace action
{
  template< typename Rule >
  struct make_expression : pegtl::nothing< Rule > {};

  template<> struct make_expression<grammar::lit_dec> {
    static void apply(pegtl::action_input const& in, ExpressionState& rExprState) {
      rExprState.SetBitSize(static_cast<u16>(std::stol(in.string())));
    }
  };

  template<> struct make_expression<pegtl::identifier> {
    static void apply(pegtl::action_input const& in, ExpressionState& rExprState) {
      rExprState.SetIdentifier(in.string());
    }
  };

  template<>
  struct make_expression<grammar::expr_bv>
  {
    static void apply(pegtl::action_input const& in, ExpressionState& rExprState)
    {
      auto BitSize = rExprState.GetBitSize();
      auto ValueString = in.string();
      auto Beg = ValueString.find('(') + 1, End = ValueString.rfind(')');
      auto Value = ValueString.substr(Beg, End - Beg);
      auto spBv = Expr::MakeBitVector(BitSize, ap_uint(Value));
      rExprState.Push(spBv);
    }
  };

  template<>
  struct make_expression<grammar::expr_id> {
    static void apply(pegtl::action_input const& in, ExpressionState& rExprState) {
      auto Id = rExprState.GetCpuInformation().ConvertNameToIdentifier(rExprState.GetIdentifier());
      auto spId = Expr::MakeId(Id, &rExprState.GetCpuInformation());
      rExprState.Push(spId);
    }
  };

  template<>
  struct make_expression<grammar::expr_assign> {
    static void apply(pegtl::action_input const& in, ExpressionState& rExprState) {
      auto spSrc = rExprState.Pop();
      auto spDst = rExprState.Pop();
      auto spAssign = Expr::MakeAssign(spDst, spSrc);
      rExprState.AddParseExpression(spAssign);
    }
  };

}

Expression::VSPType Expression::Parse(std::string const& rExpressions, CpuInformation const& rCpuInfo, u8 Mode)
{
  Expression::VSPType ParsedExprs;
  ExpressionState ExprState(ParsedExprs, rCpuInfo, Mode);
  //pegtl::trace_string<grammar::start>(rExpressions, __FILE__, ExprState);
  if (!pegtl::parse_string<grammar::start, action::make_expression>(rExpressions, __FILE__, ExprState))
    Log::Write("core").Level(LogError) << "failed to parse expressions";
  ExprState.Finalize();
  return ParsedExprs;
}

MEDUSA_NAMESPACE_END