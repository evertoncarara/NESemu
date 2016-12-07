CFLAGS   = -O2 -Wall
CC  = g++
SDL2_WIN = Z:\Carara\SDL2-2.0.5

# $@ = The file name of the target of the rule
# $? = The names of all dependences (list) 
# $< = The names of the first dependence

# Rule format
# target: dependences
#      shell commmands

NESemu:
	$(CC) .\src\NESemu.cpp .\src\MOS6502.cpp .\src\MemorySystem.cpp .\src\PictureProcessingUnit.cpp .\src\UserInput.cpp -o $@ $(CFLAGS) -lSDL2main -lSDL2 -lmingw32 -I$(SDL2_WIN)\include -L$(SDL2_WIN)\lib\x86