#include "medusa/expression.hpp"

#include <pegtl.hh>
#include <pegtl/trace.hh>

MEDUSA_NAMESPACE_BEGIN

namespace grammar
{
  // Misc
  struct sep : pegtl::seq< pegtl::one< ',' >, pegtl::opt< pegtl::space > > {};

  /// Prefixes
  struct pref_bv   : pegtl_string_t( "bv" )   {};
  struct pref_id   : pegtl_string_t( "Id" )   {};
  struct pref_sym  : pegtl_string_t( "Sym" )  {};
  struct pref_var  : pegtl_string_t( "Var" )  {};
  struct pref_mem  : pegtl_string_t( "Mem" )  {};
  struct pref_addr : pegtl_string_t( "Addr" ) {};

  /// Literals
  struct lit_dec  : pegtl::plus<pegtl::digit> {};
  struct lit_hex  : pegtl::seq< pegtl_string_t( "0x" ), pegtl::plus< pegtl::xdigit > > {};
  struct lit_str  : pegtl::seq< pegtl::one< '"' >, pegtl::plus< pegtl::sor< pegtl::alnum, pegtl::space > >, pegtl::one< '"' > > {};

  /// Address
  struct addr_pref : pegtl::opt< pegtl::one< '*', '+' > > {};
  struct addr_base : pegtl::opt< pegtl::seq< pegtl::plus< pegtl::xdigit >, pegtl::one< ':' > > > {};
  struct addr_off  : pegtl::plus< pegtl::xdigit > {};
  struct lit_addr  : pegtl::seq< addr_pref, addr_base, addr_off > {};

  /// Variable type
  struct var_alloc : pegtl_string_t( "alloc" ) {};
  struct var_use   : pegtl_string_t( "use" )   {};
  struct var_free  : pegtl_string_t( "free" )  {};
  struct var_type  : pegtl::sor< var_alloc, var_use, var_free > {};

  /// Operator

  //// unary
  struct op_un_not  : pegtl::one< '~' > {};
  struct op_un_neg  : pegtl::one< '-' > {};

  struct op_un_fneg : pegtl_string_t( "-{f}" ) {};

  struct op_un_sym  : pegtl::sor<
    op_un_not, op_un_neg,
    op_un_fneg
  > {};

  struct op_un_swap : pegtl_string_t( "byte_swap" ) {};
  struct op_un_bsf  : pegtl_string_t( "bsf " ) {};
  struct op_un_bsr  : pegtl_string_t( "bsr" ) {};
  struct op_un_fn   : pegtl::sor<
    op_un_swap, op_un_bsf, op_un_bsr
  > {};

  //// binary
  struct op_bin_bcast   : pegtl_string_t( "bcast"        ) {};
  struct op_bin_sext    : pegtl_string_t( "sign_extend"  ) {};
  struct op_bin_zext    : pegtl_string_t( "zero_extend"  ) {};
  struct op_bin_insbits : pegtl_string_t( "insert_bits"  ) {};
  struct op_bin_extbits : pegtl_string_t( "extract_bits" ) {};

  struct op_bin_lls : pegtl_string_t( "<<"    ) {};
  struct op_bin_lrs : pegtl_string_t( ">>{u}" ) {};
  struct op_bin_ars : pegtl_string_t( ">>{s}" ) {};
  struct op_bin_rol : pegtl_string_t( "rol"   ) {};
  struct op_bin_ror : pegtl_string_t( "ror"   ) {};

  struct op_bin_and : pegtl::one< '&' > {};
  struct op_bin_or  : pegtl::one< '|' > {};
  struct op_bin_xor : pegtl::one< '^' > {};

  struct op_bin_add  : pegtl::one< '+' > {};
  struct op_bin_sub  : pegtl::one< '-' > {};
  struct op_bin_mul  : pegtl::one< '*' > {};
  struct op_bin_udiv : pegtl_string_t( "/{u}" ) {};
  struct op_bin_sdiv : pegtl_string_t( "/{s}" ) {};
  struct op_bin_umod : pegtl_string_t( "%{u}" ) {};
  struct op_bin_smod : pegtl_string_t( "%{s}" ) {};

  struct op_bin_fadd : pegtl_string_t("+{f}") {};
  struct op_bin_fsub : pegtl_string_t("-{f}") {};
  struct op_bin_fmul : pegtl_string_t("*{f}") {};
  struct op_bin_fdiv : pegtl_string_t("/{f}") {};
  struct op_bin_fmod : pegtl_string_t("%{f}") {};

