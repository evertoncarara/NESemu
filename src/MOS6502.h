#ifndef MOS6502_H
#define MOS6502_H

#include "MemorySystem.h"
#include <stdio.h>  // PrintRegs()

#define BIT_SET(reg,bit)    (reg |= (1<<bit))     /* Set the indicated bit */
#define BIT_CLR(reg,bit)    (reg &= ~(1<<bit))    /* Clear the indicated bit */
#define BIT_CHECK(reg,bit)  (((reg & (1<<bit)) == 0) ? 0 : 1)      /* Return the bit value (0 or 1) */
#define BIT_ASSIGN(reg,bit,value)    ((value) ? BIT_SET(reg,bit) : BIT_CLR(reg,bit))     /* Set the indicated bit to value */

union ProcessorStatus {
    struct {
        unsigned char C : 1;        /* Carry flag */
        unsigned char Z : 1;        /* Zero flag */
        unsigned char I : 1;        /* IRQ disable(1 = disabled) */
        unsigned char D : 1;        /* Decimal mode (1 = activated) */
        unsigned char B : 1;        /* BRK instruction (1 = BRK)*/
        unsigned char X : 1;        /* Expansion */
        unsigned char V : 1;        /* Overflow flag */
        unsigned char N : 1;        /* Negative flag */
    };
    unsigned char status;           /* Access to all bit fields at the same time */
};

class MOS6502 {
    public:
        MOS6502();
        ~MOS6502();
        
        void Init(MemorySystem *memory);
        void Reset();
        int ExecuteInstruction();
        int NMI();
        void PrintRegs(){ printf("ac: %X; x: %X; y: %X; s: %X N:%X Z:%X V:%X C:%X I:%X D:%X PC: %X\n",ac,x,y,s,p.N,p.Z,p.V,p.C,p.I,p.D,pc); };
        
        
    private:    
        unsigned int pc;
        unsigned char s;
        union ProcessorStatus p;    
                                  
        char ac;
        char x;
        char y;
        
        MemorySystem *memory;
        
        typedef int (MOS6502::*FunctionPointer)();
        FunctionPointer instruction[256];   // Pointers to the functions witch implement the instructions behaviour
        
        int BRK();
        int ORA_IND_X();
        int ORA_ZPG();
        int ASL_ZPG();
        int PHP();
        int ORA_IMM();
        int ASL_ACC();
        int ORA_ABS();
        int ASL_ABS();
        
        int BPL();
        int ORA_IND_Y();
        int ORA_ZPG_X();
        int ASL_ZPG_X();
        int CLC();
        int ORA_ABS_Y();
        int ORA_ABS_X();
        int ASL_ABS_X();
        
        int JSR();
        int AND_IND_X();
        int BIT_ZPG();
        int AND_ZPG();
        int ROL_ZPG();
        int PLP();
        int AND_IMM();
        int ROL_ACC();
        int BIT_ABS();
        int AND_ABS();
        int ROL_ABS();
        
        int BMI();
        int AND_IND_Y();
        int AND_ZPG_X();
        int ROL_ZPG_X();
        int SEC();
        int AND_ABS_Y();
        int AND_ABS_X();
        int ROL_ABS_X();
        
        int RTI();
        int EOR_IND_X();
        int EOR_ZPG();
        int LSR_ZPG();
        int PHA();
        int EOR_IMM();
        int LSR_ACC();
        int JMP_ABS();
        int EOR_ABS();
        int LSR_ABS();
        
        int BVC();
        int EOR_IND_Y();
        int EOR_ZPG_X();
        int LSR_ZPG_X();
        int CLI();
        int EOR_ABS_Y();
        int EOR_ABS_X();
        int LSR_ABS_X();
        
        int RTS();
        int ADC_IND_X();
        int ADC_ZPG();
        int ROR_ZPG();
        int PLA();
        int ADC_IMM();
        int ROR_ACC();
        int JMP_IND();
        int ADC_ABS();
        int ROR_ABS();
        
        int BVS();
        int ADC_IND_Y();
        int ADC_ZPG_X();
        int ROR_ZPG_X();
        int SEI();
        int ADC_ABS_Y();
        int ADC_ABS_X();
        int ROR_ABS_X();
        
        int STA_IND_X();
        int STY_ZPG();
        int STA_ZPG();
        int STX_ZPG();
        int DEY();
        int TXA();
        int STY_ABS();
        int STA_ABS();
        int STX_ABS();
        
        int BCC();
        int STA_IND_Y();
        int STY_ZPG_X();
        int STA_ZPG_X();
        int STX_ZPG_Y();
        int TYA();
        int STA_ABS_Y();
        int TXS();
        int STA_ABS_X();
        
        int LDY_IMM();
        int LDA_IND_X();
        int LDX_IMM();
        int LDY_ZPG();
        int LDA_ZPG();
        int LDX_ZPG();
        int TAY();
        int LDA_IMM();
        int TAX();
        int LDY_ABS();
        int LDA_ABS();
        int LDX_ABS();
        
        int BCS();
        int LDA_IND_Y();
        int LDY_ZPG_X();
        int LDA_ZPG_X();
        int LDX_ZPG_Y();
        int CLV();
        int LDA_ABS_Y();
        int TSX();
        int LDY_ABS_X();
        int LDA_ABS_X();
        int LDX_ABS_Y();
        
        int CPY_IMM();
        int CMP_IND_X();
        int CPY_ZPG();
        int CMP_ZPG();
        int DEC_ZPG();
        int INY();
        int CMP_IMM();
        int DEX();
        int CPY_ABS();
        int CMP_ABS();
        int DEC_ABS();
        
        int BNE();
        int CMP_IND_Y();
        int CMP_ZPG_X();
        int DEC_ZPG_X();
        int CLD();
        int CMP_ABS_Y();
        int CMP_ABS_X();
        int DEC_ABS_X();
        
        int CPX_IMM();
        int SBC_IND_X();
        int CPX_ZPG();
        int SBC_ZPG();
        int INC_ZPG();
        int INX();
        int SBC_IMM();
        int NOP();
        int CPX_ABS();
        int SBC_ABS();
        int INC_ABS();
        
        int BEQ();
        int SBC_IND_Y();
        int SBC_ZPG_X();
        int INC_ZPG_X();
        int SED();
        int SBC_ABS_Y();
        int SBC_ABS_X();
        int INC_ABS_X();
        
};
#endif

