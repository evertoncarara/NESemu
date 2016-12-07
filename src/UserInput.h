#ifndef USERINPUT_H
#define USERINPUT_H

#include "MemorySystem.h"
#include <SDL.h> 

union Joystick {
    struct {
        unsigned char a : 1;        
        unsigned char b : 1; 		
        unsigned char select : 1;   
        unsigned char start : 1;  	
        unsigned char up : 1;       
        unsigned char down : 1;     
        unsigned char left : 1;
        unsigned char right : 1;  	
    };
    unsigned char value;        // Access to all bit fields at the same time
};

class UserInput {
    public:
        UserInput();
        ~UserInput();
        
        void Init(MemorySystem *memorySystem, union Joystick *joystick);
        char Read();

    private:
        SDL_Event e;
        union Joystick *joystick;
        MemorySystem *memorySystem;
};
#endif
