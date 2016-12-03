CC = c++
BIN = bin
INC = include
OBJ = obj
SRC = src
FLAGS = -lssl -lcrypto -I$(INC) -I/usr/local/opt/openssl/include
DIR = /usr/local/opt/openssl
MAC = -std=c++11 -lcrypto -lssl

all: $(BIN)/rsa $(BIN)/aes $(BIN)/mac $(BIN)/sig

clean:
	rm -f $(OBJ)/* $(BIN)/*

$(BIN)/rsa: $(OBJ)/rsaDriver.o $(OBJ)/rsa.o $(OBJ)/KeyGen.o $(OBJ)/certGen.o $(OBJ)/sig.o
	$(CC) -o $(BIN)/rsa $(OBJ)/rsaDriver.o $(OBJ)/rsa.o $(OBJ)/KeyGen.o $(OBJ)/certGen.o $(OBJ)/sig.o $(FLAGS)

$(BIN)/aes: $(OBJ)/aesDriver.o $(OBJ)/aes.o
	$(CC) -o $(BIN)/aes $(OBJ)/aesDriver.o $(OBJ)/aes.o $(FLAGS)

$(BIN)/sig: $(OBJ)/sigDriver.o $(OBJ)/sig.o $(OBJ)/rsa.o
	$(CC) -o $(BIN)/sig $(OBJ)/sigDriver.o $(OBJ)/sig.o $(OBJ)/rsa.o $(FLAGS)

$(OBJ)/rsaDriver.o: $(SRC)/rsaDriver.cpp
	$(CC) -c -o $(OBJ)/rsaDriver.o $(SRC)/rsaDriver.cpp $(FLAGS)

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

$(BIN)/mac: $(SRC)/MAC.cpp
	$(CC) -o $(BIN)/mac $(SRC)/MAC.cpp $(MAC)
