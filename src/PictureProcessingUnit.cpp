#include "PictureProcessingUnit.h"
#include <cassert>
#include <string.h>	// memcpy()
#include <stdio.h>

#define BIT_SET(reg,bit)    (reg |= (1<<bit))     /* Set the indicated bit */
#define BIT_CLR(reg,bit)    (reg &= ~(1<<bit))    /* Clear the indicated bit */
#define BIT_CHECK(reg,bit)  (((reg & (1<<bit)) == 0) ? 0 : 1)      /* Return the bit value (0 or 1) */
#define BIT_ASSIGN(reg,bit,value)    ((value) ? BIT_SET(reg,bit) : BIT_CLR(reg,bit))     /* Set the indicated bit to value */

#include <iostream> // cout
using namespace std; // cout

PictureProcessingUnit::PictureProcessingUnit() { 

    memory = NULL;
}

PictureProcessingUnit::~PictureProcessingUnit(){

    //Free resources and close SDL
    //Destroy window
    SDL_FreeSurface(mainWindowSurface);
    SDL_FreeSurface(nesWindowSurface);
    SDL_DestroyWindow(mainWindow);
    
    //Quit SDL subsystems
    SDL_Quit();
}

const struct ColorRGB PictureProcessingUnit::nesPalette[64]= {
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

void PictureProcessingUnit::Reset() {

    for(int i=0; i<8; i++)
        registers[i] = 0;

    vramAddressByteAccess = false;
    ppuControl->value = 0;
    ppuStatus->value = 0;
    ppuMask->value = 0;
}

int PictureProcessingUnit::Init(MemorySystem *memorySystem) { 

	//assert(ppuMemory != NULL && "PPU memory not initialized");

    memory = memorySystem->GetPPUMemory(); 

	/*** Set the memory map ***/
    patternTable[0] = &memory[0x0000];
	patternTable[1] = &memory[0x1000];
	
    nameTable[0] = &memory[0x2000];
	attributeTable[0] = &memory[0x23C0];
    
    nameTable[1] = &memory[0x2400];
	attributeTable[1] = &memory[0x27C0];
    
    nameTable[2] = &memory[0x2800];
    attributeTable[2] = &memory[0x2BC0];
	
    nameTable[3] = &memory[0x2C00];
    attributeTable[3] = &memory[0x2FC0];

    backgroundPalette[0] =  &memory[0x3F00];
    backgroundPalette[1] =  &memory[0x3F04];
    backgroundPalette[2] =  &memory[0x3F08];
    backgroundPalette[3] =  &memory[0x3F0C];

    spritePalette[0] = &memory[0x3F10];
    spritePalette[1] = &memory[0x3F14];
    spritePalette[2] = &memory[0x3F18];
    spritePalette[3] = &memory[0x3F1C];

    ppuControl = (union PPUControl *)&registers[PPUCTRL];
    ppuStatus = (union PPUStatus *)&registers[PPUSTATUS];
    ppuMask = (union PPUMask *)&registers[PPUMASK];

    /* Initialize SDL */
    if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }
    
    /* Create window */
    mainWindow = SDL_CreateWindow("Main Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if( mainWindow == NULL ) {
        cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }
    
    /* Get window surface */
    nesWindowSurface = SDL_CreateRGBSurface(0,NES_WIDTH,NES_HEIGHT,32,0,0,0,0);
    
    mainWindowSurface = SDL_GetWindowSurface(mainWindow);
    
    
    //Fill the surface white
    SDL_FillRect(nesWindowSurface, NULL, SDL_MapRGB(nesWindowSurface->format, 0xFF, 0xFF, 0xFF ));
    
    SDL_FillRect(mainWindowSurface, NULL, SDL_MapRGB(mainWindowSurface->format, 0xFF, 0xFF, 0xFF));

    return 0;
}

