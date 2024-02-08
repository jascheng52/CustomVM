CC = gcc
# CFLAGS += -g -O2 -Werror -Wall
CFLAGS += -g3 -O0

# INC = include 
# INC2 = ../arch
INC= include ../arch/include ../dataStruct/include
INC_PARAMS=$(foreach d, $(INC), -I$d)

LDFLAGS +=

OBJECTS = assembler.o


all: assembler

prog: $(OBJECTS)
		$(CC) $(OBJECTS) -o prog

%:%.c
	$(CC) $(INC_PARAMS) $(CFLAGS) $(LDFLAGS) -o $@ $< 

clean:
	rm -f *.o

.PHONY: all clean
