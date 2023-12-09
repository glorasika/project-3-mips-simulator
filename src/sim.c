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
#define base(bits) (get_bits_between(bits, 21, 5))
#define target(bits) (get_bits_between(bits, 0, 26))


uint32_t get_bits_between(uint32_t bits, int start, int size)
{
    return (bits >> start) & MASK(size);
}

uint32_t convert_to_32(int16_t immediate, int bitLength) {
    if ((immediate >> (bitLength - 1)) == 1) {
        return (uint32_t) (immediate | (0xFFFFFFFF << bitLength));
    }
    else {
        return (uint32_t) immediate;
    }
}


/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*            SPECIAL Process           */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

void ADD_process(uint32_t bits) {
    NEXT_STATE.REGS[rd(bits)] = (int32_t) CURRENT_STATE.REGS[rs(bits)] + (int32_t) CURRENT_STATE.REGS[rt(bits)];
}

void ADDU_process(uint32_t bits) {
    NEXT_STATE.REGS[rd(bits)] = CURRENT_STATE.REGS[rs(bits)] + CURRENT_STATE.REGS[rt(bits)];
}

void AND_process(uint32_t bits) {
    NEXT_STATE.REGS[rd(bits)] = CURRENT_STATE.REGS[rs(bits)] & CURRENT_STATE.REGS[rt(bits)];
}

void DIV_process(uint32_t bits) {
    if (CURRENT_STATE.REGS[rt(bits)] != 0) {
        NEXT_STATE.LO = ((int32_t) CURRENT_STATE.REGS[rs(bits)]) / ((int32_t) CURRENT_STATE.REGS[rt(bits)]);
        NEXT_STATE.HI = ((int32_t) CURRENT_STATE.REGS[rs(bits)]) % ((int32_t) CURRENT_STATE.REGS[rt(bits)]);
    }
}

void DIVU_process(uint32_t bits) {
    if (CURRENT_STATE.REGS[rt(bits)] != 0) {
        NEXT_STATE.LO = CURRENT_STATE.REGS[rs(bits)] / CURRENT_STATE.REGS[rt(bits)];
        NEXT_STATE.HI = CURRENT_STATE.REGS[rs(bits)] % CURRENT_STATE.REGS[rt(bits)];
    }
}

void JALR_process(uint32_t bits) {
    CURRENT_STATE.PC = CURRENT_STATE.REGS[rs(bits)] - 4;
    NEXT_STATE.REGS[rd(bits)] = CURRENT_STATE.PC + 4;
}

void JR_process(uint32_t bits) {
    CURRENT_STATE.PC = CURRENT_STATE.REGS[rs(bits)] - 4;
}

void MFHI_process(uint32_t bits) {
    NEXT_STATE.REGS[rd(bits)] = CURRENT_STATE.HI;
}

void MFLO_process(uint32_t bits) {
    NEXT_STATE.REGS[rd(bits)] = CURRENT_STATE.LO;
}

void MTHI_process(uint32_t bits) {
    NEXT_STATE.HI = CURRENT_STATE.REGS[rs(bits)];
}

void MTLO_process(uint32_t bits) {
    NEXT_STATE.LO = CURRENT_STATE.REGS[rs(bits)];
}

void MULT_process(uint32_t bits) {
    int64_t mult = ((int32_t) CURRENT_STATE.REGS[rs(bits)]) * ((int32_t) CURRENT_STATE.REGS[rt(bits)]);
    NEXT_STATE.LO = mult & 0xffffffff;
    NEXT_STATE.HI = (mult >> 32) & 0xffffffff;
}

void MULTU_process(uint32_t bits) {
    uint64_t mult = ((uint64_t) CURRENT_STATE.REGS[rs(bits)]) * ((uint64_t) CURRENT_STATE.REGS[rt(bits)]);
    NEXT_STATE.LO = mult & 0xffffffff;
    NEXT_STATE.HI = (mult >> 32) & 0xffffffff;
}

void NOR_process(uint32_t bits) {
    NEXT_STATE.REGS[rd(bits)] = ~(CURRENT_STATE.REGS[rs(bits)] | CURRENT_STATE.REGS[rt(bits)]);
}

void OR_process(uint32_t bits) {
    NEXT_STATE.REGS[rd(bits)] = CURRENT_STATE.REGS[rs(bits)] | CURRENT_STATE.REGS[rt(bits)];
}

void SLL_process(uint32_t bits) {
    NEXT_STATE.REGS[rd(bits)] = CURRENT_STATE.REGS[rt(bits)] << sa(bits);
}

