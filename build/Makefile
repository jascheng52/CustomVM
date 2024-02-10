CC = gcc
# CFLAGS += -g -O2 -Werror -Wall
CFLAGS += -g3 -O0

# INC = include 
# INC2 = ../arch
INC= include ../arch/include ../dataStruct/include
INC_PARAMS=$(foreach d, $(INC), -I$d)

LDFLAGS +=

OBJECTS = assembler.o allocator.o


all: assembler 

assembler: $(OBJECTS)
		$(CC) $(INC_PARAMS) $(OBJECTS) -o assembler

%.o:%.c
	$(CC) $(INC_PARAMS) $(CFLAGS) $(LDFLAGS) -c $< -o $@ 

clean:
	rm -f *.o
	rm -f assembler

.PHONY: all clean
