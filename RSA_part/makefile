CC = c++
FLAGS = -lssl -lcrypto

all: rsa

clean:
	rm -f *.o testing a.out

rsa: driver.o rsa.o
	$(CC) -o rsa driver.o rsa.o $(FLAGS)

driver.o: driver.cpp
	$(CC) -c driver.cpp $(FLAGS)

rsa.o: rsa.cpp rsa.h
	$(CC) -c rsa.cpp $(FLAGS)

test: test.o
	$(CC) -g -o testing test.o $(FLAGS)

test.o: test.cpp
	$(CC) -g -c test.cpp
