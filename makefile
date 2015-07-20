CFLAGS   = -O2 -Wall -std=c++11
CC  = g++
SDL2_WIN = Z:\Carara\SDL2-2.0.3

# $@ = The file name of the target of the rule
# $? = The names of all dependences (list) 
# $< = The names of the first dependence

# Rule format
# target: dependences
#      shell commmands

Emulator:
	$(CC) Emulator.cpp MOS6502.cpp NESMemorySystem.cpp PictureProcessingUnit.cpp -o $@ $(CFLAGS) -lSDL2main -lSDL2 -lmingw32 -I$(SDL2_WIN)\include -L$(SDL2_WIN)\lib\x86