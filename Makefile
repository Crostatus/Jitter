CC = gcc
CFLAGS += -Wall -pedantic
objects = ./src/jitter3.o ./src/jitter_data.o ./src/time_tools.o
TARGET = ./bin/jitter

.PHONY: all test clean

./bin/jitter : $(objects)
	$(CC) -I ./headers $(CFLAGS) $^ -o $@ -lpcap

./src/jitter3.o : ./src/jitter3.c
	$(CC) -I ./headers -c $(CFLAGS) $^ -o $@

./src/jitter_data.o : ./src/jitter_data.c
	$(CC) -I ./headers -c $(CFLAGS) $^ -o $@

./src/time_tools.o : ./src/time_tools.c
	$(CC) -I ./headers -c $(CFLAGS) $^ -o $@

all : $(TARGET)

clean:
	@echo "Removing garbage."
	-rm -f ./bin/jitter
	-rm -f ./src/*.o
