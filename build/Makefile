CC = gcc
# CFLAGS += -g -O2 -Werror -Wall
CFLAGS += -g -O2 

INC = include
LDFLAGS +=

OBJECTS = assembler.o


all: assembler

prog: $(OBJECTS)
		$(CC) $(OBJECTS) -o prog

%:%.c
	$(CC) -I$(INC) $(CFLAGS) $(LDFLAGS) -o $@ $< 

clean:
	rm -f *.o

.PHONY: all clean
