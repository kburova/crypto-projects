CC = c++
FLAGS = -lssl -lcrypto

rsa: main.o rsa.o KeyGen.o
	$(CC) -o rsa main.o rsa.o KeyGen.o $(FLAGS)

all: rsa keygen

clean:
	rm -f *.o testing a.out keygen rsa

keygen: main.o KeyGen.o
	$(CC) -o keygen main.o KeyGen.o $(FLAGS)

main.o: main.cpp
	$(CC) -c main.cpp $(FLAGS)

KeyGen.o: KeyGen.cpp rsa.h
	$(CC) -c KeyGen.cpp $(FLAGS)

driver.o: driver.cpp
	$(CC) -c driver.cpp $(FLAGS)

rsa.o: rsa.cpp rsa.h
	$(CC) -c rsa.cpp $(FLAGS)

test: test.o
	$(CC) -g -o testing test.o $(FLAGS)

test.o: test.cpp
	$(CC) -g -c test.cpp
