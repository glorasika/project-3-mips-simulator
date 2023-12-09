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


#define cast(T, V) ((T) V)
#define MASK(n) cast(uint32_t, (~(0xFFFFFFFF << n)))

#define rs(bits) (get_bits_between(bits, 21, 5))
#define rt(bits) (get_bits_between(bits, 16, 5))
#define rd(bits) (get_bits_between(bits, 11, 5))
#define imm(bits) (get_bits_between(bits, 0, 16))
#define sa(bits) (get_bits_between(bits, 6, 5))


uint32_t get_bits_between(uint32_t bits, int start, int size)
{
    return (bits >> start) & MASK(size);
}


void special_process(uint32_t bits) {
    uint8_t speicalOpcode = (uint8_t) (bits & 0x3F);
    uint8_t rs;
    uint8_t rt;
    uint8_t rd;
    uint8_t sa;

    switch (speicalOpcode) {
        case ADD:
            NEXT_STATE.REGS[rd(bits)] = CURRENT_STATE.REGS[rs(bits)] & CURRENT_STATE.REGS[rt(bits)];
            break;

        case ADDU:
            break;

        case AND:
            break;

        case DIV:
            break;

        case DIVU:
            break;

        case JALR:
            break;

        case JR:
            break;
        
        case MFHI:
            break;

        case MFLO:
            break;

        case MTHI:
            break;

        case MTLO:
            break;

        case MULT:
            break;

        case MULTU:
            break;

        case NOR:
            break;
        
        case OR:
            NEXT_STATE.REGS[rd(bits)] = CURRENT_STATE.REGS[rs(bits)] | CURRENT_STATE.REGS[rt(bits)];
            break;

        case SLL:
            NEXT_STATE.REGS[rd(bits)] = CURRENT_STATE.REGS[rt(bits) << sa(bits)];
            break;

        case SLLV:
            break;

        case SLT:
            break;

        case SLTU:
            break;

        case SRA:
            break;

        case SRAV:
            break;

        case SRL:
            break;

        case SRLV:
            break;

        case SUB:
            break;

        case SUBU:
            NEXT_STATE.REGS[rd(bits)] = CURRENT_STATE.REGS[rs(bits)] - CURRENT_STATE.REGS[rt(bits)];
            break;
        
        case XOR:
            NEXT_STATE.REGS[rd(bits)] = CURRENT_STATE.REGS[rs(bits)] ^ CURRENT_STATE.REGS[rt(bits)];
            break;

        case SYSCALL:
            RUN_BIT = 0;
            break;

        default:
            break;
    }
}


void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */

    uint32_t bits = mem_read_32(CURRENT_STATE.PC);

    uint8_t opcode = (uint8_t) (bits >> 26);

    printf("Last 6-bits: %u\n", opcode);

    switch (opcode) {
        RUN_BIT = 0;

        case ADDI:
            break;

        case ADDIU:
            break;

        case ANDI:
            break;

        case LB:
            break;

        case LBU:
            break;

        case LHU:
            break;

        case LUI:
            break;

        case LW:
            break;

        case ORI:
            break;
        
        case SB:
            break;

        case SH:
            break;

        case SLTI:
            break;

        case SLTIU:
            break;

        case SW:
            break;

        case XORI:
            break;

        case BEQ:
            break;

        case BGTZ:
            break;

        case BLEZ:
            break;

        case BNE:
            break;

        case J:
            break;

        case JAI:
            break;

        case REGIMM:
            break;

        case SPECIAL:
            special_process(bits);
            break;

        default:
            break;
    }

    // update program counter for next instruction
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}
