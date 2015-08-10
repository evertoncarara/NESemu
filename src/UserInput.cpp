#include "UserInput.h"

UserInput::UserInput() { 

    joystick = NULL;
    memorySystem = NULL;
}

UserInput::~UserInput() {}

void UserInput::Init(MemorySystem *memorySystem, union Joystick *joystick) {

	this->joystick = joystick; 
	this->memorySystem = memorySystem; 
}

char UserInput::Read() {

    while (SDL_PollEvent(&e) != 0) {
        
        if( e.type == SDL_QUIT ) 
            return 0;

        else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_a)
                joystick->a = 1;
    
            if (e.key.keysym.sym == SDLK_s)
               joystick->b = 1;
    
            if (e.key.keysym.sym == SDLK_SPACE)
                joystick->select = 1;
    
            if (e.key.keysym.sym == SDLK_RETURN)
                joystick->start = 1;
    
            if (e.key.keysym.sym == SDLK_UP)
                joystick->up = 1;
    
            if (e.key.keysym.sym == SDLK_DOWN)
                joystick->down = 1;
    
            if (e.key.keysym.sym == SDLK_LEFT)
                joystick->left = 1;
    
            if (e.key.keysym.sym == SDLK_RIGHT)
                joystick->right = 1;  
                
            memorySystem->UpdateJoystick1(joystick->value);
        }
        else if (e.type == SDL_KEYUP) {
            if (e.key.keysym.sym == SDLK_a)
                joystick->a = 0;
    
            if (e.key.keysym.sym == SDLK_s)
               joystick->b = 0;
    
            if (e.key.keysym.sym == SDLK_SPACE)
                joystick->select = 0;
    
            if (e.key.keysym.sym == SDLK_RETURN)
                joystick->start = 0;
    
            if (e.key.keysym.sym == SDLK_UP)
                joystick->up = 0;
    
            if (e.key.keysym.sym == SDLK_DOWN)
                joystick->down = 0;
    
            if (e.key.keysym.sym == SDLK_LEFT)
                joystick->left = 0;
    
            if (e.key.keysym.sym == SDLK_RIGHT)
                joystick->right = 0;    
        
            memorySystem->UpdateJoystick1(joystick->value);
        }
    }
    return 1;
}