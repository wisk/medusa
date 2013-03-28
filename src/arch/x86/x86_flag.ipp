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

template<u32 Flag, u8 Value> static Expression* ModifyFlag(CpuInformation const* pCpuInfo)
{
  if (Flag == 0x0) return nullptr;
  auto pExpr = new OperationExpression(
      OperationExpression::OpAff,
      new IdentifierExpression(Flag, pCpuInfo),
      new ConstantExpression(ConstantExpression::Const1Bit, Value));
  return pExpr;
}

/*

template<u32 Flag> struct UpdateFlag
{
  void operator()(Expression::List& rExprList, Expression* pRefExpr, CpuInformation const* pCpuInfo, u32 Bit)
  {
  }
};

template<> struct UpdateFlag<X86_FlPf>
{
  void operator()(Expression::List& rExprList, Expression* pRefExpr, CpuInformation const* pCpuInfo, u32 Bit)
  {
  }
};

template<> struct UpdateFlag<X86_FlAf>
{
  void operator()(Expression::List& rExprList, Expression* pRefExpr, CpuInformation const* pCpuInfo, u32 Bit)
  {
    auto pExpr = new IfElseConditionExpression(
        ConditionExpression::CondEq,
        new OperationExpression(OperationExpression::OpAnd,
            pRefExpr,
            new ConstantExpression(Bit, (1 << 4))),
        new ConstantExpression(Bit, (1 << 4)),
        ModifyFlag<X86_FlAf, 1>(pCpuInfo),
        ModifyFlag<X86_FlAf, 0>(pCpuInfo));
    rExprList.push_back(pExpr);
  }
};

template<> struct UpdateFlag<X86_FlZf>
{
  void operator()(Expression::List& rExprList, Expression* pRefExpr, CpuInformation const* pCpuInfo, u32 Bit)
  {
    auto pExpr = new IfElseConditionExpression(
      ConditionExpression::CondEq,
      pRefExpr,
      new ConstantExpression(Bit, 0),
      ModifyFlag<X86_FlZf, 1>(pCpuInfo),
      ModifyFlag<X86_FlZf, 0>(pCpuInfo));
    rExprList.push_back(pExpr);
  }
};

template<> struct UpdateFlag<X86_FlSf>
{
  void operator()(Expression::List& rExprList, Expression* pRefExpr, CpuInformation const* pCpuInfo, u32 Bit)
  {
    auto pExpr = new IfElseConditionExpression(
        ConditionExpression::CondEq,
        new OperationExpression(OperationExpression::OpAnd,
            pRefExpr,
            new ConstantExpression(Bit, (1 << (Bit - 1)))),
        new ConstantExpression(Bit, (1 << (Bit - 1))),
        ModifyFlag<X86_FlSf, 1>(pCpuInfo),
        ModifyFlag<X86_FlSf, 0>(pCpuInfo));
    rExprList.push_back(pExpr);
  }
};

template<> struct UpdateFlag<X86_FlOf>
{
  void operator()(Expression::List& rExprList, Expression* pRefExpr, CpuInformation const* pCpuInfo, u32 Bit)
  {
  }
};

template<u32 Flags, u32 CurFlag = 0> struct UpdateFlags
{
  static const u32 NextFlag = 1 << CurFlag;
  void operator()(Expression::List& rExprList, Expression* pRefExpr, CpuInformation const* pCpuInfo, u32 Bit)
  {
    UpdateFlag<Flags & (1 << CurFlag)> UpdateFlg;
    UpdateFlg(rExprList, pCpuInfo);
    UpdateFlags<Flags, NextFlag> UpdateNextFlags;
    UpdateNextFlags(rExprList, pCpuInfo);
  }
};

*/
