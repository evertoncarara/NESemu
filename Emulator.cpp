#include <iostream>
#include "NESMemorySystem.h"
#include "MOS6502.h"
#include "PictureProcessingUnit.h"

using namespace std;

int main(int argc, char *argv[]) {   
    
    if (argc < 2) {

        cout << "Syntax: " << argv[0] << " <rom_file>" << endl;
        return -1;
    }


    MOS6502 cpu;
    PictureProcessingUnit ppu;
    NESMemorySystem memorySystem;

    ppu.Init(memorySystem.GetPPUMemory(),memorySystem.GetCPUMemory());
    ppu.Reset();

    memorySystem.Load(argv[1]);                     // Load PRG-ROM and CHR-ROM (if any)
    memorySystem.SetPictureProcessingUnit(&ppu);    /* PPU registers access controlled by the memory system */

    cpu.Init(&memorySystem);
    cpu.Reset();
    
    /*** Execute sprites.nes ***/

    ppu.WriteRegister(2,0x80);    // Set in vertical blank

    for(int i=0; i<12; i++)         // ; First wait for vblank to make sure PPU is ready
        cpu.ExecuteInstruction();

    
    for(int i=0; i<256*12; i++)
        cpu.ExecuteInstruction();   // clrmem

    
    ppu.WriteRegister(2,0x80);    // Set in vertical blank


    cpu.ExecuteInstruction();
    cpu.ExecuteInstruction();   //Second wait for vblank, PPU is ready after this

    
    // ************** NEW CODE ****************
    for(int i=0; i<6; i++)
        cpu.ExecuteInstruction();   // LoadPalettes

    for(int i=0; i<32*5; i++)
        cpu.ExecuteInstruction();   // LoadPalettesLoop

    memorySystem.ppuDump(0x3F00,0x20); 

    for(int i=0; i<10; i++)
        cpu.ExecuteInstruction();   // sprite code and ppu control

    memorySystem.cpuDump(0x200,4);

    for(int i=0; i<30; i++)
        cpu.ExecuteInstruction();   // jump back to Forever, infinite loop

    cpu.NMI();
    

    for(int i=0; i<5; i++)
        cpu.ExecuteInstruction();   // NMI handler

    ppu.spriteDump(0,256);

    for(int i=0; i<30; i++)
        cpu.ExecuteInstruction();   // jump back to Forever, infinite loop

    cpu.PrintRegs();

    
    

    
          
    
    
    
    
   
    //if (ppu.LoadPatternTables(argv[1]) == 0) {
        //ppu.LoadBackgroundPalette();
        //ppu.ShowPatternTables();
      //  ppu.LoadNameTable(0);
      //  ppu.LoadAttributeTable(0);
        //ppu.ShowNameTable(0);
    //}



    return 0;
    
    
}
