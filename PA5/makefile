#PA5
#
# Haozhe Jiang
# hjiang2

CC = g++
CFLAGS = -Wall --std=c++11

PA5: PA5.o Organism.o Ant.o Doodlebug.o
	$(CC) $(CFLAGS) -o PA5 PA5.o Organism.o Ant.o Doodlebug.o

Organism.o: Organism.h Organism.cpp
	$(CC) $(CFLAGS) -c Organism.cpp

Ant.o: Ant.h Ant.cpp
	$(CC) $(CFLAGS) -c Ant.cpp

Doodlebug.o: Doodlebug.h Doodlebug.cpp
	$(CC) $(CFLAGS) -c Doodlebug.cpp

PA5.o: PA5.cpp
	$(CC) $(CFLAGS) -c PA5.cpp	

clean:
	rm -f *.o PA5
	
all: PA5