#include "PictureProcessingUnit.h"
#include <cassert>
#include <string.h>	// memcpy()

PictureProcessingUnit::PictureProcessingUnit() { 

    memory = NULL;
}

PictureProcessingUnit::~PictureProcessingUnit(){}

void PictureProcessingUnit::Reset() {

    for(int i=0; i<8; i++)
        registers[i] = 0;

    vramAddressByteAccess = 0;
}

void PictureProcessingUnit::Init(unsigned char *ppuMemory, unsigned char *cpuMemory) { 

	assert(ppuMemory != NULL && "PPU memory not initialized");

    memory = ppuMemory; 
    this->cpuMemory = cpuMemory;

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

}

unsigned char PictureProcessingUnit::ReadRegister(unsigned char n) { 

    unsigned char regValue = registers[n];

    /* Reading the status register will clear D7 mentioned above and also 
    the address latch used by PPUSCROLL and PPUADDR. It does not clear 
    the sprite 0 hit or overflow bit. */
    if (n == PPUSTATUS) {
        ppuStatus->V = 0;
        ppuStatus->bits = 0;    // ppuStatus[4:0] = 0
    }
    
    return regValue; 
}

void PictureProcessingUnit::WriteRegister(unsigned char n, unsigned char data) { 

    registers[n] = data;

    if (n == PPUADDR) {
        (vramAddressByteAccess == 0) ? vramAddress = data<<8 : vramAddress |= data;
        vramAddressByteAccess = !vramAddressByteAccess;

    } else if (n == PPUDATA) {
        memory[vramAddress] = data;
        (ppuControl->I == 0) ? vramAddress++ : vramAddress += 32;
    }
    
}

void PictureProcessingUnit::StartDMA(unsigned char addressHighByte) {

    unsigned int cpuMemoryAddress = addressHighByte<<8 | registers[OAMADDR];  // Source: CPU RAM address

    printf("cpuMemoryAddress: %X\n", cpuMemoryAddress);

    for(int i=0; i<256; i++)
        spriteMemory[i] = cpuMemory[cpuMemoryAddress+i];
}