unsigned char PictureProcessingUnit::ReadRegister(unsigned char n) { 

    unsigned char data = registers[n];

    /* Reading the status register will clear D7 mentioned above and also 
    the address latch used by PPUSCROLL and PPUADDR. It does not clear 
    the sprite 0 hit or overflow bit. */
    if (n == PPUSTATUS) {
        ppuStatus->vblank = 0;
        ppuStatus->bits = 0;    // ppuStatus[4:0] = 0
    }
    else if (n == OAMDATA)
        data = spriteMemory[spriteRamAddress];

    else if (n == PPUDATA) {
        
        /*** Address mirroring ***/
        if (vramAddress > 0x2000 && vramAddress < 0x3F00) 
            vramAddress &= 0x2FFF;  // Mirroring: $2000-$3EFF mirror of locations $2000-$2FFF.
        else if (vramAddress >= 0x3F20 && vramAddress < 0x4000)
            vramAddress &= 0x3F1F;  // Mirroring: $3F20-$3FFF mirror of locations $3F00-$3F1F.
        else if (vramAddress > 0x4000)
            vramAddress &= 0x3FFF;  // Mirroring: $4000-$FFFF mirror of locations $0000-$3FFF.
                
        
        /**
         * The first read from $2007 is invalid and the data will actually be buffered and returned on the next read. 
         * This does not apply to colour palettes.
         */
        if (vramAddress >= 0x3F00 && vramAddress < 0x3F20)  { // Palettes address
            data = memory[vramAddress];     // Read not buffered
            //delayBuffer = data;
        }
        else {
            data = delayBuffer;
            delayBuffer = memory[vramAddress];      
        }
        (ppuControl->addressIncrement == 0) ? vramAddress++ : vramAddress += 32;
    }
    
    return data; 
}

void PictureProcessingUnit::WriteRegister(unsigned char n, unsigned char data) { 

    registers[n] = data;


    if (n == PPUADDR) {
        (vramAddressByteAccess == 0) ? vramAddress = data<<8 : vramAddress |= data;
        vramAddressByteAccess = !vramAddressByteAccess;
    
    } else if (n == PPUDATA) {             
        
        /*** Address mirroring ***/
        if (vramAddress > 0x2000 && vramAddress < 0x3F00) 
            vramAddress &= 0x2FFF;  // Mirroring: $2000-$3EFF mirror of locations $2000-$2FFF.
        else if (vramAddress >= 0x3F20 && vramAddress < 0x4000)
            vramAddress &= 0x3F1F;  // Mirroring: $3F20-$3FFF mirror of locations $3F00-$3F1F.
        else if (vramAddress > 0x4000)
            vramAddress &= 0x3FFF;  // Mirroring: $4000-$FFFF mirror of locations $0000-$3FFF.
        
        memory[vramAddress] = data;
        (ppuControl->addressIncrement == 0) ? vramAddress++ : vramAddress += 32;
    }   
    else if (n == OAMADDR)
        spriteRamAddress = data;

    else if (n == OAMDATA) {
        spriteMemory[spriteRamAddress] = data;
        spriteRamAddress++;
    }
}

void PictureProcessingUnit::FillSpriteMemory(unsigned char *sourceAddress) { 

    memcpy(spriteMemory,sourceAddress,256); 
}

int PictureProcessingUnit::Execute() {

    SDL_Rect stretchRect;
     
    if (ppuMask->backgroundVisibility)
        ShowBackground();

    if (ppuMask->spriteVisibility)
        ShowSprites();
    
    //Apply the image stretched
    stretchRect.x = 0;
    stretchRect.y = 0;
    stretchRect.w = SCREEN_WIDTH;
    stretchRect.h = SCREEN_HEIGHT;
    SDL_BlitScaled(nesWindowSurface, NULL, mainWindowSurface, &stretchRect);
        
    //Update the surface
    SDL_UpdateWindowSurface(mainWindow);

    ppuStatus->vblank = 1;    // Entering the V-Blank

    return 0;
}

void PictureProcessingUnit::ShowSprites() {
     
    struct Sprite *sprite = (struct Sprite *)spriteMemory;
    unsigned char flipedTile[TILE_SIZE];
    unsigned char *tile;

    /** 
     *  Plot the sprite memory (64 tiles)
     *      Each tile has 8x8 pixels  
     */
    for(int i=0; i<64; i++, sprite++) {
        tile = &patternTable[ppuControl->spriteTable][sprite->tile*TILE_SIZE];
        if (sprite->horizontallyFlip) {
            HorizontallyFlip(flipedTile, tile);
            tile = flipedTile;
        }
        if (sprite->verticallyFlip) {
            VerticallyFlip(flipedTile, tile);
            tile = flipedTile;
        }
        PlotTile(nesWindowSurface,sprite->x,sprite->y,tile,spritePalette[sprite->palette]);   
    }
}

