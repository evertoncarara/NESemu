CFLAGS   = -O2 -Wall
CC  = g++

# $@ = The file name of the target of the rule
# $? = The names of all dependences (list) 
# $< = The names of the first dependence

# Rule format
# target: dependences
#      shell commmands

emulator:
	$(CC) Emulator.cpp MOS6502.cpp NESMemorySystem.cpp PictureProcessingUnit.cpp -o $@ $(CFLAGS)