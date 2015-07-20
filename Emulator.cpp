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
    
    NESMemorySystem memorySystem;            // 64KB
    memorySystem.SetPictureProcessingUnit(&ppu);    /* PPU registers access controlled by the memory system */      
    
    cpu.Init(&memorySystem);
    
    ppu.Init(memorySystem.GetPPUMemory());
   
    if (ppu.LoadPatternTables(argv[1]) == 0) {
        ppu.LoadBackgroundPalette();
        //ppu.ShowPatternTables();
        ppu.LoadNameTable(0);
        ppu.LoadAttributeTable(0);
        ppu.ShowNameTable(0);
    }



    return 0;
    
    
}
