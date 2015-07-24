#include "MOS6502.h"
#include <cassert>

#include <stdio.h>

MOS6502::MOS6502() {  
    
    memory = NULL;

    for (int i=0; i<256; i++)
        instruction[i] = NULL;
        
    instruction[0x01] = &MOS6502::ORA_IND_X;
    instruction[0x05] = &MOS6502::ORA_ZPG;
    instruction[0x06] = &MOS6502::ASL_ZPG;
    instruction[0x08] = &MOS6502::PHP;
    instruction[0x09] = &MOS6502::ORA_IMM;
    instruction[0x0A] = &MOS6502::ASL_ACC;
    instruction[0x0D] = &MOS6502::ORA_ABS;
    instruction[0x0E] = &MOS6502::ASL_ABS;
    instruction[0x10] = &MOS6502::BPL;
    instruction[0x11] = &MOS6502::ORA_IND_Y;
    instruction[0x15] = &MOS6502::ORA_ZPG_X;
    instruction[0x16] = &MOS6502::ASL_ZPG_X;
    instruction[0x18] = &MOS6502::CLC;
    instruction[0x19] = &MOS6502::ORA_ABS_Y;
    instruction[0x1D] = &MOS6502::ORA_ABS_X;
    instruction[0x1E] = &MOS6502::ASL_ABS_X;
    instruction[0x20] = &MOS6502::JSR;
    instruction[0x21] = &MOS6502::AND_IND_X;
    instruction[0x24] = &MOS6502::BIT_ZPG; 
    instruction[0x25] = &MOS6502::AND_ZPG;
    instruction[0x26] = &MOS6502::ROL_ZPG;
    instruction[0x28] = &MOS6502::PLP;
    instruction[0x29] = &MOS6502::AND_IMM;
    instruction[0x2A] = &MOS6502::ROL_ACC;
    instruction[0x2C] = &MOS6502::BIT_ABS;
    instruction[0x2D] = &MOS6502::AND_ABS;
    instruction[0x2E] = &MOS6502::ROL_ABS;
    instruction[0x30] = &MOS6502::BMI;
    instruction[0x31] = &MOS6502::AND_IND_Y;
    instruction[0x38] = &MOS6502::SEC;
    instruction[0x39] = &MOS6502::AND_ABS_Y;
    instruction[0x35] = &MOS6502::AND_ZPG_X;
    instruction[0x36] = &MOS6502::ROL_ZPG_X;
    instruction[0x3D] = &MOS6502::AND_ABS_X;
    instruction[0x3E] = &MOS6502::ROL_ABS_X;
    instruction[0x40] = &MOS6502::RTI;
    instruction[0x41] = &MOS6502::EOR_IND_X;
    instruction[0x45] = &MOS6502::EOR_ZPG;
    instruction[0x46] = &MOS6502::LSR_ZPG;
    instruction[0x48] = &MOS6502::PHA;
    instruction[0x49] = &MOS6502::EOR_IMM;
    instruction[0x4A] = &MOS6502::LSR_ACC;
    instruction[0x4C] = &MOS6502::JMP_ABS;
    instruction[0x4D] = &MOS6502::EOR_ABS;
    instruction[0x4E] = &MOS6502::LSR_ABS;
    instruction[0x50] = &MOS6502::BVC;
    instruction[0x51] = &MOS6502::EOR_IND_Y;
    instruction[0x55] = &MOS6502::EOR_ZPG_X;
    instruction[0x56] = &MOS6502::LSR_ZPG_X;
    instruction[0x58] = &MOS6502::CLI;
    instruction[0x59] = &MOS6502::EOR_ABS_Y;
    instruction[0x5D] = &MOS6502::EOR_ABS_X;
    instruction[0x5E] = &MOS6502::LSR_ABS_X;
    instruction[0x60] = &MOS6502::RTS;
    instruction[0x61] = &MOS6502::ADC_IND_X;
    instruction[0x65] = &MOS6502::ADC_ZPG;
    instruction[0x66] = &MOS6502::ROR_ZPG;
    instruction[0x68] = &MOS6502::PLA;
    instruction[0x69] = &MOS6502::ADC_IMM;
    instruction[0x6A] = &MOS6502::ROR_ACC;
    instruction[0x6C] = &MOS6502::JMP_IND;
    instruction[0x6D] = &MOS6502::ADC_ABS;
    instruction[0x6E] = &MOS6502::ROR_ABS;
    instruction[0x70] = &MOS6502::BVS;
    instruction[0x71] = &MOS6502::ADC_IND_Y;
    instruction[0x75] = &MOS6502::ADC_ZPG_X;
    instruction[0x76] = &MOS6502::ROR_ZPG_X;
    instruction[0x78] = &MOS6502::SEI;
    instruction[0x79] = &MOS6502::ADC_ABS_Y;
    instruction[0x7D] = &MOS6502::ADC_ABS_X;
    instruction[0x7E] = &MOS6502::ROR_ABS_X;
    instruction[0x81] = &MOS6502::STA_IND_X;
    instruction[0x84] = &MOS6502::STY_ZPG;
    instruction[0x85] = &MOS6502::STA_ZPG;
    instruction[0x86] = &MOS6502::STX_ZPG;
    instruction[0x88] = &MOS6502::DEY;
    instruction[0x8A] = &MOS6502::TXA;
    instruction[0x8C] = &MOS6502::STY_ABS;
    instruction[0x8D] = &MOS6502::STA_ABS;
    instruction[0x8E] = &MOS6502::STX_ABS;
    instruction[0x90] = &MOS6502::BCC;
    instruction[0x91] = &MOS6502::STA_IND_Y;
    instruction[0x94] = &MOS6502::STY_ZPG_X;   
    instruction[0x95] = &MOS6502::STA_ZPG_X;
    instruction[0x96] = &MOS6502::STX_ZPG_Y;
    instruction[0x98] = &MOS6502::TYA;
    instruction[0x99] = &MOS6502::STA_ABS_Y;
    instruction[0x9A] = &MOS6502::TXS;
    instruction[0x9D] = &MOS6502::STA_ABS_X;
    instruction[0xA0] = &MOS6502::LDY_IMM;
    instruction[0xA1] = &MOS6502::LDA_IND_X;
    instruction[0xA2] = &MOS6502::LDX_IMM;
    instruction[0xA4] = &MOS6502::LDY_ZPG;
    instruction[0xA5] = &MOS6502::LDA_ZPG;
    instruction[0xA6] = &MOS6502::LDX_ZPG;
    instruction[0xA9] = &MOS6502::LDA_IMM;
    instruction[0xA8] = &MOS6502::TAY;
    instruction[0xAA] = &MOS6502::TAX;
    instruction[0xAC] = &MOS6502::LDY_ABS;
    instruction[0xAD] = &MOS6502::LDA_ABS;
    instruction[0xAE] = &MOS6502::LDX_ABS;
    instruction[0xB0] = &MOS6502::BCS;
    instruction[0xB1] = &MOS6502::LDA_IND_Y;
    instruction[0xB4] = &MOS6502::LDY_ZPG_X;
    instruction[0xB5] = &MOS6502::LDA_ZPG_X;
    instruction[0xB6] = &MOS6502::LDX_ZPG_Y;
    instruction[0xB8] = &MOS6502::CLV;
    instruction[0xB9] = &MOS6502::LDA_ABS_Y;
    instruction[0xBA] = &MOS6502::TSX;
    instruction[0xBC] = &MOS6502::LDY_ABS_X;
    instruction[0xBD] = &MOS6502::LDA_ABS_X;
    instruction[0xBE] = &MOS6502::LDX_ABS_Y;
    instruction[0xBE] = &MOS6502::LDX_ABS_Y;
    instruction[0xC0] = &MOS6502::CPY_IMM;
    instruction[0xC1] = &MOS6502::CMP_IND_X;
    instruction[0xC4] = &MOS6502::CPY_ZPG;
    instruction[0xC5] = &MOS6502::CMP_ZPG;
    instruction[0xC6] = &MOS6502::DEC_ZPG;
    instruction[0xC8] = &MOS6502::INY;
    instruction[0xC9] = &MOS6502::CMP_IMM;
    instruction[0xCA] = &MOS6502::DEX;
    instruction[0xCC] = &MOS6502::CPY_ABS;
    instruction[0xCD] = &MOS6502::CMP_ABS;
    instruction[0xCE] = &MOS6502::DEC_ABS;
    instruction[0xD0] = &MOS6502::BNE;
    instruction[0xD1] = &MOS6502::CMP_IND_Y;
    instruction[0xD5] = &MOS6502::CMP_ZPG_X;
    instruction[0xD6] = &MOS6502::DEC_ZPG_X;
    instruction[0xD8] = &MOS6502::CLD;
    instruction[0xD9] = &MOS6502::CMP_ABS_Y;
    instruction[0xDD] = &MOS6502::CMP_ABS_X;
    instruction[0xDE] = &MOS6502::DEC_ABS_X;   
    instruction[0xE0] = &MOS6502::CPX_IMM;
    instruction[0xE1] = &MOS6502::SBC_IND_X;
    instruction[0xE4] = &MOS6502::CPX_ZPG;
    instruction[0xE5] = &MOS6502::SBC_ZPG;
    instruction[0xE6] = &MOS6502::INC_ZPG;
    instruction[0xE8] = &MOS6502::INX;
    instruction[0xE9] = &MOS6502::SBC_IMM;
    instruction[0xEA] = &MOS6502::NOP;
    instruction[0xEC] = &MOS6502::CPX_ABS;
    instruction[0xED] = &MOS6502::SBC_ABS;
    instruction[0xEE] = &MOS6502::INC_ABS;
    instruction[0xF0] = &MOS6502::BEQ;
    instruction[0xF1] = &MOS6502::SBC_IND_Y;
    instruction[0xF5] = &MOS6502::SBC_ZPG_X;
    instruction[0xF6] = &MOS6502::INC_ZPG_X;
    instruction[0xF8] = &MOS6502::SED;
    instruction[0xF9] = &MOS6502::SBC_ABS_Y;
    instruction[0xFD] = &MOS6502::SBC_ABS_X;
    instruction[0xFE] = &MOS6502::INC_ABS_X;    
}

