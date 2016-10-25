FLAGS = -lssl -lcrypto
CC = g++
OBJS = 

all: aes_driver

aes_driver: aes_driver.o aes.o
	$(CC)  -o aes_driver.exe aes_driver.o aes.o $(FLAGS)

aes_driver.o: aes_driver.cpp
	$(CC) -c aes_driver.cpp

aes.o: aes.cpp
	$(CC) -c aes.cpp

clean:
	rm -f a.out aes_driver.exe aesDebug *.o

debug: aes_driver_debug.o aes_debug.o
	$(CC)  -g -o aesDebug aes_driver_debug.o aes_debug.o $(FLAGS)

aes_driver_debug.o: aes_driver.cpp
	$(CC) -g -c aes_driver.cpp -o aes_driver_debug.o

aes_debug.o: aes.cpp
	$(CC) -g -c aes.cpp -o aes_debug.o

