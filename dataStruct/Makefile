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
	$(CC) -c $(INC_PARAMS) $(CFLAGS) $(LDFLAGS) -o $@ $< 

clean:
	rm -f *.o

.PHONY: all clean