void PictureProcessingUnit::spriteDump(int startAddr, int bytes) {

    printf("Sprite Memory dump...%X\n",startAddr);

    for(int i=startAddr,j=0; i<(startAddr+bytes); i++,j++) {
        if ( (i%16) == 0) {
            printf("\n");
            printf("[%04X]: ",(startAddr+j));
        }

        printf("%02X ",spriteMemory[startAddr+j]);   
    }
    printf("\n");
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

void PictureProcessingUnit::PlotTile(SDL_Surface *surface, const int x, const int y, const unsigned char tile[], const unsigned char paletteIndex) {
    
    assert(memory != NULL && "PPU memory not initialized");

    unsigned char pixelColorIndex;  // Tile pixel color index
    unsigned char nesPaletteIndex;  // NES palette index

    // tile[row]: Tile low Byte (plane 0)
    // tile[row+8]: Tile high Byte (plane 1)
        
    /* Plot 8x8 pixel tile */
    for(int row=0; row<8; row++) {   // Draw 8 pixel rows  
        for(int column=0; column<8; column++) { // Draw 8 pixel column
            pixelColorIndex = (BIT_CHECK(tile[row+8],(7-column))<<1) + BIT_CHECK(tile[row],(7-column));  // Extracts pixel index color
            nesPaletteIndex = backgroundPalette[paletteIndex][pixelColorIndex];  // Retrieves pixel color from backgroud palette
            PutPixel(surface, x+column, y+row, nesPalette[nesPaletteIndex].red, nesPalette[nesPaletteIndex].green, nesPalette[nesPaletteIndex].blue);
        }
    }
}






int PictureProcessingUnit::LoadPatternTables(char *fileName) {

	ifstream romFile(fileName, ios::binary);
    
    if (romFile.fail()) {
   		cout << "Error opening file " << fileName << "." << endl;
       	return -1;
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
    
    cout << "Loading pattern tables..." << endl;
    cout << "File: " << fileName << endl;
    cout << "Size: " << fileSize << " Bytes" << endl;
    cout << "PRG_ROM: " << static_cast<int>(iNES_header->PRG_ROM) << " * 16KB" << endl;
    cout << "CHR_ROM: " << static_cast<int>(iNES_header->CHR_ROM) << " * 8KB (0 means the board uses CHR-RAM)" << endl;
    
    unsigned int chr_offset;
    if (iNES_header->CHR_ROM > 0) {
        chr_offset = sizeof(struct iNES) + (iNES_header->PRG_ROM*16*1024);
        cout << "CHR_ROM starts at: " << chr_offset << endl;
    
    	/* Copy pattern tables from rom file to the ppu memory */
    	memcpy(memory,buffer+chr_offset,(8*1024));

		delete [] buffer;

    	cout << "Pattern tables loaded!" << endl;

        return 0;

    }  

    delete [] buffer;

    return -1;
}

int PictureProcessingUnit::ShowPatternTables() {

	/* Initialize SDL */
    if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }
  	
    /* Create window */
    SDL_Window *mainWindow = SDL_CreateWindow("Main Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if( mainWindow == NULL ) {
        cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }
    
    /* Get window surface */
    SDL_Surface *nesWindowSurface = SDL_CreateRGBSurface(0,NES_WIDTH,NES_HEIGHT,32,0,0,0,0);
	
    SDL_Surface *mainWindowSurface = SDL_GetWindowSurface(mainWindow);
	
	
    //Fill the surface white
    SDL_FillRect(nesWindowSurface, NULL, SDL_MapRGB(nesWindowSurface->format, 0xFF, 0xFF, 0xFF ));
	
    SDL_FillRect(mainWindowSurface, NULL, SDL_MapRGB(mainWindowSurface->format, 0xFF, 0xFF, 0xFF));
	
    /* Main loop flag */
    bool quit = false;
    
    /* Event handler */
    SDL_Event e;
    
    /* While application is running */
    while (!quit) {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 ) {
            //User requests quit
            if( e.type == SDL_QUIT ) 
                quit = true;
        }       
	
        
        /** 
         *  Plot a 16x16 matrix of tiles
         *      Each tile has 8x8 pixels  
         */
        for(int i=0,y=0; i<16; i++,y+=8)        // Plot 16 tiles vertically
            for(int j=0,x=0; j<16; j++,x+=8)    // Plot 16 tiles horizontally              
                PlotTile(nesWindowSurface,x,y,&memory[(i*TILE_SIZE*16)+(j*TILE_SIZE)],0);
                
        /** 
         *  Plot a 16x16 matrix of tiles
         *      Each tile has 8x8 pixels  
         */
        for(int i=0,y=0; i<16; i++,y+=8)            // Plot 16 tiles vertically
            for(int j=0,x=8*16; j<16; j++,x+=8)     // Plot 16 tiles horizontally              
                PlotTile(nesWindowSurface,x,y,&memory[16*16*TILE_SIZE+(i*TILE_SIZE*16)+(j*TILE_SIZE)],0);
    
        //Apply the image stretched
        SDL_Rect stretchRect;
        stretchRect.x = 0;
        stretchRect.y = 0;
        stretchRect.w = SCREEN_WIDTH;
        stretchRect.h = SCREEN_HEIGHT;
        SDL_BlitScaled(nesWindowSurface, NULL, mainWindowSurface, &stretchRect);
            
	
        //Update the surface
        SDL_UpdateWindowSurface(mainWindow);
    }
    
    //Free resources and close SDL
    //Destroy window
    SDL_FreeSurface(mainWindowSurface);
    SDL_FreeSurface(nesWindowSurface);
    SDL_DestroyWindow(mainWindow);
	
    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}

