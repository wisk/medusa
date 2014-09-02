#ifndef ARCH_GAMEBOY_INSTRUCTION_HPP
#define ARCH_GAMEBOY_INSTRUCTION_HPP

enum EGameBoyInstructionType
{
  GB_Invalid_Insn,// Invalid
  GB_Nop,         // No Operation

// Arithmetic
  GB_Inc,         // Increment
  GB_Dec,         // Decrement
  GB_Add,         // Add
  GB_Sub,         // Substract
  GB_Adc,         // Add with Carry
  GB_Sbc,         // Substract with Carry
  GB_Daa,         // Adjust A after BCD operation

// Logical
  GB_And,         // And
  GB_Or,          // inclusive Or
  GB_Xor,         // eXclusive Or
  GB_Bit,         // test Bit
  GB_Set,         // Set bit
  GB_Res,         // Reset bit
  GB_Rl,          // Rotate Left
  GB_Rr,          // Roate Right
  GB_Rlc,         // Rotate Left with Carry
  GB_Rrc,         // Rotate Right with Carry
  GB_Sla,         // Shift Left Arithmetic
  GB_Sra,         // Shift Right Arithmetic
  GB_Srl,         // Shift Right Logical
//GB_Sll,         // Shift Left Logical
  GB_Swap,        // Swap (nybbles)
  GB_Cpl,         // Complement (logical Not)
  GB_Ccf,         // Clear Carry Flag
  GB_Scf,         // Set Carry Flag
  GB_Cp,          // ComPare

// Memory
  GB_Ld,          // Load
  GB_Ldi,         // Load and Increment
  GB_Ldd,         // Load and Decrement
  GB_Ldh,         // Load A from/to address pointed to by (FF00h + 8-bit immediate or register C)
  GB_Ldhl,        // Add signed 8-bit immediate to SP and save result in HL
  GB_Push,        // Push
  GB_Pop,         // Pop

// Execution
  GB_Jr,          // Jump Relative
  GB_Jp,          // Jump Absolute
  GB_Call,        // Call
  GB_Rst,         // Call routine
  GB_Ret,         // Return
  GB_Reti,        // Enable interrupts and return to calling routine

// CPU
  GB_Halt,        // Halt
  GB_Stop,        // Stop
  GB_Ei,          // Enable Interrupt
  GB_Di,          // Disable Interrupt
};

enum EGameBoyConditionType
{
  GB_Cond_None,
  GB_Cond_NotZero,
  GB_Cond_Zero,
  GB_Cond_NotCarry,
  GB_Cond_Carry
};

#endif // ARCH_GAMEBOY_INSTRUCTION_HPP