MOS6502::~MOS6502(){}

void MOS6502::Init(NESMemorySystem *memory) {
    
    this->memory = memory;
}

void MOS6502::Reset() {
    
    /* Initialize pc with the reset vector at 0xFFFD:0xFFFC */
    unsigned int address;
    address = memory->Read(0xFFFC);   /* Read the low byte address */
    address = (memory->Read(0xFFFD)<<8) | address;    /* Read the high byte address */
    pc = address;

    s = 0xFD;       // Visual6502
    p.C = 0;
    p.Z = 0;
    p.I = 0;
    p.D = 0;
    p.B = 0;        
    p.X = 1;        // Hardwired (unused bit)
    p.V = 0;
    p.N = 0;
}

void MOS6502::ExecuteInstruction() {
    
    assert(memory != NULL && "CPU not initialized");

    unsigned char opcode;
    
    opcode = memory->Read(pc++);
    
    if (instruction[opcode] != NULL)
        (this->*instruction[opcode])();
    else
        printf("INVALID INSTRUCTION: opcode = %X (pc = %X)\n",opcode,pc-1);
};

void MOS6502::NMI() {

    /* Store pc and status processor register on stack */
    unsigned char pcl = pc & 0xFF;
    unsigned char pch = (pc & 0xFF00)>>8;
    memory->Write(0x100|s--,pch);         // Store pc high byte
    memory->Write(0x100|s--,pcl);         // Store pc low byte   
    memory->Write(0x100|s--,p.status);    // Store status 

    /* Initialize pc with the NMI vector at 0xFFFB:0xFFFA */
    unsigned int address;
    address = memory->Read(0xFFFA);                 // Read the low byte address 
    address = (memory->Read(0xFFFB)<<8) | address;  // Read the high byte address
    pc = address;   

    /* Disable interruptons */ 
    p.I = 1;    // MAYBE UNECESSARY 
}