  struct op_bin_sym : pegtl::sor<
    op_bin_lls, op_bin_lrs, op_bin_ars,
    op_bin_and, op_bin_or, op_bin_xor,
    op_bin_add, op_bin_sub, op_bin_mul, op_bin_udiv, op_bin_sdiv, op_bin_umod, op_bin_smod
  > {};


  struct op_bin_fn : pegtl::sor<
    op_bin_rol, op_bin_ror,
    op_bin_bcast, op_bin_sext, op_bin_zext,
    op_bin_insbits, op_bin_extbits
  > {};

  /// Symbolic

  struct sym_unk     : pegtl_string_t( "unknown" ) {};
  struct sym_retval  : pegtl_string_t( "retval"  ) {};
  struct sym_parm    : pegtl_string_t( "parm"    ) {};
  struct sym_extval  : pegtl_string_t( "extval"  ) {};
  struct sym_extfunc : pegtl_string_t( "extfunc" ) {};
  struct sym_undef   : pegtl_string_t( "undef"   ) {};
  struct sym_type    : pegtl::sor<
    sym_unk, sym_retval, sym_parm, sym_extval, sym_extfunc, sym_undef
  > {};

  //// contional operator
  // TODO(wisk):

  /// Expressions
  struct expr;

  struct expr_bv                : pegtl::seq< pref_bv,  lit_dec, pegtl::one< '(' >, lit_hex,                           pegtl::one< ')' > > {};
  struct expr_id                : pegtl::seq< pref_id,  lit_dec, pegtl::one< '(' >, pegtl::identifier,                 pegtl::one< ')' > > {};
  struct expr_var_use           : pegtl::seq< pref_var, lit_dec, pegtl::one< '[' >, var_use,                           pegtl::one< ']' >, pegtl::space, pegtl::identifier > {};
  struct expr_var_alloc_or_free : pegtl::seq< pref_var, lit_dec, pegtl::one< '[' >, pegtl::sor< var_alloc, var_free >, pegtl::one< ']' >, pegtl::space, pegtl::identifier > {};

  struct expr_sym_with_expr : pegtl::seq<
    pref_sym, pegtl::one< '(' >,
    sym_type, sep,
    lit_str, sep,
    lit_addr, sep,
    pegtl::sor< expr_bv, expr_id, expr_var_use >, pegtl::one< ')' >
  > {};
  struct expr_sym_without_expr : pegtl::seq<
    pref_sym, pegtl::one< '(' >,
    sym_type, sep,
    lit_str, sep,
    lit_addr, pegtl::one< ')' >
  > {};
  struct expr_sym : pegtl::sor< expr_sym_with_expr, expr_sym_without_expr > {};

  struct expr_un_op  : pegtl::seq< op_un_sym, pegtl::one< '(' >, expr, pegtl::one< ')' > > {};
  struct expr_un_fn  : pegtl::seq< op_un_fn, pegtl::one< '(' >, expr, pegtl::one< ')' > > {};
  struct expr_un     : pegtl::sor< expr_un_op, expr_un_fn > {};
  struct expr_bin_op : pegtl::seq< pegtl::one< '(' >, expr, pegtl::space, op_bin_sym, pegtl::space, expr, pegtl::one< ')' > > {};
  struct expr_bin_fn : pegtl::seq< op_bin_fn, pegtl::one< '(' >, expr, sep, expr, pegtl::one< ')' > > {};
  struct expr_bin    : pegtl::sor< expr_bin_op, expr_bin_fn > {};

  struct expr_mem;
  struct expr_mem_base : pegtl::sor< expr_bv, expr_id > {};
  struct expr_mem_off : pegtl::sor< expr_bv, expr_id, expr_var_use, expr_sym, expr_mem, expr_un, expr_bin > {};
    struct expr_mem_with_base : pegtl::seq<
    pref_mem, lit_dec, pegtl::one< '(' >,
    expr_mem_base, pegtl::one< ':' >,
    expr_mem_off,
    pegtl::one< ')' >
    > {};
  struct expr_mem_without_base : pegtl::seq<
    pref_mem, lit_dec, pegtl::one< '(' >,
    expr_mem_off,
    pegtl::one< ')' >
  > {};
  struct expr_mem : pegtl::sor< expr_mem_with_base, expr_mem_without_base > {}; // FIXME(wisk): the base expression already pushes an expression...

