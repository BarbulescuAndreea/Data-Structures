CC = gcc
CFLAGS = -g -Wall
HEADERS =
OBJECTS = tema11.o 
EXEC = tema11

.PHONY = default build clean

default: build

build:tema11

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

run: build
	./$(EXEC) ${ARGS}

clean:
	-rm -f $(OBJECTS)
	-rm -f $(EXEC)