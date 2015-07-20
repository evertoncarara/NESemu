#ifndef NESMEMORYSYSTEM_H
#define NESMEMORYSYSTEM_H

#include "PictureProcessingUnit.h"

class NESMemorySystem {
    public:
        NESMemorySystem();
        ~NESMemorySystem();
        
        unsigned char Read(unsigned int address);
        void Write(unsigned int address, unsigned char data);
        
        void SetPictureProcessingUnit(PictureProcessingUnit *ppu);
        unsigned char *GetPPUMemory();   
        
    private:    
        unsigned char *cpuMemory;
        unsigned char *ppuMemory;
        PictureProcessingUnit *ppu;
        

};
#endif
