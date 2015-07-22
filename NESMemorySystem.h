#ifndef NESMEMORYSYSTEM_H
#define NESMEMORYSYSTEM_H

#include "PictureProcessingUnit.h"

class NESMemorySystem {
    public:
        NESMemorySystem();
        ~NESMemorySystem();

        void Init(char *fileName);

        unsigned char Read(unsigned int address);
        void Write(unsigned int address, unsigned char data);
        
        void SetPictureProcessingUnit(PictureProcessingUnit *ppu);
        unsigned char *GetPPUMemory();

        /*** Test purposes ***/
        void cpuDump(int startAddr, int bytes);  
        void ppuDump(int startAddr, int bytes);   
        /*********************/
        
    private:    
        unsigned char *cpuMemory;
        unsigned char *ppuMemory;
        PictureProcessingUnit *ppu;
        

};
#endif
