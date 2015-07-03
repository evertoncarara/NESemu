#include <iostream>
#include "NESMemorySystem.h"
#include "MOS6502.h"
#include "PictureProcessingUnit.h"

using namespace std;

int main() {   
    
    MOS6502 cpu;
    PictureProcessingUnit ppu;
    
    NESMemorySystem memorySystem(65536);    // 64KB
    memorySystem.Load((char *)"AllSuite.65p");
    memorySystem.SetPictureProcessingUnit(&ppu);  /* PPU registers access controlled by the memory system */
      
    cpu.Init(&memorySystem);
    
    
    // initialize
    for(int i=0; i<32; i++) 
        cpu.ExecuteInstruction();
    
    memorySystem.Print(0x200,15);
    
    // test00: expected result: $022A = 0x55
    // N = 1; C = 1
    for(int i=0; i<52; i++)
        cpu.ExecuteInstruction();
    
    memorySystem.Print(0x22A,1);
    cpu.PrintRegs();
    
    // test01: expected result: $A9 = 0xAA
    // N = 1; C = 1
    for(int i=0; i<112; i++)
        cpu.ExecuteInstruction();
    
    memorySystem.Print(0xA9,1);
    cpu.PrintRegs();
    
    // test02: expected result: $71 = 0xFF
    // Z = 1; C = 1
    for(int i=0; i<37; i++)
        cpu.ExecuteInstruction();
    
    memorySystem.Print(0x71,1);
    cpu.PrintRegs();
    
    // test03: expected result: $1DD = 0x6E
    // Z = ; C = 
    for(int i=0; i<61; i++)
        cpu.ExecuteInstruction();
    
    memorySystem.Print(0x1DD,1);
    cpu.PrintRegs();
    
    // test04: expected result: $40 = 0x42
    // Z = ; C = 
    for(int i=0; i<19; i++)
        cpu.ExecuteInstruction();
    
    memorySystem.Print(0x40,1);
    cpu.PrintRegs();
    
    // test05: expected result: $40 = 0x33
    // Z = ; C = 
    for(int i=0; i<21; i++)
        cpu.ExecuteInstruction();
    
    memorySystem.Print(0x40,1);
    cpu.PrintRegs();
    
    // test06: expected result: $30 = 9D
    // Z = ; C = 
    for(int i=0; i<72; i++)
        cpu.ExecuteInstruction();
    
    memorySystem.Print(0x30,1);
    cpu.PrintRegs();
    
    // test07: expected result: $15 = 0x7F
    // Z = ; C = 
    for(int i=0; i<57; i++)
        cpu.ExecuteInstruction();
    
    memorySystem.Print(0x15,1);
    cpu.PrintRegs();
    
    // test08: expected result: $42 = 0xA5
    // Z = ; C = 
    for(int i=0; i<32; i++)
        cpu.ExecuteInstruction();
    
    memorySystem.Print(0x42,1);
    cpu.PrintRegs();
    
    // test09: expected result: $80 = 0x1F
    // Z = ; C = 
    for(int i=0; i<42; i++)
        cpu.ExecuteInstruction();
    
    memorySystem.Print(0x80,1);
    cpu.PrintRegs();
    
    // test10: expected result: $30 = 0xCE
    // Z = ; C = 
    for(int i=0; i<17; i++)
        cpu.ExecuteInstruction();
    
    memorySystem.Print(0x30,1);
    cpu.PrintRegs();
    
    // test11: expected result: $30 = 0x29
    // Z = ; C = 
    for(int i=0; i<15; i++)
        cpu.ExecuteInstruction();
    
    memorySystem.Print(0x30,1);
    cpu.PrintRegs();
    
    // test12: expected result: $33 = 0x42
    // Z = ; C = 
    for(int i=0; i<18; i++)
        cpu.ExecuteInstruction();
    
    memorySystem.Print(0x33,1);
    cpu.PrintRegs();
    
    // test13: expected result: $21 = 0x6C (simulator) $21 = 0x0C (ours)
    // Z = ; C = 
    for(int i=0; i<15; i++)
        cpu.ExecuteInstruction();
    
    memorySystem.Print(0x21,1);
    cpu.PrintRegs();
    
    // test14: expected result: $60 = 0x42
    // Z = ; C = 
    for(int i=0; i<6; i++)
        cpu.ExecuteInstruction();
    
    memorySystem.Print(0x60,1);
    cpu.PrintRegs();
    
    // suiteafinal: expected result: $0210 = FF
    // Z = ; C = 
    for(int i=0; i<666; i++)
        cpu.ExecuteInstruction();
    
    memorySystem.Print(0x210,1);
    cpu.PrintRegs();
    
    
}
