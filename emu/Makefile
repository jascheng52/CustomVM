CC = gcc
# CFLAGS += -g -O2 -Werror -Wall
CFLAGS += -g3 -O0

INC= include ../arch/include
INC_PARAMS=$(foreach d, $(INC), -I$d)

LOC_OBJ = emu.o
EX_OBJ = 
OBJECTS := $(LOC_OBJ) $(EX_OBJ)

all: emulator 

emulator: $(OBJECTS)
		$(CC) $(INC_PARAMS) $(CFLAGS) $(OBJECTS)  -o emulator

%.o:%.c
	$(CC) $(INC_PARAMS) $(CFLAGS) $(LDFLAGS) -c $< -o $@ 

clean:
	rm -f *.o
	rm -f emulator

.PHONY: all clean
