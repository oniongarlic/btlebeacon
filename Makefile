CFLAGS=-g -O2 -Wall
LIBS=$(shell pkg-config --libs bluez)

all: beacon

beacon.o: beacon.c

beacon: beacon.o
	$(CC) -o beacon beacon.o $(LIBS)

clean:
	rm -f beacon beacon.o

setcap: beacon
	sudo setcap 'cap_net_raw,cap_net_admin+eip' beacon