int PictureProcessingUnit::ShowNameTable(unsigned char number) {

	/* Initialize SDL */
    if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }
  	
    /* Create window */
    SDL_Window *mainWindow = SDL_CreateWindow("Main Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if( mainWindow == NULL ) {
        cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }
    
    /* Get window surface */
    SDL_Surface *nesWindowSurface = SDL_CreateRGBSurface(0,NES_WIDTH,NES_HEIGHT,32,0,0,0,0);
	
    SDL_Surface *mainWindowSurface = SDL_GetWindowSurface(mainWindow);
	
	
    //Fill the surface white
    SDL_FillRect(nesWindowSurface, NULL, SDL_MapRGB(nesWindowSurface->format, 0xFF, 0xFF, 0xFF ));
	
    SDL_FillRect(mainWindowSurface, NULL, SDL_MapRGB(mainWindowSurface->format, 0xFF, 0xFF, 0xFF));
	
    /* Main loop flag */
    bool quit = false;
    
    /* Event handler */
    SDL_Event e;
    
    /* While application is running */
    while (!quit) {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 ) {
            //User requests quit
            if( e.type == SDL_QUIT ) 
                quit = true;
        }      
        

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
                
            	tileIndex = nameTable[number][i*32+j];     // Access tile (j,i) in the name table

                attribute = attributeTable[number][((i>>2)*8)+(j>>2)];  // Retrieves tile attribute (attribute table 8x8 groups of 4x4 tiles)

                square = (BIT_CHECK(i,1)<<1) + BIT_CHECK(j,1);

                paletteIndex = (attribute & (3<<(square*2))) >> (square*2);

                PlotTile(nesWindowSurface,x,y,&patternTable[1][tileIndex*TILE_SIZE],paletteIndex);

            }
               
    
        //Apply the image stretched
        SDL_Rect stretchRect;
        stretchRect.x = 0;
        stretchRect.y = 0;
        stretchRect.w = SCREEN_WIDTH;
        stretchRect.h = SCREEN_HEIGHT;
        SDL_BlitScaled(nesWindowSurface, NULL, mainWindowSurface, &stretchRect);
            
	
        //Update the surface
        SDL_UpdateWindowSurface(mainWindow);
    }
    
    //Free resources and close SDL
    //Destroy window
    SDL_FreeSurface(mainWindowSurface);
    SDL_FreeSurface(nesWindowSurface);
    SDL_DestroyWindow(mainWindow);
	
    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}

void PictureProcessingUnit::LoadNameTable(unsigned char number) {

	for(int i=0; i<960; i++)	// Name table size: 960 Bytes
		nameTable[number][i] = i%10;
}

void PictureProcessingUnit::LoadBackgroundPalette() {

    backgroundPalette[0][0] = 0x3F;
    backgroundPalette[0][1] = 0x5;
    backgroundPalette[0][2] = 0x21;
    backgroundPalette[0][3] = 0x25;

    backgroundPalette[1][0] = 0x3F;
    backgroundPalette[1][1] = 0x2A;
    backgroundPalette[1][2] = 0x2;
    backgroundPalette[1][3] = 0x3;

    backgroundPalette[2][0] = 0x3F;
    backgroundPalette[2][1] = 0x38;
    backgroundPalette[2][2] = 0x18;
    backgroundPalette[2][3] = 0x29;

    backgroundPalette[3][0] = 0x3F;
    backgroundPalette[3][1] = 0x30;
    backgroundPalette[3][2] = 0x23;
    backgroundPalette[3][3] = 0x34;
}

void PictureProcessingUnit::LoadAttributeTable(unsigned char number) {

    for(int i=0; i<64; i++)
        attributeTable[number][i] = 0x00;

    attributeTable[number][7] = 0xFF;
    attributeTable[number][8] = 0xFF;
    attributeTable[number][15] = 0xFF;
    attributeTable[number][23] = 0xFF;
    attributeTable[number][31] = 0xFF;
    attributeTable[number][39] = 0xFF;
    attributeTable[number][47] = 0xFF;
    attributeTable[number][63] = 0xFF;
}
