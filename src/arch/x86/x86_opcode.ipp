/* This file has been automatically generated, you must _NOT_ edit it directly. (Tue May 26 21:55:59 2015) */
enum X86Opcode
{
  X86_Opcode_Unknown,
  X86_Opcode_Aaa,
  X86_Opcode_Aad,
  X86_Opcode_Aam,
  X86_Opcode_Aas,
  X86_Opcode_Adc,
  X86_Opcode_Add,
  X86_Opcode_Aesdec,
  X86_Opcode_Aesdeclast,
  X86_Opcode_Aesenc,
  X86_Opcode_Aesenclast,
  X86_Opcode_Aesimc,
  X86_Opcode_Aeskeygenassist,
  X86_Opcode_Altinst,
  X86_Opcode_And,
  X86_Opcode_Andn,
  X86_Opcode_Arpl,
  X86_Opcode_Bb0_reset,
  X86_Opcode_Bb1_reset,
  X86_Opcode_Bextr,
  X86_Opcode_Blendpd,
  X86_Opcode_Blendps,
  X86_Opcode_Blendvpd,
  X86_Opcode_Blendvps,
  X86_Opcode_Blendw,
  X86_Opcode_Blsi,
  X86_Opcode_Blsmsk,
  X86_Opcode_Blsr,
  X86_Opcode_Bound,
  X86_Opcode_Bsf,
  X86_Opcode_Bsr,
  X86_Opcode_Bswap,
  X86_Opcode_Bt,
  X86_Opcode_Btc,
  X86_Opcode_Btr,
  X86_Opcode_Bts,
  X86_Opcode_Bzhi,
  X86_Opcode_Call,
  X86_Opcode_Cbw,
  X86_Opcode_Cdq,
  X86_Opcode_Cdqe,
  X86_Opcode_Clc,
  X86_Opcode_Cld,
  X86_Opcode_Clflush,
  X86_Opcode_Clgi,
  X86_Opcode_Cli,
  X86_Opcode_Clts,
  X86_Opcode_Cmc,
  X86_Opcode_Cmovb,
  X86_Opcode_Cmovbe,
  X86_Opcode_Cmovl,
  X86_Opcode_Cmovle,
  X86_Opcode_Cmovnb,
  X86_Opcode_Cmovnbe,
  X86_Opcode_Cmovnl,
  X86_Opcode_Cmovnle,
  X86_Opcode_Cmovno,
  X86_Opcode_Cmovnp,
  X86_Opcode_Cmovns,
  X86_Opcode_Cmovnz,
  X86_Opcode_Cmovo,
  X86_Opcode_Cmovp,
  X86_Opcode_Cmovs,
  X86_Opcode_Cmovz,
  X86_Opcode_Cmp,
  X86_Opcode_Cmps,
  X86_Opcode_Cmpxchg,
  X86_Opcode_Cmpxchg16b,
  X86_Opcode_Cmpxchg8b,
  X86_Opcode_Cpu_read,
  X86_Opcode_Cpu_write,
  X86_Opcode_Cpuid,
  X86_Opcode_Cqo,
  X86_Opcode_Crc32,
  X86_Opcode_Cvtpd2pi,
  X86_Opcode_Cvtpi2pd,
  X86_Opcode_Cvtpi2ps,
  X86_Opcode_Cvtps2pi,
  X86_Opcode_Cvttpd2pi,
  X86_Opcode_Cvttps2pi,
  X86_Opcode_Cwd,
  X86_Opcode_Cwde,
  X86_Opcode_Daa,
  X86_Opcode_Das,
  X86_Opcode_Dec,
  X86_Opcode_Div,
  X86_Opcode_Dmint,
  X86_Opcode_Dppd,
  X86_Opcode_Dpps,
  X86_Opcode_Emms,
  X86_Opcode_Enter,
  X86_Opcode_Extractps,
  X86_Opcode_Extrq,
  X86_Opcode_F2xm1,
  X86_Opcode_F4x4,
  X86_Opcode_Fabs,
  X86_Opcode_Fadd,
  X86_Opcode_Faddp,
  X86_Opcode_Fbld,
  X86_Opcode_Fbstp,
  X86_Opcode_Fchs,
  X86_Opcode_Fcmovb,
  X86_Opcode_Fcmovbe,
  X86_Opcode_Fcmove,
  X86_Opcode_Fcmovnb,
  X86_Opcode_Fcmovnbe,
  X86_Opcode_Fcmovne,
  X86_Opcode_Fcmovnu,
  X86_Opcode_Fcmovu,
  X86_Opcode_Fcom,
  X86_Opcode_Fcomi,
  X86_Opcode_Fcomip,
  X86_Opcode_Fcomp,
  X86_Opcode_Fcompp,
  X86_Opcode_Fcos,
  X86_Opcode_Fdecstp,
  X86_Opcode_Fdiv,
  X86_Opcode_Fdivp,
  X86_Opcode_Fdivr,
  X86_Opcode_Fdivrp,
  X86_Opcode_Femms,
  X86_Opcode_Ffree,
  X86_Opcode_Ffreep,
  X86_Opcode_Fiadd,
  X86_Opcode_Ficom,
  X86_Opcode_Ficomp,
  X86_Opcode_Fidiv,
  X86_Opcode_Fidivr,
  X86_Opcode_Fild,
  X86_Opcode_Fimul,
  X86_Opcode_Fincstp,
  X86_Opcode_Fist,
  X86_Opcode_Fistp,
  X86_Opcode_Fisttp,
  X86_Opcode_Fisub,
  X86_Opcode_Fisubr,
  X86_Opcode_Fld,
  X86_Opcode_Fld1,
  X86_Opcode_Fldcw,
  X86_Opcode_Fldenv,
  X86_Opcode_Fldl2e,
  X86_Opcode_Fldl2t,
  X86_Opcode_Fldlg2,
  X86_Opcode_Fldln2,
  X86_Opcode_Fldpi,
  X86_Opcode_Fldz,
  X86_Opcode_Fmul,
  X86_Opcode_Fmulp,
  X86_Opcode_Fnclex,
  X86_Opcode_Fndisi,
  X86_Opcode_Fneni,
  X86_Opcode_Fninit,
  X86_Opcode_Fnop,
  X86_Opcode_Fnsave,
  X86_Opcode_Fnsetpm,
  X86_Opcode_Fnstcw,
  X86_Opcode_Fnstenv,
  X86_Opcode_Fnstsw,
  X86_Opcode_Fpatan,
  X86_Opcode_Fprem,
  X86_Opcode_Fprem1,
  X86_Opcode_Fptan,
  X86_Opcode_Frichop,
  X86_Opcode_Frinear,
  X86_Opcode_Frint2,
  X86_Opcode_Frndint,
  X86_Opcode_Frstor,
  X86_Opcode_Frstpm,
  X86_Opcode_Fsbp0,
  X86_Opcode_Fsbp1,
  X86_Opcode_Fsbp2,
  X86_Opcode_Fscale,
  X86_Opcode_Fsin,
  X86_Opcode_Fsincos,
  X86_Opcode_Fsqrt,
  X86_Opcode_Fst,
  X86_Opcode_Fstdw,
  X86_Opcode_Fstp,
  X86_Opcode_Fstsg,
  X86_Opcode_Fsub,
  X86_Opcode_Fsubp,
  X86_Opcode_Fsubr,
  X86_Opcode_Fsubrp,
  X86_Opcode_Ftst,
  X86_Opcode_Ftstp,
  X86_Opcode_Fucom,
  X86_Opcode_Fucomi,
  X86_Opcode_Fucomip,
  X86_Opcode_Fucomp,
  X86_Opcode_Fucompp,
  X86_Opcode_Fwait,
  X86_Opcode_Fxam,
  X86_Opcode_Fxch,
  X86_Opcode_Fxrstor,
  X86_Opcode_Fxsave,
  X86_Opcode_Fxtract,
  X86_Opcode_Fyl2x,
  X86_Opcode_Fyl2xp1,
  X86_Opcode_Getsec,
  X86_Opcode_Hint_nop,
  X86_Opcode_Hlt,
  X86_Opcode_Ibts,
  X86_Opcode_Icebp,
  X86_Opcode_Idiv,
  X86_Opcode_Imul,
  X86_Opcode_In,
  X86_Opcode_Inc,
  X86_Opcode_Ins,
  X86_Opcode_Insertps,
  X86_Opcode_Int,
  X86_Opcode_Into,
  X86_Opcode_Invd,
  X86_Opcode_Invept,
  X86_Opcode_Invlpg,
  X86_Opcode_Invlpga,
  X86_Opcode_Invpcid,
  X86_Opcode_Invvpid,
  X86_Opcode_Iret,
  X86_Opcode_Jb,
  X86_Opcode_Jbe,
  X86_Opcode_Jcxz,
  X86_Opcode_Jecxz,
  X86_Opcode_Jl,
  X86_Opcode_Jle,
  X86_Opcode_Jmp,
  X86_Opcode_Jmpe,
  X86_Opcode_Jnb,
  X86_Opcode_Jnbe,
  X86_Opcode_Jnl,
  X86_Opcode_Jnle,
  X86_Opcode_Jno,
  X86_Opcode_Jnp,
  X86_Opcode_Jns,
  X86_Opcode_Jnz,
  X86_Opcode_Jo,
  X86_Opcode_Jp,
  X86_Opcode_Jrcxz,
  X86_Opcode_Js,
  X86_Opcode_Jz,
  X86_Opcode_Lahf,
  X86_Opcode_Lar,
  X86_Opcode_Lds,
  X86_Opcode_Lea,
  X86_Opcode_Leave,
  X86_Opcode_Les,
  X86_Opcode_Lfence,
  X86_Opcode_Lfs,
  X86_Opcode_Lgdt,
  X86_Opcode_Lgs,
  X86_Opcode_Lidt,
  X86_Opcode_Lldt,
  X86_Opcode_Lmsw,
  X86_Opcode_Loadall,
  X86_Opcode_Lods,
  X86_Opcode_Loop,
  X86_Opcode_Loopnz,
  X86_Opcode_Loopz,
  X86_Opcode_Lsl,
  X86_Opcode_Lss,
  X86_Opcode_Ltr,
  X86_Opcode_Lzcnt,
  X86_Opcode_Maskmovq,
  X86_Opcode_Mfence,
  X86_Opcode_Monitor,
  X86_Opcode_Montmul,
  X86_Opcode_Mov,
  X86_Opcode_Movbe,
  X86_Opcode_Movd,
  X86_Opcode_Movdq2q,
  X86_Opcode_Movntdqa,
  X86_Opcode_Movnti,
  X86_Opcode_Movntq,
  X86_Opcode_Movntsd,
  X86_Opcode_Movntss,
  X86_Opcode_Movq,
  X86_Opcode_Movq2dq,
  X86_Opcode_Movs,
  X86_Opcode_Movsx,
  X86_Opcode_Movzx,
  X86_Opcode_Mpsadbw,
  X86_Opcode_Mul,
  X86_Opcode_Mulx,
  X86_Opcode_Mwait,
  X86_Opcode_Neg,
  X86_Opcode_Nop,
  X86_Opcode_Not,
  X86_Opcode_Or,
  X86_Opcode_Out,
  X86_Opcode_Outs,
  X86_Opcode_Pabsb,
  X86_Opcode_Pabsd,
  X86_Opcode_Pabsw,
  X86_Opcode_Packssdw,
  X86_Opcode_Packsswb,
  X86_Opcode_Packusdw,
  X86_Opcode_Packuswb,
  X86_Opcode_Paddb,
  X86_Opcode_Paddd,
  X86_Opcode_Paddq,
  X86_Opcode_Paddsb,
  X86_Opcode_Paddsw,
  X86_Opcode_Paddusb,
  X86_Opcode_Paddusw,
  X86_Opcode_Paddw,
  X86_Opcode_Palignr,
  X86_Opcode_Pand,
  X86_Opcode_Pandn,
  X86_Opcode_Pavgb,
  X86_Opcode_Pavgusb,
  X86_Opcode_Pavgw,
  X86_Opcode_Pblendvb,
  X86_Opcode_Pclmulqdq,
  X86_Opcode_Pcmpeqb,
  X86_Opcode_Pcmpeqd,
  X86_Opcode_Pcmpeqq,
  X86_Opcode_Pcmpeqw,
  X86_Opcode_Pcmpestri,
  X86_Opcode_Pcmpestrm,
  X86_Opcode_Pcmpgtb,
  X86_Opcode_Pcmpgtd,
  X86_Opcode_Pcmpgtq,
  X86_Opcode_Pcmpgtw,
  X86_Opcode_Pcmpistri,
  X86_Opcode_Pcmpistrm,
  X86_Opcode_Pd2id,
  X86_Opcode_Pdep,
  X86_Opcode_Pext,
  X86_Opcode_Pextrb,
  X86_Opcode_Pextrd,
  X86_Opcode_Pextrw,
  X86_Opcode_Pf2iw,
  X86_Opcode_Pfacc,
  X86_Opcode_Pfadd,
  X86_Opcode_Pfcmge,
  X86_Opcode_Pfcmpeq,
  X86_Opcode_Pfcmpgt,
  X86_Opcode_Pfmax,
  X86_Opcode_Pfmin,
  X86_Opcode_Pfmul,
  X86_Opcode_Pfnacc,
  X86_Opcode_Pfpnacc,
  X86_Opcode_Pfrcp,
  X86_Opcode_Pfrcpit1,
  X86_Opcode_Pfrcpit2,
  X86_Opcode_Pfrcpv,
  X86_Opcode_Pfrsqit1,
  X86_Opcode_Pfrsqrt,
  X86_Opcode_Pfrsqrtv,
  X86_Opcode_Pfsub,
  X86_Opcode_Pfsubr,
  X86_Opcode_Phaddd,
  X86_Opcode_Phaddsw,
  X86_Opcode_Phaddw,
  X86_Opcode_Phminposuw,
  X86_Opcode_Phsubd,
  X86_Opcode_Phsubsw,
  X86_Opcode_Phsubw,
  X86_Opcode_Pi2fd,
  X86_Opcode_Pi2fw,
  X86_Opcode_Pinsrb,
  X86_Opcode_Pinsrd,
  X86_Opcode_Pinsrw,
  X86_Opcode_Pmaddubsw,
  X86_Opcode_Pmaddwd,
  X86_Opcode_Pmaxsb,
  X86_Opcode_Pmaxsd,
  X86_Opcode_Pmaxsw,
  X86_Opcode_Pmaxub,
  X86_Opcode_Pmaxud,
  X86_Opcode_Pmaxuw,
  X86_Opcode_Pminsb,
  X86_Opcode_Pminsd,
  X86_Opcode_Pminsw,
  X86_Opcode_Pminub,
  X86_Opcode_Pminud,
  X86_Opcode_Pminuw,
  X86_Opcode_Pmovmskb,
  X86_Opcode_Pmovsxbd,
  X86_Opcode_Pmovsxbq,
  X86_Opcode_Pmovsxbw,
  X86_Opcode_Pmovsxdq,
  X86_Opcode_Pmovsxwd,
  X86_Opcode_Pmovsxwq,
  X86_Opcode_Pmovzxbd,
  X86_Opcode_Pmovzxbq,
  X86_Opcode_Pmovzxbw,
  X86_Opcode_Pmovzxdq,
  X86_Opcode_Pmovzxwd,
  X86_Opcode_Pmovzxwq,
  X86_Opcode_Pmuldq,
  X86_Opcode_Pmulhrsw,
  X86_Opcode_Pmulhrw,
  X86_Opcode_Pmulhuw,
  X86_Opcode_Pmulhw,
  X86_Opcode_Pmulld,
  X86_Opcode_Pmullw,
  X86_Opcode_Pmuludq,
  X86_Opcode_Pop,
  X86_Opcode_Popa,
  X86_Opcode_Popad,
  X86_Opcode_Popcnt,
  X86_Opcode_Popf,
  X86_Opcode_Por,
  X86_Opcode_Psadbw,
  X86_Opcode_Pshufb,
  X86_Opcode_Pshufw,
  X86_Opcode_Psignb,
  X86_Opcode_Psignd,
  X86_Opcode_Psignw,
  X86_Opcode_Pslld,
  X86_Opcode_Psllq,
  X86_Opcode_Psllw,
  X86_Opcode_Psrad,
  X86_Opcode_Psraw,
  X86_Opcode_Psrld,
  X86_Opcode_Psrlq,
  X86_Opcode_Psrlw,
  X86_Opcode_Psubb,
  X86_Opcode_Psubd,
  X86_Opcode_Psubq,
  X86_Opcode_Psubsb,
  X86_Opcode_Psubsw,
  X86_Opcode_Psubusb,
  X86_Opcode_Psubusw,
  X86_Opcode_Psubw,
  X86_Opcode_Pswapd,
  X86_Opcode_Ptest,
  X86_Opcode_Punpckhbw,
  X86_Opcode_Punpckhdq,
  X86_Opcode_Punpckhwd,
  X86_Opcode_Punpcklbw,
  X86_Opcode_Punpckldq,
  X86_Opcode_Push,
  X86_Opcode_Pusha,
  X86_Opcode_Pushad,
  X86_Opcode_Pushf,
  X86_Opcode_Pxor,
  X86_Opcode_Rcl,
  X86_Opcode_Rcr,
  X86_Opcode_Rdfsbase,
  X86_Opcode_Rdgsbase,
  X86_Opcode_Rdm,
  X86_Opcode_Rdmsr,
  X86_Opcode_Rdpmc,
  X86_Opcode_Rdshr,
  X86_Opcode_Rdtsc,
  X86_Opcode_Rdtscp,
  X86_Opcode_Ret,
  X86_Opcode_Retf,
  X86_Opcode_Retn,
  X86_Opcode_Rol,
  X86_Opcode_Ror,
  X86_Opcode_Rorx,
  X86_Opcode_Roundpd,
  X86_Opcode_Roundps,
  X86_Opcode_Roundsd,
  X86_Opcode_Roundss,
  X86_Opcode_Rsm,
  X86_Opcode_Sahf,
  X86_Opcode_Sal,
  X86_Opcode_Salc,
  X86_Opcode_Sar,
  X86_Opcode_Sarx,
  X86_Opcode_Sbb,
  X86_Opcode_Scas,
  X86_Opcode_Setb,
  X86_Opcode_Setbe,
  X86_Opcode_Setl,
  X86_Opcode_Setle,
  X86_Opcode_Setnb,
  X86_Opcode_Setnbe,
  X86_Opcode_Setnl,
  X86_Opcode_Setnle,
  X86_Opcode_Setno,
  X86_Opcode_Setnp,
  X86_Opcode_Setns,
  X86_Opcode_Setnz,
  X86_Opcode_Seto,
  X86_Opcode_Setp,
  X86_Opcode_Sets,
  X86_Opcode_Setz,
  X86_Opcode_Sfence,
  X86_Opcode_Sgdt,
  X86_Opcode_Shl,
  X86_Opcode_Shld,
  X86_Opcode_Shlx,
  X86_Opcode_Shr,
  X86_Opcode_Shrd,
  X86_Opcode_Shrx,
  X86_Opcode_Sidt,
  X86_Opcode_Skinit,
  X86_Opcode_Sldt,
  X86_Opcode_Smint,
  X86_Opcode_Smsw,
  X86_Opcode_Stc,
  X86_Opcode_Std,
  X86_Opcode_Stgi,
  X86_Opcode_Sti,
  X86_Opcode_Stos,
  X86_Opcode_Str,
  X86_Opcode_Sub,
  X86_Opcode_Swapgs,
  X86_Opcode_Syscall,
  X86_Opcode_Sysenter,
  X86_Opcode_Sysexit,
  X86_Opcode_Sysret,
  X86_Opcode_Test,
  X86_Opcode_Tzcnt,
  X86_Opcode_Ud,
  X86_Opcode_Ud1,
  X86_Opcode_Ud2,
  X86_Opcode_Umov,
  X86_Opcode_Vaddpd,
  X86_Opcode_Vaddps,
  X86_Opcode_Vaddsd,
  X86_Opcode_Vaddss,
  X86_Opcode_Vaddsubps,
  X86_Opcode_Vaesdec,
  X86_Opcode_Vaesdeclast,
  X86_Opcode_Vaesenc,
  X86_Opcode_Vaesenclast,
  X86_Opcode_Vaesimc,
  X86_Opcode_Vaeskeygenassist,
  X86_Opcode_Vandnpd,
  X86_Opcode_Vandnps,
  X86_Opcode_Vandpd,
  X86_Opcode_Vandps,
  X86_Opcode_Vblendpd,
  X86_Opcode_Vblendps,
  X86_Opcode_Vblendvpd,
  X86_Opcode_Vblendvps,
  X86_Opcode_Vblendw,
  X86_Opcode_Vbroadcastf128,
  X86_Opcode_Vbroadcasti128,
  X86_Opcode_Vbroadcastsd,
  X86_Opcode_Vbroadcastss,
  X86_Opcode_Vcmpgtd,
  X86_Opcode_Vcomisd,
  X86_Opcode_Vcomiss,
  X86_Opcode_Vcvtdq2pd,
  X86_Opcode_Vcvtdq2ps,
  X86_Opcode_Vcvtpd2dq,
  X86_Opcode_Vcvtpd2ps,
  X86_Opcode_Vcvtph2ps,
  X86_Opcode_Vcvtps2dq,
  X86_Opcode_Vcvtps2pd,
  X86_Opcode_Vcvtps2ph,
  X86_Opcode_Vcvtsd2si,
  X86_Opcode_Vcvtsd2ss,
  X86_Opcode_Vcvtsi2sd,
  X86_Opcode_Vcvtsi2ss,
  X86_Opcode_Vcvtss2sd,
  X86_Opcode_Vcvtss2si,
  X86_Opcode_Vcvttpd2dq,
  X86_Opcode_Vcvttps2dq,
  X86_Opcode_Vcvttsd2si,
  X86_Opcode_Vcvttss2si,
  X86_Opcode_Vdivpd,
  X86_Opcode_Vdivps,
  X86_Opcode_Vdivsd,
  X86_Opcode_Vdivss,
  X86_Opcode_Vdppd,
  X86_Opcode_Vdpps,
  X86_Opcode_Verr,
  X86_Opcode_Verw,
  X86_Opcode_Vextractf128,
  X86_Opcode_Vextracti128,
  X86_Opcode_Vextractps,
  X86_Opcode_Vfmadd132pd,
  X86_Opcode_Vfmadd132ps,
  X86_Opcode_Vfmadd132sd,
  X86_Opcode_Vfmadd132ss,
  X86_Opcode_Vfmadd213pd,
  X86_Opcode_Vfmadd213ps,
  X86_Opcode_Vfmadd213sd,
  X86_Opcode_Vfmadd213ss,
  X86_Opcode_Vfmadd231pd,
  X86_Opcode_Vfmadd231ps,
  X86_Opcode_Vfmadd231sd,
  X86_Opcode_Vfmadd231ss,
  X86_Opcode_Vfmaddsub132pd,
  X86_Opcode_Vfmaddsub132ps,
  X86_Opcode_Vfmaddsub213pd,
  X86_Opcode_Vfmaddsub213ps,
  X86_Opcode_Vfmaddsub231pd,
  X86_Opcode_Vfmaddsub231ps,
  X86_Opcode_Vfmsub132pd,
  X86_Opcode_Vfmsub132ps,
  X86_Opcode_Vfmsub132sd,
  X86_Opcode_Vfmsub132ss,
  X86_Opcode_Vfmsub213pd,
  X86_Opcode_Vfmsub213ps,
  X86_Opcode_Vfmsub213sd,
  X86_Opcode_Vfmsub213ss,
  X86_Opcode_Vfmsub231pd,
  X86_Opcode_Vfmsub231ps,
  X86_Opcode_Vfmsub231sd,
  X86_Opcode_Vfmsub231ss,
  X86_Opcode_Vfmsubadd132pd,
  X86_Opcode_Vfmsubadd132ps,
  X86_Opcode_Vfmsubadd213pd,
  X86_Opcode_Vfmsubadd213ps,
  X86_Opcode_Vfmsubadd231pd,
  X86_Opcode_Vfmsubadd231ps,
  X86_Opcode_Vfnmadd132pd,
  X86_Opcode_Vfnmadd132ps,
  X86_Opcode_Vfnmadd132sd,
  X86_Opcode_Vfnmadd132ss,
  X86_Opcode_Vfnmadd213pd,
  X86_Opcode_Vfnmadd213ps,
  X86_Opcode_Vfnmadd213sd,
  X86_Opcode_Vfnmadd213ss,
  X86_Opcode_Vfnmadd231pd,
  X86_Opcode_Vfnmadd231ps,
  X86_Opcode_Vfnmadd231sd,
  X86_Opcode_Vfnmadd231ss,
  X86_Opcode_Vfnmsub132pd,
  X86_Opcode_Vfnmsub132ps,
  X86_Opcode_Vfnmsub132sd,
  X86_Opcode_Vfnmsub132ss,
  X86_Opcode_Vfnmsub213pd,
  X86_Opcode_Vfnmsub213ps,
  X86_Opcode_Vfnmsub213sd,
  X86_Opcode_Vfnmsub213ss,
  X86_Opcode_Vfnmsub231pd,
  X86_Opcode_Vfnmsub231ps,
  X86_Opcode_Vfnmsub231sd,
  X86_Opcode_Vfnmsub231ss,
  X86_Opcode_Vhaddpd,
  X86_Opcode_Vhaddps,
  X86_Opcode_Vhsubpd,
  X86_Opcode_Vhsubps,
  X86_Opcode_Vinsertf128,
  X86_Opcode_Vinserti128,
  X86_Opcode_Vinsertps,
  X86_Opcode_Vlddqu,
  X86_Opcode_Vldmxcsr,
  X86_Opcode_Vmaskmovdqu,
  X86_Opcode_Vmaskmovpd,
  X86_Opcode_Vmaskmovps,
  X86_Opcode_Vmaxpd,
  X86_Opcode_Vmaxps,
  X86_Opcode_Vmaxsd,
  X86_Opcode_Vmaxss,
  X86_Opcode_Vmcall,
  X86_Opcode_Vmclear,
  X86_Opcode_Vminpd,
  X86_Opcode_Vminps,
  X86_Opcode_Vminsd,
  X86_Opcode_Vminss,
  X86_Opcode_Vmlaunch,
  X86_Opcode_Vmload,
  X86_Opcode_Vmmcall,
  X86_Opcode_Vmodqa,
  X86_Opcode_Vmovapd,
  X86_Opcode_Vmovaps,
  X86_Opcode_Vmovd,
  X86_Opcode_Vmovddup,
  X86_Opcode_Vmovdqa,
  X86_Opcode_Vmovdqu,
  X86_Opcode_Vmovhpd,
  X86_Opcode_Vmovhps,
  X86_Opcode_Vmovlpd,
  X86_Opcode_Vmovlps,
  X86_Opcode_Vmovmskpd,
  X86_Opcode_Vmovmskps,
  X86_Opcode_Vmovntdq,
  X86_Opcode_Vmovntdqa,
  X86_Opcode_Vmovntpd,
  X86_Opcode_Vmovntps,
  X86_Opcode_Vmovq,
  X86_Opcode_Vmovsd,
  X86_Opcode_Vmovshdup,
  X86_Opcode_Vmovsldup,
  X86_Opcode_Vmovss,
  X86_Opcode_Vmovupd,
  X86_Opcode_Vmovups,
  X86_Opcode_Vmpsadbw,
  X86_Opcode_Vmptrld,
  X86_Opcode_Vmptrst,
  X86_Opcode_Vmread,
  X86_Opcode_Vmresume,
  X86_Opcode_Vmrun,
  X86_Opcode_Vmsave,
  X86_Opcode_Vmulpd,
  X86_Opcode_Vmulps,
  X86_Opcode_Vmulsd,
  X86_Opcode_Vmulss,
  X86_Opcode_Vmwrite,
  X86_Opcode_Vmxoff,
  X86_Opcode_Vmxon,
  X86_Opcode_Vorpd,
  X86_Opcode_Vorps,
  X86_Opcode_Vpabsb,
  X86_Opcode_Vpabsd,
  X86_Opcode_Vpabsw,
  X86_Opcode_Vpackssdw,
  X86_Opcode_Vpacksswb,
  X86_Opcode_Vpackusdw,
  X86_Opcode_Vpackuswb,
  X86_Opcode_Vpaddb,
  X86_Opcode_Vpaddd,
  X86_Opcode_Vpaddq,
  X86_Opcode_Vpaddsb,
  X86_Opcode_Vpaddsw,
  X86_Opcode_Vpaddusb,
  X86_Opcode_Vpaddusw,
  X86_Opcode_Vpaddw,
  X86_Opcode_Vpalignr,
  X86_Opcode_Vpand,
  X86_Opcode_Vpandn,
  X86_Opcode_Vpavgb,
  X86_Opcode_Vpavgw,
  X86_Opcode_Vpblendd,
  X86_Opcode_Vpblendvp,
  X86_Opcode_Vpbroadcastb,
  X86_Opcode_Vpbroadcastd,
  X86_Opcode_Vpbroadcastq,
  X86_Opcode_Vpbroadcastw,
  X86_Opcode_Vpclmulqdq,
  X86_Opcode_Vpcmpeqb,
  X86_Opcode_Vpcmpeqd,
  X86_Opcode_Vpcmpeqq,
  X86_Opcode_Vpcmpeqw,
  X86_Opcode_Vpcmpestri,
  X86_Opcode_Vpcmpestrm,
  X86_Opcode_Vpcmpgtb,
  X86_Opcode_Vpcmpgtq,
  X86_Opcode_Vpcmpgtw,
  X86_Opcode_Vpcmpistri,
  X86_Opcode_Vpcmpistrm,
  X86_Opcode_Vperm2f128,
  X86_Opcode_Vperm2i128,
  X86_Opcode_Vpermd,
  X86_Opcode_Vpermilpd,
  X86_Opcode_Vpermilps,
  X86_Opcode_Vpermps,
  X86_Opcode_Vpermq,
  X86_Opcode_Vpextrb,
  X86_Opcode_Vpextrd,
  X86_Opcode_Vpextrw,
  X86_Opcode_Vpgatherdd,
  X86_Opcode_Vpgatherdpd,
  X86_Opcode_Vpgatherdps,
  X86_Opcode_Vpgatherdq,
  X86_Opcode_Vpgatherqd,
  X86_Opcode_Vpgatherqpd,
  X86_Opcode_Vpgatherqps,
  X86_Opcode_Vpgatherqq,
  X86_Opcode_Vphaddd,
  X86_Opcode_Vphaddsw,
  X86_Opcode_Vphaddw,
  X86_Opcode_Vphminposuw,
  X86_Opcode_Vphsubd,
  X86_Opcode_Vphsubsw,
  X86_Opcode_Vphsubw,
  X86_Opcode_Vpinsrb,
  X86_Opcode_Vpinsrd,
  X86_Opcode_Vpinsrw,
  X86_Opcode_Vpmaddubsw,
  X86_Opcode_Vpmaddwd,
  X86_Opcode_Vpmaskmovd,
  X86_Opcode_Vpmaskmovq,
  X86_Opcode_Vpmaxsb,
  X86_Opcode_Vpmaxsd,
  X86_Opcode_Vpmaxsw,
  X86_Opcode_Vpmaxub,
  X86_Opcode_Vpmaxud,
  X86_Opcode_Vpmaxuw,
  X86_Opcode_Vpminsb,
  X86_Opcode_Vpminsd,
  X86_Opcode_Vpminsw,
  X86_Opcode_Vpminub,
  X86_Opcode_Vpminud,
  X86_Opcode_Vpminuw,
  X86_Opcode_Vpmovmskb,
  X86_Opcode_Vpmovsxbd,
  X86_Opcode_Vpmovsxbq,
  X86_Opcode_Vpmovsxbw,
  X86_Opcode_Vpmovsxdq,
  X86_Opcode_Vpmovsxwd,
  X86_Opcode_Vpmovsxwq,
  X86_Opcode_Vpmovzxbd,
  X86_Opcode_Vpmovzxbq,
  X86_Opcode_Vpmovzxbw,
  X86_Opcode_Vpmovzxdq,
  X86_Opcode_Vpmovzxwd,
  X86_Opcode_Vpmovzxwq,
  X86_Opcode_Vpmuldq,
  X86_Opcode_Vpmulhrsw,
  X86_Opcode_Vpmulhuw,
  X86_Opcode_Vpmulhw,
  X86_Opcode_Vpmulld,
  X86_Opcode_Vpmullw,
  X86_Opcode_Vpmuludq,
  X86_Opcode_Vpor,
  X86_Opcode_Vpsadbw,
  X86_Opcode_Vpshufb,
  X86_Opcode_Vpshufd,
  X86_Opcode_Vpshufhw,
  X86_Opcode_Vpshuflw,
  X86_Opcode_Vpsignb,
  X86_Opcode_Vpsignd,
  X86_Opcode_Vpsignw,
  X86_Opcode_Vpsllvd,
  X86_Opcode_Vpsllvq,
  X86_Opcode_Vpsllw,
  X86_Opcode_Vpsrad,
  X86_Opcode_Vpsravd,
  X86_Opcode_Vpsraw,
  X86_Opcode_Vpsrld,
  X86_Opcode_Vpsrlvd,
  X86_Opcode_Vpsrlvq,
  X86_Opcode_Vpsrlw,
  X86_Opcode_Vpsubb,
  X86_Opcode_Vpsubd,
  X86_Opcode_Vpsubq,
  X86_Opcode_Vpsubsb,
  X86_Opcode_Vpsubsw,
  X86_Opcode_Vpsubusb,
  X86_Opcode_Vpsubusw,
  X86_Opcode_Vpsubw,
  X86_Opcode_Vptest,
  X86_Opcode_Vpunpckhbw,
  X86_Opcode_Vpunpckhdq,
  X86_Opcode_Vpunpckhqdq,
  X86_Opcode_Vpunpckhwd,
  X86_Opcode_Vpunpcklqdq,
  X86_Opcode_Vpunpcklwd,
  X86_Opcode_Vpupcklbw,
  X86_Opcode_Vpupckldq,
  X86_Opcode_Vpxor,
  X86_Opcode_Vrcpps,
  X86_Opcode_Vrcpss,
  X86_Opcode_Vroundpd,
  X86_Opcode_Vroundps,
  X86_Opcode_Vroundsd,
  X86_Opcode_Vroundss,
  X86_Opcode_Vrsqrtps,
  X86_Opcode_Vrsqrtss,
  X86_Opcode_Vshufpd,
  X86_Opcode_Vshufps,
  X86_Opcode_Vsplld,
  X86_Opcode_Vspllq,
  X86_Opcode_Vsqrtps,
  X86_Opcode_Vsqrtpsd,
  X86_Opcode_Vsqrtsd,
  X86_Opcode_Vsqrtss,
  X86_Opcode_Vstmxcsr,
  X86_Opcode_Vsubpd,
  X86_Opcode_Vsubps,
  X86_Opcode_Vsubsd,
  X86_Opcode_Vsubss,
  X86_Opcode_Vtestpd,
  X86_Opcode_Vtestps,
  X86_Opcode_Vucomisd,
  X86_Opcode_Vucomiss,
  X86_Opcode_Vunpckhpd,
  X86_Opcode_Vunpckhps,
  X86_Opcode_Vunpcklpd,
  X86_Opcode_Vunpcklps,
  X86_Opcode_Vxorpd,
  X86_Opcode_Vxorps,
  X86_Opcode_Vzeroupper,
  X86_Opcode_Wbindvd,
  X86_Opcode_Wrfsbase,
  X86_Opcode_Wrgsbase,
  X86_Opcode_Wrmsr,
  X86_Opcode_Wrshr,
  X86_Opcode_Xadd,
  X86_Opcode_Xbts,
  X86_Opcode_Xchg,
  X86_Opcode_Xcrypt,
  X86_Opcode_Xgetbv,
  X86_Opcode_Xlat,
  X86_Opcode_Xor,
  X86_Opcode_Xrstor,
  X86_Opcode_Xsave,
  X86_Opcode_Xsaveopt,
  X86_Opcode_Xsetbv,
  X86_Opcode_Xsha,
  X86_Opcode_Xstore
};
private:
  typedef bool (X86Architecture:: *TDisassembler)(BinaryStream const&, TOffset, Instruction&, u8);
  static const TDisassembler m_Table_1[0x100];
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
  bool Table_1_10(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_11(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_12(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_13(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_14(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_15(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_16(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_17(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_18(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_19(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_1a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_1b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_1c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_1d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_1e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_1f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_20(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_21(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_22(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_23(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_24(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_25(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_26(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_27(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_28(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_29(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_2a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_2b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_2c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_2d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_2e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_2f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_30(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_31(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_32(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_33(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_34(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_35(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_36(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_37(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_38(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_39(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_3a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_3b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_3c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_3d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_3e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_3f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_40(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_41(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_42(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_43(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_44(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_45(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_46(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_47(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_48(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_49(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_4a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_4b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_4c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_4d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_4e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_4f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_50(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_51(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_52(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_53(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_54(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_55(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_56(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_57(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_58(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_59(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_5a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_5b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_5c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_5d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_5e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_5f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_60(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_61(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_62(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_63(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_64(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_65(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_66(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_67(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_68(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_69(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_6a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_6b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_6c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_6d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_6e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_6f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_70(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_71(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_72(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_73(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_74(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_75(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_76(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_77(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_78(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_79(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_7a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_7b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_7c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_7d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_7e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_7f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_80(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_81(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_82(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_83(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_84(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_85(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_86(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_87(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_88(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_89(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_8a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_8b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_8c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_8d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_8e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_8f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_90(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_91(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_92(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_93(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_94(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_95(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_96(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_97(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_98(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_99(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_9a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_9b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_9c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_9d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_9e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_9f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_a0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_a1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_a2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_a3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_a4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_a5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_a6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_a7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_a8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_a9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_aa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_ab(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_ac(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_ad(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_ae(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_af(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_b0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_b1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_b2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_b3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_b4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_b5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_b6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_b7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_b8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_b9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_ba(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_bb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_bc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_bd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_be(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_bf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_c0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_c1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_c2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_c3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_c4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_c5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_c6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_c7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_c8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_c9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_ca(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_cb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_cc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_cd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_ce(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_cf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_d0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_d1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_d2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_d3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_d4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_d5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_d6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_d7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_d8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_d9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_da(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_db(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_dc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_dd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_de(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_df(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_e0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_e1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_e2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_e3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_e4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_e5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_e6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_e7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_e8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_e9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_ea(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_ec(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_ee(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_ef(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_f0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_f1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_f2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_f3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_f4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_f5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_f6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_f7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_f8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_f9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_fa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_fb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_fc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_fd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_fe(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_1_ff(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);

  static const TDisassembler m_Table_2[0x100];
  bool Table_2_00(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_01(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_02(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_03(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_04(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_05(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_06(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_07(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_08(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_09(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_0a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_0b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_0c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_0d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_0e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_0f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_10(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_11(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_12(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_13(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_14(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_15(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_16(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_17(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_18(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_19(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_1a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_1b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_1c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_1d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_1e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_1f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_20(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_21(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_22(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_23(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_24(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_25(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_26(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_27(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_28(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_29(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_2a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_2b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_2c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_2d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_2e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_2f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_30(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_31(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_32(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_33(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_34(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_35(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_36(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_37(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_38(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_39(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_3a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_3b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_3c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_3d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_3e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_3f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_40(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_41(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_42(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_43(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_44(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_45(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_46(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_47(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_48(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_49(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_4a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_4b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_4c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_4d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_4e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_4f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_50(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_51(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_52(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_53(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_54(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_55(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_56(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_57(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_58(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_59(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_5a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_5b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_5c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_5d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_5e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_5f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_60(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_61(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_62(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_63(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_64(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_65(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_66(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_67(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_68(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_69(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_6a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_6b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_6c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_6d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_6e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_6f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_70(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_71(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_72(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_73(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_74(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_75(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_76(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_77(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_78(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_79(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_7a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_7b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_7c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_7d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_7e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_7f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_80(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_81(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_82(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_83(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_84(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_85(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_86(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_87(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_88(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_89(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_8a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_8b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_8c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_8d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_8e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_8f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_90(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_91(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_92(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_93(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_94(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_95(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_96(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_97(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_98(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_99(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_9a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_9b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_9c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_9d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_9e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_9f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_a0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_a1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_a2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_a3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_a4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_a5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_a6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_a7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_a8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_a9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_aa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_ab(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_ac(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_ad(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_ae(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_af(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_b0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_b1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_b2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_b3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_b4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_b5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_b6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_b7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_b8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_b9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_ba(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_bb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_bc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_bd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_be(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_bf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_c0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_c1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_c2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_c3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_c4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_c5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_c6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_c7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_c8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_c9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_ca(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_cb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_cc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_cd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_ce(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_cf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_d0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_d1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_d2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_d3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_d4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_d5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_d6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_d7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_d8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_d9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_da(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_db(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_dc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_dd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_de(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_df(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_e0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_e1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_e2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_e3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_e4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_e5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_e6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_e7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_e8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_e9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_ea(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_ec(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_ee(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_ef(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_f0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_f1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_f2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_f3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_f4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_f5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_f6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_f7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_f8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_f9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_fa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_fb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_fc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_fd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_fe(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_2_ff(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);

  static const TDisassembler m_Table_3dnow1[0x100];
  bool Table_3dnow1_00(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_01(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_02(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_03(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_04(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_05(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_06(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_07(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_08(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_09(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_0a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_0b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_0c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_0d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_0e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_0f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_10(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_11(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_12(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_13(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_14(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_15(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_16(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_17(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_18(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_19(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_1a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_1b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_1c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_1d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_1e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_1f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_20(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_21(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_22(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_23(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_24(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_25(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_26(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_27(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_28(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_29(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_2a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_2b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_2c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_2d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_2e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_2f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_30(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_31(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_32(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_33(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_34(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_35(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_36(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_37(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_38(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_39(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_3a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_3b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_3c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_3d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_3e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_3f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_40(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_41(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_42(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_43(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_44(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_45(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_46(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_47(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_48(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_49(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_4a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_4b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_4c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_4d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_4e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_4f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_50(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_51(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_52(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_53(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_54(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_55(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_56(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_57(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_58(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_59(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_5a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_5b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_5c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_5d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_5e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_5f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_60(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_61(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_62(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_63(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_64(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_65(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_66(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_67(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_68(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_69(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_6a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_6b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_6c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_6d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_6e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_6f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_70(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_71(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_72(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_73(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_74(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_75(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_76(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_77(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_78(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_79(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_7a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_7b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_7c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_7d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_7e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_7f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_80(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_81(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_82(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_83(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_84(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_85(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_86(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_87(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_88(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_89(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_8a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_8b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_8c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_8d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_8e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_8f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_90(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_91(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_92(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_93(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_94(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_95(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_96(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_97(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_98(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_99(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_9a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_9b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_9c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_9d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_9e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_9f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_a0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_a1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_a2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_a3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_a4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_a5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_a6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_a7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_a8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_a9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_aa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_ab(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_ac(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_ad(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_ae(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_af(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_b0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_b1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_b2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_b3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_b4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_b5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_b6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_b7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_b8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_b9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_ba(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_bb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_bc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_bd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_be(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_bf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_c0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_c1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_c2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_c3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_c4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_c5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_c6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_c7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_c8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_c9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_ca(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_cb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_cc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_cd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_ce(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_cf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_d0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_d1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_d2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_d3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_d4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_d5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_d6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_d7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_d8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_d9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_da(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_db(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_dc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_dd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_de(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_df(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_e0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_e1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_e2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_e3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_e4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_e5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_e6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_e7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_e8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_e9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_ea(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_ec(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_ee(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_ef(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_f0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_f1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_f2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_f3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_f4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_f5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_f6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_f7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_f8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_f9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_fa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_fb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_fc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_fd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_fe(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3dnow1_ff(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);

  static const TDisassembler m_Table_3_38[0x100];
  bool Table_3_38_00(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_01(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_02(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_03(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_04(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_05(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_06(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_07(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_08(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_09(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_0a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_0b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_0c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_0d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_0e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_0f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_10(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_11(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_12(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_13(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_14(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_15(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_16(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_17(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_18(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_19(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_1a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_1b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_1c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_1d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_1e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_1f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_20(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_21(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_22(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_23(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_24(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_25(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_26(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_27(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_28(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_29(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_2a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_2b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_2c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_2d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_2e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_2f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_30(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_31(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_32(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_33(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_34(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_35(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_36(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_37(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_38(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_39(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_3a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_3b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_3c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_3d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_3e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_3f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_40(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_41(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_42(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_43(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_44(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_45(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_46(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_47(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_48(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_49(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_4a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_4b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_4c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_4d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_4e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_4f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_50(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_51(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_52(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_53(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_54(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_55(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_56(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_57(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_58(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_59(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_5a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_5b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_5c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_5d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_5e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_5f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_60(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_61(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_62(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_63(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_64(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_65(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_66(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_67(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_68(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_69(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_6a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_6b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_6c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_6d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_6e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_6f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_70(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_71(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_72(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_73(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_74(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_75(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_76(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_77(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_78(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_79(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_7a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_7b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_7c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_7d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_7e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_7f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_80(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_81(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_82(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_83(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_84(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_85(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_86(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_87(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_88(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_89(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_8a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_8b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_8c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_8d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_8e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_8f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_90(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_91(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_92(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_93(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_94(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_95(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_96(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_97(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_98(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_99(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_9a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_9b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_9c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_9d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_9e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_9f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_a0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_a1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_a2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_a3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_a4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_a5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_a6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_a7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_a8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_a9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_aa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_ab(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_ac(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_ad(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_ae(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_af(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_b0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_b1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_b2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_b3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_b4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_b5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_b6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_b7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_b8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_b9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_ba(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_bb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_bc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_bd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_be(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_bf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_c0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_c1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_c2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_c3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_c4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_c5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_c6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_c7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_c8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_c9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_ca(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_cb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_cc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_cd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_ce(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_cf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_d0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_d1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_d2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_d3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_d4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_d5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_d6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_d7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_d8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_d9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_da(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_db(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_dc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_dd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_de(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_df(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_e0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_e1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_e2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_e3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_e4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_e5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_e6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_e7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_e8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_e9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_ea(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_ec(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_ee(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_ef(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_f0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_f1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_f2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_f3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_f4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_f5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_f6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_f7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_f8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_f9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_fa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_fb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_fc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_fd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_fe(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_38_ff(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);

  static const TDisassembler m_Table_3_3a[0x100];
  bool Table_3_3a_00(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_01(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_02(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_03(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_04(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_05(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_06(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_07(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_08(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_09(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_0a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_0b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_0c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_0d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_0e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_0f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_10(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_11(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_12(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_13(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_14(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_15(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_16(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_17(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_18(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_19(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_1a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_1b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_1c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_1d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_1e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_1f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_20(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_21(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_22(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_23(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_24(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_25(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_26(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_27(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_28(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_29(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_2a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_2b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_2c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_2d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_2e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_2f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_30(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_31(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_32(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_33(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_34(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_35(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_36(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_37(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_38(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_39(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_3a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_3b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_3c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_3d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_3e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_3f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_40(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_41(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_42(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_43(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_44(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_45(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_46(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_47(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_48(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_49(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_4a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_4b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_4c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_4d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_4e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_4f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_50(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_51(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_52(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_53(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_54(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_55(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_56(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_57(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_58(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_59(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_5a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_5b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_5c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_5d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_5e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_5f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_60(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_61(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_62(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_63(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_64(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_65(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_66(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_67(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_68(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_69(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_6a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_6b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_6c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_6d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_6e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_6f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_70(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_71(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_72(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_73(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_74(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_75(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_76(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_77(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_78(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_79(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_7a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_7b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_7c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_7d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_7e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_7f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_80(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_81(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_82(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_83(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_84(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_85(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_86(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_87(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_88(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_89(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_8a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_8b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_8c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_8d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_8e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_8f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_90(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_91(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_92(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_93(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_94(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_95(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_96(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_97(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_98(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_99(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_9a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_9b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_9c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_9d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_9e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_9f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_a0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_a1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_a2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_a3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_a4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_a5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_a6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_a7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_a8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_a9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_aa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_ab(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_ac(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_ad(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_ae(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_af(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_b0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_b1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_b2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_b3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_b4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_b5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_b6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_b7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_b8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_b9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_ba(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_bb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_bc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_bd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_be(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_bf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_c0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_c1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_c2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_c3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_c4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_c5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_c6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_c7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_c8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_c9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_ca(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_cb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_cc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_cd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_ce(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_cf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_d0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_d1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_d2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_d3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_d4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_d5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_d6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_d7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_d8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_d9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_da(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_db(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_dc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_dd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_de(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_df(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_e0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_e1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_e2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_e3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_e4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_e5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_e6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_e7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_e8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_e9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_ea(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_ec(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_ee(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_ef(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_f0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_f1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_f2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_f3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_f4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_f5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_f6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_f7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_f8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_f9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_fa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_fb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_fc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_fd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_fe(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_3_3a_ff(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);

  static const TDisassembler m_Table_fp1[0x40];
  bool Table_fp1_c0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_c1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_c2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_c3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_c4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_c5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_c6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_c7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_c8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_c9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_ca(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_cb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_cc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_cd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_ce(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_cf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_d0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_d1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_d2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_d3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_d4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_d5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_d6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_d7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_d8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_d9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_da(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_db(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_dc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_dd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_de(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_df(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_e0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_e1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_e2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_e3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_e4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_e5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_e6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_e7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_e8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_e9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_ea(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_ec(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_ee(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_ef(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_f0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_f1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_f2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_f3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_f4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_f5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_f6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_f7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_f8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_f9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_fa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_fb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_fc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_fd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_fe(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp1_ff(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);

  static const TDisassembler m_Table_fp2[0x40];
  bool Table_fp2_c0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_c1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_c2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_c3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_c4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_c5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_c6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_c7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_c8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_c9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_ca(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_cb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_cc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_cd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_ce(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_cf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_d0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_d1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_d2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_d3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_d4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_d5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_d6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_d7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_d8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_d9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_da(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_db(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_dc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_dd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_de(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_df(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_e0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_e1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_e2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_e3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_e4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_e5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_e6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_e7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_e8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_e9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_ea(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_ec(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_ee(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_ef(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_f0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_f1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_f2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_f3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_f4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_f5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_f6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_f7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_f8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_f9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_fa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_fb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_fc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_fd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_fe(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp2_ff(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);

  static const TDisassembler m_Table_fp3[0x40];
  bool Table_fp3_c0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_c1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_c2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_c3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_c4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_c5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_c6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_c7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_c8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_c9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_ca(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_cb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_cc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_cd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_ce(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_cf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_d0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_d1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_d2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_d3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_d4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_d5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_d6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_d7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_d8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_d9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_da(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_db(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_dc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_dd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_de(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_df(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_e0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_e1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_e2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_e3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_e4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_e5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_e6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_e7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_e8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_e9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_ea(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_ec(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_ee(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_ef(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_f0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_f1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_f2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_f3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_f4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_f5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_f6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_f7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_f8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_f9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_fa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_fb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_fc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_fd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_fe(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp3_ff(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);

  static const TDisassembler m_Table_fp4[0x40];
  bool Table_fp4_c0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_c1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_c2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_c3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_c4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_c5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_c6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_c7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_c8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_c9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_ca(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_cb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_cc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_cd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_ce(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_cf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_d0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_d1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_d2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_d3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_d4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_d5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_d6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_d7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_d8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_d9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_da(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_db(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_dc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_dd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_de(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_df(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_e0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_e1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_e2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_e3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_e4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_e5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_e6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_e7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_e8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_e9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_ea(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_ec(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_ee(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_ef(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_f0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_f1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_f2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_f3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_f4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_f5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_f6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_f7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_f8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_f9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_fa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_fb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_fc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_fd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_fe(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp4_ff(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);

  static const TDisassembler m_Table_fp5[0x40];
  bool Table_fp5_c0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_c1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_c2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_c3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_c4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_c5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_c6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_c7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_c8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_c9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_ca(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_cb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_cc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_cd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_ce(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_cf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_d0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_d1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_d2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_d3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_d4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_d5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_d6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_d7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_d8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_d9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_da(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_db(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_dc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_dd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_de(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_df(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_e0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_e1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_e2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_e3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_e4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_e5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_e6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_e7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_e8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_e9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_ea(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_ec(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_ee(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_ef(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_f0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_f1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_f2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_f3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_f4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_f5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_f6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_f7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_f8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_f9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_fa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_fb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_fc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_fd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_fe(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp5_ff(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);

  static const TDisassembler m_Table_fp6[0x40];
  bool Table_fp6_c0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_c1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_c2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_c3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_c4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_c5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_c6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_c7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_c8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_c9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_ca(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_cb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_cc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_cd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_ce(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_cf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_d0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_d1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_d2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_d3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_d4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_d5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_d6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_d7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_d8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_d9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_da(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_db(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_dc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_dd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_de(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_df(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_e0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_e1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_e2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_e3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_e4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_e5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_e6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_e7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_e8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_e9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_ea(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_ec(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_ee(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_ef(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_f0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_f1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_f2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_f3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_f4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_f5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_f6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_f7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_f8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_f9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_fa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_fb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_fc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_fd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_fe(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp6_ff(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);

  static const TDisassembler m_Table_fp7[0x40];
  bool Table_fp7_c0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_c1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_c2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_c3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_c4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_c5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_c6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_c7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_c8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_c9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_ca(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_cb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_cc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_cd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_ce(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_cf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_d0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_d1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_d2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_d3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_d4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_d5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_d6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_d7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_d8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_d9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_da(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_db(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_dc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_dd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_de(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_df(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_e0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_e1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_e2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_e3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_e4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_e5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_e6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_e7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_e8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_e9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_ea(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_ec(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_ee(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_ef(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_f0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_f1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_f2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_f3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_f4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_f5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_f6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_f7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_f8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_f9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_fa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_fb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_fc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_fd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_fe(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp7_ff(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);

  static const TDisassembler m_Table_fp8[0x40];
  bool Table_fp8_c0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_c1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_c2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_c3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_c4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_c5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_c6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_c7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_c8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_c9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_ca(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_cb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_cc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_cd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_ce(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_cf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_d0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_d1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_d2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_d3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_d4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_d5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_d6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_d7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_d8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_d9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_da(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_db(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_dc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_dd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_de(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_df(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_e0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_e1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_e2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_e3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_e4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_e5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_e6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_e7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_e8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_e9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_ea(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_ec(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_ee(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_ef(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_f0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_f1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_f2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_f3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_f4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_f5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_f6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_f7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_f8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_f9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_fa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_fb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_fc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_fd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_fe(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_fp8_ff(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);

  static const TDisassembler m_Table_sse5a[0x100];
  bool Table_sse5a_00(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_01(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_02(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_03(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_04(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_05(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_06(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_07(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_08(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_09(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_0a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_0b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_0c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_0d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_0e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_0f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_10(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_11(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_12(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_13(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_14(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_15(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_16(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_17(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_18(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_19(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_1a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_1b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_1c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_1d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_1e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_1f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_20(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_21(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_22(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_23(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_24(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_25(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_26(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_27(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_28(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_29(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_2a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_2b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_2c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_2d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_2e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_2f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_30(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_31(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_32(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_33(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_34(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_35(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_36(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_37(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_38(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_39(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_3a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_3b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_3c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_3d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_3e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_3f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_40(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_41(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_42(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_43(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_44(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_45(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_46(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_47(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_48(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_49(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_4a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_4b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_4c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_4d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_4e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_4f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_50(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_51(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_52(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_53(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_54(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_55(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_56(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_57(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_58(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_59(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_5a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_5b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_5c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_5d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_5e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_5f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_60(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_61(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_62(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_63(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_64(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_65(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_66(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_67(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_68(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_69(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_6a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_6b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_6c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_6d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_6e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_6f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_70(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_71(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_72(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_73(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_74(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_75(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_76(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_77(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_78(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_79(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_7a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_7b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_7c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_7d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_7e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_7f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_80(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_81(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_82(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_83(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_84(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_85(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_86(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_87(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_88(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_89(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_8a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_8b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_8c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_8d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_8e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_8f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_90(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_91(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_92(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_93(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_94(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_95(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_96(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_97(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_98(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_99(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_9a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_9b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_9c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_9d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_9e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_9f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_a0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_a1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_a2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_a3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_a4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_a5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_a6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_a7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_a8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_a9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_aa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_ab(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_ac(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_ad(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_ae(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_af(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_b0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_b1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_b2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_b3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_b4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_b5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_b6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_b7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_b8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_b9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_ba(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_bb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_bc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_bd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_be(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_bf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_c0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_c1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_c2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_c3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_c4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_c5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_c6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_c7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_c8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_c9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_ca(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_cb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_cc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_cd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_ce(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_cf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_d0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_d1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_d2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_d3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_d4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_d5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_d6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_d7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_d8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_d9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_da(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_db(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_dc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_dd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_de(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_df(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_e0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_e1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_e2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_e3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_e4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_e5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_e6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_e7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_e8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_e9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_ea(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_ec(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_ee(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_ef(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_f0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_f1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_f2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_f3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_f4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_f5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_f6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_f7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_f8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_f9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_fa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_fb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_fc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_fd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_fe(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Table_sse5a_ff(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);

  bool Operand__rBX_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Ho_Wo_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Ho_Wo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__d64_rSP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rSP_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Cy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_ES(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Ho_Wd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Yv_Xv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Woqo_Vx_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Pq_Qq_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_r15b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Ho_Wq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ty_Ry(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__d64_rSI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ib_eAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__DL_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Gv_Ev(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Ey(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__m32fp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Ew(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Ev(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Moq_Vo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_d64_rSI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Mod(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__CH_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__DX_AL(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_d64_rSP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Moq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Sw_Ew(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Jb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Uo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ey_Vo_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r15(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__GS(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ov_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r10(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r11(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vy_Wy_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r15_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Pq_Uq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Mp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_eBX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ry(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Nq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Ho_Uod_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ry_Cy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Ux(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ev_Gv_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Uq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r8b_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ev_Gv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__eAX_DX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r13_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vx_Mo_b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rDX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rCX_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Ho_Ey(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Pq_Mw_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ST5_ST0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Yv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Mqo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Mq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rDI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__d64_rCX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Gy_Ey(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Lx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_d64_r14(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Mo_Voq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_CS(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_m32fp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Md_Vo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__eDI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Gy_Ev(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Mo_Vod(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Uod_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Ho_Edb_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_d64_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_CL(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__eDX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Gy_Eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Wo_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_CH(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__My_Gy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Gb_Eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vx_Moq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Woq_Vo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__DX_Xz(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Eyw_Vo_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Gb_Ev(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__DX_Xb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Ey_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__BL_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vx_Mod(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__AH_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r11_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r15b_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Md(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r14_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Uo_Ib_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Mo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_ST2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_ST3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_ST0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_ST1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_ST6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_ST7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_ST4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_ST5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Woq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vy_Hy_Wy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Jz(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Wow(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Eb_CL(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Gy_Ux(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Gv_M(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Mw(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rBP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vx_Wx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Wod(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__eSI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_SS(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Mq_Vo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Gv_Mp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Mq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_eCX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Sw(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Gv_Mv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__By_Ey(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_rBX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__d64_r11(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__FS(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__d64_r10(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Md_x_Ho(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_eAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__d64_r12(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__eBP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Ho_Woq_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ST0_ST0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__d64_r15(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Ap(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__eBX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__d64_r8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rBX_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vy_Woo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_AH(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__m80bcd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_AL(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Gy_Uo_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rDX_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ST2_ST0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_AX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ES(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Yb_Xb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Eb_1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Mod(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Wod(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Jz(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Wob(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Woo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ey_Vo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Qq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ob_AL(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__AL_Xb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Nq_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Wow(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r14b_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ev_CL(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Woq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Moq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__CS(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Jb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Qd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_rDI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r15_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r10b_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__By_Ey_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__SS(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ev_1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_d64_r15(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rBX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_d64_r13(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_d64_r12(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_d64_r11(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_d64_r10(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ST0_ST7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_rDX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Uod(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_r10b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vx_Uo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Gy_By_Ey(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r13_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Pq_Ey(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Yz_DX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Edb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Wo_Vy_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Ob(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Vo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_m80dec(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__m64fp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r14(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Woq_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Vx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Vy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ST0_ST6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Ov(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r12(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Iw_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_eBP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r13(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vx_Md_x_Hx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Mq_Pq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rSI_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_d64_rBP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_r12b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Gy_Nq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_d64_rBX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ST4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ST5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ST6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ST7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ST0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ST1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ST2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ST3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Gv_Ev_Iz(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Gy_Ey_By(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Wx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rAX_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_r9b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Hx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Hy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Yb_AL(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rAX_Iz(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__d64_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vy_Moq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vx_Mq_x_Hx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ib_AL(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r11_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Ho_Moq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Iz(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__BH_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Xb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Eyb_Vo_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Iw(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vx_Mo_w(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Xv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vx_Hx_Mx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r12_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__m64int(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r9_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_rBP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Xz(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ev_Iz(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Rv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Md(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Ma(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Gv_Ew(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Mo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Ibsq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_m80fp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Mx_Vx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Mw(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Mv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Gv_Eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Gv_Ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Gw(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Mq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Mp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ev_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Ty(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r10_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rSP_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_My(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Mx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Eyb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Nq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rBP_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vy_Wod(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Eyw(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ev_Gv_CL(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__d64_rBP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__d64_rBX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Uo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_r14b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__CL_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Mx_Hx_Vx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Ho_Mw_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Ey(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Wow_Vo_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rSI_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Edb_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Gb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Eb_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Yb_DX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__AL_DX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_eSI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_m32int(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_eSP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__m80fp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vy_Uo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__d64_r13(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vx_Hx_Wx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Pq_Woq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__AL_Ob(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Hx_Hx_Wx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_d64_rCX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Rv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rSP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Ry(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Wx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Gy_Nq_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ST6_ST0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Qq_Pq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__d64_r9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__d64_rDI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ew_Sw(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__eAX_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Wob_Vo_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Wo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vx_Hx_Wx_Lx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Eb_Gb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_r8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_r9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ey_Pq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__d64_r14(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_DS(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_DX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r10_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_d64_rDI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Pq_Wo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_r13b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_DH(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_DL(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_d64_r9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_d64_r8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vx_Hx_Wx_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__eAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Dy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_m64fp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r14_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vy_Hy_Wo_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ev(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rDI_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__eCX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Gy_Woq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Iw(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_FS(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r11b_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_r11b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Iz(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Dy_Ry(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Pq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Yv_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ux_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ew_Gw(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__AX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rSI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Yb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Ho(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vx_Mx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ST0_ST2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ST0_ST3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rAX_Xv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__DX_eAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Gv_Ev_Ibs(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rAX_Ov(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_rCX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__DH_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Yz(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Pq_Nq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ST0_ST1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ST7_ST0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Ho_Ey_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ry_Dy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__m16int(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Ho_Wod(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r12_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_m64int(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vx_Mqo_Hx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rCX_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Hx_Ux_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_BL(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_m16int(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vo_Ho_Woq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Mv_Gv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_BH(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ST1_ST0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Izsq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__eSP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vx_Wx_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ey_Gy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_By(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Woqo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_m80bcd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Ibs(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Gy_Mo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Gy_Wod(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_r10(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_r11(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_r12(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_r13(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_r14(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_r15(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_GS(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vy_Hy_Wy_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_r8b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r9_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r9b_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__DS(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vy_Woq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Gy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__m80dec(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__m32int(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__d64_rDX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r8_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ST4_ST0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Pq_Qd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Gv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r13b_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ST3_ST0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ry_Ty(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Pq_Qq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Vy_Mo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_eDX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Wo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_x(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_w(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Vod(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__M(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Wd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_rSP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Wy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Gv_Ma(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rDX_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_eDI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_rSI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Wq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rCX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ST0_ST4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__ST0_ST5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_d64_rDX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_Voq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r12b_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ev_Ibs(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__r8_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rDI_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  Expression::SPType Decode_M(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Wx_Vx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__rBP_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__AL_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
  bool Operand__Ap(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);
