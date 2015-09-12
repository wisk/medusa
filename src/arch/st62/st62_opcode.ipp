/* This file has been automatically generated, you must _NOT_ edit it directly. (Sat Sep 12 13:13:34 2015) */
enum ST62Opcode
{
  ST62_Opcode_Unknown,
  ST62_Opcode_Add,
  ST62_Opcode_Addi,
  ST62_Opcode_And,
  ST62_Opcode_Andi,
  ST62_Opcode_Call,
  ST62_Opcode_Com,
  ST62_Opcode_Cp,
  ST62_Opcode_Cpi,
  ST62_Opcode_Dec,
  ST62_Opcode_Inc,
  ST62_Opcode_Jp,
  ST62_Opcode_Jrc,
  ST62_Opcode_Jrnc,
  ST62_Opcode_Jrnz,
  ST62_Opcode_Jrr,
  ST62_Opcode_Jrs,
  ST62_Opcode_Jrz,
  ST62_Opcode_Ld,
  ST62_Opcode_Ldi,
  ST62_Opcode_Nop,
  ST62_Opcode_Res,
  ST62_Opcode_Ret,
  ST62_Opcode_Reti,
  ST62_Opcode_Rlc,
  ST62_Opcode_Set,
  ST62_Opcode_Stop,
  ST62_Opcode_Sub,
  ST62_Opcode_Subi,
  ST62_Opcode_Wait
};
private:
  typedef bool (St62Architecture:: *TDisassembler)(BinaryStream const&, TOffset, Instruction&, u8);
  static const TDisassembler m_Table_1[0x10];
  bool Table_1_00(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_01(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_02(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_03(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_04(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_05(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_06(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_07(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_08(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_09(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_0a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_0b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_0c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_0d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_0e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_0f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);

  static const TDisassembler m_Table_5[0x10];
  bool Table_5_00(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_5_01(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_5_02(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_5_03(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_5_04(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_5_05(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_5_06(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_5_07(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_5_08(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_5_09(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_5_0a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_5_0b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_5_0c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_5_0d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_5_0e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_5_0f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);

  static const TDisassembler m_Table_7[0x10];
  bool Table_7_00(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_7_01(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_7_02(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_7_03(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_7_04(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_7_05(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_7_06(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_7_07(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_7_08(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_7_09(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_7_0a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_7_0b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_7_0c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_7_0d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_7_0e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_7_0f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);

  static const TDisassembler m_Table_d[0x10];
  bool Table_d_00(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_d_01(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_d_02(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_d_03(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_d_04(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_d_05(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_d_06(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_d_07(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_d_08(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_d_09(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_d_0a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_d_0b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_d_0c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_d_0d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_d_0e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_d_0f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);

  static const TDisassembler m_Table_f[0x10];
  bool Table_f_00(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_f_01(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_f_02(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_f_03(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_f_04(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_f_05(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_f_06(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_f_07(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_f_08(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_f_09(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_f_0a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_f_0b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_f_0c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_f_0d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_f_0e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_f_0f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);

  bool Operand__A(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_imm(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ee(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rX(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  bool Operand__direct(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rY(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_bitdirect(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_ext(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  bool Operand__W(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  bool Operand__V(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Y(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  bool Operand__X(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_pcr(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_ee(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_rX(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_rY(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Y(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  bool Operand__bitdirect(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  bool Operand__pcr(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_V(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_direct(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  bool Operand__imm(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_W(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ext(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_X(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_A(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode);
