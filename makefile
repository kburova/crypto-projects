CC = c++
BIN = bin
INC = include
OBJ = obj
SRC = src
FLAGS = -std=c++11 -lssl -lcrypto -I$(INC) -I/usr/local/opt/openssl/include
DIR = /usr/local/opt/openssl

all: $(BIN)/rsa $(BIN)/aes $(BIN)/mac $(BIN)/sig $(BIN)/lock

clean:
	rm -f $(OBJ)/* $(BIN)/*

$(BIN)/lock: $(OBJ)/LockDriver.o $(OBJ)/rsa.o $(OBJ)/aes.o $(OBJ)/sig.o $(OBJ)/lock.o $(OBJ)/MAC.o
	$(CC) -o $(BIN)/lock $(OBJ)/LockDriver.o $(OBJ)/rsa.o $(OBJ)/aes.o $(OBJ)/sig.o $(OBJ)/lock.o $(OBJ)/MAC.o $(FLAGS)

$(BIN)/rsa: $(OBJ)/rsaDriver.o $(OBJ)/rsa.o $(OBJ)/KeyGen.o $(OBJ)/certGen.o $(OBJ)/sig.o
	$(CC) -o $(BIN)/rsa $(OBJ)/rsaDriver.o $(OBJ)/rsa.o $(OBJ)/KeyGen.o $(OBJ)/certGen.o $(OBJ)/sig.o $(FLAGS)

$(BIN)/aes: $(OBJ)/aesDriver.o $(OBJ)/aes.o
	$(CC) -o $(BIN)/aes $(OBJ)/aesDriver.o $(OBJ)/aes.o $(FLAGS)

$(BIN)/sig: $(OBJ)/sigDriver.o $(OBJ)/sig.o $(OBJ)/rsa.o
	$(CC) -o $(BIN)/sig $(OBJ)/sigDriver.o $(OBJ)/sig.o $(OBJ)/rsa.o $(FLAGS)

$(BIN)/mac: $(OBJ)/macDriver.o $(OBJ)/MAC.o
	$(CC) -o $(BIN)/mac $(OBJ)/macDriver.o $(OBJ)/MAC.o $(FLAGS)

$(OBJ)/macDriver.o: $(SRC)/macDriver.cpp
	$(CC) -c -o $(OBJ)/macDriver.o $(SRC)/macDriver.cpp $(FLAGS)

$(OBJ)/rsaDriver.o: $(SRC)/rsaDriver.cpp
	$(CC) -c -o $(OBJ)/rsaDriver.o $(SRC)/rsaDriver.cpp $(FLAGS)

$(OBJ)/LockDriver.o: $(SRC)/LockDriver.cpp
	$(CC) -c -o $(OBJ)/LockDriver.o $(SRC)/LockDriver.cpp $(FLAGS)

$(OBJ)/KeyGen.o: $(SRC)/KeyGen.cpp $(INC)/rsa.h
	$(CC) -c -o $(OBJ)/KeyGen.o $(SRC)/KeyGen.cpp $(FLAGS)

$(OBJ)/rsa.o: $(SRC)/rsa.cpp $(INC)/rsa.h
	$(CC) -c -o $(OBJ)/rsa.o $(SRC)/rsa.cpp $(FLAGS)

$(OBJ)/aesDriver.o: $(SRC)/aesDriver.cpp
	$(CC) -c -o $(OBJ)/aesDriver.o $(SRC)/aesDriver.cpp $(FLAGS)

$(OBJ)/aes.o: $(SRC)/aes.cpp $(INC)/aes.h
	$(CC) -c -o $(OBJ)/aes.o $(SRC)/aes.cpp $(FLAGS)

$(OBJ)/sigDriver.o: $(SRC)/sigDriver.cpp
	$(CC) -c -o $(OBJ)/sigDriver.o $(SRC)/sigDriver.cpp $(FLAGS)

$(OBJ)/sig.o: $(SRC)/sig.cpp $(INC)/sig.h
	$(CC) -c -o $(OBJ)/sig.o $(SRC)/sig.cpp $(FLAGS)

$(OBJ)/certGen.o: $(SRC)/certGen.cpp $(INC)/rsa.h
	$(CC) -c -o $(OBJ)/certGen.o $(SRC)/certGen.cpp $(FLAGS)

$(OBJ)/lock.o: $(SRC)/lock.cpp $(INC)/lock.h
	$(CC) -c -o $(OBJ)/lock.o $(SRC)/lock.cpp $(FLAGS)

$(OBJ)/MAC.o: $(SRC)/MAC.cpp $(INC)/mac.h
	$(CC) -c -o $(OBJ)/MAC.o $(SRC)/MAC.cpp $(FLAGS)

