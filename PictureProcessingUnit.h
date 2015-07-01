#ifndef PICTUREPROCESSINGUNIT_H
#define PICTUREPROCESSINGUNIT_H

#include <stdio.h>

class PictureProcessingUnit {
    public:
        PictureProcessingUnit();
        ~PictureProcessingUnit();
        
        inline unsigned char ReadRegister(unsigned char n) { return registers[n]; }
        inline void WriteRegister(unsigned char n, unsigned char data) { registers[n] = data; }
                
    private:      
        unsigned char registers[8];
};
#endif

