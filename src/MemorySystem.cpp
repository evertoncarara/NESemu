#include "MemorySystem.h"
#include <string.h>     // memcpy()
#include <cstdlib>      // abort()
#include <cassert>
#include <iostream>
#include <fstream>
#include "PictureProcessingUnit.h"
#include "MOS6502.h"

#include <stdio.h>

#define PPU         0x2000
#define APU         0x4000
#define OAMDMA      0x4014  // OAM DMA register (high byte) Access: write
#define JOYSTICK1   0x4016  // Player 1 joystick address
#define PRG_ROM_LOW 0x8000  // PRG-ROM lower bank address
#define PRG_ROM_UP  0xC000  // PRG-ROM upper bank address
#define OAMADDR     3       // OAM (Object Attribute Memory) address port (Access: write)



using namespace std;

/*** 
*   NES Memory map
*       RAM             : 0000-1FFF (0000-07FF: 2KB RAM. Mirrors: 0800-0FFF, 1000-17FF, 1800-1FFF)
*       PPU             : 2000-3FFF (2000-2007: PPU regs. Mirrors: 2008-3FFF repeats every 8 bytes)
*       APU             : 4000-401F
*
*       Cartridge space
*       Expansion ROM   : 4020-5FFF
*       SRAM            : 6000-7FFF
*       PRG-ROM         : 8000-FFFF
***/

MemorySystem::MemorySystem() {  

    cpuMemory = new unsigned char[64*1024]; // 64KB
    ppuMemory = new unsigned char[64*1024]; // 64 KB
    ppu = NULL;
}

MemorySystem::~MemorySystem() { 
    delete [] cpuMemory; 
    delete [] ppuMemory;
}

/*** 
 *  Load game code into the CPU memory
 *  Load pattern table,if any, into the PPU memory 
 ***/
void MemorySystem::Load(char *fileName) {

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
    unsigned char mapper;
    mapper =  (iNES_header->flag7 & 0xF0) | ((iNES_header->flag6 & 0xF0)>>4);
    cout << "Mapper: " << static_cast<int>(mapper) << endl;
    
    
    /* Game code loading */
    cout << "Loading game code..." << endl;

    /* Load lower bank (0x8000) */
    memcpy(&cpuMemory[PRG_ROM_LOW],buffer+sizeof(struct iNES),16*1024);   // Copy 16KB

    /* Load upper bank (0xC000) */
    if (iNES_header->PRG_ROM == 2)
        memcpy(&cpuMemory[PRG_ROM_UP],buffer+sizeof(struct iNES)+16*1024,16*1024);   // Copy 16KB
    else    // Mirror the lower bank
        memcpy(&cpuMemory[PRG_ROM_UP],buffer+sizeof(struct iNES),16*1024);   // Copy 16KB

    cout << "Game code loaded!" << endl;

    
    /* Pattern tables loading */
    cout << "Loading pattern tables..." << endl;
    unsigned int chr_offset;
    if (iNES_header->CHR_ROM > 0) {
        chr_offset = sizeof(struct iNES) + (iNES_header->PRG_ROM*16*1024);
        cout << "CHR_ROM starts at: " << chr_offset << endl;
    
        /* Copy pattern tables from rom file (CHR-RAM) to the ppu memory */
        memcpy(ppuMemory,buffer+chr_offset,(8*1024));   // Copy 8KB

        cout << "Pattern tables loaded!" << endl;
    }  

    delete [] buffer;
}

void MemorySystem::Init(PictureProcessingUnit *ppu) {  

    this->ppu = ppu;
}

unsigned char MemorySystem::Read(unsigned int address) {

    unsigned char data;

    if (address >= PRG_ROM_LOW) {           /* Game code */
        data = cpuMemory[address];
    }
    else if (address < PPU ) {          /* CPU RAM */
        address = address & 0x7FF;      /* Mirroring */
        data = cpuMemory[address];
    } 
    else if (address >= PPU && address < APU) {     /* PPU registers */
        assert(ppu != NULL && "PPU not set");
        address = (address & 0x2007) & 7;      /* Mirroring and select the register number (0-7) */
        data = ppu->ReadRegister(address);        
    }
    else if (address == JOYSTICK1) {
        data = (latchJoystick1>>joystick1Shift) & 0x01;
        joystick1Shift++;
    }
    else {
        //printf("WARNING: CPU reading from unimplemented address: %X\n",address);
        //abort();
        data = 0;
    }  
    
    return data;
}

void MemorySystem::Write(unsigned int address, unsigned char data) {
        
    if (address < PPU ) {               /* CPU RAM */
        address = address & 0x7FF;      /* Mirroring */
        cpuMemory[address] = data;
    } 
    else if (address >= PPU && address < APU) {     /* PPU registers */
        assert(ppu != NULL && "PPU not set");
        address = (address & 0x2007) & 7;      /* Mirroring and select the register number (0-7) */
        ppu->WriteRegister(address,data);        
    }
    else if (address == OAMDMA)
        StartDMA(data);
    
    /***
     * The joysticks are accessed through memory port addresses $4016 and $4017.  First the game code write the value $01 
     * then the value $00 to port $4016.  This tells the joysticks to latch the current button positions. Then the game code 
     * read from $4016 for first player or $4017 for second player. The buttons are sent one at a time, in bit 0.  
     * If bit 0 is 1, the button is pressed. If bit 0 is 0, the button is not pressed.  
     **/
    else if (address == JOYSTICK1 && data == 0) {
        latchJoystick1 = joystick1State;
        joystick1Shift = 0;     // Set shift to read the first button
    }
    
    else {
        //printf("WARNING: CPU writing from unimplemented address: [%X]: %X\n",address,data);
        //abort();
    } 
 }

void MemorySystem::StartDMA(unsigned char addressHighByte) {

    unsigned int cpuMemoryAddress = addressHighByte<<8 | ppu->ReadRegister(OAMADDR);  // Source: CPU RAM address

    ppu->FillSpriteMemory(&cpuMemory[cpuMemoryAddress]);
}

void MemorySystem::cpuDump(int startAddr, int bytes) {

    printf("CPU Memory dump...\n");

    for(int i=startAddr,j=0; i<(startAddr+bytes); i++,j++) {
        if ( (i%16) == 0) {
            printf("\n");
            printf("[%04X]: ",(startAddr+j));
        }

        printf("%02X ",cpuMemory[startAddr+j]);   
    }
    printf("\n");
}

void MemorySystem::ppuDump(int startAddr, int bytes) {

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

