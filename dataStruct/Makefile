CC = gcc
CFLAGS += -g -O2 -Werror -Wall

INC= include 
INC_PARAMS=$(foreach d, $(INC), -I$d)

LDFLAGS +=

OBJECTS = list.o


all: list

prog: $(OBJECTS)
		$(CC) $(OBJECTS) -o prog

%:%.c
	$(CC) $(INC_PARAMS) $(CFLAGS) $(LDFLAGS)  -c $<

clean:
	rm -f *.o

.PHONY: all clean