void SLLV_process(uint32_t bits) {
    NEXT_STATE.REGS[rd(bits)] = CURRENT_STATE.REGS[rt(bits)] << get_bits_between(CURRENT_STATE.REGS[rs(bits)], 0, 5);
}

void SLT_process(uint32_t bits) {
    if ((int32_t) CURRENT_STATE.REGS[rs(bits)] < (int32_t) CURRENT_STATE.REGS[rt(bits)]) {
        NEXT_STATE.REGS[rd(bits)] = 1;
    }
    else {
        NEXT_STATE.REGS[rd(bits)] = 0;
    }
}

void SLTU_process(uint32_t bits) {
    if (CURRENT_STATE.REGS[rs(bits)] < CURRENT_STATE.REGS[rt(bits)]) {
        NEXT_STATE.REGS[rd(bits)] = 1;
    }
    else {
        NEXT_STATE.REGS[rd(bits)] = 0;
    }
}

void SRA_process(uint32_t bits) {
    NEXT_STATE.REGS[rd(bits)] = CURRENT_STATE.REGS[rt(bits)] >> sa(bits);
}

void SRAV_process(uint32_t bits) {
    NEXT_STATE.REGS[rd(bits)] = (int32_t) CURRENT_STATE.REGS[rt(bits)] >> get_bits_between(CURRENT_STATE.REGS[rs(bits)], 0, 5);
}

void SRL_process(uint32_t bits) {
    NEXT_STATE.REGS[rd(bits)] = get_bits_between(CURRENT_STATE.REGS[rt(bits)] >> sa(bits), 0, 32 - sa(bits));
}

void SRLV_process(uint32_t bits) {
    NEXT_STATE.REGS[rd(bits)] = get_bits_between(CURRENT_STATE.REGS[rt(bits)] >> get_bits_between(CURRENT_STATE.REGS[rs(bits)], 0, 5), 0, (32 - get_bits_between(CURRENT_STATE.REGS[rs(bits)], 0, 5)));
}

void SUB_process(uint32_t bits) {
    NEXT_STATE.REGS[rd(bits)] = (int32_t) CURRENT_STATE.REGS[rs(bits)] - (int32_t) CURRENT_STATE.REGS[rt(bits)];
}

void SUBU_process(uint32_t bits) {
    NEXT_STATE.REGS[rd(bits)] = CURRENT_STATE.REGS[rs(bits)] - CURRENT_STATE.REGS[rt(bits)];
}

void XOR_process(uint32_t bits) {
    NEXT_STATE.REGS[rd(bits)] = CURRENT_STATE.REGS[rs(bits)] ^ CURRENT_STATE.REGS[rt(bits)];
}

void SYSCALL_process(uint32_t bits) {
    if (CURRENT_STATE.REGS[2] == 10) {
        RUN_BIT = 0;
    }
}


void special_process(uint32_t bits) {
    uint8_t speicalOpcode = (uint8_t) (bits & 0x3F);

    switch (speicalOpcode) {
        case ADD:
            ADD_process(bits);
            break;

        case ADDU:
            ADDU_process(bits);
            break;

        case AND:
            AND_process(bits);
            break;

        case DIV:
            DIV_process(bits);
            break;

        case DIVU:
            DIVU_process(bits);
            break;

        case JALR:
            JALR_process(bits);
            break;

        case JR:
            JR_process(bits);
            break;
        
        case MFHI:
            MFHI_process(bits);
            break;

        case MFLO:
            MFLO_process(bits);
            break;

        case MTHI:
            MTHI_process(bits);
            break;

        case MTLO:
            MTLO_process(bits);
            break;

        case MULT:
            MULT_process(bits);
            break;

        case MULTU:
            MULTU_process(bits);
            break;

        case NOR:
            NOR_process(bits);
            break;
        
        case OR:
            OR_process(bits);
            break;

        case SLL:
            SLL_process(bits);
            break;

        case SLLV:
            SLLV_process(bits);
            break;

        case SLT:
            SLT_process(bits);
            break;

        case SLTU:
            SLTU_process(bits);
            break;

        case SRA:
            SRA_process(bits);
            break;

        case SRAV:
            SRAV_process(bits);
            break;

        case SRL:
            SRL_process(bits);
            break;

        case SRLV:
            SRLV_process(bits);
            break;

        case SUB:
            SUB_process(bits);
            break;

        case SUBU:
            SUBU_process(bits);
            break;
        
        case XOR:
            XOR_process(bits);
            break;

        case SYSCALL:
            SYSCALL_process(bits);
            break;

        default:
            break;
    }
}