  struct expr_addr_with_base : pegtl::seq<
    pref_addr, lit_dec, pegtl::one< '(' >,
    expr_mem_base, pegtl::one< ':' >,
    expr_mem_off,
    pegtl::one< ')' >
  > {};
  struct expr_addr_without_base : pegtl::seq<
    pref_addr, lit_dec, pegtl::one< '(' >,
    expr_mem_off,
    pegtl::one< ')' >
  > {};
  struct expr_addr : pegtl::sor< expr_addr_with_base, expr_addr_without_base > {}; // FIXME(wisk): same issue here (see above)

  struct expr_lval   : pegtl::sor< expr_id, expr_var_use, expr_sym, expr_mem > {};
  struct expr_rval   : pegtl::sor< expr_bv, expr_id, expr_var_use, expr_sym, expr_mem, expr_addr, expr_un, expr_bin > {};
  struct expr_assign : pegtl::seq< pegtl::one< '(' >, expr_lval, pegtl::space, pegtl::one< '=' >, pegtl::space, expr_rval, pegtl::one< ')' > > {};

  struct expr        : pegtl::sor< expr_bv, expr_id, expr_var_use, expr_sym, expr_mem, expr_addr, expr_un, expr_bin > {};
  struct start       : pegtl::sor< expr_var_alloc_or_free, expr_assign > {};
}

// Actions

/// state
class ExpressionState
{
public:
  ExpressionState(Expression::VSPType& rParsedExpressions, CpuInformation const& rCpuInfo, u8 Mode)
    : m_rResExpr(rParsedExpressions)
    , m_rCpuInfo(rCpuInfo), m_Mode(Mode)
  {}

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

  void AddParsedExpression(Expression::SPType spParsedExpression)
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

  void PushOperationType(OperationExpression::Type OpType)  { m_OpTypeStk.push(OpType);              }
  bool PopOperationType(OperationExpression::Type& rOpType) { return _SafePop(m_OpTypeStk, rOpType); }

  void PushVariableType(VariableExpression::Type VarType)  { m_VarTypeStk.push(VarType);              }
  bool PopVariableType(VariableExpression::Type& rVarType) { return _SafePop(m_VarTypeStk, rVarType); }

  void PushSymbolicType(SymbolicExpression::Type SymType)  { m_SymTypeStk.push(SymType);              }
  bool PopSymbolicType(SymbolicExpression::Type& rSymType) { return _SafePop(m_SymTypeStk, rSymType); }

  void PushBitSize(u16 BitSize)  { m_BsStk.push(BitSize);              }
  bool PopBitSize(u16& rBitSize) { return _SafePop(m_BsStk, rBitSize); }

  void PushBitVector(BitVector const& rBitVector) { m_BvStk.push(rBitVector);             }
  bool PopBitVector(BitVector& rBitVector)        { return _SafePop(m_BvStk, rBitVector); }

  void PushIdentifier(std::string const& rIdentifier) { m_IdStk.push(rIdentifier);             }
  bool PopIdentifier(std::string& rIdentifier)        { return _SafePop(m_IdStk, rIdentifier); }

  void PushAddress(Address const& rAddress) { m_AddrStk.push(rAddress);             }
  bool PopAddress(Address& rAddress)        { return _SafePop(m_AddrStk, rAddress); }

private:
  template<typename T> bool _SafePop(std::stack<T>& rStack, T& rPoppedValue)
  {
    if (rStack.empty())
      return false;
    rPoppedValue = rStack.top();
    rStack.pop();
    return true;
  }


  Expression::VSPType& m_rResExpr;
  CpuInformation const& m_rCpuInfo;
  u8 m_Mode;

  std::stack<Expression::SPType>        m_ExprStk;
  std::stack<OperationExpression::Type> m_OpTypeStk;
  std::stack<VariableExpression::Type>  m_VarTypeStk;
  std::stack<SymbolicExpression::Type>  m_SymTypeStk;
  std::stack<u16>                       m_BsStk;
  std::stack<BitVector>                 m_BvStk;
  std::stack<std::string>               m_IdStk;
  std::stack<Address>                   m_AddrStk;
};


namespace action
{
  /// Default (do nothing)
  template< typename Rule > struct make_expression : pegtl::nothing< Rule > {};

