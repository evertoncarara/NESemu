#ifndef MOS6502_H
#define MOS6502_H

#include "NESMemorySystem.h"
#include <stdio.h>

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
        
        void Init(NESMemorySystem *memory);
        void Reset();
        void ExecuteInstruction();
        void NMI();
        void IRQ();
        void PrintRegs(){ printf("ac: %X; x: %X; y: %X; s: %X N:%X Z:%X V:%X C:%X I:%X D:%X PC: %X\n",ac,x,y,s,p.N,p.Z,p.V,p.C,p.I,p.D,pc); };
        
        
    private:    
        unsigned int pc;
        unsigned char s;
        union ProcessorStatus p;    
                                  
        char ac;
        char x;
        char y;
        
        NESMemorySystem *memory;
        
        typedef void (MOS6502::*FunctionPointer)();
        FunctionPointer instruction[256];   // Pointers to the functions witch implement the instructions behaviour
        
        void BRK();
        void ORA_IND_X();
        void ORA_ZPG();
        void ASL_ZPG();
        void PHP();
        void ORA_IMM();
        void ASL_ACC();
        void ORA_ABS();
        void ASL_ABS();
        
        void BPL();
        void ORA_IND_Y();
        void ORA_ZPG_X();
        void ASL_ZPG_X();
        void CLC();
        void ORA_ABS_Y();
        void ORA_ABS_X();
        void ASL_ABS_X();
        
        void JSR();
        void AND_IND_X();
        void BIT_ZPG();
        void AND_ZPG();
        void ROL_ZPG();
        void PLP();
        void AND_IMM();
        void ROL_ACC();
        void BIT_ABS();
        void AND_ABS();
        void ROL_ABS();
        
        void BMI();
        void AND_IND_Y();
        void AND_ZPG_X();
        void ROL_ZPG_X();
        void SEC();
        void AND_ABS_Y();
        void AND_ABS_X();
        void ROL_ABS_X();
        
        void RTI();
        void EOR_IND_X();
        void EOR_ZPG();
        void LSR_ZPG();
        void PHA();
        void EOR_IMM();
        void LSR_ACC();
        void JMP_ABS();
        void EOR_ABS();
        void LSR_ABS();
        
        void BVC();
        void EOR_IND_Y();
        void EOR_ZPG_X();
        void LSR_ZPG_X();
        void CLI();
        void EOR_ABS_Y();
        void EOR_ABS_X();
        void LSR_ABS_X();
        
        void RTS();
        void ADC_IND_X();
        void ADC_ZPG();
        void ROR_ZPG();
        void PLA();
        void ADC_IMM();
        void ROR_ACC();
        void JMP_IND();
        void ADC_ABS();
        void ROR_ABS();
        
        void BVS();
        void ADC_IND_Y();
        void ADC_ZPG_X();
        void ROR_ZPG_X();
        void SEI();
        void ADC_ABS_Y();
        void ADC_ABS_X();
        void ROR_ABS_X();
        
        void STA_IND_X();
        void STY_ZPG();
        void STA_ZPG();
        void STX_ZPG();
        void DEY();
        void TXA();
        void STY_ABS();
        void STA_ABS();
        void STX_ABS();
        
        void BCC();
        void STA_IND_Y();
        void STY_ZPG_X();
        void STA_ZPG_X();
        void STX_ZPG_Y();
        void TYA();
        void STA_ABS_Y();
        void TXS();
        void STA_ABS_X();
        
        void LDY_IMM();
        void LDA_IND_X();
        void LDX_IMM();
        void LDY_ZPG();
        void LDA_ZPG();
        void LDX_ZPG();
        void TAY();
        void LDA_IMM();
        void TAX();
        void LDY_ABS();
        void LDA_ABS();
        void LDX_ABS();
        
        void BCS();
        void LDA_IND_Y();
        void LDY_ZPG_X();
        void LDA_ZPG_X();
        void LDX_ZPG_Y();
        void CLV();
        void LDA_ABS_Y();
        void TSX();
        void LDY_ABS_X();
        void LDA_ABS_X();
        void LDX_ABS_Y();
        
        void CPY_IMM();
        void CMP_IND_X();
        void CPY_ZPG();
        void CMP_ZPG();
        void DEC_ZPG();
        void INY();
        void CMP_IMM();
        void DEX();
        void CPY_ABS();
        void CMP_ABS();
        void DEC_ABS();
        
        void BNE();
        void CMP_IND_Y();
        void CMP_ZPG_X();
        void DEC_ZPG_X();
        void CLD();
        void CMP_ABS_Y();
        void CMP_ABS_X();
        void DEC_ABS_X();
        
        void CPX_IMM();
        void SBC_IND_X();
        void CPX_ZPG();
        void SBC_ZPG();
        void INC_ZPG();
        void INX();
        void SBC_IMM();
        void NOP();
        void CPX_ABS();
        void SBC_ABS();
        void INC_ABS();
        
        void BEQ();
        void SBC_IND_Y();
        void SBC_ZPG_X();
        void INC_ZPG_X();
        void SED();
        void SBC_ABS_Y();
        void SBC_ABS_X();
        void INC_ABS_X();
        
};
#endif

