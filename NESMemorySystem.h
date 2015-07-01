#ifndef NESMEMORYSYSTEM_H
#define NESMEMORYSYSTEM_H

#include "PictureProcessingUnit.h"

class NESMemorySystem {
    public:
        NESMemorySystem(unsigned int size);
        ~NESMemorySystem();
        
        unsigned char Read(unsigned int address);
        void Write(unsigned int address, unsigned char data);
        
        inline void SetPictureProcessingUnit(PictureProcessingUnit *ppu) { this->ppu = ppu; }
        
        void Load(char *imageFile);
        
        inline void Print(int begin, int size) { 
            int i; 
            printf("[%X]: ",begin);
            for(i=begin; i<(begin+size); i++)
                printf("%X ",array[i]);
            printf("\n");                
        };
        
    private:    
        unsigned char *array;
        PictureProcessingUnit *ppu;
        

};
#endif
