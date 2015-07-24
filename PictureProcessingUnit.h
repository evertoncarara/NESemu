#ifndef PICTUREPROCESSINGUNIT_H
#define PICTUREPROCESSINGUNIT_H

#include <stdio.h>
#include <SDL.h>

#include <iostream>
#include <fstream>

#define PPUCTRL     0   // PPU control register (Access: write)
#define PPUMASK     1   // PPU mask register (Access: write)
#define PPUSTATUS   2   // PPU status register (Access: read)
#define OAMADDR     3   // OAM (Object Attribute Memory) address port (Access: write)
#define OAMDATA     4   // OAM data port (Access: read, write)
#define PPUSCROLL   5   // PPU scrolling position register (Access: write twice)
#define PPUADDR     6   // PPU address register (Access: write twice)
#define PPUDATA     7   // PPU data port (Access: read, write)

#define BIT_CHECK(reg,bit)  (((reg & (1<<bit)) == 0) ? 0 : 1)      /* Return the bit value (0 or 1) */

/* NES Resolution (256x 240 pixels) */
#define NES_WIDTH    256
#define NES_HEIGHT   240

#define SCREEN_WIDTH    256*3
#define SCREEN_HEIGHT   240*3

/* NES tile (8x8 pixels) */
#define TILE_SIZE   16  // 16 Bytes 

/* 32x30 tiles (name tables) */
#define SCREEN_COLUMN_TILES 32
#define SCREEN_ROW_TILES    30


using namespace std;

struct ColorRGB {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

struct iNES {
    char NES[4];                // Constant $4E $45 $53 $1A ("NES" followed by MS-DOS end-of-file)
    unsigned char PRG_ROM;      // Size of PRG ROM in 16 KB units
    unsigned char CHR_ROM;      // Size of CHR ROM in 8 KB units (Value 0 means the board uses CHR RAM)
    char flag6;
    char flag7;
    char flag8;                 // Size of PRG RAM in 8 KB units (Value 0 infers 8 KB for compatibility; see PRG RAM circuit)
    char flag9;
    char flag10;
    char zero[5];               // Zero filled
};

union PPUControl {
    struct {
        unsigned char N : 2;    // Name Table number
        unsigned char I : 1;    // VRAM address increment per CPU read/write of PPUDATA (0: add 1, going across; 1: add 32, going down)
        unsigned char S : 1;    // Sprite pattern table number (0: $0000; 1: $1000; ignored in 8x16 mode)
        unsigned char B : 1;    // Background pattern table number (0: $0000; 1: $1000)
        unsigned char H : 1;    // Sprite size (0: 8x8; 1: 8x16)
        unsigned char P : 1;    // PPU master/slave select (0: read backdrop from EXT pins; 1: output color on EXT pins)
        unsigned char V : 1;    // Generate an NMI at the start of the vertical blanking interval (0: off; 1: on)
    };
    unsigned char status;           /* Access to all bit fields at the same time */
};

union PPUStatus {
    struct {
        unsigned char bits : 5;     // Least significant bits previously written into a PPU register
        unsigned char O : 1;        // Sprite overflow (more than eight sprites appear on a scanline)
        unsigned char S : 1;        // Sprite 0 Hit
        unsigned char V : 1;        // Vertical blank has started (0: not in vblank; 1: in vblank)
    };
    unsigned char status;           /* Access to all bit fields at the same time */
};

class PictureProcessingUnit {
    public:
        PictureProcessingUnit();
        ~PictureProcessingUnit();
        
        void Init(unsigned char *ppuMemory, unsigned char *cpuMemory);
        void Reset();

        /* Registers access */
        // IMPLEMENTAR PROTEÇÃO CONTRA ESCRITA EM REGS SÓ DE LEITURA
        unsigned char ReadRegister(unsigned char n);
        void WriteRegister(unsigned char n, unsigned char data);
        void StartDMA(unsigned char addressHighByte);
        
        /* Sprite memory access */
        inline void WriteSpriteMemory(unsigned char address, unsigned char data) { spriteMemory[address] = data; }

        
        
        
        /*** TEST PURPOSES ***/
        int LoadPatternTables(char *fileName);
        int ShowPatternTables();
        
        void LoadNameTable(unsigned char number);
        int ShowNameTable(unsigned char number);
        
        void LoadBackgroundPalette();

        void LoadAttributeTable(unsigned char number);
        void spriteDump(int startAddr, int bytes);
        /********************/
                
    private:      
        void PutPixel(SDL_Surface *surface, const int x, const int y, const Uint8 r, const Uint8 g, const Uint8 b);
        void PlotTile(SDL_Surface *surface, const int x, const int y, const unsigned char tile[], const unsigned char paletteIndex);

        unsigned char registers[8];
        union PPUStatus *ppuStatus;
        union PPUControl *ppuControl;

        unsigned char *memory;              // Main PPU memory
        unsigned char *cpuMemory;            // Only read access in order to execute DMA
        
        /* Memory sections */
        unsigned char *patternTable[2];     
        unsigned char *nameTable[4];
        unsigned char *attributeTable[4];
        unsigned char *backgroundPalette[4];  
        unsigned char *spritePalette[4];

        unsigned char spriteMemory[256];    // SPR-RAM (OAM: Object Attribute Memory) 

        unsigned int vramAddress;
        char vramAddressByteAccess;                // 

         

        // NES palette
        struct ColorRGB nesPalette[64]= {
            {124,124,124}, {0,0,252},     {0,0,188},     {68,40,188},
            {148,0,132},   {168,0,32},    {168,16,0},    {136,20,0},
            {80,48,0},     {0,120,0},     {0,104,0},     {0,88,0},
            {0,64,88},     {0,0,0},       {0,0,0},       {0,0,0},
            {188,188,188}, {0,120,248},   {0,88,248},    {104,68,252},
            {216,0,204},   {228,0,88},    {248,56,0},    {228,92,16},
            {172,124,0},   {0,184,0},     {0,168,0},     {0,168,68},
            {0,136,136},   {0,0,0},       {0,0,0},       {0,0,0},
            {248,248,248}, {60,188,252},  {104,136,252}, {152,120,248},
            {248,120,248}, {248,88,152},  {248,120,88},  {252,160,68},
            {248,184,0},   {184,248,24},  {88,216,84},   {88,248,152},
            {0,232,216},   {120,120,120}, {0,0,0},       {0,0,0},
            {252,252,252}, {164,228,252}, {184,184,248}, {216,184,248},
            {248,184,248}, {248,164,192}, {240,208,176}, {252,224,168},
            {248,216,120}, {216,248,120}, {184,248,184}, {184,248,216},
            {0,252,252},   {248,216,248}, {0,0,0},       {0,0,0}
        };
};
#endif