/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*             REGIMM Process           */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

void BGEZ_process(uint32_t bits) {
    if ((CURRENT_STATE.REGS[rs(bits)] >> 31) == 0) {
        CURRENT_STATE.PC = CURRENT_STATE.PC + ((convert_to_32(imm(bits), 16) << 2) - 4);
    }
}

void BGEZAL_process(uint32_t bits) {
    NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
    if ((CURRENT_STATE.REGS[rs(bits)] >> 31) == 0) {
        CURRENT_STATE.PC = CURRENT_STATE.PC + ((convert_to_32(imm(bits), 16) << 2) - 4);
    }
}

void BLTZ_process(uint32_t bits) {
    if ((CURRENT_STATE.REGS[rs(bits)] >> 31) != 0) {
        CURRENT_STATE.PC = CURRENT_STATE.PC + ((convert_to_32(imm(bits), 16) << 2) - 4);
    }
}

void BLTZAL_process(uint32_t bits) {
    NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
    if ((CURRENT_STATE.REGS[rs(bits)] >> 31) != 0) {
        CURRENT_STATE.PC = CURRENT_STATE.PC + ((convert_to_32(imm(bits), 16) << 2) - 4);
    }
}


void regimm_process(uint32_t bits) {
    uint8_t regimmOpcode = (uint8_t) get_bits_between(bits, 16, 5);

    switch (regimmOpcode) {
        case BGEZ:
            BGEZ_process(bits);
            break;

        case BGEZAL:
            BGEZAL_process(bits);
            break;

        case BLTZ:
            BLTZ_process(bits);
            break;

        case BLTZAL:
            BLTZAL_process(bits);
            break;

        default:
            break;
    }
}


/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*         Immediate Processes          */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

void ADDI_process(uint32_t bits) {
    NEXT_STATE.REGS[rt(bits)] = (int32_t) convert_to_32(imm(bits), 16) + (int32_t) CURRENT_STATE.REGS[rs(bits)];
}

void ADDIU_process(uint32_t bits) {
    NEXT_STATE.REGS[rt(bits)] = convert_to_32(imm(bits), 16) + CURRENT_STATE.REGS[rs(bits)];
    // printf("ADDIU\n");
}

void ANDI_process(uint32_t bits) {
    NEXT_STATE.REGS[rt(bits)] = CURRENT_STATE.REGS[rs(bits)] & imm(bits);
}

void LB_process(uint32_t bits) {
    uint32_t vAddr = mem_read_32(convert_to_32(imm(bits), 16) + CURRENT_STATE.REGS[base(bits)]);
    NEXT_STATE.REGS[rt(bits)] = convert_to_32(get_bits_between(vAddr, 0, 8), 8);
}

void LBU_process(uint32_t bits) {
    uint32_t vAddr = mem_read_32(convert_to_32(imm(bits), 16) + CURRENT_STATE.REGS[base(bits)]);
    NEXT_STATE.REGS[rt(bits)] = get_bits_between(vAddr, 0, 8);
}

void LH_process(uint32_t bits) {
    uint32_t vAddr = mem_read_32(convert_to_32(imm(bits), 16) + CURRENT_STATE.REGS[base(bits)]);
    NEXT_STATE.REGS[rt(bits)] = convert_to_32(get_bits_between(vAddr, 0, 16), 16);
}

void LHU_process(uint32_t bits) {
    uint32_t vAddr = mem_read_32(convert_to_32(imm(bits), 16) + CURRENT_STATE.REGS[base(bits)]);
    NEXT_STATE.REGS[rt(bits)] = get_bits_between(vAddr, 0, 16);
}

void LUI_process(uint32_t bits) {
    NEXT_STATE.REGS[rt(bits)] = imm(bits) << 16;
}

void LW_process(uint32_t bits) {
    NEXT_STATE.REGS[rt(bits)] = mem_read_32(convert_to_32(imm(bits), 16) + CURRENT_STATE.REGS[base(bits)]);
}

void ORI_process(uint32_t bits) {
    NEXT_STATE.REGS[rt(bits)] = imm(bits) | CURRENT_STATE.REGS[rs(bits)];
}

void SB_process(uint32_t bits) {
    mem_write_32(convert_to_32(imm(bits), 16) + CURRENT_STATE.REGS[base(bits)], get_bits_between(CURRENT_STATE.REGS[rt(bits)], 0, 8));
}

void SH_process(uint32_t bits) {
    mem_write_32(convert_to_32(imm(bits), 16) + CURRENT_STATE.REGS[base(bits)], get_bits_between(CURRENT_STATE.REGS[rt(bits)], 0, 16));
}