  template<> struct make_expression<grammar::lit_dec> {
    static void apply(pegtl::action_input const& in, ExpressionState& rExprState) {
      rExprState.PushBitSize(static_cast<u16>(std::stol(in.string())));
    }
  };

  template<> struct make_expression<grammar::lit_str> {
    static void apply(pegtl::action_input const& in, ExpressionState& rExprState) {
      auto Str = in.string();
      Str.erase(0, 1); // remove first "
      Str.erase(Str.size() - 1); // remove last "
      rExprState.PushIdentifier(Str);
    }
  };

  template<> struct make_expression<grammar::lit_addr> {
    static void apply(pegtl::action_input const& in, ExpressionState& rExprState) {
      Address Addr(in.string());
      rExprState.PushAddress(Addr);
    }
  };

  template<> struct make_expression<pegtl::identifier> {
    static void apply(pegtl::action_input const& in, ExpressionState& rExprState) {
      rExprState.PushIdentifier(in.string());
    }
  };

  template<> struct make_expression<grammar::expr_bv>
  {
    static void apply(pegtl::action_input const& in, ExpressionState& rExprState)
    {
      u16 BitSize;
      if (!rExprState.PopBitSize(BitSize))
        return;
      auto ValueString = in.string();
      auto Beg = ValueString.find('(') + 1, End = ValueString.rfind(')');
      auto Value = ValueString.substr(Beg, End - Beg);
      auto spBv = Expr::MakeBitVector(BitSize, ap_uint(Value));
      rExprState.Push(spBv);
    }
  };

  template<> struct make_expression<grammar::expr_id> {
    static void apply(pegtl::action_input const& in, ExpressionState& rExprState) {
      u16 IdBitSize;
      if (!rExprState.PopBitSize(IdBitSize))
        return;
      std::string IdStr;
      if (!rExprState.PopIdentifier(IdStr))
        return;
      auto Id = rExprState.GetCpuInformation().ConvertNameToIdentifier(IdStr);
      if (IdBitSize != rExprState.GetCpuInformation().GetSizeOfRegisterInBit(Id))
        return;
      auto spId = Expr::MakeId(Id, &rExprState.GetCpuInformation());
      rExprState.Push(spId);
    }
  };

  template<bool Dereference, bool WithBase> struct make_memory_expression {
    static void apply(pegtl::action_input const& in, ExpressionState& rExprState) {
      u16 AccBitSize;
      if (!rExprState.PopBitSize(AccBitSize))
        return;
      Expression::SPType spBaseExpr;
      auto spOffExpr  = rExprState.Pop();
      if (WithBase)
        spBaseExpr = rExprState.Pop();
      auto spMemExpr = Expr::MakeMem(AccBitSize, spBaseExpr, spOffExpr, Dereference);
      rExprState.Push(spMemExpr);
    }
  };
  template<> struct make_expression<grammar::expr_mem_with_base>     : make_memory_expression<true,  true>  {};
  template<> struct make_expression<grammar::expr_mem_without_base>  : make_memory_expression<true,  false> {};
  template<> struct make_expression<grammar::expr_addr_with_base>    : make_memory_expression<false, true>  {};
  template<> struct make_expression<grammar::expr_addr_without_base> : make_memory_expression<false, false> {};


