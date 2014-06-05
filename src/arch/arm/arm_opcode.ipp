/* This file has been automatically generated, you must _NOT_ edit it directly. (Wed Jun  4 17:41:26 2014) */
enum ARM_Opcode
{
  ARM_Opcode_Unknown,
  ARM_Opcode_Abs,
  ARM_Opcode_Adc,
  ARM_Opcode_Adcs,
  ARM_Opcode_Add,
  ARM_Opcode_Addhn,
  ARM_Opcode_Addhn2,
  ARM_Opcode_Addp,
  ARM_Opcode_Adds,
  ARM_Opcode_Addv,
  ARM_Opcode_Addw,
  ARM_Opcode_Adr,
  ARM_Opcode_Adrp,
  ARM_Opcode_Aesd,
  ARM_Opcode_Aese,
  ARM_Opcode_Aesimc,
  ARM_Opcode_Aesmc,
  ARM_Opcode_And,
  ARM_Opcode_Ands,
  ARM_Opcode_Asr,
  ARM_Opcode_Asrs,
  ARM_Opcode_Asrv,
  ARM_Opcode_B,
  ARM_Opcode_Bfc,
  ARM_Opcode_Bfi,
  ARM_Opcode_Bfm,
  ARM_Opcode_Bic,
  ARM_Opcode_Bics,
  ARM_Opcode_Bif,
  ARM_Opcode_Bit,
  ARM_Opcode_Bkpt,
  ARM_Opcode_Bl,
  ARM_Opcode_Blr,
  ARM_Opcode_Blx,
  ARM_Opcode_Br,
  ARM_Opcode_Brk,
  ARM_Opcode_Bsl,
  ARM_Opcode_Bx,
  ARM_Opcode_Bxj,
  ARM_Opcode_Cb,
  ARM_Opcode_Cbnz,
  ARM_Opcode_Cbz,
  ARM_Opcode_Ccmn,
  ARM_Opcode_Ccmp,
  ARM_Opcode_Cdp,
  ARM_Opcode_Cdp2,
  ARM_Opcode_Chka,
  ARM_Opcode_Clrex,
  ARM_Opcode_Cls,
  ARM_Opcode_Clz,
  ARM_Opcode_Cmeq,
  ARM_Opcode_Cmge,
  ARM_Opcode_Cmgt,
  ARM_Opcode_Cmhi,
  ARM_Opcode_Cmhs,
  ARM_Opcode_Cmle,
  ARM_Opcode_Cmlt,
  ARM_Opcode_Cmn,
  ARM_Opcode_Cmp,
  ARM_Opcode_Cmtst,
  ARM_Opcode_Cnt,
  ARM_Opcode_Csel,
  ARM_Opcode_Csinc,
  ARM_Opcode_Csinv,
  ARM_Opcode_Csneg,
  ARM_Opcode_Dbg,
  ARM_Opcode_Dcps1,
  ARM_Opcode_Dcps2,
  ARM_Opcode_Dcps3,
  ARM_Opcode_Dmb,
  ARM_Opcode_Drps,
  ARM_Opcode_Dsb,
  ARM_Opcode_Dup,
  ARM_Opcode_Eon,
  ARM_Opcode_Eor,
  ARM_Opcode_Eors,
  ARM_Opcode_Eret,
  ARM_Opcode_Ext,
  ARM_Opcode_Extr,
  ARM_Opcode_Fabd,
  ARM_Opcode_Fabs,
  ARM_Opcode_Facge,
  ARM_Opcode_Facgt,
  ARM_Opcode_Fadd,
  ARM_Opcode_Faddp,
  ARM_Opcode_Fccmp,
  ARM_Opcode_Fccmpe,
  ARM_Opcode_Fcmeq,
  ARM_Opcode_Fcmge,
  ARM_Opcode_Fcmgt,
  ARM_Opcode_Fcmle,
  ARM_Opcode_Fcmlt,
  ARM_Opcode_Fcmp,
  ARM_Opcode_Fcmpe,
  ARM_Opcode_Fcsel,
  ARM_Opcode_Fcvt,
  ARM_Opcode_Fcvtas,
  ARM_Opcode_Fcvtau,
  ARM_Opcode_Fcvtl,
  ARM_Opcode_Fcvtl2,
  ARM_Opcode_Fcvtms,
  ARM_Opcode_Fcvtmu,
  ARM_Opcode_Fcvtn,
  ARM_Opcode_Fcvtn2,
  ARM_Opcode_Fcvtns,
  ARM_Opcode_Fcvtnu,
  ARM_Opcode_Fcvtps,
  ARM_Opcode_Fcvtpu,
  ARM_Opcode_Fcvtxn,
  ARM_Opcode_Fcvtxn2,
  ARM_Opcode_Fcvtzs,
  ARM_Opcode_Fcvtzu,
  ARM_Opcode_Fdiv,
  ARM_Opcode_Fldmx,
  ARM_Opcode_Fmadd,
  ARM_Opcode_Fmax,
  ARM_Opcode_Fmaxnm,
  ARM_Opcode_Fmaxnmp,
  ARM_Opcode_Fmaxnmv,
  ARM_Opcode_Fmaxp,
  ARM_Opcode_Fmaxv,
  ARM_Opcode_Fmin,
  ARM_Opcode_Fminnm,
  ARM_Opcode_Fminnmp,
  ARM_Opcode_Fminnmv,
  ARM_Opcode_Fminp,
  ARM_Opcode_Fminv,
  ARM_Opcode_Fmla,
  ARM_Opcode_Fmls,
  ARM_Opcode_Fmov,
  ARM_Opcode_Fmsub,
  ARM_Opcode_Fmul,
  ARM_Opcode_Fmulx,
  ARM_Opcode_Fneg,
  ARM_Opcode_Fnmadd,
  ARM_Opcode_Fnmsub,
  ARM_Opcode_Fnmul,
  ARM_Opcode_Frecpe,
  ARM_Opcode_Frecps,
  ARM_Opcode_Frecpx,
  ARM_Opcode_Frinta,
  ARM_Opcode_Frinti,
  ARM_Opcode_Frintm,
  ARM_Opcode_Frintn,
  ARM_Opcode_Frintp,
  ARM_Opcode_Frintx,
  ARM_Opcode_Frintz,
  ARM_Opcode_Frsqrte,
  ARM_Opcode_Frsqrts,
  ARM_Opcode_Fsqrt,
  ARM_Opcode_Fstmx,
  ARM_Opcode_Fsub,
  ARM_Opcode_Hb,
  ARM_Opcode_Hblp,
  ARM_Opcode_Hbp,
  ARM_Opcode_Hint,
  ARM_Opcode_Hlt,
  ARM_Opcode_Hvc,
  ARM_Opcode_Ins,
  ARM_Opcode_Isb,
  ARM_Opcode_It,
  ARM_Opcode_Ld1,
  ARM_Opcode_Ld2,
  ARM_Opcode_Ld3,
  ARM_Opcode_Ld4,
  ARM_Opcode_Ldar,
  ARM_Opcode_Ldarb,
  ARM_Opcode_Ldarh,
  ARM_Opcode_Ldaxp,
  ARM_Opcode_Ldaxr,
  ARM_Opcode_Ldaxrb,
  ARM_Opcode_Ldaxrh,
  ARM_Opcode_Ldc,
  ARM_Opcode_Ldc2,
  ARM_Opcode_Ldm,
  ARM_Opcode_Ldmda,
  ARM_Opcode_Ldmdb,
  ARM_Opcode_Ldmib,
  ARM_Opcode_Ldnp,
  ARM_Opcode_Ldp,
  ARM_Opcode_Ldpsw,
  ARM_Opcode_Ldr,
  ARM_Opcode_Ldrb,
  ARM_Opcode_Ldrbt,
  ARM_Opcode_Ldrd,
  ARM_Opcode_Ldrex,
  ARM_Opcode_Ldrexb,
  ARM_Opcode_Ldrexd,
  ARM_Opcode_Ldrexh,
  ARM_Opcode_Ldrh,
  ARM_Opcode_Ldrht,
  ARM_Opcode_Ldrsb,
  ARM_Opcode_Ldrsbt,
  ARM_Opcode_Ldrsh,
  ARM_Opcode_Ldrsht,
  ARM_Opcode_Ldrsw,
  ARM_Opcode_Ldrt,
  ARM_Opcode_Ldtr,
  ARM_Opcode_Ldtrb,
  ARM_Opcode_Ldtrh,
  ARM_Opcode_Ldtrsb,
  ARM_Opcode_Ldtrsh,
  ARM_Opcode_Ldtrsw,
  ARM_Opcode_Ldur,
  ARM_Opcode_Ldurb,
  ARM_Opcode_Ldurh,
  ARM_Opcode_Ldursb,
  ARM_Opcode_Ldursh,
  ARM_Opcode_Ldursw,
  ARM_Opcode_Ldxp,
  ARM_Opcode_Ldxr,
  ARM_Opcode_Ldxrb,
  ARM_Opcode_Ldxrh,
  ARM_Opcode_Leavex,
  ARM_Opcode_Lsl,
  ARM_Opcode_Lsls,
  ARM_Opcode_Lslv,
  ARM_Opcode_Lsr,
  ARM_Opcode_Lsrs,
  ARM_Opcode_Lsrv,
  ARM_Opcode_Madd,
  ARM_Opcode_Mcr,
  ARM_Opcode_Mcr2,
  ARM_Opcode_Mcrr,
  ARM_Opcode_Mcrr2,
  ARM_Opcode_Mla,
  ARM_Opcode_Mls,
  ARM_Opcode_Mov,
  ARM_Opcode_Movi,
  ARM_Opcode_Movk,
  ARM_Opcode_Movn,
  ARM_Opcode_Movs,
  ARM_Opcode_Movt,
  ARM_Opcode_Movw,
  ARM_Opcode_Movz,
  ARM_Opcode_Mrc,
  ARM_Opcode_Mrc2,
  ARM_Opcode_Mrrc,
  ARM_Opcode_Mrrc2,
  ARM_Opcode_Mrs,
  ARM_Opcode_Msr,
  ARM_Opcode_Msub,
  ARM_Opcode_Mul,
  ARM_Opcode_Muls,
  ARM_Opcode_Mvn,
  ARM_Opcode_Mvni,
  ARM_Opcode_Mvns,
  ARM_Opcode_Neg,
  ARM_Opcode_Nop,
  ARM_Opcode_Not,
  ARM_Opcode_Orn,
  ARM_Opcode_Orr,
  ARM_Opcode_Orrs,
  ARM_Opcode_Pkhtb,
  ARM_Opcode_Pld,
  ARM_Opcode_Pli,
  ARM_Opcode_Pmul,
  ARM_Opcode_Pmull,
  ARM_Opcode_Pmull2,
  ARM_Opcode_Pop,
  ARM_Opcode_Prfm,
  ARM_Opcode_Prfum,
  ARM_Opcode_Push,
  ARM_Opcode_Qadd,
  ARM_Opcode_Qadd16,
  ARM_Opcode_Qadd8,
  ARM_Opcode_Qasx,
  ARM_Opcode_Qdadd,
  ARM_Opcode_Qdsub,
  ARM_Opcode_Qsax,
  ARM_Opcode_Qsub,
  ARM_Opcode_Qsub16,
  ARM_Opcode_Qsub8,
  ARM_Opcode_Raddhn,
  ARM_Opcode_Raddhn2,
  ARM_Opcode_Rbit,
  ARM_Opcode_Ret,
  ARM_Opcode_Rev,
  ARM_Opcode_Rev16,
  ARM_Opcode_Rev32,
  ARM_Opcode_Rev64,
  ARM_Opcode_Revsh,
  ARM_Opcode_Ror,
  ARM_Opcode_Rors,
  ARM_Opcode_Rorv,
  ARM_Opcode_Rrx,
  ARM_Opcode_Rsb,
  ARM_Opcode_Rsbs,
  ARM_Opcode_Rsc,
  ARM_Opcode_Rshrn,
  ARM_Opcode_Rshrn2,
  ARM_Opcode_Rsubhn,
  ARM_Opcode_Rsubhn2,
  ARM_Opcode_Saba,
  ARM_Opcode_Sabal,
  ARM_Opcode_Sabal2,
  ARM_Opcode_Sabd,
  ARM_Opcode_Sabdl,
  ARM_Opcode_Sabdl2,
  ARM_Opcode_Sadalp,
  ARM_Opcode_Sadd16,
  ARM_Opcode_Sadd8,
  ARM_Opcode_Saddl,
  ARM_Opcode_Saddl2,
  ARM_Opcode_Saddlp,
  ARM_Opcode_Saddlv,
  ARM_Opcode_Saddw,
  ARM_Opcode_Saddw2,
  ARM_Opcode_Sasx,
  ARM_Opcode_Sbc,
  ARM_Opcode_Sbcs,
  ARM_Opcode_Sbfm,
  ARM_Opcode_Sbfx,
  ARM_Opcode_Scvtf,
  ARM_Opcode_Sdiv,
  ARM_Opcode_Sel,
  ARM_Opcode_Setend,
  ARM_Opcode_Sev,
  ARM_Opcode_Sha1c,
  ARM_Opcode_Sha1h,
  ARM_Opcode_Sha1m,
  ARM_Opcode_Sha1p,
  ARM_Opcode_Sha1su0,
  ARM_Opcode_Sha1su1,
  ARM_Opcode_Sha256h,
  ARM_Opcode_Sha256h2,
  ARM_Opcode_Sha256su0,
  ARM_Opcode_Sha256su1,
  ARM_Opcode_Shadd,
  ARM_Opcode_Shadd16,
  ARM_Opcode_Shadd8,
  ARM_Opcode_Shasx,
  ARM_Opcode_Shl,
  ARM_Opcode_Shll,
  ARM_Opcode_Shll2,
  ARM_Opcode_Shsax,
  ARM_Opcode_Shsub,
  ARM_Opcode_Shsub16,
  ARM_Opcode_Shsub8,
  ARM_Opcode_Sli,
  ARM_Opcode_Smaddl,
  ARM_Opcode_Smax,
  ARM_Opcode_Smaxp,
  ARM_Opcode_Smaxv,
  ARM_Opcode_Smc,
  ARM_Opcode_Smin,
  ARM_Opcode_Sminp,
  ARM_Opcode_Sminv,
  ARM_Opcode_Smla,
  ARM_Opcode_Smlad,
  ARM_Opcode_Smlal,
  ARM_Opcode_Smlal2,
  ARM_Opcode_Smlald,
  ARM_Opcode_Smlaw,
  ARM_Opcode_Smlsd,
  ARM_Opcode_Smlsl,
  ARM_Opcode_Smlsl2,
  ARM_Opcode_Smlsld,
  ARM_Opcode_Smmla,
  ARM_Opcode_Smmls,
  ARM_Opcode_Smmul,
  ARM_Opcode_Smov,
  ARM_Opcode_Smsubl,
  ARM_Opcode_Smuad,
  ARM_Opcode_Smul,
  ARM_Opcode_Smulh,
  ARM_Opcode_Smull,
  ARM_Opcode_Smull2,
  ARM_Opcode_Smulw,
  ARM_Opcode_Smusd,
  ARM_Opcode_Sqabs,
  ARM_Opcode_Sqadd,
  ARM_Opcode_Sqdmlal,
  ARM_Opcode_Sqdmlal2,
  ARM_Opcode_Sqdmlsl,
  ARM_Opcode_Sqdmlsl2,
  ARM_Opcode_Sqdmulh,
  ARM_Opcode_Sqdmull,
  ARM_Opcode_Sqdmull2,
  ARM_Opcode_Sqneg,
  ARM_Opcode_Sqrdmulh,
  ARM_Opcode_Sqrshl,
  ARM_Opcode_Sqrshrn,
  ARM_Opcode_Sqrshrn2,
  ARM_Opcode_Sqrshrun,
  ARM_Opcode_Sqrshrun2,
  ARM_Opcode_Sqshl,
  ARM_Opcode_Sqshlu,
  ARM_Opcode_Sqshrn,
  ARM_Opcode_Sqshrun,
  ARM_Opcode_Sqsub,
  ARM_Opcode_Sqxtn,
  ARM_Opcode_Sqxtn2,
  ARM_Opcode_Sqxtun,
  ARM_Opcode_Sqxtun2,
  ARM_Opcode_Srhadd,
  ARM_Opcode_Sri,
  ARM_Opcode_Srshl,
  ARM_Opcode_Srshr,
  ARM_Opcode_Srsra,
  ARM_Opcode_Ssat,
  ARM_Opcode_Ssat16,
  ARM_Opcode_Ssax,
  ARM_Opcode_Sshl,
  ARM_Opcode_Sshr,
  ARM_Opcode_Ssra,
  ARM_Opcode_Ssub16,
  ARM_Opcode_Ssub8,
  ARM_Opcode_Ssubl,
  ARM_Opcode_Ssubl2,
  ARM_Opcode_Ssubw,
  ARM_Opcode_Ssubw2,
  ARM_Opcode_St1,
  ARM_Opcode_St2,
  ARM_Opcode_St3,
  ARM_Opcode_St4,
  ARM_Opcode_Stc,
  ARM_Opcode_Stc2,
  ARM_Opcode_Stlr,
  ARM_Opcode_Stlrb,
  ARM_Opcode_Stlrh,
  ARM_Opcode_Stlxp,
  ARM_Opcode_Stlxr,
  ARM_Opcode_Stlxrb,
  ARM_Opcode_Stlxrh,
  ARM_Opcode_Stm,
  ARM_Opcode_Stmda,
  ARM_Opcode_Stmdb,
  ARM_Opcode_Stmib,
  ARM_Opcode_Stnp,
  ARM_Opcode_Stp,
  ARM_Opcode_Str,
  ARM_Opcode_Strb,
  ARM_Opcode_Strbt,
  ARM_Opcode_Strd,
  ARM_Opcode_Strex,
  ARM_Opcode_Strexb,
  ARM_Opcode_Strexd,
  ARM_Opcode_Strexh,
  ARM_Opcode_Strh,
  ARM_Opcode_Strht,
  ARM_Opcode_Strt,
  ARM_Opcode_Sttr,
  ARM_Opcode_Sttrb,
  ARM_Opcode_Sttrh,
  ARM_Opcode_Stur,
  ARM_Opcode_Sturb,
  ARM_Opcode_Sturh,
  ARM_Opcode_Stxp,
  ARM_Opcode_Stxr,
  ARM_Opcode_Stxrb,
  ARM_Opcode_Stxrh,
  ARM_Opcode_Sub,
  ARM_Opcode_Subhn,
  ARM_Opcode_Subhn2,
  ARM_Opcode_Subs,
  ARM_Opcode_Subw,
  ARM_Opcode_Suqadd,
  ARM_Opcode_Svc,
  ARM_Opcode_Swp,
  ARM_Opcode_Sxtab,
  ARM_Opcode_Sxtab16,
  ARM_Opcode_Sxtah,
  ARM_Opcode_Sxtb,
  ARM_Opcode_Sxtb16,
  ARM_Opcode_Sxth,
  ARM_Opcode_Sys,
  ARM_Opcode_Sysl,
  ARM_Opcode_Tbh,
  ARM_Opcode_Tbl,
  ARM_Opcode_Tbnz,
  ARM_Opcode_Tbx,
  ARM_Opcode_Tbz,
  ARM_Opcode_Teq,
  ARM_Opcode_Trn1,
  ARM_Opcode_Trn2,
  ARM_Opcode_Tst,
  ARM_Opcode_Uaba,
  ARM_Opcode_Uabal,
  ARM_Opcode_Uabal2,
  ARM_Opcode_Uabd,
  ARM_Opcode_Uabdl,
  ARM_Opcode_Uabdl2,
  ARM_Opcode_Uadalp,
  ARM_Opcode_Uadd16,
  ARM_Opcode_Uadd8,
  ARM_Opcode_Uaddl,
  ARM_Opcode_Uaddl2,
  ARM_Opcode_Uaddlp,
  ARM_Opcode_Uaddlv,
  ARM_Opcode_Uaddw,
  ARM_Opcode_Uaddw2,
  ARM_Opcode_Uasx,
  ARM_Opcode_Ubfm,
  ARM_Opcode_Ubfx,
  ARM_Opcode_Ucvtf,
  ARM_Opcode_Udiv,
  ARM_Opcode_Uhadd,
  ARM_Opcode_Uhadd16,
  ARM_Opcode_Uhadd8,
  ARM_Opcode_Uhasx,
  ARM_Opcode_Uhsax,
  ARM_Opcode_Uhsub,
  ARM_Opcode_Uhsub16,
  ARM_Opcode_Uhsub8,
  ARM_Opcode_Umaal,
  ARM_Opcode_Umaddl,
  ARM_Opcode_Umax,
  ARM_Opcode_Umaxp,
  ARM_Opcode_Umaxv,
  ARM_Opcode_Umin,
  ARM_Opcode_Uminp,
  ARM_Opcode_Uminv,
  ARM_Opcode_Umlal,
  ARM_Opcode_Umlal2,
  ARM_Opcode_Umlsl,
  ARM_Opcode_Umlsl2,
  ARM_Opcode_Umov,
  ARM_Opcode_Umsubl,
  ARM_Opcode_Umulh,
  ARM_Opcode_Umull,
  ARM_Opcode_Umull2,
  ARM_Opcode_Uqadd,
  ARM_Opcode_Uqadd16,
  ARM_Opcode_Uqadd8,
  ARM_Opcode_Uqasx,
  ARM_Opcode_Uqrshl,
  ARM_Opcode_Uqrshrn,
  ARM_Opcode_Uqrshrn2,
  ARM_Opcode_Uqsax,
  ARM_Opcode_Uqshl,
  ARM_Opcode_Uqshrn,
  ARM_Opcode_Uqsub,
  ARM_Opcode_Uqsub16,
  ARM_Opcode_Uqsub8,
  ARM_Opcode_Uqxtn,
  ARM_Opcode_Uqxtn2,
  ARM_Opcode_Urecpe,
  ARM_Opcode_Urhadd,
  ARM_Opcode_Urshl,
  ARM_Opcode_Urshr,
  ARM_Opcode_Ursqrte,
  ARM_Opcode_Ursra,
  ARM_Opcode_Usad8,
  ARM_Opcode_Usada8,
  ARM_Opcode_Usat,
  ARM_Opcode_Usat16,
  ARM_Opcode_Usax,
  ARM_Opcode_Ushl,
  ARM_Opcode_Ushr,
  ARM_Opcode_Usqadd,
  ARM_Opcode_Usra,
  ARM_Opcode_Usub16,
  ARM_Opcode_Usub8,
  ARM_Opcode_Usubl,
  ARM_Opcode_Usubl2,
  ARM_Opcode_Usubw,
  ARM_Opcode_Usubw2,
  ARM_Opcode_Uxtab,
  ARM_Opcode_Uxtab16,
  ARM_Opcode_Uxtah,
  ARM_Opcode_Uxtb,
  ARM_Opcode_Uxtb16,
  ARM_Opcode_Uxth,
  ARM_Opcode_Uzp1,
  ARM_Opcode_Uzp2,
  ARM_Opcode_V,
  ARM_Opcode_Vaba,
  ARM_Opcode_Vabal,
  ARM_Opcode_Vabd,
  ARM_Opcode_Vabdl,
  ARM_Opcode_Vabs,
  ARM_Opcode_Vadd,
  ARM_Opcode_Vaddhn,
  ARM_Opcode_Vaddl,
  ARM_Opcode_Vaddw,
  ARM_Opcode_Vand,
  ARM_Opcode_Vbic,
  ARM_Opcode_Vceq,
  ARM_Opcode_Vcge,
  ARM_Opcode_Vcgt,
  ARM_Opcode_Vcle,
  ARM_Opcode_Vcls,
  ARM_Opcode_Vclt,
  ARM_Opcode_Vclz,
  ARM_Opcode_Vcmp,
  ARM_Opcode_Vcnt,
  ARM_Opcode_Vcvt,
  ARM_Opcode_Vdiv,
  ARM_Opcode_Vdup,
  ARM_Opcode_Veor,
  ARM_Opcode_Vext,
  ARM_Opcode_Vh,
  ARM_Opcode_Vld1,
  ARM_Opcode_Vldm,
  ARM_Opcode_Vldr,
  ARM_Opcode_Vmov,
  ARM_Opcode_Vmovl,
  ARM_Opcode_Vmovn,
  ARM_Opcode_Vmrs,
  ARM_Opcode_Vmsr,
  ARM_Opcode_Vmul,
  ARM_Opcode_Vmull,
  ARM_Opcode_Vmvn,
  ARM_Opcode_Vneg,
  ARM_Opcode_Vnmla,
  ARM_Opcode_Vnmls,
  ARM_Opcode_Vnmul,
  ARM_Opcode_Vorn,
  ARM_Opcode_Vorr,
  ARM_Opcode_Vp,
  ARM_Opcode_Vpadal,
  ARM_Opcode_Vpadd,
  ARM_Opcode_Vpaddl,
  ARM_Opcode_Vpop,
  ARM_Opcode_Vpush,
  ARM_Opcode_Vqabs,
  ARM_Opcode_Vqadd,
  ARM_Opcode_Vqd,
  ARM_Opcode_Vqdmulh,
  ARM_Opcode_Vqdmull,
  ARM_Opcode_Vqmov,
  ARM_Opcode_Vqneg,
  ARM_Opcode_Vqrdmulh,
  ARM_Opcode_Vqrshl,
  ARM_Opcode_Vqrshr,
  ARM_Opcode_Vqshl,
  ARM_Opcode_Vqshr,
  ARM_Opcode_Vqsub,
  ARM_Opcode_Vraddhn,
  ARM_Opcode_Vrecpe,
  ARM_Opcode_Vrecps,
  ARM_Opcode_Vrev,
  ARM_Opcode_Vrhadd,
  ARM_Opcode_Vrshl,
  ARM_Opcode_Vrshr,
  ARM_Opcode_Vrshrn,
  ARM_Opcode_Vrsqrte,
  ARM_Opcode_Vrsqrts,
  ARM_Opcode_Vrsra,
  ARM_Opcode_Vrsubhn,
  ARM_Opcode_Vshl,
  ARM_Opcode_Vshll,
  ARM_Opcode_Vshr,
  ARM_Opcode_Vshrn,
  ARM_Opcode_Vsli,
  ARM_Opcode_Vsqrt,
  ARM_Opcode_Vsra,
  ARM_Opcode_Vsri,
  ARM_Opcode_Vst1,
  ARM_Opcode_Vstm,
  ARM_Opcode_Vstr,
  ARM_Opcode_Vsub,
  ARM_Opcode_Vsubhn,
  ARM_Opcode_Vsubl,
  ARM_Opcode_Vsubw,
  ARM_Opcode_Vswp,
  ARM_Opcode_Vtrn,
  ARM_Opcode_Vtst,
  ARM_Opcode_Vuzp,
  ARM_Opcode_Vzip,
  ARM_Opcode_Wfe,
  ARM_Opcode_Wfi,
  ARM_Opcode_Xtn,
  ARM_Opcode_Xtn2,
  ARM_Opcode_Yield,
  ARM_Opcode_Zip1,
  ARM_Opcode_Zip2
};
static char const *m_Mnemonic[0x29c];
bool Instruction_ADC_T1_fbe08000_f1400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADC_A1_0fe00000_02a00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADC_T2_ffe08000_eb400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADC_A1_0fe00010_00a00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADC_A1_0fe00090_00a00010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADCS_T1_0000ffc0_00004140(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADD_T3_fbe08000_f1000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADD_A1_0fe00000_02800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADD_T2_0000ff00_00004400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADD_T3_ffe08000_eb000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADD_A1_0fe00010_00800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADD_A1_0fe00090_00800010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADD_T1_0000f800_0000a800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADD_T2_0000ff80_0000b000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADD_T3_fbef8000_f10d0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADD_A1_0fef0000_028d0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADD_T1_0000ff78_00004468(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADD_T2_0000ff87_00004485(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADD_T3_ffef8000_eb0d0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADD_A1_0fef0010_008d0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADDS_T1_0000fe00_00001c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADDS_T2_0000f800_00003000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADDS_T1_0000fe00_00001800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADDW_T4_fbf08000_f2000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADDW_T4_fbff8000_f20d0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADR_T1_0000f800_0000a000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADR_T3_fbff8000_f20f0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ADR_A1_0fff0000_028f0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_AND_T1_fbe08000_f0000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_AND_A1_0fe00000_02000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_AND_T2_ffe08000_ea000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_AND_A1_0fe00010_00000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_AND_A1_0fe00090_00000010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ANDS_T1_0000ffc0_00004000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ASR_T2_ffef8030_ea4f0020(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ASR_A1_0fef0070_01a00040(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ASR_T2_ffe0f0f0_fa40f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ASR_A1_0fef00f0_01a00050(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ASRS_T1_0000f800_00001000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ASRS_T1_0000ffc0_00004100(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_B_T1_0000f000_0000d000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_B_T2_0000f800_0000e000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_B_T3_f800d000_f0008000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_B_T4_f800d000_f0009000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_B_A1_0f000000_0a000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_BFC_T1_ffff8020_f36f0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_BFC_A1_0fe0007f_07c0001f(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_BFI_T1_fff08020_f3600000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_BFI_A1_0fe00070_07c00010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_BIC_T1_fbe08000_f0200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_BIC_A1_0fe00000_03c00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_BIC_T2_ffe08000_ea200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_BIC_A1_0fe00010_01c00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_BIC_A1_0fe00090_01c00010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_BICS_T1_0000ffc0_00004380(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_BKPT_T1_0000ff00_0000be00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_BKPT_A1_0ff000f0_01200070(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_BL_T1_f800d000_f000d000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_BL_A1_0f000000_0b000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_BLX_T2_f800d001_f000c000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_BLX_A2_fe000000_fa000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_BLX_T1_0000ff87_00004780(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_BLX_A1_0ffffff0_012fff30(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_BX_T1_0000ff87_00004700(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_BX_A1_0ffffff0_012fff10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_BXJ_T1_fff0ffff_f3c08f00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_BXJ_A1_0ffffff0_012fff20(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_CB_T1_0000f500_0000b100(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_CDP_T1_ff000010_ee000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_CDP_A1_0f000010_0e000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_CDP2_T2_ff000010_fe000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_CDP2_A2_ff000010_fe000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_CHKA_E1_0000ff00_0000ca00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_CLREX_T1_ffffffff_f3bf8f2f(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_CLREX_A1_ffffffff_f57ff01f(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_CLZ_T1_fff0f0f0_fab0f080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_CLZ_A1_0fff0ff0_016f0f10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_CMN_T1_fbf08f00_f1100f00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_CMN_A1_0ff0f000_03700000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_CMN_T1_0000ffc0_000042c0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_CMN_T2_fff08f00_eb100f00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_CMN_A1_0ff0f010_01700000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_CMN_A1_0ff0f090_01700010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_CMP_T1_0000f800_00002800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_CMP_T2_fbf08f00_f1b00f00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_CMP_A1_0ff0f000_03500000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_CMP_T1_0000ffc0_00004280(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_CMP_T2_0000ff00_00004500(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_CMP_T3_fff08f00_ebb00f00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_CMP_A1_0ff0f010_01500000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_CMP_A1_0ff0f090_01500010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_DBG_T1_fffffff0_f3af80f0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_DBG_A1_0ffffff0_0320f0f0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_DMB_T1_fffffff0_f3bf8f50(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_DMB_A1_fffffff0_f57ff050(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_DSB_T1_fffffff0_f3bf8f40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_DSB_A1_fffffff0_f57ff040(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_EOR_T1_fbe08000_f0800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_EOR_A1_0fe00000_02200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_EOR_T2_ffe08000_ea800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_EOR_A1_0fe00010_00200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_EOR_A1_0fe00090_00200010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_EORS_T1_0000ffc0_00004040(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_FLDMX_A2_0e100f00_0c100a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_FLDMX_A2_0fbf0f00_0cbd0a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_FSTMX_A2_0fbf0f00_0d2d0a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_FSTMX_A2_0e100f00_0c000a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_HB_E1_0000fe00_0000c200(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_HBLP_E1_0000fc00_0000c400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_HBP_E1_0000ff00_0000c000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ISB_T1_fffffff0_f3bf8f60(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ISB_A1_fffffff0_f57ff060(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_IT_T1_0000ff00_0000bf00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDC_T1_fe100000_ec100000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDC_A1_0e100000_0c100000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDC_T1_fe1f0000_ec1f0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDC_A1_0e1f0000_0c1f0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDC2_T2_fe100000_fc100000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDC2_A2_fe100000_fc100000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDC2_T2_fe1f0000_fc1f0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDC2_A2_fe1f0000_fc1f0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDM_T1_0000f800_0000c800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDM_T2_ffd02000_e8900000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDM_A1_0fd00000_08900000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDMDA_A1_0fd00000_08100000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDMDB_T1_ffd02000_e9100000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDMDB_A1_0fd00000_09100000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDMIB_A1_0fd00000_09900000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDR_T1_0000f800_00006800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDR_T2_0000f800_00009800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDR_T3_fff00000_f8d00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDR_T4_fff00800_f8500800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDR_A1_0e500000_04100000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDR_T1_0000f800_00004800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDR_T2_ff7f0000_f85f0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDR_A1_0f7f0000_051f0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDR_T1_0000fe00_00005800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDR_T2_fff00fc0_f8500000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDR_A1_0e500010_06100000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDR_E1_0000fe00_00005800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDR_E1_0000fe00_0000cc00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDR_E2_0000ff00_0000cb00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDR_E3_0000fe00_0000c800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRB_T1_0000f800_00007800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRB_T2_fff00000_f8900000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRB_T3_fff00800_f8100800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRB_A1_0e500000_04500000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRB_T1_ff7f0000_f81f0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRB_A1_0f7f0000_055f0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRB_T1_0000fe00_00005c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRB_T2_fff00fc0_f8100000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRB_A1_0e500010_06500000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRBT_T1_fff00f00_f8100e00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRBT_A1_0f700000_04700000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRBT_A2_0f700010_06700000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRD_T1_fe500000_e8500000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRD_A1_0e5000f0_004000d0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRD_T1_fe7f0000_e85f0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRD_A1_0f7f00f0_014f00d0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRD_A1_0e500ff0_000000d0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDREX_T1_fff00f00_e8500f00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDREX_A1_0ff00fff_01900f9f(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDREXB_T1_fff00fff_e8d00f4f(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDREXB_A1_0ff00fff_01d00f9f(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDREXD_T1_fff000ff_e8d0007f(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDREXD_A1_0ff00fff_01b00f9f(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDREXH_T1_fff00fff_e8d00f5f(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDREXH_A1_0ff00fff_01f00f9f(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRH_T1_0000f800_00008800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRH_T2_fff00000_f8b00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRH_T3_fff00800_f8300800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRH_A1_0e5000f0_005000b0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRH_T1_ff7f0000_f83f0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRH_A1_0f7f00f0_015f00b0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRH_T1_0000fe00_00005a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRH_T2_fff00fc0_f8300000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRH_A1_0e500ff0_001000b0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRH_E1_0000fe00_00005a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRHT_T1_fff00f00_f8300e00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRHT_A1_0f7000f0_007000b0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRHT_A2_0f700ff0_003000b0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRSB_T1_fff00000_f9900000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRSB_T2_fff00800_f9100800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRSB_A1_0e5000f0_005000d0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRSB_T1_ff7f0000_f91f0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRSB_A1_0f7f00f0_015f00d0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRSB_T1_0000fe00_00005600(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRSB_T2_fff00fc0_f9100000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRSB_A1_0e500ff0_001000d0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRSBT_T1_fff00f00_f9100e00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRSBT_A1_0f7000f0_007000d0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRSBT_A2_0f700ff0_003000d0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRSH_T1_fff00000_f9b00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRSH_T2_fff00800_f9300800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRSH_A1_0e5000f0_005000f0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRSH_T1_ff7f0000_f93f0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRSH_A1_0f7f00f0_015f00f0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRSH_T1_0000fe00_00005e00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRSH_T2_fff00fc0_f9300000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRSH_A1_0e500ff0_001000f0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRSH_E1_0000fe00_00005e00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRSHT_T1_fff00f00_f9300e00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRSHT_A1_0f7000f0_007000f0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRSHT_A2_0f700ff0_003000f0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRT_T1_fff00f00_f8500e00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRT_A1_0f700000_04300000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LDRT_A2_0f700010_06300000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LEAVEX_E1_ffffffef_f3bf8f0f(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LSL_T2_ffef8030_ea4f0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LSL_A1_0fef0070_01a00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LSL_T2_ffe0f0f0_fa00f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LSL_A1_0fef00f0_01a00010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LSLS_T1_0000f800_00000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LSLS_T1_0000ffc0_00004080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LSR_T2_ffef8030_ea4f0010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LSR_A1_0fef0070_01a00020(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LSR_T2_ffe0f0f0_fa20f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LSR_A1_0fef00f0_01a00030(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LSRS_T1_0000f800_00000800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_LSRS_T1_0000ffc0_000040c0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MCR_T1_ff100010_ee000010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MCR_A1_0f100010_0e000010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MCR2_T2_ff100010_fe000010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MCR2_A2_ff100010_fe000010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MCRR_T1_fff00000_ec400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MCRR_A1_0ff00000_0c400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MCRR2_T2_fff00000_fc400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MCRR2_A2_fff00000_fc400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MLA_T1_fff000f0_fb000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MLA_A1_0fe000f0_00200090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MLS_T1_fff000f0_fb000010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MLS_A1_0ff000f0_00600090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MOV_T2_fbef8000_f04f0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MOV_A1_0fef0000_03a00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MOV_T1_0000ff00_00004600(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MOV_T3_ffeff0f0_ea4f0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MOV_A1_0fef0ff0_01a00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MOVS_T1_0000f800_00002000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MOVS_T2_0000ffc0_00000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MOVT_T1_fbf08000_f2c00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MOVT_A1_0ff00000_03400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MOVW_T3_fbf08000_f2400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MOVW_A2_0ff00000_03000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MRC_T1_ff100010_ee100010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MRC_A1_0f100010_0e100010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MRC2_T2_ff100010_fe100010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MRC2_A2_ff100010_fe100010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MRRC_T1_fff00000_ec500000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MRRC_A1_0ff00000_0c500000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MRRC2_T2_fff00000_fc500000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MRRC2_A2_fff00000_fc500000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MRS_T1_fffff0ff_f3ef8000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MRS_A1_0fff0fff_010f0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MSR_A1_0ff3f000_0320f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MSR_T1_fff0f3ff_f3808000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MSR_A1_0ff3fff0_0120f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MUL_T2_fff0f0f0_fb00f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MUL_A1_0fe0f0f0_00000090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MULS_T1_0000ffc0_00004340(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MVN_T1_fbef8000_f06f0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MVN_A1_0fef0000_03e00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MVN_T2_ffef8000_ea6f0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MVN_A1_0fef0010_01e00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MVN_A1_0fef0090_01e00010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_MVNS_T1_0000ffc0_000043c0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_NOP_T1_0000ffff_0000bf00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_NOP_T2_ffffffff_f3af8000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_NOP_A1_0fffffff_0320f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ORN_T1_fbe08000_f0600000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ORN_T1_ffe08000_ea600000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ORR_T1_fbe08000_f0400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ORR_A1_0fe00000_03800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ORR_T2_ffe08000_ea400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ORR_A1_0fe00010_01800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ORR_A1_0fe00090_01800010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ORRS_T1_0000ffc0_00004300(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_PKHTB_T1_fff08010_eac00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_PKHTB_A1_0ff00030_06800010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_PLD_T1_ffd0f000_f890f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_PLD_T2_ffd0ff00_f810fc00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_PLD_A1_ff30f000_f510f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_PLD_T1_ff7ff000_f81ff000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_PLD_A1_ff7ff000_f55ff000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_PLD_T1_ffd0ffc0_f810f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_PLD_A1_ff30f010_f710f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_PLI_T1_fff0f000_f990f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_PLI_T2_fff0ff00_f910fc00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_PLI_T3_ff7ff000_f91ff000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_PLI_A1_ff70f000_f450f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_PLI_T1_fff0ffc0_f910f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_PLI_A1_ff70f010_f650f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_POP_T1_0000fe00_0000bc00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_POP_T2_ffff2000_e8bd0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_POP_T3_ffff0fff_f85d0b04(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_POP_A1_0fff0000_08bd0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_POP_A2_0fff0fff_049d0004(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_PUSH_T1_0000fe00_0000b400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_PUSH_T2_ffffa000_e8ad0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_PUSH_T3_ffff0fff_f84d0d04(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_PUSH_A1_0fff0000_092d0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_PUSH_A2_0fff0fff_052d0004(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_QADD_T1_fff0f0f0_fa80f080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_QADD_A1_0ff00ff0_01000050(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_QADD16_T1_fff0f0f0_fa90f010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_QADD16_A1_0ff00ff0_06200f10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_QADD8_T1_fff0f0f0_fa80f010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_QADD8_A1_0ff00ff0_06200f90(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_QASX_T1_fff0f0f0_faa0f010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_QASX_A1_0ff00ff0_06200f30(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_QDADD_T1_fff0f0f0_fa80f090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_QDADD_A1_0ff00ff0_01400050(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_QDSUB_T1_fff0f0f0_fa80f0b0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_QDSUB_A1_0ff00ff0_01600050(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_QSAX_T1_fff0f0f0_fae0f010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_QSAX_A1_0ff00ff0_06200f50(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_QSUB_T1_fff0f0f0_fa80f0a0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_QSUB_A1_0ff00ff0_01200050(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_QSUB16_T1_fff0f0f0_fad0f010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_QSUB16_A1_0ff00ff0_06200f70(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_QSUB8_T1_fff0f0f0_fac0f010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_QSUB8_A1_0ff00ff0_06200ff0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_RBIT_T1_fff0f0f0_fa90f0a0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_RBIT_A1_0fff0ff0_06ff0f30(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_REV_T1_0000ffc0_0000ba00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_REV_T2_fff0f0f0_fa90f080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_REV_A1_0fff0ff0_06bf0f30(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_REV16_T1_0000ffc0_0000ba40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_REV16_T2_fff0f0f0_fa90f090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_REV16_A1_0fff0ff0_06bf0fb0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_REVSH_T1_0000ffc0_0000bac0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_REVSH_T2_fff0f0f0_fa90f0b0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_REVSH_A1_0fff0ff0_06ff0fb0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ROR_T1_ffef8030_ea4f0030(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ROR_A1_0fef0070_01a00060(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ROR_T2_ffe0f0f0_fa60f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ROR_A1_0fef00f0_01a00070(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_RORS_T1_0000ffc0_000041c0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_RRX_T1_ffeff0f0_ea4f0030(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_RRX_A1_0fef0ff0_01a00060(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_RSB_T2_fbe08000_f1c00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_RSB_A1_0fe00000_02600000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_RSB_T1_ffe08000_ebc00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_RSB_A1_0fe00010_00600000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_RSB_A1_0fe00090_00600010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_RSBS_T1_0000ffc0_00004240(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_RSC_A1_0fe00000_02e00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_RSC_A1_0fe00010_00e00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_RSC_A1_0fe00090_00e00010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SADD16_T1_fff0f0f0_fa90f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SADD16_A1_0ff00ff0_06100f10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SADD8_T1_fff0f0f0_fa80f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SADD8_A1_0ff00ff0_06100f90(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SASX_T1_fff0f0f0_faa0f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SASX_A1_0ff00ff0_06100f30(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SBC_T1_fbe08000_f1600000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SBC_A1_0fe00000_02c00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SBC_T2_ffe08000_eb600000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SBC_A1_0fe00010_00c00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SBC_A1_0fe00090_00c00010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SBCS_T1_0000ffc0_00004180(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SBFX_T1_fff08020_f3400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SBFX_A1_0fe00070_07a00050(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SDIV_T1_fff0f0f0_fb90f0f0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SEL_T1_fff0f0f0_faa0f080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SEL_A1_0ff00ff0_06800fb0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SETEND_T1_0000fff7_0000b650(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SETEND_A1_fffffdff_f1010000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SEV_T1_0000ffff_0000bf40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SEV_T2_ffffffff_f3af8004(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SEV_A1_0fffffff_0320f004(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SHADD16_T1_fff0f0f0_fa90f020(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SHADD16_A1_0ff00ff0_06300f10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SHADD8_T1_fff0f0f0_fa80f020(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SHADD8_A1_0ff00ff0_06300f90(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SHASX_T1_fff0f0f0_faa0f020(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SHASX_A1_0ff00ff0_06300f30(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SHSAX_T1_fff0f0f0_fae0f020(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SHSAX_A1_0ff00ff0_06300f50(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SHSUB16_T1_fff0f0f0_fad0f020(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SHSUB16_A1_0ff00ff0_06300f70(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SHSUB8_T1_fff0f0f0_fac0f020(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SHSUB8_A1_0ff00ff0_06300ff0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMLA_T1_fff000c0_fb100000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMLA_A1_0ff00090_01000080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMLAD_T1_fff000e0_fb200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMLAD_A1_0ff000d0_07000010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMLAL_T1_fff000f0_fbc00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMLAL_A1_0fe000f0_00e00090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMLAL_T1_fff000c0_fbc00080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMLAL_A1_0ff00090_01400080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMLALD_T1_fff000e0_fbc000c0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMLALD_A1_0ff000d0_07400010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMLAW_T1_fff000e0_fb300000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMLAW_A1_0ff000b0_01200080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMLSD_T1_fff000e0_fb400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMLSD_A1_0ff000d0_07000050(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMLSLD_T1_fff000e0_fbd000c0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMLSLD_A1_0ff000d0_07400050(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMMLA_T1_fff000e0_fb500000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMMLA_A1_0ff000d0_07500010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMMLS_T1_fff000e0_fb600000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMMLS_A1_0ff000d0_075000d0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMMUL_T1_fff0f0e0_fb50f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMMUL_A1_0ff0f0d0_0750f010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMUAD_T1_fff0f0e0_fb20f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMUAD_A1_0ff0f0d0_0700f010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMUL_T1_fff0f0c0_fb10f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMUL_A1_0ff00090_01600080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMULL_T1_fff000f0_fb800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMULL_A1_0fe000f0_00c00090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMULW_T1_fff0f0e0_fb30f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMULW_A1_0ff000b0_012000a0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMUSD_T1_fff0f0e0_fb40f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SMUSD_A1_0ff0f0d0_0700f050(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SSAT_T1_ffd08020_f3000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SSAT_A1_0fe00030_06a00010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SSAT16_T1_fff0f0f0_f3200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SSAT16_A1_0ff00ff0_06a00f30(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SSAX_T1_fff0f0f0_fae0f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SSAX_A1_0ff00ff0_06100f50(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SSUB16_T1_fff0f0f0_fad0f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SSUB16_A1_0ff00ff0_06100f70(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SSUB8_T1_fff0f0f0_fac0f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SSUB8_A1_0ff00ff0_06100ff0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STC_T1_fe100000_ec000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STC_A1_0e100000_0c000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STC2_T2_fe100000_fc000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STC2_A2_fe100000_fc000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STM_T1_0000f800_0000c000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STM_T2_ffd0a000_e8800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STM_A1_0fd00000_08800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STMDA_A1_0fd00000_08000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STMDB_T1_ffd0a000_e9000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STMDB_A1_0fd00000_09000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STMIB_A1_0fd00000_09800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STR_T1_0000f800_00006000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STR_T2_0000f800_00009000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STR_T3_fff00000_f8c00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STR_T4_fff00800_f8400800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STR_A1_0e500000_04000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STR_T1_0000fe00_00005000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STR_T2_fff00fc0_f8400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STR_A1_0e500010_06000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STR_E1_0000fe00_00005000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STR_E1_0000fe00_0000ce00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRB_T1_0000f800_00007000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRB_T2_fff00000_f8800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRB_T3_fff00800_f8000800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRB_A1_0e500000_04400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRB_T1_0000fe00_00005400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRB_T2_fff00fc0_f8000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRB_A1_0e500010_06400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRBT_T1_fff00f00_f8000e00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRBT_A1_0f700000_04600000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRBT_A2_0f700010_06600000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRD_T1_fe500000_e8400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRD_A1_0e5000f0_004000f0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRD_A1_0e500ff0_000000f0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STREX_T1_fff00000_e8400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STREX_A1_0ff00ff0_01800f90(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STREXB_T1_fff00ff0_e8c00f40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STREXB_A1_0ff00ff0_01c00f90(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STREXD_T1_fff000f0_e8c00070(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STREXD_A1_0ff00ff0_01a00f90(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STREXH_T1_fff00ff0_e8c00f50(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STREXH_A1_0ff00ff0_01e00f90(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRH_T1_0000f800_00008000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRH_T2_fff00000_f8a00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRH_T3_fff00800_f8200800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRH_A1_0e5000f0_004000b0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRH_T1_0000fe00_00005200(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRH_T2_fff00fc0_f8200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRH_A1_0e500ff0_000000b0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRH_E1_0000fe00_00005200(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRHT_T1_fff00f00_f8200e00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRHT_A1_0f7000f0_006000b0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRHT_A2_0f700ff0_002000b0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRT_T1_fff00f00_f8400e00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRT_A1_0f700000_04200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_STRT_A2_0f700010_06200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SUB_T2_fbff8000_f2af0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SUB_A2_0fff0000_024f0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SUB_T3_fbe08000_f1a00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SUB_A1_0fe00000_02400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SUB_T2_ffe08000_eba00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SUB_A1_0fe00010_00400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SUB_A1_0fe00090_00400010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SUB_T1_0000ff80_0000b080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SUB_T2_fbef8000_f1ad0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SUB_A1_0fef0000_024d0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SUB_T1_ffef8000_ebad0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SUB_A1_0fef0010_004d0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SUBS_T1_0000fe00_00001e00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SUBS_T2_0000f800_00003800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SUBS_T1_0000fe00_00001a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SUBW_T4_fbf08000_f2a00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SUBW_T3_fbff8000_f2ad0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SVC_T1_0000ff00_0000df00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SVC_A1_0f000000_0f000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SWP_A1_0fb00ff0_01000090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SXTAB_T1_fff0f0c0_fa40f080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SXTAB_A1_0ff003f0_06a00070(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SXTAB16_T1_fff0f0c0_fa20f080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SXTAB16_A1_0ff003f0_06800070(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SXTAH_T1_fff0f0c0_fa00f080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SXTAH_A1_0ff003f0_06b00070(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SXTB_T1_0000ffc0_0000b240(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SXTB_T2_fffff0c0_fa4ff080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SXTB_A1_0fff03f0_06af0070(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SXTB16_T1_fffff0c0_fa2ff080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SXTB16_A1_0fff03f0_068f0070(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SXTH_T1_0000ffc0_0000b200(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SXTH_T2_fffff0c0_fa0ff080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_SXTH_A1_0fff03f0_06bf0070(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_TBH_T1_fff0ffe0_e8d0f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_TEQ_T1_fbf08f00_f0900f00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_TEQ_A1_0ff0f000_03300000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_TEQ_T1_fff08f00_ea900f00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_TEQ_A1_0ff0f010_01300000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_TEQ_A1_0ff0f090_01300010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_TST_T1_fbf08f00_f0100f00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_TST_A1_0ff0f000_03100000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_TST_T1_0000ffc0_00004200(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_TST_T2_fff08f00_ea100f00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_TST_A1_0ff0f010_01100000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_TST_A1_0ff0f090_01100010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UADD16_T1_fff0f0f0_fa90f040(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UADD16_A1_0ff00ff0_06500f10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UADD8_T1_fff0f0f0_fa80f040(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UADD8_A1_0ff00ff0_06500f90(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UASX_T1_fff0f0f0_faa0f040(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UASX_A1_0ff00ff0_06500f30(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UBFX_T1_fff08020_f3c00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UBFX_A1_0fe00070_07e00050(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UDIV_T1_fff0f0f0_fbb0f0f0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UHADD16_T1_fff0f0f0_fa90f060(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UHADD16_A1_0ff00ff0_06700f10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UHADD8_T1_fff0f0f0_fa80f060(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UHADD8_A1_0ff00ff0_06700f90(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UHASX_T1_fff0f0f0_faa0f060(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UHASX_A1_0ff00ff0_06700f30(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UHSAX_T1_fff0f0f0_fae0f060(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UHSAX_A1_0ff00ff0_06700f50(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UHSUB16_T1_fff0f0f0_fad0f060(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UHSUB16_A1_0ff00ff0_06700f70(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UHSUB8_T1_fff0f0f0_fac0f060(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UHSUB8_A1_0ff00ff0_06700ff0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UMAAL_T1_fff000f0_fbe00060(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UMAAL_A1_0ff000f0_00400090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UMLAL_T1_fff000f0_fbe00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UMLAL_A1_0fe000f0_00a00090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UMULL_T1_fff000f0_fba00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UMULL_A1_0fe000f0_00800090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UQADD16_T1_fff0f0f0_fa90f050(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UQADD16_A1_0ff00ff0_06600f10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UQADD8_T1_fff0f0f0_fa80f050(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UQADD8_A1_0ff00ff0_06600f90(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UQASX_T1_fff0f0f0_faa0f050(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UQASX_A1_0ff00ff0_06600f30(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UQSAX_T1_fff0f0f0_fae0f050(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UQSAX_A1_0ff00ff0_06600f50(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UQSUB16_T1_fff0f0f0_fad0f050(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UQSUB16_A1_0ff00ff0_06600f70(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UQSUB8_T1_fff0f0f0_fac0f050(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UQSUB8_A1_0ff00ff0_06600ff0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_USAD8_T1_fff0f0f0_fb70f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_USAD8_A1_0ff0f0f0_0780f010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_USADA8_T1_fff000f0_fb700000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_USADA8_A1_0ff000f0_07800010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_USAT_T1_ffd08020_f3800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_USAT_A1_0fe00030_06e00010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_USAT16_T1_fff0f0f0_f3a00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_USAT16_A1_0ff00ff0_06e00f30(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_USAX_T1_fff0f0f0_fae0f040(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_USAX_A1_0ff00ff0_06500f50(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_USUB16_T1_fff0f0f0_fad0f040(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_USUB16_A1_0ff00ff0_06500f70(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_USUB8_T1_fff0f0f0_fac0f040(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_USUB8_A1_0ff00ff0_06500ff0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UXTAB_T1_fff0f0c0_fa50f080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UXTAB_A1_0ff003f0_06e00070(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UXTAB16_T1_fff0f0c0_fa30f080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UXTAB16_A1_0ff003f0_06c00070(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UXTAH_T1_fff0f0c0_fa10f080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UXTAH_A1_0ff003f0_06f00070(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UXTB_T1_0000ffc0_0000b2c0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UXTB_T2_fffff0c0_fa5ff080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UXTB_A1_0fff03f0_06ef0070(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UXTB16_T1_fffff0c0_fa3ff080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UXTB16_A1_0fff03f0_06cf0070(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UXTH_T1_0000ffc0_0000b280(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UXTH_T2_fffff0c0_fa1ff080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_UXTH_A1_0fff03f0_06ff0070(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_V_T1_ff800f10_ff000e10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_V_A1_ff800f10_f3000e10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_V_T1_ff800f10_ff000110(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_V_A1_ff800f10_f3000110(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_V_T1_ef800f00_ef000600(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_V_A1_fe800f00_f2000600(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_V_T1_ff800f10_ef000f00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_V_A1_ff800f10_f2000f00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_V_T1_ef800f10_ef000900(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_V_A1_fe800f10_f2000900(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_V_T2_ef800d50_ef800800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_V_A2_fe800d50_f2800800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_V_T1_ff800f10_ef000d10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_V_A1_ff800f10_f2000d10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_V_T2_ffb00e10_ee000a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_V_A2_0fb00e10_0e000a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_V_T1_ef800a50_ef800040(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_V_A1_fe800a50_f2800040(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_V_T2_ef800b50_ef800240(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_V_A2_fe800b50_f2800240(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_V_T1_ffb00c10_ffb00800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_V_A1_ffb00c10_f3b00800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VABA_T1_ef800f10_ef000710(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VABA_A1_fe800f10_f2000710(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VABAL_T2_ef800f50_ef800500(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VABAL_A2_fe800f50_f2800500(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VABD_T1_ef800f10_ef000700(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VABD_A1_fe800f10_f2000700(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VABD_T1_ffa00f10_ff200d00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VABD_A1_ffa00f10_f3200d00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VABDL_T2_ef800f50_ef800700(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VABDL_A2_fe800f50_f2800700(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VABS_T1_ffb30b90_ffb10300(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VABS_A1_ffb30b90_f3b10300(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VABS_T2_ffbf0ed0_eeb00ac0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VABS_A2_0fbf0ed0_0eb00ac0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VADD_T1_ff800f10_ef000800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VADD_A1_ff800f10_f2000800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VADD_T1_ffa00f10_ef000d00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VADD_A1_ffa00f10_f2000d00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VADD_T2_ffb00e50_ee300a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VADD_A2_0fb00e50_0e300a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VADDHN_T1_ff800f50_ef800400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VADDHN_A1_ff800f50_f2800400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VADDL_T1_ef800e50_ef800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VADDW_A1_fe800e50_f2800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VAND_T1_ffb00f10_ef000110(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VAND_A1_ffb00f10_f2000110(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VBIC_T1_efb800b0_ef800030(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VBIC_A1_feb800b0_f2800030(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VBIC_T1_ffb00f10_ef100110(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VBIC_A1_ffb00f10_f2100110(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCEQ_T1_ff800f10_ff000810(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCEQ_A1_ff800f10_f3000810(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCEQ_T2_ffa00f10_ef000e00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCEQ_A2_ffa00f10_f2000e00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCEQ_T1_ffb30b90_ffb10100(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCEQ_A1_ffb30b90_f3b10100(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCGE_T1_ef800f10_ef000310(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCGE_A1_fe800f10_f2000310(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCGE_T2_ffa00f10_ff000e00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCGE_A2_ffa00f10_f3000e00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCGE_T1_ffb30b90_ffb10080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCGE_A1_ffb30b90_f3b10080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCGT_T1_ef800f10_ef000300(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCGT_A1_fe800f10_f2000300(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCGT_T2_ffa00f10_ff200e00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCGT_A2_ffa00f10_f3200e00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCGT_T1_ffb30b90_ffb10000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCGT_A1_ffb30b90_f3b10000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCLE_T1_ffb30b90_ffb10180(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCLE_A1_ffb30b90_f3b10180(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCLS_T1_ffb30f90_ffb00400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCLS_A1_ffb30f90_f3b00400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCLT_T1_ffb30b90_ffb10200(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCLT_A1_ffb30b90_f3b10200(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCLZ_T1_ffb30f90_ffb00480(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCLZ_A1_ffb30f90_f3b00480(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCMP_T1_ffbf0e50_eeb40a40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCMP_A1_0fbf0e50_0eb40a40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCMP_T2_ffbf0e7f_eeb50a40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCMP_A2_0fbf0e7f_0eb50a40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCNT_T1_ffb30f90_ffb00500(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCNT_A1_ffb30f90_f3b00500(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCVT_T1_ffb30e10_ffb30600(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCVT_A1_ffb30e10_f3b30600(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCVT_T1_ffb80e50_eeb80a40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCVT_A1_0fb80e50_0eb80a40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCVT_T1_ef800e90_ef800e10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCVT_A1_fe800e90_f2800e10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCVT_T1_ffba0e50_eeba0a40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCVT_A1_0fba0e50_0eba0a40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCVT_T1_ffbf0ed0_eeb70ac0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCVT_A1_0fbf0ed0_0eb70ac0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCVT_T1_ffb30ed0_ffb20600(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCVT_A1_ffb30ed0_f3b20600(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCVT_T1_ffbe0f50_eeb20a40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VCVT_A1_0fbe0f50_0eb20a40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VDIV_T1_ffb00e50_ee800a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VDIV_A1_0fb00e50_0e800a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VDUP_T1_ffb00f90_ffb00c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VDUP_A1_ffb00f90_f3b00c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VDUP_T1_ff900f5f_ee800b10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VDUP_A1_0f900f5f_0e800b10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VEOR_T1_ffb00f10_ff000110(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VEOR_A1_ffb00f10_f3000110(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VEXT_T1_ffb00010_efb00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VEXT_A1_ffb00010_f2b00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VH_T1_ef800d10_ef000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VH_A1_fe800d10_f2000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VLD1_T1_ffb00000_f9200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VLD1_A1_ffb00000_f4200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VLD1_T1_ffb00300_f9a00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VLD1_A1_ffb00300_f4a00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VLD1_T1_ffb00f00_f9a00c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VLD1_A1_ffb00f00_f4a00c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VLDM_T1_fe100f00_ec100b00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VLDM_A1_0e100f00_0c100b00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VLDM_T2_fe100f00_ec100a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VLDR_T1_ff300f00_ed100b00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VLDR_A1_0f300f00_0d100b00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VLDR_T2_ff300f00_ed100a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VLDR_A2_0f300f00_0d100a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMOV_T1_efb80090_ef800010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMOV_A1_feb80090_f2800010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMOV_T2_ffb00ef0_eeb00a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMOV_A2_0fb00ef0_0eb00a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMOV_T2_ffbf0ed0_eeb00a40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMOV_A2_0fbf0ed0_0eb00a40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMOV_T1_ff900f1f_ee000b10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMOV_A1_0f900f1f_0e000b10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMOV_T1_ff100f1f_ee100b10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMOV_A1_0f100f1f_0e100b10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMOV_T1_ffe00f7f_ee000a10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMOV_A1_0fe00f7f_0e000a10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMOV_T1_ffe00fd0_ec400a10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMOV_A1_0fe00fd0_0c400a10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMOV_T1_ffe00fd0_ec400b10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMOV_A1_0fe00fd0_0c400b10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMOVL_T1_ef870fd0_ef800a10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMOVL_A1_fe870fd0_f2800a10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMOVN_T1_ffb30fd0_ffb20200(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMOVN_A1_ffb30fd0_f3b20200(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMRS_T1_ffff0fff_eef10a10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMRS_A1_0fff0fff_0ef10a10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMSR_T1_ffff0fff_eee10a10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMSR_A1_0fff0fff_0ee10a10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMUL_T1_ef800f10_ef000910(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMUL_A1_fe800f10_f2000910(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMUL_T1_ffa00f10_ff000d10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMUL_A1_ffa00f10_f3000d10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMUL_T2_ffb00e50_ee200a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMUL_A2_0fb00e50_0e200a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMUL_T1_ef800e50_ef800840(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMUL_A1_fe800e50_f2800840(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMULL_T2_ef800d50_ef800c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMULL_A2_fe800d50_f2800c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMULL_T2_ef800f50_ef800a40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMULL_A2_fe800f50_f2800a40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMVN_T1_ffb30f90_ffb00580(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VMVN_A1_ffb30f90_f3b00580(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VNEG_T1_ffb30b90_ffb10380(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VNEG_A1_ffb30b90_f3b10380(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VNEG_T2_ffbf0ed0_eeb10a40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VNEG_A2_0fbf0ed0_0eb10a40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VNMLA_T1_ffb00e10_ee100a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VNMLS_A1_0fb00e10_0e100a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VNMUL_T2_ffb00e50_ee200a40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VNMUL_A2_0fb00e50_0e200a40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VORN_T1_ffb00f10_ef300110(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VORN_A1_ffb00f10_f2300110(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VORR_T1_efb800b0_ef800010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VORR_A1_feb800b0_f2800010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VORR_T1_ffb00f10_ef200110(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VORR_A1_ffb00f10_f2200110(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VP_T1_ef800f00_ef000a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VP_A1_fe800f00_f2000a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VP_T1_ff800f10_ff000f00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VP_A1_ff800f10_f3000f00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VPADAL_T1_ffb30f10_ffb00600(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VPADAL_A1_ffb30f10_f3b00600(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VPADD_T1_ff800f10_ef000b10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VPADD_A1_ff800f10_f2000b10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VPADD_T1_ffa00f10_ff000d00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VPADD_A1_ffa00f10_f3000d00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VPADDL_T1_ffb30f10_ffb00200(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VPADDL_A1_ffb30f10_f3b00200(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VPOP_T1_ffbf0f00_ecbd0b00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VPOP_A1_0fbf0f00_0cbd0b00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VPOP_T2_ffbf0f00_ecbd0a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VPUSH_T1_ffbf0f00_ed2d0b00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VPUSH_A1_0fbf0f00_0d2d0b00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VPUSH_T2_ffbf0f00_ed2d0a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQABS_T1_ffb30f90_ffb00700(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQABS_A1_ffb30f90_f3b00700(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQADD_T1_ef800f10_ef000010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQADD_A1_fe800f10_f2000010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQD_T1_ff800d50_ef800900(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQD_A1_ff800d50_f2800900(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQD_T2_ff800b50_ef800340(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQD_A2_ff800b50_f2800340(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQDMULH_T1_ff800f10_ef000b00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQDMULH_A1_ff800f10_f2000b00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQDMULH_T2_ef800f50_ef800c40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQDMULH_A2_fe800f50_f2800c40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQDMULL_T1_ff800f50_ef800d00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQDMULL_A1_ff800f50_f2800d00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQDMULL_T2_ff800f50_ef800b40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQDMULL_A2_ff800f50_f2800b40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQMOV_T1_ffb30f10_ffb20200(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQMOV_A1_ffb30f10_f3b20200(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQNEG_T1_ffb30f90_ffb00780(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQNEG_A1_ffb30f90_f3b00780(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQRDMULH_T1_ff800f10_ff000b00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQRDMULH_A1_ff800f10_f3000b00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQRDMULH_T2_ef800f50_ef800d40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQRDMULH_A2_fe800f50_f2800d40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQRSHL_T1_ef800f10_ef000510(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQRSHL_A1_fe800f10_f2000510(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQRSHR_T1_ef800ed0_ef800850(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQRSHR_A1_fe800ed0_f2800850(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQSHL_T1_ef800f10_ef000410(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQSHL_A1_fe800f10_f2000410(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQSHL_T1_ef800e10_ef800610(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQSHL_A1_fe800e10_f2800610(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQSHR_T1_ef800ed0_ef800810(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQSHR_A1_fe800ed0_f2800810(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQSUB_T1_ef800f10_ef000210(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VQSUB_A1_fe800f10_f2000210(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VRADDHN_T1_ff800f50_ff800400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VRADDHN_A1_ff800f50_f3800400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VRECPE_T1_ffb30e90_ffb30400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VRECPE_A1_ffb30e90_f3b30400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VRECPS_T1_ffa00f10_ef000f10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VRECPS_A1_ffa00f10_f2000f10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VREV_T1_ffb30e10_ffb00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VREV_A1_ffb30e10_f3b00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VRHADD_T1_ef800f10_ef000100(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VRHADD_A1_fe800f10_f2000100(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VRSHL_T1_ef800f10_ef000500(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VRSHL_A1_fe800f10_f2000500(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VRSHR_T1_ef800f10_ef800210(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VRSHR_A1_fe800f10_f2800210(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VRSHRN_T1_ff800fd0_ef800850(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VRSHRN_A1_ff800fd0_f2800850(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VRSQRTE_T1_ffb30e90_ffb30480(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VRSQRTE_A1_ffb30e90_f3b30480(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VRSQRTS_T1_ffa00f10_ef200f10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VRSQRTS_A1_ffa00f10_f2200f10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VRSRA_T1_ef800f10_ef800310(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VRSRA_A1_fe800f10_f2800310(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VRSUBHN_T1_ff800f50_ff800600(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VRSUBHN_A1_ff800f50_f3800600(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSHL_T1_ff800f10_ef800510(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSHL_A1_ff800f10_f2800510(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSHL_T1_ef800f10_ef000400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSHL_A1_fe800f10_f2000400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSHLL_T1_ef800fd0_ef800a10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSHLL_A1_fe800fd0_f2800a10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSHLL_T2_ffb30fd0_ffb20300(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSHLL_A2_ffb30fd0_f3b20300(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSHR_T1_ef800f10_ef800010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSHR_A1_fe800f10_f2800010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSHRN_T1_ff800fd0_ef800810(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSHRN_A1_ff800fd0_f2800810(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSLI_T1_ff800f10_ff800510(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSLI_A1_ff800f10_f3800510(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSQRT_T1_ffbf0ed0_eeb10ac0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSQRT_A1_0fbf0ed0_0eb10ac0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSRA_T1_ef800f10_ef800110(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSRA_A1_fe800f10_f2800110(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSRI_T1_ff800f10_ff800410(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSRI_A1_ff800f10_f3800410(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VST1_T1_ffb00000_f9000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VST1_A1_ffb00000_f4000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VST1_T1_ffb00300_f9800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VST1_A1_ffb00300_f4800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSTM_T1_fe100f00_ec000b00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSTM_A1_0e100f00_0c000b00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSTM_T2_fe100f00_ec000a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSTR_T1_ff300f00_ed000b00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSTR_A1_0f300f00_0d000b00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSTR_T2_ff300f00_ed000a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSTR_A2_0f300f00_0d000a00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSUB_T1_ff800f10_ff000800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSUB_A1_ff800f10_f3000800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSUB_T1_ffa00f10_ef200d00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSUB_A1_ffa00f10_f2200d00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSUB_T2_ffb00e50_ee300a40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSUB_A2_0fb00e50_0e300a40(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSUBHN_T1_ff800f50_ef800600(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSUBHN_A1_ff800f50_f2800600(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSUBL_T1_ef800e50_ef800200(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSUBW_A1_fe800e50_f2800200(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSWP_T1_ffb30f90_ffb20000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VSWP_A1_ffb30f90_f3b20000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VTRN_T1_ffb30f90_ffb20080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VTRN_A1_ffb30f90_f3b20080(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VTST_T1_ff800f10_ef000810(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VTST_A1_ff800f10_f2000810(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VUZP_T1_ffb30f90_ffb20100(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VUZP_A1_ffb30f90_f3b20100(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VZIP_T1_ffb30f90_ffb20180(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_VZIP_A1_ffb30f90_f3b20180(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_WFE_T1_0000ffff_0000bf20(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_WFE_T2_ffffffff_f3af8002(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_WFE_A1_0fffffff_0320f002(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_WFI_T1_0000ffff_0000bf30(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_WFI_T2_ffffffff_f3af8003(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_WFI_A1_0fffffff_0320f003(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_YIELD_T1_0000ffff_0000bf10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_YIELD_T2_ffffffff_f3af8001(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_YIELD_A1_0fffffff_0320f001(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_abs_X1_fffffc00_1e20b800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_abs_X1_fffffc00_0e20b800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_adc_X1_ffe0fc00_1a000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_adcs_X1_ffe0fc00_3a000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_add_X1_fffffc00_0b000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_add_X1_ff000000_01000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_add_X1_fffffc00_0b200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_add_X1_ffe0fc00_5e200400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_add_X1_ffe0fc00_0e208400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_addhn_X1_ffe0fc00_0e204000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_addhn2_X1_ffe0fc00_4e204000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_addp_X1_fffffc00_1e31b800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_addp_X1_ffe0fc00_0e20bc00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_adds_X1_fffffc00_2b000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_adds_X1_ff000000_21000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_adds_X1_fffffc00_2b200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_addv_X1_fffffc00_0e31b800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_adr_X1_9f000000_00000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_adrp_X1_9f000000_80000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_aesd_X1_fffffc00_0e285800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_aese_X1_fffffc00_0e284800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_aesimc_X1_fffffc00_0e287800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_aesmc_X1_fffffc00_0e286800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_and_X1_fffffc00_0a000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_and_X1_ff800000_02000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_and_X1_ffe0fc00_0e201c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ands_X1_ff800000_62000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ands_X1_fffffc00_6a000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_asrv_X1_ffe0fc00_1a802800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_b_X1_fc000000_04000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_b_X1_ff00001f_44000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_bfm_X1_ffc00000_23000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_bic_X1_fffffc00_0a200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_bic_X1_ffffffe0_2f001400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_bic_X1_ffffffe0_2f009400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_bic_X1_ffe0fc00_0e601c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_bics_X1_fffffc00_6a200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_bif_X1_ffe0fc00_2ee01c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_bit_X1_ffe0fc00_2ea01c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_bl_X1_fc000000_84000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_blr_X1_fffffc1f_46200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_br_X1_fffffc1f_46000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_brk_X1_ffe0001f_c4200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_bsl_X1_ffe0fc00_2e601c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cbnz_X1_ff000000_25000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cbz_X1_ff000000_24000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ccmn_X1_ffe0fc10_1a000800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ccmn_X1_ffe0fc10_1a400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ccmp_X1_ffe0fc10_5a000800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ccmp_X1_ffe0fc10_5a400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_clrex_X1_fffff0ff_45001040(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cls_X1_fffffc00_1a401400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cls_X1_fffffc00_0e204800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_clz_X1_fffffc00_1ac01000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_clz_X1_fffffc00_2e204800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cmeq_X1_fffffc00_1e209800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cmeq_X1_ffe0fc00_3e208c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cmeq_X1_fffffc00_0e209800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cmeq_X1_ffe0fc00_2e208c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cmge_X1_fffffc00_3e208800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cmge_X1_ffe0fc00_5e203c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cmge_X1_fffffc00_2e208800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cmge_X1_ffe0fc00_0e203c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cmgt_X1_fffffc00_5e208800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cmgt_X1_ffe0fc00_5e203400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cmgt_X1_fffffc00_0e208800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cmgt_X1_ffe0fc00_0e203400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cmhi_X1_ffe0fc00_3e203400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cmhi_X1_ffe0fc00_2e203400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cmhs_X1_ffe0fc00_3e203c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cmhs_X1_ffe0fc00_2e203c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cmle_X1_fffffc00_3e209800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cmle_X1_fffffc00_2e209800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cmlt_X1_fffffc00_1e20a800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cmlt_X1_fffffc00_0e20a800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cmtst_X1_ffe0fc00_5e208c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cmtst_X1_ffe0fc00_0e208c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_cnt_X1_fffffc00_0e205800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_csel_X1_ffe0fc00_1a800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_csinc_X1_ffe0fc00_1a000400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_csinv_X1_ffe0fc00_5a800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_csneg_X1_ffe0fc00_5a000400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_dcps1_X1_ffe0001f_c4200001(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_dcps2_X1_ffe0001f_c4200002(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_dcps3_X1_ffe0001f_c4200003(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_dmb_X1_ffffffff_45001020(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_drps_X1_ffffffff_46a00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_dsb_X1_ffffffff_45001000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_dup_X1_fffffc00_5e000400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_dup_X1_fffffc00_0e000400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_dup_X1_fffffc00_0e000c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_eon_X1_fffffc00_4a200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_eor_X1_fffffc00_4a000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_eor_X1_ff800000_42000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_eor_X1_ffe0fc00_2e201c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_eret_X1_ffffffff_46800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ext_X1_ffe08400_2e000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_extr_X1_ffe00000_03800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fabd_X1_ffe0fc00_3e209400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fabd_X1_ffe0fc00_2ea0d400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fabs_X1_fffffc00_1e20c000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fabs_X1_fffffc00_0e20f800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_facge_X1_ffe0fc00_3e206c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_facge_X1_ffe0fc00_0e20ec00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_facgt_X1_ffe0fc00_3ea06c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_facgt_X1_ffe0fc00_0ea0ec00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fadd_X1_ffe0fc00_1e202800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fadd_X1_ffe0fc00_0e20d400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_faddp_X1_fffffc00_0e30d800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_faddp_X1_ffe0fc00_2e20d400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fccmp_X1_ffe0fc10_1e200400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fccmpe_X1_ffe0fc10_1e200410(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcmeq_X1_fffffc00_1e20d800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcmeq_X1_ffe0fc00_5e202400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcmeq_X1_fffffc00_0e20d800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcmeq_X1_ffe0fc00_0e20e400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcmge_X1_fffffc00_3e20c800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcmge_X1_ffe0fc00_3e202400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcmge_X1_fffffc00_2e20c800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcmge_X1_ffe0fc00_2e20e400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcmgt_X1_fffffc00_1e20c800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcmgt_X1_ffe0fc00_3ea02400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcmgt_X1_fffffc00_0e20c800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcmgt_X1_ffe0fc00_0ea0e400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcmle_X1_fffffc00_3e20d800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcmle_X1_fffffc00_2e20d800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcmlt_X1_fffffc00_1e20e800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcmlt_X1_fffffc00_0e20e800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcmp_X1_ffe0fc1f_1e202000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcmp_X1_fffffc1f_1e202008(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcmpe_X1_ffe0fc1f_1e202010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcmpe_X1_fffffc1f_1e202018(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcsel_X1_ffe0fc00_1e200c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvt_X1_fffffc00_1e224000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtas_X1_fffffc00_1e240000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtas_X1_fffffc00_1e21c800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtas_X1_fffffc00_0e21c800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtau_X1_fffffc00_1e250000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtau_X1_fffffc00_3e21c800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtau_X1_fffffc00_2e21c800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtl_X1_fffffc00_0e217800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtl2_X1_fffffc00_4e217800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtms_X1_fffffc00_1e300000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtms_X1_fffffc00_1e21b800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtms_X1_fffffc00_0e21b800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtmu_X1_fffffc00_1e310000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtmu_X1_fffffc00_3e21b800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtmu_X1_fffffc00_2e21b800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtn_X1_fffffc00_0e216800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtn2_X1_fffffc00_4e216800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtns_X1_fffffc00_1e200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtns_X1_fffffc00_1e21a800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtns_X1_fffffc00_0e21a800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtnu_X1_fffffc00_1e210000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtnu_X1_fffffc00_3e21a800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtnu_X1_fffffc00_2e21a800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtps_X1_fffffc00_1e280000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtps_X1_fffffc00_1ea1a800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtps_X1_fffffc00_0ea1a800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtpu_X1_fffffc00_1e290000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtpu_X1_fffffc00_3ea1a800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtpu_X1_fffffc00_2ea1a800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtxn_X1_fffffc00_3e206800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtxn_X1_fffffc00_2e216800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtxn2_X1_fffffc00_6e216800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtzs_X1_fffffc00_1e380000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtzs_X1_ffff0000_1e000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtzs_X1_fffffc00_1ea1b800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtzs_X1_fffffc00_5f00ac00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtzs_X1_fffffc00_0ea1b800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtzs_X1_fffffc00_0f00bc00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtzu_X1_fffffc00_1e390000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtzu_X1_ffff0000_1e010000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtzu_X1_fffffc00_3ea1b800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtzu_X1_fffffc00_3f00bc00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtzu_X1_fffffc00_2ea1b800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fcvtzu_X1_fffffc00_2f00bc00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fdiv_X1_ffe0fc00_1e201800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fdiv_X1_ffe0fc00_2e20fc00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmadd_X1_ffe08000_1f000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmax_X1_ffe0fc00_1e204800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmax_X1_ffe0fc00_0e20f400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmaxnm_X1_ffe0fc00_1e206800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmaxnm_X1_ffe0fc00_0e20c400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmaxnmp_X1_fffffc00_1e30c800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmaxnmp_X1_ffe0fc00_2e20c400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmaxnmv_X1_fffffc00_0e30c800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmaxp_X1_fffffc00_1e30f800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmaxp_X1_ffe0fc00_2e20f400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmaxv_X1_fffffc00_0e30f800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmin_X1_ffe0fc00_1e205800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmin_X1_ffe0fc00_0ea0f400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fminnm_X1_ffe0fc00_1e207800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fminnm_X1_ffe0fc00_0ea0c400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fminnmp_X1_fffffc00_1eb0c800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fminnmp_X1_ffe0fc00_2ea0c400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fminnmv_X1_fffffc00_0eb0c800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fminp_X1_fffffc00_1eb0f800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fminp_X1_ffe0fc00_2ea0f400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fminv_X1_fffffc00_0eb0f800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmla_X1_ffe0fc00_5f000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmla_X1_ffe0fc00_0f001000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmla_X1_ffe0fc00_0e20cc00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmls_X1_ffe0fc00_5f004000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmls_X1_ffe0fc00_0f005000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmls_X1_ffe0fc00_0ea0cc00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmov_X1_fffffc00_1e204000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmov_X1_ffe01fe0_1e201000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmov_X1_fffffc00_1e270000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmov_X1_fffffc00_1e260000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmov_X1_fffffc00_1e2e0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmov_X1_fffffc00_1e2f0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmov_X1_ffffffe0_0f00f400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmov_X1_ffffffe0_2f00f400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmsub_X1_ffe08000_1f008000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmul_X1_ffe0fc00_1e200800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmul_X1_ffe0fc00_5f009000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmul_X1_ffe0fc00_0f009000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmul_X1_ffe0fc00_2e20dc00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmulx_X1_ffe0fc00_3f008000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmulx_X1_ffe0fc00_5e209c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmulx_X1_ffe0fc00_2f009000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fmulx_X1_ffe0fc00_0e20dc00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fneg_X1_fffffc00_1e214000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fneg_X1_fffffc00_2e20f800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fnmadd_X1_ffe08000_1f200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fnmsub_X1_ffe08000_1f208000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fnmul_X1_ffe0fc00_1e208800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_frecpe_X1_fffffc00_1ea1d800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_frecpe_X1_fffffc00_0ea1d800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_frecps_X1_ffe0fc00_5e207c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_frecps_X1_ffe0fc00_0e20fc00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_frecpx_X1_fffffc00_1e21f800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_frinta_X1_fffffc00_1e264000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_frinta_X1_fffffc00_2e218800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_frinti_X1_fffffc00_1e26c000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_frinti_X1_fffffc00_2ea19800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_frintm_X1_fffffc00_1e254000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_frintm_X1_fffffc00_0e219800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_frintn_X1_fffffc00_1e244000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_frintn_X1_fffffc00_0e218800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_frintp_X1_fffffc00_1e24c000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_frintp_X1_fffffc00_0ea18800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_frintx_X1_fffffc00_1e274000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_frintx_X1_fffffc00_2e219800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_frintz_X1_fffffc00_1e25c000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_frintz_X1_fffffc00_0ea19800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_frsqrte_X1_fffffc00_3ea1d800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_frsqrte_X1_fffffc00_2ea1d800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_frsqrts_X1_ffe0fc00_5ea07c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_frsqrts_X1_ffe0fc00_0ea0fc00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fsqrt_X1_fffffc00_1e21c000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fsqrt_X1_fffffc00_0e21f800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fsub_X1_ffe0fc00_1e203800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_fsub_X1_ffe0fc00_0ea0d400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_hint_X1_fffff01f_45002000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_hlt_X1_ffe0001f_c4000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_hvc_X1_ffe0001f_c4000002(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ins_X1_fffffc00_2e000400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ins_X1_fffffc00_0e001c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_isb_X1_ffffffff_450010c0(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ld1_X1_ffffffff_0dc00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ld1_X1_ffffffff_0d400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ld2_X1_ffffffff_0ce00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ld2_X1_ffffffff_0d600000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ld3_X1_ffffffff_0dc02000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ld3_X1_ffffffff_0d402000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ld4_X1_ffffffff_0ce02000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ld4_X1_ffffffff_0d602000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ld4_X1_ffffffff_0cc00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ld4_X1_ffffffff_0c400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldar_X1_ffffffe0_88c00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldarb_X1_ffffffe0_08c00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldarh_X1_ffffffe0_48c00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldaxp_X1_ffff83e0_08608000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldaxr_X1_ffffffe0_88408000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldaxrb_X1_ffffffe0_08408000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldaxrh_X1_ffffffe0_48408000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldnp_X1_fec003e0_2c400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldnp_X1_fec003e0_28400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldp_X1_fec003e0_2c400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldp_X1_fec003e0_2cc00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldp_X1_fec003e0_28c00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldpsw_X1_fec003e0_68400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldpsw_X1_fec003e0_68c00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldr_X1_ff000000_1c000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldr_X1_ffffffe0_3c400800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldr_X1_ffe007e0_3c400400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldr_X1_ffc00000_1d400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldr_X1_ff000000_18000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldr_X1_ffffffe0_b8600800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldr_X1_ffe007e0_b8400400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldr_X1_ffc00000_99400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldrb_X1_ffffffe0_38600800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldrb_X1_ffe007e0_38400400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldrb_X1_ffc00000_19400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldrh_X1_ffffffe0_78600800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldrh_X1_ffe007e0_78400400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldrh_X1_ffc00000_59400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldrsb_X1_ffffffe0_38a00800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldrsb_X1_ffe007e0_38800400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldrsb_X1_ffc00000_19800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldrsh_X1_ffffffe0_78a00800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldrsh_X1_ffe007e0_78800400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldrsh_X1_ffc00000_59800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldrsw_X1_ff000000_98000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldrsw_X1_ffffffe0_b8a00800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldrsw_X1_ffe007e0_b8800400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldrsw_X1_ffc00000_99800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldtr_X1_ffe007e0_b8400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldtrb_X1_ffe007e0_38400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldtrh_X1_ffe007e0_78400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldtrsb_X1_ffe007e0_38800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldtrsh_X1_ffe007e0_78800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldtrsw_X1_ffe007e0_b8800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldur_X1_ffe007e0_3c400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldur_X1_ffe007e0_b8400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldurb_X1_ffe007e0_38400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldurh_X1_ffe007e0_78400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldursb_X1_ffe007e0_38800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldursh_X1_ffe007e0_78800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldursw_X1_ffe007e0_b8800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldxp_X1_ffff83e0_08600000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldxr_X1_ffffffe0_88400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldxrb_X1_ffffffe0_08400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ldxrh_X1_ffffffe0_48400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_lslv_X1_ffe0fc00_1ac02000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_lsrv_X1_ffe0fc00_1a402400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_madd_X1_ffe08000_1b000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_mla_X1_ffe0fc00_0f000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_mla_X1_ffe0fc00_0e209400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_mls_X1_ffe0fc00_0f004000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_mls_X1_ffe0fc00_2e209400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_movi_X1_ffffffe0_2f00e400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_movi_X1_ffffffe0_0f00e400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_movi_X1_ffffffe0_0f000400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_movi_X1_ffffffe0_0f008400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_movi_X1_ffffffe0_0f00c400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_movk_X1_ffe00000_22800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_movn_X1_ffe00000_02800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_movz_X1_ffe00000_42800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_mrs_X1_ffffffe0_45300000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_msr_X1_fffff0ff_45000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_msr_X1_ffffffe0_45100000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_msub_X1_ffe08000_1b008000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_mul_X1_ffe0fc00_0f008000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_mul_X1_ffe0fc00_0e209c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_mvni_X1_ffffffe0_2f000400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_mvni_X1_ffffffe0_2f008400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_mvni_X1_ffffffe0_2f00c400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_neg_X1_fffffc00_3e20b800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_neg_X1_fffffc00_2e20b800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_not_X1_fffffc00_2e205800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_orn_X1_fffffc00_2a200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_orn_X1_ffe0fc00_0ee01c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_orr_X1_fffffc00_2a000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_orr_X1_ff800000_22000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_orr_X1_ffffffe0_0f001400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_orr_X1_ffffffe0_0f009400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_orr_X1_ffe0fc00_0ea01c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_pmul_X1_ffe0fc00_2e209c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_pmull_X1_ffe0fc00_0e20e000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_pmull_X1_ffe0fc00_0e60e000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_pmull2_X1_ffe0fc00_4e20e000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_pmull2_X1_ffe0fc00_4e60e000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_prfm_X1_ff00001f_d8000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_prfm_X1_ffffffff_f8a00800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_prfm_X1_ffc0001f_d9800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_prfum_X1_ffe007ff_f8800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_raddhn_X1_ffe0fc00_2e204000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_raddhn2_X1_ffe0fc00_6e204000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_rbit_X1_fffffc00_1ac00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_rbit_X1_fffffc00_2e605800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ret_X1_fffffc1f_46400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_rev_X1_fffffc00_5a800800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_rev_X1_fffffc00_5a000c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_rev16_X1_fffffc00_1a400400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_rev16_X1_fffffc00_0e201800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_rev32_X1_fffffc00_da800800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_rev32_X1_fffffc00_2e200800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_rev64_X1_fffffc00_0e200800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_rorv_X1_ffe0fc00_1a002c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_rshrn_X1_fffffc00_0f008c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_rshrn2_X1_fffffc00_4f008c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_rsubhn_X1_ffe0fc00_2e206000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_rsubhn2_X1_ffe0fc00_6e206000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_saba_X1_ffe0fc00_0e207c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sabal_X1_ffe0fc00_0e205000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sabal2_X1_ffe0fc00_4e205000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sabd_X1_ffe0fc00_0e207400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sabdl_X1_ffe0fc00_0e207000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sabdl2_X1_ffe0fc00_4e207000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sadalp_X1_fffffc00_0e206800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_saddl_X1_ffe0fc00_0e200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_saddl2_X1_ffe0fc00_4e200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_saddlp_X1_fffffc00_0e202800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_saddlv_X1_fffffc00_0e303800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_saddw_X1_ffe0fc00_0e201000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_saddw2_X1_ffe0fc00_4e201000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sbc_X1_ffe0fc00_5a000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sbcs_X1_ffe0fc00_7a000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sbfm_X1_ffc00000_03000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_scvtf_X1_fffffc00_1e220000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_scvtf_X1_ffff0000_1e020000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_scvtf_X1_fffffc00_1e21d800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_scvtf_X1_fffffc00_5f008400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_scvtf_X1_fffffc00_0e21d800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sdiv_X1_ffe0fc00_1a000c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sha1c_X1_ffe0fc00_5e000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sha1h_X1_fffffc00_5e200800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sha1m_X1_ffe0fc00_5e002000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sha1p_X1_ffe0fc00_5e001000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sha1su0_X1_ffe0fc00_5e003000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sha1su1_X1_fffffc00_5e201800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sha256h_X1_ffe0fc00_5e004000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sha256h2_X1_ffe0fc00_5e005000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sha256su0_X1_fffffc00_5e202800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sha256su1_X1_ffe0fc00_5e006000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_shadd_X1_ffe0fc00_0e200400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_shl_X1_fffffc00_5f005400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_shll_X1_fffffc00_0e213800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_shll2_X1_fffffc00_4e213800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_shsub_X1_ffe0fc00_0e202400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sli_X1_fffffc00_3f005000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_smaddl_X1_ffe08000_1b200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_smax_X1_ffe0fc00_0e206400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_smaxp_X1_ffe0fc00_0e20a400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_smaxv_X1_fffffc00_0e30a800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_smc_X1_ffe0001f_c4000003(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_smin_X1_ffe0fc00_0e206c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sminp_X1_ffe0fc00_0e20ac00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sminv_X1_fffffc00_0e31a800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_smlal_X1_ffe0fc00_0f002000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_smlal_X1_ffe0fc00_0e208000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_smlal2_X1_ffe0fc00_4f002000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_smlal2_X1_ffe0fc00_4e208000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_smlsl_X1_ffe0fc00_0f006000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_smlsl_X1_ffe0fc00_0e20a000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_smlsl2_X1_ffe0fc00_4f006000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_smlsl2_X1_ffe0fc00_4e20a000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_smov_X1_fffffc00_0e002c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_smsubl_X1_ffe08000_1b208000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_smulh_X1_ffe0fc00_1b400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_smull_X1_ffe0fc00_0f00a000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_smull_X1_ffe0fc00_0e20c000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_smull2_X1_ffe0fc00_4f00a000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_smull2_X1_ffe0fc00_4e20c000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqabs_X1_fffffc00_5e207800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqabs_X1_fffffc00_0e207800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqadd_X1_ffe0fc00_5e200c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqadd_X1_ffe0fc00_0e200c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqdmlal_X1_ffe0fc00_5f002000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqdmlal_X1_ffe0fc00_5e201000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqdmlal_X1_ffe0fc00_0f003000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqdmlal_X1_ffe0fc00_0e209000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqdmlal2_X1_ffe0fc00_4f003000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqdmlal2_X1_ffe0fc00_4e209000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqdmlsl_X1_ffe0fc00_5f006000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqdmlsl_X1_ffe0fc00_5e203000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqdmlsl_X1_ffe0fc00_0f007000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqdmlsl_X1_ffe0fc00_0e20b000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqdmlsl2_X1_ffe0fc00_4f007000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqdmlsl2_X1_ffe0fc00_4e20b000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqdmulh_X1_ffe0fc00_5f008000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqdmulh_X1_ffe0fc00_5e20b400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqdmulh_X1_ffe0fc00_0f00c000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqdmulh_X1_ffe0fc00_0e20b400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqdmull_X1_ffe0fc00_5f00b000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqdmull_X1_ffe0fc00_5e205000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqdmull_X1_ffe0fc00_0f00b000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqdmull_X1_ffe0fc00_0e20d000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqdmull2_X1_ffe0fc00_4f00b000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqdmull2_X1_ffe0fc00_4e20d000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqneg_X1_fffffc00_3e207800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqneg_X1_fffffc00_2e207800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqrdmulh_X1_ffe0fc00_5f00d000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqrdmulh_X1_ffe0fc00_3e20b400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqrdmulh_X1_ffe0fc00_0f00d000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqrdmulh_X1_ffe0fc00_2e20b400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqrshl_X1_ffe0fc00_5e201c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqrshl_X1_ffe0fc00_0e205c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqrshrn_X1_fffffc00_5f008c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqrshrn_X1_fffffc00_0f009c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqrshrn2_X1_fffffc00_4f009c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqrshrun_X1_fffffc00_3f008c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqrshrun_X1_fffffc00_2f008c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqrshrun2_X1_fffffc00_6f008c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqshl_X1_fffffc00_5f007400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqshl_X1_ffe0fc00_5e204c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqshl_X1_ffe0fc00_0e204c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqshlu_X1_fffffc00_3f006000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqshrn_X1_fffffc00_5f009400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqshrun_X1_fffffc00_3f008400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqsub_X1_ffe0fc00_5e202c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqsub_X1_ffe0fc00_0e202c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqxtn_X1_fffffc00_5e204800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqxtn_X1_fffffc00_0e214800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqxtn2_X1_fffffc00_4e214800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqxtun_X1_fffffc00_3e202800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqxtun_X1_fffffc00_2e212800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sqxtun2_X1_fffffc00_6e212800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_srhadd_X1_ffe0fc00_0e201400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sri_X1_fffffc00_3f004000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_srshl_X1_ffe0fc00_5e201400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_srshl_X1_ffe0fc00_0e205400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_srshr_X1_fffffc00_5f002400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_srsra_X1_fffffc00_5f003400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sshl_X1_ffe0fc00_5e204400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sshl_X1_ffe0fc00_0e204400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sshr_X1_fffffc00_5f000400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ssra_X1_fffffc00_5f001400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ssubl_X1_ffe0fc00_0e202000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ssubl2_X1_ffe0fc00_4e202000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ssubw_X1_ffe0fc00_0e203000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ssubw2_X1_ffe0fc00_4e203000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_st1_X1_ffffffff_0d800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_st1_X1_ffffffff_0d000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_st2_X1_ffffffff_0ca00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_st2_X1_ffffffff_0d200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_st3_X1_ffffffff_0d802000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_st3_X1_ffffffff_0d002000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_st4_X1_ffffffff_0ca02000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_st4_X1_ffffffff_0d202000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_st4_X1_ffffffff_0c800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_st4_X1_ffffffff_0c000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_stlr_X1_ffffffe0_88800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_stlrb_X1_ffffffe0_08800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_stlrh_X1_ffffffe0_48800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_stlxp_X1_ffe083e0_08208000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_stlxr_X1_ffe0ffe0_88008000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_stlxrb_X1_ffe0ffe0_08008000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_stlxrh_X1_ffe0ffe0_48008000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_stnp_X1_fec003e0_2c000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_stnp_X1_fec003e0_28000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_stp_X1_fec003e0_2c000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_stp_X1_fec003e0_2c800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_stp_X1_fec003e0_28800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_str_X1_ffffffe0_3c000800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_str_X1_ffe007e0_3c000400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_str_X1_ffc00000_1d000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_str_X1_ffffffe0_b8200800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_str_X1_ffe007e0_b8000400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_str_X1_ffc00000_99000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_strb_X1_ffffffe0_38200800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_strb_X1_ffe007e0_38000400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_strb_X1_ffc00000_19000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_strh_X1_ffffffe0_78200800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_strh_X1_ffe007e0_78000400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_strh_X1_ffc00000_59000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sttr_X1_ffe007e0_b8000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sttrb_X1_ffe007e0_38000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sttrh_X1_ffe007e0_78000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_stur_X1_ffe007e0_3c000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_stur_X1_ffe007e0_b8000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sturb_X1_ffe007e0_38000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sturh_X1_ffe007e0_78000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_stxp_X1_ffe083e0_08200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_stxr_X1_ffe0ffe0_88000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_stxrb_X1_ffe0ffe0_08000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_stxrh_X1_ffe0ffe0_48000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sub_X1_fffffc00_4b000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sub_X1_ff000000_41000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sub_X1_fffffc00_4b200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sub_X1_ffe0fc00_3e200400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sub_X1_ffe0fc00_2e208400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_subhn_X1_ffe0fc00_0e206000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_subhn2_X1_ffe0fc00_4e206000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_subs_X1_fffffc00_6b000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_subs_X1_ff000000_61000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_subs_X1_fffffc00_6b200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_suqadd_X1_fffffc00_5e203800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_suqadd_X1_fffffc00_0e203800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_svc_X1_ffe0001f_c4000001(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sys_X1_fff80000_45080000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_sysl_X1_fff80000_45280000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_tbl_X1_ffe0fc00_0e000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_tbnz_X1_7f000000_27000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_tbx_X1_ffe0fc00_0e001000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_tbz_X1_7f000000_26000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_trn1_X1_ffe0fc00_0e000800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_trn2_X1_ffe0fc00_0e004800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uaba_X1_ffe0fc00_2e207c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uabal_X1_ffe0fc00_2e205000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uabal2_X1_ffe0fc00_6e205000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uabd_X1_ffe0fc00_2e207400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uabdl_X1_ffe0fc00_2e207000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uabdl2_X1_ffe0fc00_6e207000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uadalp_X1_fffffc00_2e206800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uaddl_X1_ffe0fc00_2e200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uaddl2_X1_ffe0fc00_6e200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uaddlp_X1_fffffc00_2e202800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uaddlv_X1_fffffc00_2e303800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uaddw_X1_ffe0fc00_2e201000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uaddw2_X1_ffe0fc00_6e201000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ubfm_X1_ffc00000_43000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ucvtf_X1_fffffc00_1e230000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ucvtf_X1_ffff0000_1e030000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ucvtf_X1_fffffc00_3e21d800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ucvtf_X1_fffffc00_3f00a400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ucvtf_X1_fffffc00_2e21d800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_udiv_X1_ffe0fc00_1a800800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uhadd_X1_ffe0fc00_2e200400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uhsub_X1_ffe0fc00_2e202400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_umaddl_X1_ffe08000_0ba00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_umax_X1_ffe0fc00_2e206400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_umaxp_X1_ffe0fc00_2e20a400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_umaxv_X1_fffffc00_2e30a800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_umin_X1_ffe0fc00_2e206c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uminp_X1_ffe0fc00_2e20ac00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uminv_X1_fffffc00_2e31a800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_umlal_X1_ffe0fc00_2f002000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_umlal_X1_ffe0fc00_2e208000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_umlal2_X1_ffe0fc00_6f002000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_umlal2_X1_ffe0fc00_6e208000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_umlsl_X1_ffe0fc00_2f006000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_umlsl_X1_ffe0fc00_2e20a000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_umlsl2_X1_ffe0fc00_6f006000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_umlsl2_X1_ffe0fc00_6e20a000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_umov_X1_fffffc00_0e003c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_umsubl_X1_ffe08000_0ba08000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_umulh_X1_ffe0fc00_1bc00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_umull_X1_ffe0fc00_2f00a000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_umull_X1_ffe0fc00_2e20c000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_umull2_X1_ffe0fc00_6f00a000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_umull2_X1_ffe0fc00_6e20c000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uqadd_X1_ffe0fc00_3e200c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uqadd_X1_ffe0fc00_2e200c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uqrshl_X1_ffe0fc00_3e201c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uqrshl_X1_ffe0fc00_2e205c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uqrshrn_X1_fffffc00_3f009c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uqrshrn_X1_fffffc00_2f009c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uqrshrn2_X1_fffffc00_6f009c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uqshl_X1_fffffc00_3f007000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uqshl_X1_ffe0fc00_3e204c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uqshl_X1_ffe0fc00_2e204c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uqshrn_X1_fffffc00_3f009400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uqsub_X1_ffe0fc00_3e202c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uqsub_X1_ffe0fc00_2e202c00(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uqxtn_X1_fffffc00_3e204800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uqxtn_X1_fffffc00_2e214800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uqxtn2_X1_fffffc00_6e214800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_urecpe_X1_fffffc00_0ea1c800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_urhadd_X1_ffe0fc00_2e201400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_urshl_X1_ffe0fc00_3e201400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_urshl_X1_ffe0fc00_2e205400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_urshr_X1_fffffc00_3f002000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ursqrte_X1_fffffc00_2ea1c800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ursra_X1_fffffc00_3f003000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ushl_X1_ffe0fc00_3e204400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ushl_X1_ffe0fc00_2e204400(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_ushr_X1_fffffc00_3f000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_usqadd_X1_fffffc00_3e203800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_usqadd_X1_fffffc00_2e203800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_usra_X1_fffffc00_3f001000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_usubl_X1_ffe0fc00_2e202000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_usubl2_X1_ffe0fc00_6e202000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_usubw_X1_ffe0fc00_2e203000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_usubw2_X1_ffe0fc00_6e203000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uzp1_X1_ffe0fc00_0e001800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_uzp2_X1_ffe0fc00_0e005800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_xtn_X1_fffffc00_0e212800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_xtn2_X1_fffffc00_4e212800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_zip1_X1_ffe0fc00_0e003800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool Instruction_zip2_X1_ffe0fc00_0e007800(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool DisassembleArm(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
bool DisassembleThumb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);
