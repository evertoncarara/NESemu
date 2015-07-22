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

    ppu.Init(memorySystem.GetPPUMemory());
    ppu.Reset();

    memorySystem.SetPictureProcessingUnit(&ppu);    /* PPU registers access controlled by the memory system */
    memorySystem.Init(argv[1]);

    cpu.Init(&memorySystem);
    cpu.Reset();
    
    for(int i=0; i<10; i++)
        cpu.ExecuteInstruction();

    cpu.PrintRegs();

    
    //memorySystem.ppuDump(0x0000,40);

    
          
    
    
    
    
   
    //if (ppu.LoadPatternTables(argv[1]) == 0) {
        ppu.LoadBackgroundPalette();
        ppu.ShowPatternTables();
      //  ppu.LoadNameTable(0);
      //  ppu.LoadAttributeTable(0);
        //ppu.ShowNameTable(0);
    //}



    return 0;
    
    
}