void SLTI_process(uint32_t bits) {
    CURRENT_STATE.REGS[rs(bits)] -= (int32_t) convert_to_32(imm(bits), 16);
    if ((int32_t) CURRENT_STATE.REGS[rs(bits)] < (int32_t) convert_to_32(imm(bits), 16)) {
        NEXT_STATE.REGS[rt(bits)] = 1;
    }
    else {
        NEXT_STATE.REGS[rt(bits)] = 0;
    }
}

void SLTIU_process(uint32_t bits) {
    CURRENT_STATE.REGS[rs(bits)] -= convert_to_32(imm(bits), 16);
    if (CURRENT_STATE.REGS[rs(bits)] < convert_to_32(imm(bits), 16)) {
        NEXT_STATE.REGS[rt(bits)] = 1;
    }
    else {
        NEXT_STATE.REGS[rt(bits)] = 0;
    }
}

void SW_process(uint32_t bits) {
    mem_write_32(convert_to_32(imm(bits), 16) + CURRENT_STATE.REGS[base(bits)], CURRENT_STATE.REGS[rt(bits)]);
}

void XORI_process(uint32_t bits) {
    NEXT_STATE.REGS[rt(bits)] = CURRENT_STATE.REGS[rs(bits)] ^ imm(bits);
}


/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*           Branch Processes           */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

void BEQ_process(uint32_t bits) {
    if (CURRENT_STATE.REGS[rs(bits)] == CURRENT_STATE.REGS[rt(bits)]) {
        CURRENT_STATE.PC = CURRENT_STATE.PC + ((convert_to_32(imm(bits), 16) << 2) - 4);
    }
}

void BGTZ_process(uint32_t bits) {
    if ((CURRENT_STATE.REGS[rs(bits)] >> 31) == 0 && CURRENT_STATE.REGS[rs(bits)] != 0) {
        CURRENT_STATE.PC = CURRENT_STATE.PC + ((convert_to_32(imm(bits), 16) << 2) - 4);
    }
}

void BLEZ_process(uint32_t bits) {
    if (((CURRENT_STATE.REGS[rs(bits)] >> 31) == 1) || (CURRENT_STATE.REGS[rs(bits)] == 0)) {
        CURRENT_STATE.PC = CURRENT_STATE.PC + ((convert_to_32(imm(bits), 16) << 2) - 4);
    }
}

void BNE_process(uint32_t bits) {
    if (CURRENT_STATE.REGS[rs(bits)] != CURRENT_STATE.REGS[rt(bits)]) {
        CURRENT_STATE.PC = CURRENT_STATE.PC + ((convert_to_32(imm(bits), 16) << 2) - 4);
    }
}


/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*             Jump Processes           */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

void J_process(uint32_t bits) {
    CURRENT_STATE.PC = ((target(bits)) << 2) - 4;
}

void JAI_process(uint32_t bits) {
    NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
    CURRENT_STATE.PC = ((target(bits)) << 2) - 4;
}


void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */

    uint32_t bits = mem_read_32(CURRENT_STATE.PC);

    uint8_t opcode = (uint8_t) (bits >> 26);

    // printf("Last 6-bits: %d\n", opcode);

    switch (opcode) {
        case ADDI:
            ADDI_process(bits);
            break;

        case ADDIU:
            ADDIU_process(bits);
            break;

        case ANDI:
            ANDI_process(bits);
            break;

        case LB:
            LB_process(bits);
            break;

        case LBU:
            LBU_process(bits);
            break;

        case LHU:
            LHU_process(bits);
            break;

        case LUI:
            LUI_process(bits);
            break;

        case LW:
            LW_process(bits);
            break;

        case ORI:
            ORI_process(bits);
            break;
        
        case SB:
            SB_process(bits);
            break;

        case SH:
            SH_process(bits);
            break;

        case SLTI:
            SLTI_process(bits);
            break;

        case SLTIU:
            SLTIU_process(bits);
            break;

        case SW:
            SW_process(bits);
            break;

        case XORI:
            XORI_process(bits);
            break;

        case BEQ:
            BEQ_process(bits);
            break;

        case BGTZ:
            BGTZ_process(bits);
            break;

        case BLEZ:
            BLEZ_process(bits);
            break;

        case BNE:
            BNE_process(bits);
            break;

        case J:
            J_process(bits);
            break;

        case JAI:
            JAI_process(bits);
            break;

        case REGIMM:
            regimm_process(bits);
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
