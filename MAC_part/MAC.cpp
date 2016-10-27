#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <fstream>
#include <sstream>
#include "aes.cpp"
using namespace std;

const int KEY_SIZE = 32;

void processBlock(unsigned char * key, 
				  unsigned char * block, 
				  unsigned char * cipher){

	cipher = (unsigned char *)encode(key, block).c_str();	
}

void CBCGen(unsigned char * key, 
			unsigned char * message,
			int MES_SIZE,
			unsigned char * IV){
	
	int lastBlockSize = MES_SIZE % 16;
	int d = MES_SIZE/16;
	unsigned char CurrentState[16];
	cout << strlen((const char*)message)<<endl;
	for (int i = 0; i < d; i++){
		
	}
}

bool HashMacVer(){
	return 0;
}

void HashMacGen(unsigned char * key,
				unsigned char * message ){

}

bool CBCVer(){
	return 1;
}


int main(int argc, char** argv){

	fstream M_file,K_file;
	string k, m, temp;
	int i,j, MES_SIZE;
	int mode, task;

	unsigned char key[KEY_SIZE];

	// Read the key and message from the files //

	K_file.open(argv[1]);
	if (K_file.fail()){
		fprintf(stderr, "Couln't open key file -- %s\n", argv[1]);
		exit(1);
	}
	K_file >> k;
	K_file.close();
	
	M_file.open(argv[2]);
	if (M_file.fail()){
		fprintf(stderr, "Couln't open message file -- %s\n", argv[2]);
		exit(1);
	}
	M_file >> m;
	M_file.close();

	MES_SIZE = m.size()/2;
	unsigned char message[MES_SIZE];
	
	// Convert hex strings to actual hex values // 
	for (i = 0; i < KEY_SIZE; i++){
		temp = k.substr(i*2,2);
		key[i]=strtol(temp.c_str(),NULL,16);
	}
	for (i = 0; i < MES_SIZE; i++){
		temp = m.substr(i*2,2);
		message[i]=strtol(temp.c_str(),NULL,16);
	}

	unsigned char IV[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

	// Ask user for a choice of mode and task //

	cout << "Select CBC-MAC or Hash-and-MAC" << endl
		<<"Type 1 for CBC-MAC or 2 for Hash-and-MAC : ";
	cin >> mode;
	cout << "Type 1 Tag Generation or type 2 for for Verification : ";
	cin >> task;

	if (mode == 1 and task == 1)
		CBCGen(key,message,MES_SIZE,IV);
	else if ( mode == 1 and task == 2)
		CBCVer();
	else if ( mode == 2 and task == 1)
		HashMacGen(key,message);
	else
		HashMacVer();

	return 0;
}