  /// Operation
  template< OperationExpression::Type OpType > struct operation_type_action {
    static void apply(pegtl::action_input const& in, ExpressionState& rExprState) {
      rExprState.PushOperationType(OpType);
    }
  };
  template<> struct make_expression<grammar::op_un_not>      : operation_type_action<OperationExpression::OpNot>         {};
  template<> struct make_expression<grammar::op_un_neg>      : operation_type_action<OperationExpression::OpNeg>         {};
  template<> struct make_expression<grammar::op_un_fneg>     : operation_type_action<OperationExpression::OpFNeg>        {};
  template<> struct make_expression<grammar::op_un_swap>     : operation_type_action<OperationExpression::OpSwap>        {};
  template<> struct make_expression<grammar::op_un_bsf>      : operation_type_action<OperationExpression::OpBsf>         {};
  template<> struct make_expression<grammar::op_un_bsr>      : operation_type_action<OperationExpression::OpBsr>         {};
  template<> struct make_expression<grammar::op_bin_sext>    : operation_type_action<OperationExpression::OpSext>        {};
  template<> struct make_expression<grammar::op_bin_zext>    : operation_type_action<OperationExpression::OpZext>        {};
  template<> struct make_expression<grammar::op_bin_insbits> : operation_type_action<OperationExpression::OpInsertBits>  {};
  template<> struct make_expression<grammar::op_bin_extbits> : operation_type_action<OperationExpression::OpExtractBits> {};
  template<> struct make_expression<grammar::op_bin_bcast>   : operation_type_action<OperationExpression::OpBcast>       {};
  template<> struct make_expression<grammar::op_bin_lls>     : operation_type_action<OperationExpression::OpLls>         {};
  template<> struct make_expression<grammar::op_bin_lrs>     : operation_type_action<OperationExpression::OpLrs>         {};
  template<> struct make_expression<grammar::op_bin_ars>     : operation_type_action<OperationExpression::OpArs>         {};
  template<> struct make_expression<grammar::op_bin_rol>     : operation_type_action<OperationExpression::OpRol>         {};
  template<> struct make_expression<grammar::op_bin_ror>     : operation_type_action<OperationExpression::OpRor>         {};
  template<> struct make_expression<grammar::op_bin_and>     : operation_type_action<OperationExpression::OpAnd>         {};
  template<> struct make_expression<grammar::op_bin_or>      : operation_type_action<OperationExpression::OpOr>          {};
  template<> struct make_expression<grammar::op_bin_xor>     : operation_type_action<OperationExpression::OpXor>         {};
  template<> struct make_expression<grammar::op_bin_add>     : operation_type_action<OperationExpression::OpAdd>         {};
  template<> struct make_expression<grammar::op_bin_sub>     : operation_type_action<OperationExpression::OpSub>         {};
  template<> struct make_expression<grammar::op_bin_mul>     : operation_type_action<OperationExpression::OpMul>         {};
  template<> struct make_expression<grammar::op_bin_udiv>    : operation_type_action<OperationExpression::OpUDiv>        {};
  template<> struct make_expression<grammar::op_bin_sdiv>    : operation_type_action<OperationExpression::OpSDiv>        {};
  template<> struct make_expression<grammar::op_bin_umod>    : operation_type_action<OperationExpression::OpUMod>        {};
  template<> struct make_expression<grammar::op_bin_smod>    : operation_type_action<OperationExpression::OpSMod>        {};
  template<> struct make_expression<grammar::op_bin_fadd>    : operation_type_action<OperationExpression::OpFAdd>        {};
  template<> struct make_expression<grammar::op_bin_fsub>    : operation_type_action<OperationExpression::OpFSub>        {};
  template<> struct make_expression<grammar::op_bin_fmul>    : operation_type_action<OperationExpression::OpFMul>        {};
  template<> struct make_expression<grammar::op_bin_fdiv>    : operation_type_action<OperationExpression::OpFDiv>        {};
  template<> struct make_expression<grammar::op_bin_fmod>    : operation_type_action<OperationExpression::OpFMod>        {};
  template<> struct make_expression<grammar::expr_un> {
    static void apply(pegtl::action_input const& in, ExpressionState& rExprState) {
      OperationExpression::Type UnOpType;
      if (!rExprState.PopOperationType(UnOpType))
        return;
      auto spExpr = rExprState.Pop();
      auto spUn   = Expr::MakeUnOp(UnOpType, spExpr);
      rExprState.Push(spUn);
    }
  };
  template<> struct make_expression<grammar::expr_bin> {
    static void apply(pegtl::action_input const& in, ExpressionState& rExprState) {
      OperationExpression::Type BinOpType;
      if (!rExprState.PopOperationType(BinOpType))
        return;
      auto spRightExpr = rExprState.Pop();
      auto spLeftExpr  = rExprState.Pop();
      auto spBin       = Expr::MakeBinOp(BinOpType, spLeftExpr, spRightExpr);
      rExprState.Push(spBin);
    }
  };

