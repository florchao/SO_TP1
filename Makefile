CFLAGS = -Wall -lrt -pthread
IDIR = ./include
ODIR = ./obj
BDIR = ./bin
SRCDIR = ./src

all: master view slave

master: folders master.o shmManager.o semManager.o 
	gcc $(ODIR)/master.o $(ODIR)/semManager.o $(ODIR)/shmManager.o $(CFLAGS) -o $(BDIR)/master
view: view.o shmManager.o semManager.o 
	gcc $(ODIR)/view.o $(ODIR)/semManager.o $(ODIR)/shmManager.o $(CFLAGS) -o $(BDIR)/view
slave: slave.o 
	gcc $(ODIR)/slave.o $(CFLAGS) -o $(BDIR)/slave

folders:
	mkdir -p $(ODIR)
	mkdir -p $(BDIR)

master.o: 
	gcc -c $(CFLAGS) $(SRCDIR)/master.c  -o $(ODIR)/master.o
slave.o: 
	gcc -c $(CFLAGS) $(SRCDIR)/slave.c -o $(ODIR)/slave.o
view.o: 
	gcc -c $(CFLAGS) $(SRCDIR)/view.c -o $(ODIR)/view.o
semManager.o: 
	gcc -c $(CFLAGS) $(SRCDIR)/shmManager.c -o $(ODIR)/shmManager.o
shmManager.o: 
	gcc -c $(CFLAGS) $(SRCDIR)/semManager.c -o $(ODIR)/semManager.o

clean:
	@rm -rf $(BDIR)
	@rm -rf $(ODIR)
	@rm -rf result

.PHONY: all clean