#include "NESMemorySystem.h"
#include <string.h>     // memcpy()
#include <cstdlib>     // abort()

#include <stdio.h>

#define PPU     0x2000
#define APU     0x4000
#define PRG_ROM_LOW 0x8000
#define PRG_ROM_UP  0xC000


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

    cpuMemory = new unsigned char[64*1024]; // 64KB
    ppuMemory = new unsigned char[64*1024]; // 64 KB
}

NESMemorySystem::~NESMemorySystem() { 
    delete [] cpuMemory; 
    delete [] ppuMemory;
}

void NESMemorySystem::Init(char *fileName) {

    ifstream romFile(fileName, ios::binary);
    
    if (romFile.fail()) {
        cout << "Error opening file " << fileName << "." << endl;
        abort();
    }
    
    romFile.seekg(0, ios::end);    // Seek to end of file
    unsigned int fileSize = romFile.tellg();    // Get current file pointer
    romFile.seekg(0, ios::beg);    // Seek back to beginning of file
    
    /* Allocate memory to read the ROM file */
    char *buffer = new char[fileSize];
    
    /* Read ROM file */
    romFile.read(buffer, fileSize);
    romFile.close();
    
    /* Access the iNES header */
    struct iNES *iNES_header = (struct iNES *)buffer;
    
    
    cout << "File: " << fileName << endl;
    cout << "Size: " << fileSize << " Bytes" << endl;
    cout << "PRG_ROM: " << static_cast<int>(iNES_header->PRG_ROM) << " * 16KB" << endl;
    cout << "CHR_ROM: " << static_cast<int>(iNES_header->CHR_ROM) << " * 8KB (0 means the board uses CHR-RAM)" << endl;
    
    
    cout << "Loading game code..." << endl;

    /* Load lower bank (0x8000) */
    memcpy(&cpuMemory[PRG_ROM_LOW],buffer+sizeof(struct iNES),16*1024);   // Copy 16KB

    /* Load upper bank (0xC000) */
    if (iNES_header->PRG_ROM == 2)
        memcpy(&cpuMemory[PRG_ROM_UP],buffer+sizeof(struct iNES)+16*1024,16*1024);   // Copy 16KB
    else    // Mirror the lower bank
        memcpy(&cpuMemory[PRG_ROM_UP],buffer+sizeof(struct iNES),16*1024);   // Copy 16KB

    cout << "Game code loaded!" << endl;

    cout << "Loading pattern tables..." << endl;
    unsigned int chr_offset;
    if (iNES_header->CHR_ROM > 0) {
        chr_offset = sizeof(struct iNES) + (iNES_header->PRG_ROM*16*1024);
        cout << "CHR_ROM starts at: " << chr_offset << endl;
    
        /* Copy pattern tables from rom file to the ppu memory */
        memcpy(ppuMemory,buffer+chr_offset,(8*1024));

        cout << "Pattern tables loaded!" << endl;
    }  

    delete [] buffer;
}

void NESMemorySystem::cpuDump(int startAddr, int bytes) {

    printf("CPU Memory dump...%X\n",startAddr);

    for(int i=startAddr,j=0; i<(startAddr+bytes); i++,j++) {
        if ( (i%16) == 0) {
            printf("\n");
            printf("[%04X]: ",(startAddr+j));
        }

        printf("%02X ",cpuMemory[startAddr+j]);   
    }
    printf("\n");
}

void NESMemorySystem::ppuDump(int startAddr, int bytes) {

    printf("PPU Memory dump...\n");

    for(int i=startAddr,j=0; i<(startAddr+bytes); i++,j++) {
        if ( (i%16) == 0) {
            printf("\n");
            printf("[%04X]: ",startAddr+j);
        }

        printf("%02X ",ppuMemory[startAddr+j]);     
    }
    printf("\n");
}

unsigned char NESMemorySystem::Read(unsigned int address) {

    unsigned char data;

    if (address >= PRG_ROM_LOW) {           /* Game code */
        data = cpuMemory[address];
    }
    else if (address < PPU ) {          /* CPU RAM */
        address = address & 0x7FF;      /* Mirroring */
        data = cpuMemory[address];
    } 
    else if (address >= PPU && address < APU) {     /* PPU registers */
        address = (address & 0x2007) & 7;      /* Mirroring and select the register number (0-7) */
        data = ppu->ReadRegister(address);        
    }
    else {
        printf("WARNING: CPU reading from unimplemented address: %X\n",address);
        abort();
    }  
    
    return data;
}

void NESMemorySystem::Write(unsigned int address, unsigned char data) {
    
    cpuMemory[address] = data;
    
    if (address < PPU ) {               /* CPU RAM */
        address = address & 0x7FF;      /* Mirroring */
        cpuMemory[address] = data;
    } 
    else if (address >= PPU && address < APU) {     /* PPU registers */
        address = (address & 0x2007) & 7;      /* Mirroring and select the register number (0-7) */
        ppu->WriteRegister(address,data);        
    }
    else {
        printf("WARNING: CPU writing from unimplemented address: %X\n",address);
        //abort();
    } 
 }

void NESMemorySystem::SetPictureProcessingUnit(PictureProcessingUnit *ppu) { this->ppu = ppu; }

 unsigned char *NESMemorySystem::GetPPUMemory() { return ppuMemory; }
