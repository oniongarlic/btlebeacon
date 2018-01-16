CFLAGS=-O2
LIBS=-lbluetooth

all: beacon

beacon.o: beacon.c

beacon: beacon.o
	$(CC) -o beacon beacon.o $(LIBS)
