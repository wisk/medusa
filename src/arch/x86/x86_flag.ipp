template<u32 Flag> struct ClearFlag
{
  void operator()(Expression::List& rExprList, CpuInformation const* pCpuInfo)
  {
    if (Flag == 0x0) return;
    auto pExpr = new OperationExpression(
      OperationExpression::OpAff,
      new IdentifierExpression(Flag, pCpuInfo),
      new ConstantExpression(ConstantExpression::Const1Bit, 0));
    rExprList.push_back(pExpr);
  }
};

template<u32 Flags, u8 CurFlag = 0> struct ClearFlags
{
  static const u32 NextFlag = CurFlag + 1;
  void operator()(Expression::List& rExprList, CpuInformation const* pCpuInfo)
  {
    ClearFlag<Flags & (1 << CurFlag)> ClrFlg;
    ClrFlg(rExprList, pCpuInfo);
    ClearFlags<Flags, NextFlag> ClearNextFlags;
    ClearNextFlags(rExprList, pCpuInfo);
  }
};

template<u32 Flags> struct ClearFlags<Flags, 31>
{
  void operator()(Expression::List& rExprList, CpuInformation const* pCpuInfo)
  {
    ClearFlag<Flags & 0x80000000> ClrFlg;
    ClrFlg(rExprList, pCpuInfo);
  }
};

template<u32 Flag> struct SetFlag
{
  void operator()(Expression::List& rExprList, CpuInformation const* pCpuInfo)
  {
    if (Flag == 0x0) return;
    auto pExpr = new OperationExpression(
      OperationExpression::OpAff,
      new IdentifierExpression(Flag, pCpuInfo),
      new ConstantExpression(ConstantExpression::Const1Bit, 1));
  }
};

template<u32 Flags, u32 CurFlag = 0> struct SetFlags
{
  static const u32 NextFlag = 1 << CurFlag;
  void operator()(Expression::List& rExprList, CpuInformation const* pCpuInfo)
  {
    SetFlag<Flags & (1 << CurFlag)> SetFlg;
    SetFlg(rExprList, pCpuInfo);
    SetFlags<Flags, NextFlag> SetNextFlags;
    SetNextFlags(rExprList, pCpuInfo);
  }
};

template<u32 Flag> struct UpdateFlag
{
  void operator()(Expression::List& rExprList, Expression* pRefExpr, CpuInformation const* pCpuInfo)
  {
    assert(0);
    return nullptr;
  }
};

template<> struct UpdateFlag<X86_FlZf>
{
  void operator()(Expression::List& rExprList, Expression* pRefExpr, CpuInformation const* pCpuInfo)
  {
  /*
    SetFlag<X86_FlZf>   SetZFlag;
    ClearFlag<X86_FlZf> ClearZFlag;

    auto pExpr = new IfElseConditionExpression(
      ConditionExpression::CondEq,
      pRefExpr,
      new ConstantExpression(0, 0),
      SetZFlag(pCpuInfo),
      ClearZFlag(pCpuInfo));
    rExprList.push_back(pExpr);
	*/
  }
};
