CC = gcc
CFLAGS = -g -Wall
HEADERS =
OBJECTS = bnet.o 
EXEC = bnet

.PHONY = default build clean

default: build

build:bnet

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

run: build
	./$(EXEC) ${ARGS}

clean:
	-rm -f $(OBJECTS)
	-rm -f $(EXEC)