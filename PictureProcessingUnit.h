#ifndef PICTUREPROCESSINGUNIT_H
#define PICTUREPROCESSINGUNIT_H

#include <stdio.h>
#include <SDL.h>

#include <iostream>
#include <fstream>

#define PPUCTRL     0   // PPU control register (Access: write)
#define PPUMASK     1   // PPU mask register (Access: write)
#define PPUSTATUS   2   // PPU status register (Access: read)
#define OAMADDR     3   // OAM (Object Attribute Memor) address port (Access: write)
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

class PictureProcessingUnit {
    public:
        PictureProcessingUnit();
        ~PictureProcessingUnit();
        
        void Init(unsigned char *ppuMemory);
        void Reset();

        /* Registers access */
        // IMPLEMENTAR PROTEÇÃO CONTRA ESCRITA EM REGS SÓ DE LEITURA
        inline unsigned char ReadRegister(unsigned char n) { return registers[n]; }
        inline void WriteRegister(unsigned char n, unsigned char data) { registers[n] = data; }
        
        /* Sprite memory access */
        inline void WriteSpriteMemory(unsigned char address, unsigned char data) { spriteMemory[address] = data; }

        
        
        
        /*** TEST PURPOSES ***/
        int LoadPatternTables(char *fileName);
        int ShowPatternTables();
        
        void LoadNameTable(unsigned char number);
        int ShowNameTable(unsigned char number);
        
        void LoadBackgroundPalette();

        void LoadAttributeTable(unsigned char number);
        /********************/
                
    private:      
        void PutPixel(SDL_Surface *surface, const int x, const int y, const Uint8 r, const Uint8 g, const Uint8 b);
        void PlotTile(SDL_Surface *surface, const int x, const int y, const unsigned char tile[], const unsigned char paletteIndex);

        unsigned char registers[8];
        unsigned char OAMDMA;               // 0x4014: OAM DMA register (high byte) Access: write
        unsigned char *memory;              // Main PPU memory
        
        /* Memory sections */
        unsigned char *patternTable[2];     
        unsigned char *nameTable[4];
        unsigned char *attributeTable[4];
        unsigned char *backgroundPalette[4];  
        unsigned char *spritePalette[4];

        unsigned char spriteMemory[256];    // SPR-RAM (OAM: Object Attribute Memory)   
         

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

