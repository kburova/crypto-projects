CC = c++
BIN = bin
INC = include
OBJ = obj
SRC = src
FLAGS = -lssl -lcrypto -I$(INC)
DIR = /usr/local/opt/openssl
MAC = -std=c++11 -lcrypto -lssl -I$(DIR)/include -L$(DIR)/lib

all: $(BIN)/rsa $(BIN)/aes $(BIN)/mac

clean:
	rm -f $(OBJ)/* $(BIN)/*

$(BIN)/rsa: $(OBJ)/rsaDriver.o $(OBJ)/rsa.o $(OBJ)/KeyGen.o
	$(CC) -o $(BIN)/rsa $(OBJ)/rsaDriver.o $(OBJ)/rsa.o $(OBJ)/KeyGen.o $(FLAGS)

$(BIN)/aes: $(OBJ)/aesDriver.o $(OBJ)/aes.o
	$(CC) -g -O0 -o $(BIN)/aes $(OBJ)/aesDriver.o $(OBJ)/aes.o $(FLAGS)

$(OBJ)/rsaDriver.o: $(SRC)/rsaDriver.cpp
	$(CC) -c -o $(OBJ)/rsaDriver.o $(SRC)/rsaDriver.cpp $(FLAGS)

$(OBJ)/KeyGen.o: $(SRC)/KeyGen.cpp $(INC)/rsa.h
	$(CC) -c -o $(OBJ)/KeyGen.o $(SRC)/KeyGen.cpp $(FLAGS)

$(OBJ)/rsa.o: $(SRC)/rsa.cpp $(INC)/rsa.h
	$(CC) -c -o $(OBJ)/rsa.o $(SRC)/rsa.cpp $(FLAGS)

$(OBJ)/aesDriver.o: $(SRC)/aesDriver.cpp
	$(CC) -c -g -O0 -o $(OBJ)/aesDriver.o $(SRC)/aesDriver.cpp $(FLAGS)

$(OBJ)/aes.o: $(SRC)/aes.cpp $(INC)/aes.h
	$(CC) -c -g -O0 -o $(OBJ)/aes.o $(SRC)/aes.cpp $(FLAGS)

$(BIN)/mac: $(SRC)/MAC.cpp
	$(CC) -o $(BIN)/mac $(MAC)
