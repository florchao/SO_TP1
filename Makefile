CFLAGS = -Wall -lrt -pthread
IDIR = ./include
ODIR = ./obj
BDIR = ./bin
SRCDIR = ./src

all: 
	mkdir -p $(ODIR)
	mkdir -p $(BDIR)
	gcc -c $(CFLAGS) $(SRCDIR)/master.c  -o $(ODIR)/master.o
	gcc -c $(CFLAGS) $(SRCDIR)/slave.c -o $(ODIR)/slave.o
	gcc -c $(CFLAGS) $(SRCDIR)/view.c -o $(ODIR)/view.o
	gcc $(ODIR)/master.o $(CFLAGS) -o $(BDIR)/master
	gcc $(ODIR)/slave.o $(CFLAGS) -o $(BDIR)/slave
	gcc $(ODIR)/view.o $(CFLAGS) -o $(BDIR)/view

# falta borrar la shmem?
clean:
	rm -rf $(BDIR)
	rm -rf $(ODIR)
	rm -rf result
	
.PHONY: all clean