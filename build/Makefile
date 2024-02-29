CC = gcc
# CFLAGS += -g -O2 -Werror -Wall
CFLAGS += -g3 -O0

# INC = include 
# INC2 = ../arch
INC= include ../arch/include ../dataStruct/include
INC_PARAMS=$(foreach d, $(INC), -I$d)

LDFLAGS +=


LOC_OBJ = assembler.o allocator.o util.o argumentParse.o
EX_OBJ = ../dataStruct/list.o
OBJECTS := $(LOC_OBJ) $(EX_OBJ)

all: assembler 

assembler: $(OBJECTS)
		$(CC) $(INC_PARAMS) $(CFLAGS) $(OBJECTS)  -o assembler

%.o:%.c
	$(CC) $(INC_PARAMS) $(CFLAGS) $(LDFLAGS) -c $< -o $@ 

clean:
	rm -f *.o
	rm -f assembler

.PHONY: all clean
