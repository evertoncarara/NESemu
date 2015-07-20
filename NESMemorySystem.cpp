#include "NESMemorySystem.h"
#include <stdio.h>

#define PPU     0x2000
#define APU     0x4000
#define PRG_ROM 0x8000


/*** 
*   NES Memory map
*       RAM             : 0000-1FFF (0000-07FF: 2KB RAM. Mirrors: 0800-0FFF, 1000-17FF, 1800-1FFF)
*       IO (PPU)        : 2000-3FFF (2000-2007: PPU regs. Mirrors: 2008-3FFF repeats every 8 bytes)
*       IO (APU)        : 4000-401F
*
*       Cartridge space
*       Expansion ROM   : 4020-5FFF
*       SRAM            : 6000-7FFF
*       PRG-ROM         : 8000-FFFF
***/

NESMemorySystem::NESMemorySystem() {  

    cpuMemory = new unsigned char[65536];
    ppuMemory = new unsigned char[65536];
}

NESMemorySystem::~NESMemorySystem() { 
    delete [] cpuMemory; 
    delete [] ppuMemory;
}

unsigned char NESMemorySystem::Read(unsigned int address) {

    unsigned char data;

    //if (address >= PRG_ROM) {           /* Game code */
    //    data = cpuMemory[address];
    //}
    //else if (address < PPU ) {          /* CPU RAM */
    //    address = address & 0x7FF;      /* Mirroring */
        data = cpuMemory[address];
    //} 
    //else if (address >= PPU && address < APU) {     /* PPU registers */
    //    address = (address & 0x2007) & 7;      /* Mirroring and select the register number (0-7) */
    //    data = ppu->ReadRegister(address);        
    //}   
    
    return data;
}

void NESMemorySystem::Write(unsigned int address, unsigned char data) {
    
    // AllSuite.65p
    cpuMemory[address] = data;
    
    //if (address < PPU ) {               /* CPU RAM */
    //    address = address & 0x7FF;      /* Mirroring */
    //    cpuMemory[address] = data;
    //} 
    //else if (address >= PPU && address < APU) {     /* PPU registers */
    //    address = (address & 0x2007) & 7;      /* Mirroring and select the register number (0-7) */
    //    ppu->WriteRegister(address,data);        
    //}
}

void NESMemorySystem::SetPictureProcessingUnit(PictureProcessingUnit *ppu) { this->ppu = ppu; }

 unsigned char *NESMemorySystem::GetPPUMemory() { return ppuMemory; }
