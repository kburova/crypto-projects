#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include "cryptopp/cryptlib.h"
#include <fstream>
#include "aes.cpp"
using namespace std;
/*
#include "cryptopp/osrng.h"
using CryptoPP::AutoSeededRandomPool;
using CryptoPP::Exception;
#include "cryptopp/hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;
#include "cryptopp/filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;
#include "cryptopp/aes.h"
using CryptoPP::AES;
#include "cryptopp/modes.h"
using CryptoPP::ECB_Mode;
void KeyGen(char* argv)
{
	ofstream file;
	file.open(argv);
	AutoSeededRandomPool prng;

	byte key[AES::DEFAULT_KEYLENGTH];
	prng.GenerateBlock(key, sizeof(key));

	string encoded;

	encoded.clear();
	StringSource(key, sizeof(key), true,
			new HexEncoder(
				new StringSink(encoded)
				) // HexEncoder
			); // StringSource
	file << encoded;
	file.close();
}

void aes(string &plain, byte *key, string &cipher){ 
	
	string encoded;
	try
	{
		cout << "plain text: " << plain << endl;

		ECB_Mode< AES >::Encryption e;
		e.SetKey(key, sizeof(key));

		// The StreamTransformationFilter adds padding
		//  as required. ECB and CBC Mode must be padded
		//  to the block size of the cipher.
		StringSource(plain, true,
				new StreamTransformationFilter(e,
					new StringSink(cipher)
					) // StreamTransformationFilter      
				); // StringSource
	}
	catch(const CryptoPP::Exception& e)
	{
		cerr << e.what() << endl;
		exit(1);
	}
	encoded.clear();
	StringSource(cipher, true,
			new HexEncoder(
				new StringSink(encoded)
				) // HexEncoder
			); // StringSource
	cout << "cipher text: " << encoded << endl;
}
*/


void CBCGen(char** argv){

	FILE * file;
	long size, read;
	unsigned char * key;
	unsigned char * message;

	// Read the key from the file //

	file = fopen (argv[1],"r");
	if (file == NULL){
		fprintf(stderr, "File doesn't exist\n");
		exit (1);
	}

	fseek ( file , 0 , SEEK_END);
	size = ftell ( file);
	rewind (file);

	key = (unsigned char *)malloc (sizeof(unsigned char) * (size-1) );

	if (key == NULL){
		fprintf(stderr, "Couln't allocate memory for key\n");
		exit (1);
	}

	read = fread (key, 1, size-1, file);

	printf ( "key: %s\n", key);
	if ( read != size-1 ){
		fprintf(stderr, "Didn't read all bytes\n");
		exit (1);
	}

	fclose(file);
	
	// Read the message from the file //

	file = fopen (argv[2],"r");
	if (file == NULL){
		fprintf(stderr, "File doesn't exist\n");
		exit (1);
	}

	fseek ( file , 0 , SEEK_END);
	size = ftell ( file);
	rewind (file);
	message = (unsigned char *)malloc (sizeof(unsigned char) * (size-1));

	if (message == NULL){
		fprintf(stderr, "Couln't allocate memory for key\n");
		exit (1);
	}

	read = fread (message, 1, size-1, file);
	
	if ( read != size-1 ){
		fprintf(stderr, "Didn't read all bytes\n");
		exit (1);
	}

	fclose(file);

	unsigned char temp[32];
	memcpy(temp,message,32);
	aesEcb(key, temp);

}
bool HashMacVer(){
	return 0;
}
void HashMacGen(char** argv){

}
bool CBCVer(){
	return 1;
}


int main(int argc, char** argv){

	int mode, task;

	cout << "Select CBC-MAC or Hash-and-MAC" << endl
		<<"Type 1 for CBC-MAC or 2 for Hash-and-MAC : ";
	cin >> mode;
	cout << "Type 1 Tag Generation or type 2 for for Verification : ";
	cin >> task;

	if (mode == 1 and task == 1)
		CBCGen(argv);
	else if ( mode == 1 and task == 2)
		CBCVer();
	else if ( mode == 2 and task == 1)
		HashMacGen(argv);
	else
		HashMacVer();

	return 0;
}