void PictureProcessingUnit::ShowBackground() {

        unsigned char tileIndex;
        unsigned char attribute;
        unsigned char square;
        unsigned char paletteIndex;

        /** 
         *  Plot a name table (32x30 tiles)
         *      Each tile has 8x8 pixels  
         *      Tile coordenate: (j,i) = (column,row)
         */
        for(int i=0,y=0; i<SCREEN_ROW_TILES; i++,y+=8)      // Plot 30 tiles vertically
            for(int j=0,x=0; j<SCREEN_COLUMN_TILES; j++,x+=8) {     // Plot 32 tiles horizontally              
                
                tileIndex = nameTable[ppuControl->nameTable][i*32+j];     // Access tile (j,i) in the name table

                attribute = attributeTable[ppuControl->nameTable][((i>>2)*8)+(j>>2)];  // Retrieves tile attribute (attribute table 8x8 groups of 4x4 tiles)

                square = (BIT_CHECK(i,1)<<1) + BIT_CHECK(j,1);

                paletteIndex = (attribute & (3<<(square*2))) >> (square*2);

                PlotTile(nesWindowSurface,x,y,&patternTable[ppuControl->backgroundTable][tileIndex*TILE_SIZE],backgroundPalette[paletteIndex]);

            }
}

void PictureProcessingUnit::HorizontallyFlip(unsigned char flipedTile[], unsigned char tile[]) {

    for(int i=0; i<TILE_SIZE; i++){
        BIT_ASSIGN(flipedTile[i],7,BIT_CHECK(tile[i],0));
        BIT_ASSIGN(flipedTile[i],6,BIT_CHECK(tile[i],1));
        BIT_ASSIGN(flipedTile[i],5,BIT_CHECK(tile[i],2));
        BIT_ASSIGN(flipedTile[i],4,BIT_CHECK(tile[i],3));
        BIT_ASSIGN(flipedTile[i],3,BIT_CHECK(tile[i],4));
        BIT_ASSIGN(flipedTile[i],2,BIT_CHECK(tile[i],5));
        BIT_ASSIGN(flipedTile[i],1,BIT_CHECK(tile[i],6));
        BIT_ASSIGN(flipedTile[i],0,BIT_CHECK(tile[i],7));
    }
}

void PictureProcessingUnit::VerticallyFlip(unsigned char flipedTile[], unsigned char tile[]) {

    flipedTile[0] = tile[15];
    flipedTile[1] = tile[14];
    flipedTile[2] = tile[13];
    flipedTile[3] = tile[12];
    flipedTile[4] = tile[11];
    flipedTile[5] = tile[10];
    flipedTile[6] = tile[9];
    flipedTile[7] = tile[8];
    flipedTile[8] = tile[7];
    flipedTile[9] = tile[6];
    flipedTile[10] = tile[5];
    flipedTile[11] = tile[4];
    flipedTile[12] = tile[3];
    flipedTile[13] = tile[2];
    flipedTile[14] = tile[1];
    flipedTile[15] = tile[0];
}

void PictureProcessingUnit::PutPixel(SDL_Surface *surface, const int x, const int y, const Uint8 r, const Uint8 g, const Uint8 b) {
    
    int offset = y * surface->w + x;
    Uint32 color = SDL_MapRGB(surface->format, r, g, b);

    if(surface->format->BytesPerPixel == 1)
        *( static_cast<Uint8*>(surface->pixels) + offset) = color;
    else if(surface->format->BytesPerPixel == 2)
        *( static_cast<Uint16*>(surface->pixels) + offset) = color;
    else
        *( static_cast<Uint32*>(surface->pixels) + offset) = color;
}

void PictureProcessingUnit::PlotTile(SDL_Surface *surface, const int x, const int y, const unsigned char tile[], const unsigned char palette[]) {
    
    assert(memory != NULL && "PPU memory not initialized");

    unsigned char pixelColorIndex;  // Tile pixel color index
    unsigned char nesPaletteIndex;  // NES palette index

    // tile[row]: Tile low Byte (plane 0)
    // tile[row+8]: Tile high Byte (plane 1)
        
    /* Plot 8x8 pixel tile */
    for(int row=0; row<8; row++) {   // Draw 8 pixel rows  
        for(int column=0; column<8; column++) { // Draw 8 pixel column
            pixelColorIndex = (BIT_CHECK(tile[row+8],(7-column))<<1) + BIT_CHECK(tile[row],(7-column));  // Extracts pixel index color
            nesPaletteIndex = palette[pixelColorIndex];  // Retrieves pixel color from backgroud palette
            PutPixel(surface, x+column, y+row, nesPalette[nesPaletteIndex].red, nesPalette[nesPaletteIndex].green, nesPalette[nesPaletteIndex].blue);
        }
    }
}

void PictureProcessingUnit::spriteDump() {

    printf("Sprite Memory dump...");

    for(int i=0; i<256; i++) {
        if ( (i%16) == 0) {
            printf("\n");
            printf("[%04X]: ",i);
        }

        printf("%02X ",spriteMemory[i]);   
    }
    printf("\n");
}


