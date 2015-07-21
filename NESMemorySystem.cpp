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

NESMemorySystem::NESMemorySystem(unsigned int size) {  

    array = new unsigned char[size];
        
    array[0] = 0xA9;    // lda #
    array[1] = 0x12;
    array[2] = 0xA0;    // ldy #
    array[3] = 0x13;
    array[4] = 0x8D;    // sta $2ABC
    array[5] = 0xBC;    // low address byte
    array[6] = 0x2A;    // high address byte
    array[7] = 0xAD;    // lda $3000
    array[8] = 0xBC;    // low address byte
    array[9] = 0x2A;    // high address byte
}

NESMemorySystem::~NESMemorySystem(){ delete array;}

unsigned char NESMemorySystem::Read(unsigned int address) {

    unsigned char data;

    //if (address >= PRG_ROM) {           /* Game code */
    //    data = array[address];
    //}
    //else if (address < PPU ) {          /* CPU RAM */
    //    address = address & 0x7FF;      /* Mirroring */
        data = array[address];
    //} 
    //else if (address >= PPU && address < APU) {     /* PPU registers */
    //    address = (address & 0x2007) & 0x000F;      /* Mirroring and select the register number (0-7) */
    //    data = ppu->ReadRegister(address);        
    //}   
    
    return data;
}

void NESMemorySystem::Write(unsigned int address, unsigned char data) {
    
    // AllSuite.65p
    array[address] = data;
    
    //if (address < PPU ) {               /* CPU RAM */
    //    address = address & 0x7FF;      /* Mirroring */
    //    array[address] = data;
    //} 
    //else if (address >= PPU && address < APU) {     /* PPU registers */
    //    address = (address & 0x2007) & 0x000F;      /* Mirroring and select the register number (0-7) */
    //    ppu->WriteRegister(address,data);        
    //}
}

void NESMemorySystem::Load(char *imageFile) {
    
    FILE *inFile;
    
    if ( !(inFile = fopen(imageFile,"rb")) ) {
        printf("Error opening file '%s'.\n",imageFile);
        return;
    }
    fread(array, 1, 65536, inFile); // 64KB
    fclose(inFile);
}