  /// Variable
  template< VariableExpression::Type VarType > struct variable_type_action {
    static void apply(pegtl::action_input const& in, ExpressionState& rExprState) {
      rExprState.PushVariableType(VarType);
    }
  };
  template<> struct make_expression<grammar::var_alloc> : variable_type_action<VariableExpression::Alloc> {};
  template<> struct make_expression<grammar::var_use>   : variable_type_action<VariableExpression::Use>   {};
  template<> struct make_expression<grammar::var_free>  : variable_type_action<VariableExpression::Free>  {};
  template<> struct make_expression<grammar::expr_var_use> {
    static void apply(pegtl::action_input const& in, ExpressionState& rExprState) {
      std::string VarName;
      if (!rExprState.PopIdentifier(VarName))
        return;
      VariableExpression::Type VarType;
      if (!rExprState.PopVariableType(VarType))
        return;
      u16 VarBitSize;
      if (!rExprState.PopBitSize(VarBitSize))
        return;
      auto spVar = Expr::MakeVar(VarName, VarType, VarBitSize);
      rExprState.Push(spVar);
    }
  };
  template<> struct make_expression<grammar::expr_var_alloc_or_free> { // TODO(wisk): code dup
    static void apply(pegtl::action_input const& in, ExpressionState& rExprState) {
      std::string VarName;
      if (!rExprState.PopIdentifier(VarName))
        return;
      VariableExpression::Type VarType;
      if (!rExprState.PopVariableType(VarType))
        return;
      u16 VarBitSize;
      if (!rExprState.PopBitSize(VarBitSize))
        return;
      auto spVar = Expr::MakeVar(VarName, VarType, VarBitSize);
      rExprState.Push(spVar);
    }
  };

  /// Symbolic
  template< SymbolicExpression::Type SymType > struct symbolic_type_action {
    static void apply(pegtl::action_input const& in, ExpressionState& rExprState) {
      rExprState.PushSymbolicType(SymType);
    }
  };
  template<> struct make_expression<grammar::sym_unk>     : symbolic_type_action<SymbolicExpression::Unknown> {};
  template<> struct make_expression<grammar::sym_retval>  : symbolic_type_action<SymbolicExpression::ReturnedValue> {};
  template<> struct make_expression<grammar::sym_parm>    : symbolic_type_action<SymbolicExpression::FromParameter> {};
  template<> struct make_expression<grammar::sym_extval>  : symbolic_type_action<SymbolicExpression::ExternalValue> {};
  template<> struct make_expression<grammar::sym_extfunc> : symbolic_type_action<SymbolicExpression::ExternalFunction> {};
  template<> struct make_expression<grammar::sym_undef>   : symbolic_type_action<SymbolicExpression::Undefined> {};
  template<> struct make_expression<grammar::expr_sym_with_expr> {
    static void apply(pegtl::action_input const& in, ExpressionState& rExprState)
    {
      SymbolicExpression::Type SymType;
      if (!rExprState.PopSymbolicType(SymType))
        return;
      std::string SymVal;
      if (!rExprState.PopIdentifier(SymVal))
        return;
      Address SymAddr;
      if (!rExprState.PopAddress(SymAddr))
        return;
      auto spExpr = rExprState.Pop();
      if (spExpr == nullptr)
        return;
      auto spSym = Expr::MakeSym(SymType, SymVal, SymAddr, spExpr);
      rExprState.Push(spSym);
    }
  };
  template<> struct make_expression<grammar::expr_sym_without_expr> {
    static void apply(pegtl::action_input const& in, ExpressionState& rExprState) {
      SymbolicExpression::Type SymType;
      if (!rExprState.PopSymbolicType(SymType))
        return;
      std::string SymVal;
      if (!rExprState.PopIdentifier(SymVal))
        return;
      Address SymAddr;
      if (!rExprState.PopAddress(SymAddr))
        return;
      auto spSym = Expr::MakeSym(SymType, SymVal, SymAddr, nullptr);
      rExprState.Push(spSym);
    }
  };

  template<>
  struct make_expression<grammar::expr_assign> {
    static void apply(pegtl::action_input const& in, ExpressionState& rExprState) {
      auto spSrc = rExprState.Pop();
      auto spDst = rExprState.Pop();
      if (spSrc == nullptr || spDst == nullptr)
        return;
      auto spAssign = Expr::MakeAssign(spDst, spSrc);
      rExprState.AddParsedExpression(spAssign);
    }
  };

}

Expression::VSPType Expression::Parse(std::string const& rExpressions, CpuInformation const& rCpuInfo, u8 Mode)
{
  Expression::VSPType ParsedExprs;
  ExpressionState ExprState(ParsedExprs, rCpuInfo, Mode);
  //pegtl::trace_string<grammar::start>(rExpressions, __FILE__, ExprState);
  if (!pegtl::parse_string<grammar::start, action::make_expression>(rExpressions, __FILE__, ExprState))
    Log::Write("core").Level(LogError) << "failed to parse expressions" << LogEnd;
  ExprState.Finalize();
  return ParsedExprs;
}

MEDUSA_NAMESPACE_END
