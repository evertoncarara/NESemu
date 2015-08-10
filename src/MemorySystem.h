#ifndef MEMORYSYSTEM_H
#define MEMORYSYSTEM_H

class PictureProcessingUnit;

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

class MemorySystem {
    public:
        MemorySystem();
        ~MemorySystem();

        void Load(char *fileName);
        void Init(PictureProcessingUnit *ppu);
        unsigned char Read(unsigned int address);               // CPU memory access
        void Write(unsigned int address, unsigned char data);   // CPU memory access
        void StartDMA(unsigned char addressHighByte);
        unsigned char *GetPPUMemory() { return ppuMemory; };
        void UpdateJoystick1(unsigned char joyState) { joystick1State = joyState; }
        
        
    

        /*** Test purposes ***/
        void cpuDump(int startAddr, int bytes);  
        void ppuDump(int startAddr, int bytes);   
        /*********************/
        
    private:    
        unsigned char *cpuMemory;
        unsigned char *ppuMemory;
        PictureProcessingUnit *ppu;
        unsigned char joystick1State, latchJoystick1, joystick1Shift;
};
#endif
