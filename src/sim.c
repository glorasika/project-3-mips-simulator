#include <stdio.h>
#include "shell.h"

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*          I-Type (Immediate)          */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
#define ADDI 0b001000
#define ADDIU 0b001001
#define ANDI 0b001100

#define LB 0b100000
#define LBU 0b100100
#define LH 0b100001
#define LHU 0b100101
#define LUI 0b001111
#define LW 0b100011
#define ORI 0b001101
#define SB 0b101000
#define SH 0b101001
#define SLTI 0b001010
#define SLTIU 0b001011
#define SW 0b101011
#define XORI 0b001110


/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*                 BRANCH               */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
#define BEQ 0b000100
#define BGTZ 0b000111
#define BLEZ 0b000110
#define BNE 0b000101


#define REGIMM 0b000001
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*                 REGIMM               */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
#define BGEZ 0b00001
#define BGEZAL 0b10001
#define BLTZ 0b00000
#define BLTZAL 0b10000


/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*             J-Type (Jump)            */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
#define J 0b000010
#define JAI 0b000011


#define SPECIAL 0b000000
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*                SPECIAL               */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
#define ADD 0b100000
#define ADDU 0b100001
#define AND 0b100100
#define DIV 0b011010
#define DIVU 0b011011
#define JALR 0b001001
#define JR 0b001000
#define MFHI 0b010000
#define MFLO 0b010010
#define MTHI 0b010001
#define MTLO 0b010011
#define MULT 0b011000
#define MULTU 0b011001
#define NOR 0b100111
#define OR 0b100101

#define SLL 0b000000
#define SLLV 0b000100
#define SLT 0b101010
#define SLTU 0b101011
#define SRA 0b000011
#define SRAV 0b000111
#define SRL 0b000010
#define SRLV 0b000110
#define SUB 0b100010
#define SUBU 0b100011
#define XOR 0b100110


/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*                SYSCALL               */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
#define SYSCALL 0b001100


void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */

    uint32_t instruction = mem_read_32(CURRENT_STATE.PC);

    uint8_t opcode = (uint8_t)(instruction >> 26);
    uint8_t special = (uint8_t)(instruction & 0x3F);

    printf("First 6-bits: %u\n", special);
    printf("Last 6-bits: %u\n", opcode);

    // check for syscall
    if (opcode == 0 && special == 12) {
        RUN_BIT = 0;
    }

    // update program counter for next instruction
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}
