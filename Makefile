CC=gcc
CFLAGS=-Wall
LIBS=-lbcm2835 -lrt
OBJECTS=gpio.o uinput.o rpi-nc.o
BIN=bin

rpi-nc: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o bin/$@ $(LIBS)

test-pin: uinput.o test-pin.o
	$(CC) $(CFLAGS) $^ -o bin/$@ $(LIBS)

.PHONY: clean

clean:
	rm -f bin/* *.o *~