void MOS6502::LDA_IMM() { 
    
    ac = memory->Read(pc++);

    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::LDX_IMM() { 
    
    x = memory->Read(pc++); 
    
    (x == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (x < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::LDY_IMM() { 
    
    y = memory->Read(pc++); 
    
    (y == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (y < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::LDA_ABS() {
    
    unsigned int address;           /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    
    ac = memory->Read(address);     /* ac <- mem[address] */
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::LDX_ABS() {
    
    unsigned int address;           /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    
    x = memory->Read(address);      /* x <- mem[address] */
    
    (x == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (x < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::LDY_ABS() {
    
    unsigned int address;           /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    
    y = memory->Read(address);      /* y <- mem[address] */
    
    (y == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (y < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::LDA_ABS_X() {
    
    unsigned int address;       /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    address = address + (unsigned char)x;
    ac = memory->Read(address);     /* ac <- mem[address] */
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::LDA_ABS_Y() {
    
    unsigned int address;       /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    address = address + (unsigned char)y;
    ac = memory->Read(address);     /* ac <- mem[address] */
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::LDX_ABS_Y() {
    
    unsigned int address;           /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    address = address + (unsigned char)y;
    x = memory->Read(address);      /* x <- mem[address] */
    
    (x == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (x < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::LDY_ABS_X() {
    
    unsigned int address;       /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    address = address + (unsigned char)x;
    y = memory->Read(address);     /* y <- mem[address] */
    
    (y == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (y < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::LDA_ZPG() { 
    
    ac = memory->Read(memory->Read(pc++)); 
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::LDX_ZPG() { 
    
    x = memory->Read(memory->Read(pc++)); 
   
    (x == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (x < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::LDY_ZPG() { 
    
    y = memory->Read(memory->Read(pc++)); 
    
    (y == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (y < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::LDA_ZPG_X() { 

    unsigned char zeroPageAddress = memory->Read(pc++)+(unsigned char)x;
    
    ac = memory->Read(zeroPageAddress); 
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::LDY_ZPG_X() { 
    
    unsigned char zeroPageAddress = memory->Read(pc++)+(unsigned char)x;
    
    y = memory->Read(zeroPageAddress); 
    
    (y == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (y < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::LDX_ZPG_Y() { 

    unsigned char zeroPageAddress = memory->Read(pc++)+(unsigned char)y;
    
    x = memory->Read(zeroPageAddress); 
    
    (x == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (x < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
} 

void MOS6502::LDA_IND_X() {
    
    unsigned char indAddress;   /* Zero page address */
    unsigned int address;       /* Absolute address */
    
    indAddress = memory->Read(pc++) + (unsigned char)x;
    
    address = memory->Read(indAddress++);   /* Read the low byte address */
    address = (memory->Read(indAddress++)<<8) | address; /* Read the high byte address and concatenates with the low byte  */

    ac = memory->Read(address);     /* ac <- mem[mem[indAddress+x]] */
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::LDA_IND_Y() {
    
    unsigned char indAddress;   /* Zero page address */
    unsigned int address;       /* Absolute address */
    
    indAddress = memory->Read(pc++);
    
    address = memory->Read(indAddress++);   /* Read the low byte address */
    address = (memory->Read(indAddress++)<<8) | address; /* Read the high byte address and concatenates with the low byte  */
    address = address + (unsigned char)y;
    ac = memory->Read(address);              /* ac <- mem[mem[indAddress]+y] */
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::STA_ABS() {
    
    unsigned int address;           /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    
    memory->Write(address,ac);      /* mem[address] <- ac */
}

void MOS6502::STX_ABS() {
    
    unsigned int address;           /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    
    memory->Write(address,x);       /* mem[address] <- x */
}

void MOS6502::STY_ABS() {
    
    unsigned int address;           /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    
    memory->Write(address,y);       /* mem[address] <- y */
}

void MOS6502::STA_ABS_X() {
    
    unsigned int address;           /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    address = address + (unsigned char)x;
    memory->Write(address,ac);      /* mem[address] <- ac */
}

void MOS6502::STA_ABS_Y() {
    
    unsigned int address;           /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    address = address + (unsigned char)y;
    memory->Write(address,ac);      /* mem[address] <- ac */
}

void MOS6502::STA_ZPG() { memory->Write(memory->Read(pc++),ac); }

void MOS6502::STY_ZPG() { memory->Write(memory->Read(pc++),y); }

void MOS6502::STX_ZPG() { memory->Write(memory->Read(pc++),x); } 

void MOS6502::STA_ZPG_X() { 
    
    unsigned char zeroPageAddress = memory->Read(pc++)+(unsigned char)x;
    
    memory->Write(zeroPageAddress,ac); 
} 

void MOS6502::STY_ZPG_X() { 
    
    unsigned char zeroPageAddress = memory->Read(pc++)+(unsigned char)x;
    
    memory->Write(zeroPageAddress,y); 
} 

void MOS6502::STX_ZPG_Y() { 
    
    unsigned char zeroPageAddress = memory->Read(pc++)+(unsigned char)y;
    
    memory->Write(zeroPageAddress,x); 
}

void MOS6502::STA_IND_X() {
    
    unsigned char indAddress;   /* Zero page address */
    unsigned int address;       /* Absolute address */
    
    indAddress = memory->Read(pc++) + (unsigned char)x;
    
    address = memory->Read(indAddress++);   /* Read the low byte address */
    address = (memory->Read(indAddress++)<<8) | address; /* Read the high byte address and concatenates with the low byte  */

    memory->Write(address,ac);              /* mem[mem[indAddress+x]] <- ac */
}

void MOS6502::STA_IND_Y() {
    
    unsigned char indAddress;   /* Zero page address */
    unsigned int address;       /* Absolute address */
    
    indAddress = memory->Read(pc++);
    
    address = memory->Read(indAddress++);   /* Read the low byte address */
    address = (memory->Read(indAddress++)<<8) | address; /* Read the high byte address and concatenates with the low byte  */
    address = address + (unsigned char)y;
    memory->Write(address,ac);              /* mem[mem[indAddress]+y] <- ac */
}

void MOS6502::AND_IMM() { 
    
    ac = ac & memory->Read(pc++);

    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::ORA_IMM() { 
    
    ac = ac | memory->Read(pc++);

    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::EOR_IMM() { 
    
    ac = ac ^ memory->Read(pc++);

    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::AND_ZPG() { 
    
    ac = ac & memory->Read(memory->Read(pc++)); 

    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::ORA_ZPG() { 
    
    ac = ac | memory->Read(memory->Read(pc++)); 

    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::AND_ZPG_X() { 

    unsigned char zeroPageAddress = memory->Read(pc++)+(unsigned char)x;
    
    ac = ac & memory->Read(zeroPageAddress); 
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::ORA_ZPG_X() { 

    unsigned char zeroPageAddress = memory->Read(pc++)+(unsigned char)x;
    
    ac = ac | memory->Read(zeroPageAddress); 
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::EOR_ZPG_X() { 

    unsigned char zeroPageAddress = memory->Read(pc++)+(unsigned char)x;
    
    ac = ac ^ memory->Read(zeroPageAddress); 
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::EOR_ABS() {
    
    unsigned int address;           /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    
    ac = ac ^ memory->Read(address);     /* ac <- mem[address] */
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::AND_ABS() {
    
    unsigned int address;           /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    
    ac = ac & memory->Read(address);     /* ac <- mem[address] */
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::ORA_ABS() {
    
    unsigned int address;           /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    
    ac = ac | memory->Read(address);     /* ac <- mem[address] */
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::EOR_ZPG() { 
    
    ac = ac ^ memory->Read(memory->Read(pc++)); 

    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::AND_ABS_X() {
    
    unsigned int address;       /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    address = address + (unsigned char)x;
    ac = ac & memory->Read(address);     /* ac <- mem[address] */
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::ORA_ABS_X() {
    
    unsigned int address;       /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    address = address + (unsigned char)x;
    ac = ac | memory->Read(address);     /* ac <- mem[address] */
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::EOR_ABS_X() {
    
    unsigned int address;       /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    address = address + (unsigned char)x;
    ac = ac ^ memory->Read(address);     /* ac <- mem[address] */
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::AND_ABS_Y() {
    
    unsigned int address;       /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    address = address + (unsigned char)y;
    ac = ac & memory->Read(address);     /* ac <- mem[address] */
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::ORA_ABS_Y() {
    
    unsigned int address;       /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    address = address + (unsigned char)y;
    ac = ac | memory->Read(address);     /* ac <- mem[address] */
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::EOR_ABS_Y() {
    
    unsigned int address;       /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    address = address + (unsigned char)y;
    ac = ac ^ memory->Read(address);     /* ac <- mem[address] */
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::AND_IND_X() {
    
    unsigned char indAddress;   /* Zero page address */
    unsigned int address;       /* Absolute address */
    
    indAddress = memory->Read(pc++) + (unsigned char)x;
    
    address = memory->Read(indAddress++);   /* Read the low byte address */
    address = (memory->Read(indAddress++)<<8) | address; /* Read the high byte address and concatenates with the low byte  */

    ac = ac & memory->Read(address);     /* ac <- mem[mem[indAddress+x]] */
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::ORA_IND_X() {
    
    unsigned char indAddress;   /* Zero page address */
    unsigned int address;       /* Absolute address */
    
    indAddress = memory->Read(pc++) + (unsigned char)x;
    
    address = memory->Read(indAddress++);   /* Read the low byte address */
    address = (memory->Read(indAddress++)<<8) | address; /* Read the high byte address and concatenates with the low byte  */

    ac = ac | memory->Read(address);     /* ac <- mem[mem[indAddress+x]] */
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::EOR_IND_X() {
    
    unsigned char indAddress;   /* Zero page address */
    unsigned int address;       /* Absolute address */
    
    indAddress = memory->Read(pc++) + (unsigned char)x;
    
    address = memory->Read(indAddress++);   /* Read the low byte address */
    address = (memory->Read(indAddress++)<<8) | address; /* Read the high byte address and concatenates with the low byte  */

    ac = ac ^ memory->Read(address);     /* ac <- mem[mem[indAddress+x]] */
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::AND_IND_Y() {
    
    unsigned char indAddress;   /* Zero page address */
    unsigned int address;       /* Absolute address */
    
    indAddress = memory->Read(pc++);
    
    address = memory->Read(indAddress++);   /* Read the low byte address */
    address = (memory->Read(indAddress++)<<8) | address; /* Read the high byte address and concatenates with the low byte  */
    address = address + (unsigned char)y;    
    ac = ac & memory->Read(address);              /* ac <- mem[mem[indAddress]+y] */
        
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::ORA_IND_Y() {
    
    unsigned char indAddress;   /* Zero page address */
    unsigned int address;       /* Absolute address */
    
    indAddress = memory->Read(pc++);
    
    address = memory->Read(indAddress++);   /* Read the low byte address */
    address = (memory->Read(indAddress++)<<8) | address; /* Read the high byte address and concatenates with the low byte  */
    address = address + (unsigned char)y;
    ac = ac | memory->Read(address);              /* ac <- mem[mem[indAddress]+y] */
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::EOR_IND_Y() {
    
    unsigned char indAddress;   /* Zero page address */
    unsigned int address;       /* Absolute address */
    
    indAddress = memory->Read(pc++);
    
    address = memory->Read(indAddress++);   /* Read the low byte address */
    address = (memory->Read(indAddress++)<<8) | address; /* Read the high byte address and concatenates with the low byte  */
    address = address + (unsigned char)y;
    ac = ac ^ memory->Read(address);              /* ac <- mem[mem[indAddress]+y] */
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::INC_ZPG() { 
    
    char data;
    unsigned char zeroPageAddress = memory->Read(pc++);
    
    data = memory->Read(zeroPageAddress);
    data++;
    memory->Write(zeroPageAddress,data);
    
    (data == 0) ? p.Z = 1 : p.Z = 0;       /* Set/Reset Zero flag */
    
    (data < 0) ? p.N = 1 : p.N = 0;        /* Set/Reset Negative flag */
}

void MOS6502::INC_ZPG_X() { 

    char data;  
    unsigned char zeroPageAddress = memory->Read(pc++)+(unsigned char)x;
    
    data = memory->Read(zeroPageAddress); 
    data++;
    memory->Write(zeroPageAddress,data);
    
    (data == 0) ? p.Z = 1 : p.Z = 0;       /* Set/Reset Zero flag */
    
    (data < 0) ? p.N = 1 : p.N = 0;        /* Set/Reset Negative flag */
}

void MOS6502::INC_ABS() {
    
    char data;
    unsigned int address;           /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    
    data = memory->Read(address);     /* data <- mem[address] */
    data++;
    memory->Write(address,data);
    
    (data == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (data < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::INC_ABS_X() {
    
    char data;
    unsigned int address;       /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    address = address + (unsigned char)x;
    data = memory->Read(address);     /* data <- mem[address] */
    data++;
    memory->Write(address,data);
    
    (data == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (data < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::DEC_ZPG() { 
    
    char data;
    unsigned char zeroPageAddress = memory->Read(pc++);
    
    data = memory->Read(zeroPageAddress);
    data--;
    memory->Write(zeroPageAddress,data);
    
    (data == 0) ? p.Z = 1 : p.Z = 0;       /* Set/Reset Zero flag */
    
    (data < 0) ? p.N = 1 : p.N = 0;        /* Set/Reset Negative flag */
}

void MOS6502::DEC_ZPG_X() { 

    char data;  
    unsigned char zeroPageAddress = memory->Read(pc++)+(unsigned char)x;
    
    data = memory->Read(zeroPageAddress); 
    data--;
    memory->Write(zeroPageAddress,data);
    
    (data == 0) ? p.Z = 1 : p.Z = 0;       /* Set/Reset Zero flag */
    
    (data < 0) ? p.N = 1 : p.N = 0;        /* Set/Reset Negative flag */
}

void MOS6502::DEC_ABS() {
    
    char data;
    unsigned int address;           /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    
    data = memory->Read(address);     /* data <- mem[address] */
    data--;
    memory->Write(address,data);
    
    (data == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (data < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::DEC_ABS_X() {
    
    char data;
    unsigned int address;       /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    address = address + (unsigned char)x;
    data = memory->Read(address);     /* data <- mem[address] */
    data--;
    memory->Write(address,data);
    
    (data == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (data < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
}

void MOS6502::LSR_ACC() {
           
    p.C = BIT_CHECK(ac,0);     /* Carry flag = ac[0] */
    
    ac = (unsigned char)ac >> 1;            /* Result must be positive */
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    p.N = 0;   /* Clear negative flag */
 }

void MOS6502::ASL_ACC() {
            
    p.C = BIT_CHECK(ac,7);     /* Carry flag = ac[7] */
    
    ac = ac << 1;
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;    /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;     /* Set/Reset Negative flag */
}

void MOS6502::ASL_ZPG() { 
    
    char data;
    unsigned char zeroPageAddress = memory->Read(pc++);
    
    data = memory->Read(zeroPageAddress);
    
    p.C = BIT_CHECK(data,7);     /* Carry flag = data[7] */ 
    
    data = data << 1;
    memory->Write(zeroPageAddress,data);
    
    (data == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (data < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::LSR_ZPG() { 
    
    unsigned char data;     /* Result is always positive */
    unsigned char zeroPageAddress = memory->Read(pc++);
    
    data = memory->Read(zeroPageAddress);
    
    p.C = BIT_CHECK(data,0);     /* Carry flag = data[0] */ 
    
    data = data >> 1;
    memory->Write(zeroPageAddress,data);
    
    (data == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    p.N = 0;   /* Clear negative flag */
}

void MOS6502::ASL_ZPG_X() { 

    char data;  
    unsigned char zeroPageAddress = memory->Read(pc++)+(unsigned char)x;
    
    data = memory->Read(zeroPageAddress); 
    
    p.C = BIT_CHECK(data,7);     /* Carry flag = data[7] */ 
    
    data = data << 1;
    memory->Write(zeroPageAddress,data);
    
    (data == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (data < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::LSR_ZPG_X() { 

    unsigned char data;     /* Result is always positive */
    unsigned char zeroPageAddress = memory->Read(pc++)+(unsigned char)x;
    
    data = memory->Read(zeroPageAddress);
    
    p.C = BIT_CHECK(data,0);     /* Carry flag = data[0] */ 
    
    data = data >> 1;
    memory->Write(zeroPageAddress,data);
    
    (data == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    p.N = 0;   /* Clear negative flag */
}

void MOS6502::LSR_ABS_X() {
    
    unsigned char data;         /* Result is always positive */
    unsigned int address;       /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    address = address + (unsigned char)x;
    data = memory->Read(address);     /* data <- mem[address] */
    
    p.C = BIT_CHECK(data,0);     /* Carry flag = data[0] */ 
    
    data = data >> 1;
    memory->Write(address,data);
    
    (data == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    p.N = 0;   /* Clear negative flag */
}

void MOS6502::ASL_ABS_X() {
    
    char data;
    unsigned int address;       /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    address = address + (unsigned char)x;
    data = memory->Read(address);     /* data <- mem[address] */
    
    p.C = BIT_CHECK(data,7);     /* Carry flag = data[7] */ 
    
    data = data << 1;
    memory->Write(address,data);
    
    (data == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (data < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::LSR_ABS() {
    
    unsigned char data;             /* Result is always positive */
    unsigned int address;           /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    
    data = memory->Read(address);     /* data <- mem[address] */
    
    p.C = BIT_CHECK(data,0);     /* Carry flag = data[0] */ 
    
    data = data >> 1;
    memory->Write(address,data);
    
    (data == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    p.N = 0;   /* Clear negative flag */
}

void MOS6502::ASL_ABS() {
    
    char data;
    unsigned int address;           /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    
    data = memory->Read(address);     /* data <- mem[address] */
    
    p.C = BIT_CHECK(data,7);     /* Carry flag = data[7] */ 
    
    data = data << 1;
    memory->Write(address,data);
    
    (data == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (data < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::ROL_ACC() {
            
    char carry = p.C;
    
    p.C = BIT_CHECK(ac,7);         /* Carry flag = ac[7] */
    
    ac = (ac << 1) | carry;
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;    /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;     /* Set/Reset Negative flag */
}

void MOS6502::ROR_ACC() {
            
    char carry = p.C;
    
    p.C = BIT_CHECK(ac,0);         /* Carry flag = ac[0] */
    
    ac = (carry << 7) | (ac >> 1);
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;    /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;     /* Set/Reset Negative flag */
}

void MOS6502::ROL_ZPG() { 
    
    char data;
    unsigned char zeroPageAddress = memory->Read(pc++);
    char carry = p.C;
    
    data = memory->Read(zeroPageAddress);
    
    p.C = BIT_CHECK(data,7);         /* Carry flag = data[7] */
    
    data = (data << 1) | carry;
    
    memory->Write(zeroPageAddress,data);    
    
    (data == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (data < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::ROR_ZPG() { 
    
    unsigned char data;             /* Result is always positive */
    unsigned char zeroPageAddress = memory->Read(pc++);
    char carry = p.C;
    
    data = memory->Read(zeroPageAddress);
    
    p.C = BIT_CHECK(data,0);         /* Carry flag = data[0] */
    
    data = (carry << 7) | (data >> 1);
    
    memory->Write(zeroPageAddress,data);    
    
    (data == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (data < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::ROL_ZPG_X() { 

    char data;  
    unsigned char zeroPageAddress = memory->Read(pc++)+(unsigned char)x;
    char carry = p.C;
    
    data = memory->Read(zeroPageAddress);
    
    p.C = BIT_CHECK(data,7);         /* Carry flag = data[7] */
    
    data = (data << 1) | carry;
    
    memory->Write(zeroPageAddress,data);    
    
    (data == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (data < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::ROR_ZPG_X() { 
    
    unsigned char data;             /* Result is always positive */
    unsigned char zeroPageAddress = memory->Read(pc++)+(unsigned char)x;
    char carry = p.C;
    
    data = memory->Read(zeroPageAddress);
    
    p.C = BIT_CHECK(data,0);         /* Carry flag = data[0] */
    
    data = (carry << 7) | (data >> 1);
    
    memory->Write(zeroPageAddress,data);    
    
    (data == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (data < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::ROL_ABS() {
    
    char data;
    unsigned int address;           /* Absolute address */
    char carry = p.C;
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    
    data = memory->Read(address);     /* data <- mem[address] */
    
    p.C = BIT_CHECK(data,7);     /* Carry flag = data[7] */ 
    
    data = (data << 1) | carry;
    
    memory->Write(address,data);
    
    (data == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (data < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::ROR_ABS() {
    
    unsigned char data;             /* Result is always positive */
    unsigned int address;           /* Absolute address */
    char carry = p.C;
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    
    data = memory->Read(address);     /* data <- mem[address] */
    
    p.C = BIT_CHECK(data,0);         /* Carry flag = data[0] */
    
    data = (carry << 7) | (data >> 1);
    
    memory->Write(address,data);
    
    (data == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    p.N = 0;   /* Clear negative flag */
}

void MOS6502::ROL_ABS_X() {
    
    char data;
    unsigned int address;           /* Absolute address */
    char carry = p.C;
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    address = address + (unsigned char)x;
    
    data = memory->Read(address);     /* data <- mem[address] */
    
    p.C = BIT_CHECK(data,7);     /* Carry flag = data[7] */ 
    
    data = (data << 1) | carry;
    
    memory->Write(address,data);
    
    (data == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (data < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::ROR_ABS_X() {
    
    unsigned char data;             /* Result is always positive */
    unsigned int address;           /* Absolute address */
    char carry = p.C;
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    address = address + (unsigned char)x;
    
    data = memory->Read(address);     /* data <- mem[address] */
    
    p.C = BIT_CHECK(data,0);         /* Carry flag = data[0] */
    
    data = (carry << 7) | (data >> 1);
    
    memory->Write(address,data);
    
    (data == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    p.N = 0;   /* Clear negative flag */
}


/* The CMP instruction does not use signed arithmetic; it simply compares two numbers 
between zero and 255, treating both numbers as positive integers. If you use signed 
arithmetic, you'll need to correct for this when comparing two numbers.
*/
void MOS6502::CMP_IMM() {
          
    char data = memory->Read(pc++);
    char result = ac - data;                /* ac - # */
           
    (result == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (result < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
    
    /* Unsigned comparison */
    ((unsigned char)data <= (unsigned char)ac) ? p.C = 1 : p.C = 0;       /* Set/Reset Carry flag */  
}

void MOS6502::CMP_ZPG() {
    
    char result;
    char data = (char)memory->Read(memory->Read(pc++));
    
    result = ac - data;       
            
    (result == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (result < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
    
    /* Unsigned comparison */
    ((unsigned char)data <= (unsigned char)ac) ? p.C = 1 : p.C = 0;       /* Set/Reset Carry flag */  
}

void MOS6502::CMP_ABS() {
    
    unsigned int address;                       /* Absolute address */
    char result;
    char data;
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    
    data = (char)memory->Read(address);        
    result = ac - data;                     
                
    (result == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (result < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
    
    /* Unsigned comparison */
    ((unsigned char)data <= (unsigned char)ac) ? p.C = 1 : p.C = 0;       /* Set/Reset Carry flag */
}

void MOS6502::CMP_ZPG_X() {
        
    unsigned char zeroPageAddress = memory->Read(pc++)+(unsigned char)x;
    char data = (char)memory->Read(zeroPageAddress);
    char result;
    
    result = ac - data;       
            
    (result == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (result < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
    
    /* Unsigned comparison */
    ((unsigned char)data <= (unsigned char)ac) ? p.C = 1 : p.C = 0;       /* Set/Reset Carry flag */  
}

void MOS6502::CMP_ABS_X() {
    
    unsigned int address;                       /* Absolute address */
    char result;
    char data;
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    address = address + (unsigned char)x;
    
    data = (char)memory->Read(address);        
    result = ac - data;                     
                
    (result == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (result < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
    
    /* Unsigned comparison */
    ((unsigned char)data <= (unsigned char)ac) ? p.C = 1 : p.C = 0;       /* Set/Reset Carry flag */
}

void MOS6502::CMP_ABS_Y() {
    
    unsigned int address;                       /* Absolute address */
    char result;
    char data;
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    address = address + (unsigned char)y;
    
    data = (char)memory->Read(address);        
    result = ac - data;                     
                
    (result == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (result < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
    
    /* Unsigned comparison */
    ((unsigned char)data <= (unsigned char)ac) ? p.C = 1 : p.C = 0;       /* Set/Reset Carry flag */
}

void MOS6502::CMP_IND_X() {
    
    unsigned char indAddress;   /* Zero page address */
    unsigned int address;       /* Absolute address */
    char result;
    char data;
    
    indAddress = memory->Read(pc++) + (unsigned char)x;
    
    address = memory->Read(indAddress++);   /* Read the low byte address */
    address = (memory->Read(indAddress++)<<8) | address; /* Read the high byte address and concatenates with the low byte  */
    
    data = (char)memory->Read(address);        
    result = ac - data;                     
                
    (result == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (result < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
    
    /* Unsigned comparison */
    ((unsigned char)data <= (unsigned char)ac) ? p.C = 1 : p.C = 0;       /* Set/Reset Carry flag */
}

void MOS6502::CMP_IND_Y() {
    
    unsigned char indAddress;   /* Zero page address */
    unsigned int address;       /* Absolute address */
    char result;
    char data;
    
    indAddress = memory->Read(pc++);
    
    address = memory->Read(indAddress++);   /* Read the low byte address */
    address = (memory->Read(indAddress++)<<8) | address; /* Read the high byte address and concatenates with the low byte  */
    address = address + (unsigned char)y;
    
    data = (char)memory->Read(address);        
    result = ac - data;                     
                
    (result == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (result < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
    
    /* Unsigned comparison */
    ((unsigned char)data <= (unsigned char)ac) ? p.C = 1 : p.C = 0;       /* Set/Reset Carry flag */
}

void MOS6502::CPX_IMM() {
          
    char data = memory->Read(pc++);
    char result = x - data;                /* x - # */
           
    (result == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (result < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
    
    /* Unsigned comparison */
    ((unsigned char)data <= (unsigned char)x) ? p.C = 1 : p.C = 0;       /* Set/Reset Carry flag */  
}

void MOS6502::CPX_ABS() {
    
    unsigned int address;                       /* Absolute address */
    char result;
    char data;
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    
    data = (char)memory->Read(address);        
    result = x - data;                     
                
    (result == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (result < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
    
    /* Unsigned comparison */
    ((unsigned char)data <= (unsigned char)x) ? p.C = 1 : p.C = 0;       /* Set/Reset Carry flag */
}

void MOS6502::CPX_ZPG() {
    
    char result;
    char data = (char)memory->Read(memory->Read(pc++));
    
    result = x - data;       
            
    (result == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (result < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
    
    /* Unsigned comparison */
    ((unsigned char)data <= (unsigned char)x) ? p.C = 1 : p.C = 0;       /* Set/Reset Carry flag */  
}

void MOS6502::CPY_IMM() {
          
    char data = memory->Read(pc++);
    char result = y - data;                /* y - # */
           
    (result == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (result < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
    
    /* Unsigned comparison */
    ((unsigned char)data <= (unsigned char)y) ? p.C = 1 : p.C = 0;       /* Set/Reset Carry flag */  
}

void MOS6502::CPY_ABS() {
    
    unsigned int address;                       /* Absolute address */
    char result;
    char data;
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    
    data = (char)memory->Read(address);        
    result = y - data;                     
                
    (result == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (result < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
    
    /* Unsigned comparison */
    ((unsigned char)data <= (unsigned char)y) ? p.C = 1 : p.C = 0;       /* Set/Reset Carry flag */
}

void MOS6502::CPY_ZPG() {
    
    char result;
    char data = (char)memory->Read(memory->Read(pc++));
    
    result = y - data;       
            
    (result == 0) ? p.Z = 1 : p.Z = 0;     /* Set/Reset Zero flag */
    
    (result < 0) ? p.N = 1 : p.N = 0;      /* Set/Reset Negative flag */
    
    /* Unsigned comparison */
    ((unsigned char)data <= (unsigned char)y) ? p.C = 1 : p.C = 0;       /* Set/Reset Carry flag */  
}

void MOS6502::BIT_ZPG() { 
    
    unsigned char result;
    unsigned char data = memory->Read(memory->Read(pc++));
        
    result = ac & data; 
    
    (result == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    p.N = BIT_CHECK(data,7);                /* Set/Reset Negative flag */
    
    p.V = BIT_CHECK(data,6);                /* Set/Reset Carry flag */
}

void MOS6502::BIT_ABS() {
    
    unsigned int address;           /* Absolute address */
    unsigned char result;
    unsigned char data;
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    
    data = memory->Read(address);     /* ac <- mem[address] */
    
    result = ac & data; 
    
    (result == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    p.N = BIT_CHECK(data,7);                /* Set/Reset Negative flag */
    
    p.V = BIT_CHECK(data,6);                /* Set/Reset Carry flag */
}

void MOS6502::JMP_ABS() {
    
    unsigned int address;           /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    
    pc = address;                   
}

void MOS6502::JMP_IND() {
    
    unsigned int indAddress;            /* Indirect address */
    unsigned int address;               /* Absolute address */
    
    /* Read the indirect address */
    indAddress = memory->Read(pc++);   /* Read the low byte address */
    indAddress = (memory->Read(pc++)<<8) | indAddress;    /* Read the high byte address */
        
    /* Read the absolute address */
    address = memory->Read(indAddress++);   /* Read the low byte address */
    address = (memory->Read(indAddress++)<<8) | address;    /* Read the high byte address */
    
    pc = address;  
}

void MOS6502::JSR() {
    
    unsigned int address;           /* Absolute address */
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    
    memory->Write(0x100|s--,(pc & 0xFF00)>>8);    /* Store the return address high byte */
    memory->Write(0x100|s--,(pc & 0xFF));         /* Store the return address low byte */
    
    pc = address;                   
}

void MOS6502::RTS() {
    
    unsigned int address;           /* Absolute address */
    
    address = memory->Read(0x100|++s);   /* Read the low byte address */
    address = (memory->Read(0x100|++s)<<8) | address;    /* Read the high byte address */
    
    pc = address;
}

void MOS6502::RTI() {
    
    unsigned int address;           /* Absolute address */
    
    p.status = memory->Read(0x100|++s);   /* Retrieves processor status register */
    p.I = 0;    // Enable IRQs

    address = memory->Read(0x100|++s);   /* Read the low byte address */
    address = (memory->Read(0x100|++s)<<8) | address;    /* Read the high byte address */
    
    pc = address;
}

void MOS6502::PHP() { memory->Write(0x100|s--,p.status); }

void MOS6502::PLP() { p.status = memory->Read(0x100|++s); }

void MOS6502::PHA() { memory->Write(0x100|s--,ac); }

void MOS6502::PLA() { ac = memory->Read(0x100|++s); }

void MOS6502::TAX() {
    
    x = ac;
    
    (x < 0) ? p.N = 1 : p.N = 0;        /* Set/Reset Negative flag */
    
    (x == 0) ? p.Z = 1: p.Z = 0;        /* Set/Reset Zero flag */
}

void MOS6502::TAY() {
    
    y = ac;
    
    (y < 0) ? p.N = 1 : p.N = 0;        /* Set/Reset Negative flag */
    
    (y == 0) ? p.Z = 1: p.Z = 0;        /* Set/Reset Zero flag */
}

void MOS6502::TXA() {
    
    ac = x;
    
    (ac < 0) ? p.N = 1 : p.N = 0;        /* Set/Reset Negative flag */
    
    (ac == 0) ? p.Z = 1: p.Z = 0;        /* Set/Reset Zero flag */
}

void MOS6502::TYA() {
    
    ac = y;
    
    (ac < 0) ? p.N = 1 : p.N = 0;        /* Set/Reset Negative flag */
    
    (ac == 0) ? p.Z = 1: p.Z = 0;        /* Set/Reset Zero flag */
}

void MOS6502::DEX() {
    
    x--;
    
    (x < 0) ? p.N = 1 : p.N = 0;        /* Set/Reset Negative flag */
    
    (x == 0) ? p.Z = 1: p.Z = 0;        /* Set/Reset Zero flag */
}

void MOS6502::DEY() {
    
    y--;
    
    (y < 0) ? p.N = 1 : p.N = 0;        /* Set/Reset Negative flag */
    
    (y == 0) ? p.Z = 1: p.Z = 0;        /* Set/Reset Zero flag */
}

void MOS6502::INX() {
    
    x++;
    
    (x < 0) ? p.N = 1 : p.N = 0;        /* Set/Reset Negative flag */
    
    (x == 0) ? p.Z = 1: p.Z = 0;        /* Set/Reset Zero flag */
}

void MOS6502::INY() {
    
    y++;
    
    (y < 0) ? p.N = 1 : p.N = 0;        /* Set/Reset Negative flag */
    
    (y == 0) ? p.Z = 1: p.Z = 0;        /* Set/Reset Zero flag */
}

void MOS6502::TXS() { s = x; }

void MOS6502::TSX() {
    
    x = s;
    
    (x < 0) ? p.N = 1 : p.N = 0;        /* Set/Reset Negative flag */
    
    (x == 0) ? p.Z = 1: p.Z = 0;        /* Set/Reset Zero flag */
}

void MOS6502::ADC_IMM() { 
    
    char data = memory->Read(pc++);
    int resultSigned;                   /* Used to detect overflow */
    unsigned int resultUnsigned;        /* Used to detect carry */
    
    resultUnsigned = (unsigned char)ac + (unsigned char)data + p.C;
    resultSigned = ac + data + p.C;
    
     /* Overflow detection (2's complement arithmetic) */
    (resultSigned > 127 || resultSigned < -128) ? p.V = 1 : p.V = 0;   

    /* Carry detection (same as overflow in unsigned arithmetic) */     
    (resultUnsigned > 255) ? p.C = 1 : p.C = 0;

    ac = (char)resultSigned;
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::SBC_IMM() { 
    
    char data = ~memory->Read(pc++);    /* Complement */
    int resultSigned;                   /* Used to detect overflow */
    unsigned int resultUnsigned;        /* Used to detect carry */
    
    resultUnsigned = (unsigned char)ac + (unsigned char)data + p.C;
    resultSigned = ac + data + p.C;   
    
     /* Overflow detection (2's complement arithmetic) */
    (resultSigned > 127 || resultSigned < -128) ? p.V = 1 : p.V = 0;   

    /* Carry detection (same as overflow in unsigned arithmetic) */     
    (resultUnsigned > 255) ? p.C = 1 : p.C = 0;

    ac = (char)resultSigned;
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::ADC_ZPG_X() { 
    
    unsigned char zeroPageAddress = memory->Read(pc++)+(unsigned char)x;
    char data = memory->Read(zeroPageAddress);
    int resultSigned;                   /* Used to detect overflow */
    unsigned int resultUnsigned;        /* Used to detect carry */
    
    resultUnsigned = (unsigned char)ac + (unsigned char)data + p.C;
    resultSigned = ac + data + p.C;
   
     /* Overflow detection (2's complement arithmetic) */
    (resultSigned > 127 || resultSigned < -128) ? p.V = 1 : p.V = 0;   

    /* Carry detection (same as overflow in unsigned arithmetic) */     
    (resultUnsigned > 255) ? p.C = 1 : p.C = 0;

    ac = (char)resultSigned;
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::SBC_ZPG_X() { 
    
    unsigned char zeroPageAddress = memory->Read(pc++)+(unsigned char)x;
    char data = ~memory->Read(zeroPageAddress);     /* Complement */
    int resultSigned;                               /* Used to detect overflow */
    unsigned int resultUnsigned;                    /* Used to detect carry */
    
    resultUnsigned = (unsigned char)ac + (unsigned char)data + p.C;
    resultSigned = ac + data + p.C;
        
     /* Overflow detection (2's complement arithmetic) */
    (resultSigned > 127 || resultSigned < -128) ? p.V = 1 : p.V = 0;   

    /* Carry detection (same as overflow in unsigned arithmetic) */     
    (resultUnsigned > 255) ? p.C = 1 : p.C = 0;

    ac = (char)resultSigned;
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::ADC_ZPG() { 
    
    char data = memory->Read(memory->Read(pc++));
    int resultSigned;                   /* Used to detect overflow */
    unsigned int resultUnsigned;        /* Used to detect carry */
    
    resultUnsigned = (unsigned char)ac + (unsigned char)data + p.C;
    resultSigned = ac + data + p.C;
   
     /* Overflow detection (2's complement arithmetic) */
    (resultSigned > 127 || resultSigned < -128) ? p.V = 1 : p.V = 0;   

    /* Carry detection (same as overflow in unsigned arithmetic) */     
    (resultUnsigned > 255) ? p.C = 1 : p.C = 0;

    ac = (char)resultSigned;
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::SBC_ZPG() { 
    
    char data = ~memory->Read(memory->Read(pc++));  /* Complement */
    int resultSigned;                   /* Used to detect overflow */
    unsigned int resultUnsigned;        /* Used to detect carry */
    
    resultUnsigned = (unsigned char)ac + (unsigned char)data + p.C;
    resultSigned = ac + data + p.C;
   
     /* Overflow detection (2's complement arithmetic) */
    (resultSigned > 127 || resultSigned < -128) ? p.V = 1 : p.V = 0;   

    /* Carry detection (same as overflow in unsigned arithmetic) */     
    (resultUnsigned > 255) ? p.C = 1 : p.C = 0;

    ac = (char)resultSigned;
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::ADC_ABS() { 
    
    unsigned int address;               /* Absolute address */
    int resultSigned;                   /* Used to detect overflow */
    unsigned int resultUnsigned;        /* Used to detect carry */
    char data;
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    
    data = memory->Read(address);    
    
    resultUnsigned = (unsigned char)ac + (unsigned char)data + p.C;
    resultSigned = ac + data + p.C;
   
     /* Overflow detection (2's complement arithmetic) */
    (resultSigned > 127 || resultSigned < -128) ? p.V = 1 : p.V = 0;   

    /* Carry detection (same as overflow in unsigned arithmetic) */     
    (resultUnsigned > 255) ? p.C = 1 : p.C = 0;

    ac = (char)resultSigned;
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::SBC_ABS() { 
    
    unsigned int address;               /* Absolute address */
    int resultSigned;                   /* Used to detect overflow */
    unsigned int resultUnsigned;        /* Used to detect carry */
    char data;
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    
    data = ~memory->Read(address);      /* Complement */    
    
    resultUnsigned = (unsigned char)ac + (unsigned char)data + p.C;
    resultSigned = ac + data + p.C;
   
     /* Overflow detection (2's complement arithmetic) */
    (resultSigned > 127 || resultSigned < -128) ? p.V = 1 : p.V = 0;   

    /* Carry detection (same as overflow in unsigned arithmetic) */     
    (resultUnsigned > 255) ? p.C = 1 : p.C = 0;

    ac = (char)resultSigned;
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::ADC_ABS_X() { 
    
    unsigned int address;               /* Absolute address */
    int resultSigned;                   /* Used to detect overflow */
    unsigned int resultUnsigned;        /* Used to detect carry */
    char data;
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    address = address + (unsigned char)x;
    
    data = memory->Read(address);    
    
    resultUnsigned = (unsigned char)ac + (unsigned char)data + p.C;
    resultSigned = ac + data + p.C;
   
     /* Overflow detection (2's complement arithmetic) */
    (resultSigned > 127 || resultSigned < -128) ? p.V = 1 : p.V = 0;   

    /* Carry detection (same as overflow in unsigned arithmetic) */     
    (resultUnsigned > 255) ? p.C = 1 : p.C = 0;

    ac = (char)resultSigned;
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::SBC_ABS_X() { 
    
    unsigned int address;               /* Absolute address */
    int resultSigned;                   /* Used to detect overflow */
    unsigned int resultUnsigned;        /* Used to detect carry */
    char data;
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    address = address + (unsigned char)x;
    
    data = ~memory->Read(address);      /* Complement */    
    
    resultUnsigned = (unsigned char)ac + (unsigned char)data + p.C;
    resultSigned = ac + data + p.C;
   
     /* Overflow detection (2's complement arithmetic) */
    (resultSigned > 127 || resultSigned < -128) ? p.V = 1 : p.V = 0;   

    /* Carry detection (same as overflow in unsigned arithmetic) */     
    (resultUnsigned > 255) ? p.C = 1 : p.C = 0;

    ac = (char)resultSigned;
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::ADC_ABS_Y() { 
    
    unsigned int address;               /* Absolute address */
    int resultSigned;                   /* Used to detect overflow */
    unsigned int resultUnsigned;        /* Used to detect carry */
    char data;
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    address = address + (unsigned char)y;
    
    data = memory->Read(address);    
    
    resultUnsigned = (unsigned char)ac + (unsigned char)data + p.C;
    resultSigned = ac + data + p.C;
   
     /* Overflow detection (2's complement arithmetic) */
    (resultSigned > 127 || resultSigned < -128) ? p.V = 1 : p.V = 0;   

    /* Carry detection (same as overflow in unsigned arithmetic) */     
    (resultUnsigned > 255) ? p.C = 1 : p.C = 0;

    ac = (char)resultSigned;
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::SBC_ABS_Y() { 
    
    unsigned int address;               /* Absolute address */
    int resultSigned;                   /* Used to detect overflow */
    unsigned int resultUnsigned;        /* Used to detect carry */
    char data;
    
    address = memory->Read(pc++);   /* Read the low byte address */
    address = (memory->Read(pc++)<<8) | address;    /* Read the high byte address */
    address = address + (unsigned char)y;
    
    data = ~memory->Read(address);      /* Complement */    
    
    resultUnsigned = (unsigned char)ac + (unsigned char)data + p.C;
    resultSigned = ac + data + p.C;
   
     /* Overflow detection (2's complement arithmetic) */
    (resultSigned > 127 || resultSigned < -128) ? p.V = 1 : p.V = 0;   

    /* Carry detection (same as overflow in unsigned arithmetic) */     
    (resultUnsigned > 255) ? p.C = 1 : p.C = 0;

    ac = (char)resultSigned;
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::ADC_IND_X() { 
    
    unsigned char indAddress;           /* Zero page address */
    unsigned int address;               /* Absolute address */
    int resultSigned;                   /* Used to detect overflow */
    unsigned int resultUnsigned;        /* Used to detect carry */
    char data;
    
    indAddress = memory->Read(pc++) + (unsigned char)x;
    
    address = memory->Read(indAddress++);   /* Read the low byte address */
    address = (memory->Read(indAddress++)<<8) | address; /* Read the high byte address and concatenates with the low byte  */
    
    data = memory->Read(address);    
    
    resultUnsigned = (unsigned char)ac + (unsigned char)data + p.C;
    resultSigned = ac + data + p.C;
   
     /* Overflow detection (2's complement arithmetic) */
    (resultSigned > 127 || resultSigned < -128) ? p.V = 1 : p.V = 0;   

    /* Carry detection (same as overflow in unsigned arithmetic) */     
    (resultUnsigned > 255) ? p.C = 1 : p.C = 0;

    ac = (char)resultSigned;
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::SBC_IND_X() { 
    
    unsigned char indAddress;           /* Zero page address */
    unsigned int address;               /* Absolute address */
    int resultSigned;                   /* Used to detect overflow */
    unsigned int resultUnsigned;        /* Used to detect carry */
    char data;
    
    indAddress = memory->Read(pc++) + (unsigned char)x;
    
    address = memory->Read(indAddress++);   /* Read the low byte address */
    address = (memory->Read(indAddress++)<<8) | address; /* Read the high byte address and concatenates with the low byte  */
    
    data = ~memory->Read(address);          /* Complement */   
    
    resultUnsigned = (unsigned char)ac + (unsigned char)data + p.C;
    resultSigned = ac + data + p.C;
   
     /* Overflow detection (2's complement arithmetic) */
    (resultSigned > 127 || resultSigned < -128) ? p.V = 1 : p.V = 0;   

    /* Carry detection (same as overflow in unsigned arithmetic) */     
    (resultUnsigned > 255) ? p.C = 1 : p.C = 0;

    ac = (char)resultSigned;
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::ADC_IND_Y() { 
    
    unsigned char indAddress;           /* Zero page address */
    unsigned int address;               /* Absolute address */
    int resultSigned;                   /* Used to detect overflow */
    unsigned int resultUnsigned;        /* Used to detect carry */
    char data;
    
    indAddress = memory->Read(pc++);
    
    address = memory->Read(indAddress++);   /* Read the low byte address */
    address = (memory->Read(indAddress++)<<8) | address; /* Read the high byte address and concatenates with the low byte  */
    address = address + (unsigned char)y;
    
    data = memory->Read(address);    
    
    resultUnsigned = (unsigned char)ac + (unsigned char)data + p.C;
    resultSigned = ac + data + p.C;
   
     /* Overflow detection (2's complement arithmetic) */
    (resultSigned > 127 || resultSigned < -128) ? p.V = 1 : p.V = 0;   

    /* Carry detection (same as overflow in unsigned arithmetic) */     
    (resultUnsigned > 255) ? p.C = 1 : p.C = 0;

    ac = (char)resultSigned;
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::SBC_IND_Y() { 
    
    unsigned char indAddress;           /* Zero page address */
    unsigned int address;               /* Absolute address */
    int resultSigned;                   /* Used to detect overflow */
    unsigned int resultUnsigned;        /* Used to detect carry */
    char data;
    
    indAddress = memory->Read(pc++);
    
    address = memory->Read(indAddress++);   /* Read the low byte address */
    address = (memory->Read(indAddress++)<<8) | address; /* Read the high byte address and concatenates with the low byte  */
    address = address + (unsigned char)y;
    
    data = ~memory->Read(address);          /* Complement */    
    
    resultUnsigned = (unsigned char)ac + (unsigned char)data + p.C;
    resultSigned = ac + data + p.C;
   
     /* Overflow detection (2's complement arithmetic) */
    (resultSigned > 127 || resultSigned < -128) ? p.V = 1 : p.V = 0;   

    /* Carry detection (same as overflow in unsigned arithmetic) */     
    (resultUnsigned > 255) ? p.C = 1 : p.C = 0;

    ac = (char)resultSigned;
    
    (ac == 0) ? p.Z = 1 : p.Z = 0;      /* Set/Reset Zero flag */
    
    (ac < 0) ? p.N = 1 : p.N = 0;       /* Set/Reset Negative flag */
}

void MOS6502::BEQ() {
    
    char offset = memory->Read(pc++);

    if (p.Z == 1)
        pc = pc + offset;
}

void MOS6502::BNE() {
    
    char offset = memory->Read(pc++);

    if (p.Z == 0)
        pc = pc + offset;
}

void MOS6502::BMI() {
    
    char offset = memory->Read(pc++);

    if (p.N == 1)
        pc = pc + offset;
}

void MOS6502::BPL() {
    
    char offset = memory->Read(pc++);

    if (p.N == 0)
        pc = pc + offset;
}

void MOS6502::BCC() {
    
    char offset = memory->Read(pc++);

    if (p.C == 0)
        pc = pc + offset;
}

void MOS6502::BCS() {
    
    char offset = memory->Read(pc++);

    if (p.C == 1)
        pc = pc + offset;
}

void MOS6502::BVS() {
    
    char offset = memory->Read(pc++);

    if (p.V == 1)
        pc = pc + offset;
}

void MOS6502::BVC() {
    
    char offset = memory->Read(pc++);

    if (p.V == 0)
        pc = pc + offset;
}

void MOS6502::CLC() { p.C = 0; }

void MOS6502::SEC() { p.C = 1; }

void MOS6502::CLV() { p.V = 0; }

void MOS6502::CLD() { p.D = 0; }

void MOS6502::SED() { p.D = 1; }

void MOS6502::SEI() { p.I = 1; }

void MOS6502::CLI() { p.I = 0; }

void MOS6502::NOP() {}

