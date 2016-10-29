#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <fstream>
#include <sstream>
#include "aes.cpp"
using namespace std;

const int KEY_SIZE = 32;
//typedef std::basic_string<unsigned char> u_string;

void processBlock(unsigned char * key, 
				  unsigned char * block){
	u_string c;
	printf("Before aes : ");
	for (int i = 0; i < 16;i++){
		printf("%02x",block[i]);
	}
	printf ("\n");
	c = (unsigned char *)encode(key, block).c_str();


	printf("After aes  : ");
	for (int i = 0; i < 16;i++){
		printf("%02x",c[i]);
		block[i] = c[i];
	}
	printf ("\n\n");
}

void CBCGen(unsigned char * key, 
			unsigned char * message,
			int MES_SIZE,
			unsigned char * IV,
			unsigned char * tag){

	int i,j;
	int lastBlockSize = MES_SIZE % 16;
	int d = MES_SIZE/16;

	unsigned char CurrentState[17];
	for ( i = 0; i < 16; i++ ){
		CurrentState[i] = IV[i];
	}
	CurrentState[16] = '\0';

	// XOR 4 first bytes of current state with message //
	// size, and last 12 with padding 0x0C or in other //
	// words, pad message size with 'padding size' char//
	// to block size and XOR with current state        //

	printf("Mes Block %d: ", 0);
	for (i = 0; i < 1; i++){
		CurrentState[i] ^= ((MES_SIZE >> i*8) & 0xff);
		printf("%02x", CurrentState[i]);
		//	printf("i: %02x\n",((MES_SIZE >> i*8) & 0xff) );	
	}
	for (i = 1; i < 16; i++){
		CurrentState[i] ^= 0x0C;
		printf("%02x", CurrentState[i]);
		//	printf("i: %02x\n", CurrentState[i]);
	}
	cout<<endl;
	//	printf("i: %08x\n", MES_SIZE);	
	//printf("1st block: %s\nsize of block: %lu\n", CurrentState, strlen((const char*)CurrentState));	
	processBlock((unsigned char*)key,(unsigned char*)CurrentState);

	// for each block XOR Current state with message //
	// and run result through Pseudorand F_k, output //
	// new current state						     //

	for ( i = 0; i < d; i++){
		printf("Mes Block %d: ",i+1);
		for ( j = 0; j < 16; j++){
			CurrentState[j] ^= message[i*16 + j];
			printf("%02x",message[i*16 + j]);
		}
		printf("\n");
		processBlock((unsigned char*)key,(unsigned char*)CurrentState);
	}

	if (lastBlockSize != 0){
		for ( j = 0; j < lastBlockSize; j++){
			CurrentState[j] ^= message[i*16 + j];
		}
		for (j = lastBlockSize; j < 16; j++){
			CurrentState[j] ^= (16-lastBlockSize);
		}
		processBlock(key,CurrentState);
	}

	for ( i = 0; i < 16; i++ ){
		tag[i]=CurrentState[i];
	}
	tag[16]='\0';

}

void HashMacGen(unsigned char * key,
				unsigned char * message,
				int MES_SIZE,
				unsigned char * tag){

	unsigned char hash[MD5_DIGEST_LENGTH+1];
	u_string c;
	
	MD5(message, MES_SIZE, hash);
	hash[MD5_DIGEST_LENGTH] = '\0';
	cout << "Message size: " << MES_SIZE << endl;
	cout << "hash: "<< hash <<endl;
	cout << "Hash size: " << sizeof(hash) << endl;

	c = (unsigned char *)encode((unsigned char*)key, (unsigned char*)hash).c_str();
	for (int i = 0; i<16; i++){
		tag[i] = c[i];
	}
	tag[16]='\0';
}

bool HashMacVer( unsigned char * key,
				 unsigned char * message,
				 int MES_SIZE,
				 unsigned char * tag){
	
	unsigned char tagToVerify[17];
	HashMacGen(key, message, MES_SIZE, tagToVerify);

	if (strcmp((const char*)tag,(const char*)tagToVerify) == 0)
		return 1;

	return 0;
}

bool CBCVer(unsigned char * key, 
			unsigned char * message,
			int MES_SIZE,
			unsigned char * IV,
			unsigned char * tag){
	
	unsigned char tagToVerify[17];
	CBCGen(key, message, MES_SIZE,IV, tagToVerify);

//	printf("Tag in the file: %s\n", tag);
//	printf("Tag to verify  : %s\n", tagToVerify);
	
	if (strcmp((const char*)tag,(const char*)tagToVerify) == 0) 
		return 1;

	return 0;
}


int main(int argc, char** argv){

	ifstream M_file,K_file;
	fstream T_file;
	string k, m, temp;
	int i,j, MES_SIZE;
	int mode, task;

	unsigned char key[KEY_SIZE+1];
	unsigned char tag[16+1];

	//    Read the key and message from the files    //

	K_file.open(argv[1]);
	if (K_file.fail()){
		fprintf(stderr, "Couln't open key file -- %s\n", argv[1]);
		exit(1);
	}
	K_file >> k;
	K_file.close();	
	printf("Read key: %s\n", k.c_str());

	M_file.open(argv[2]);
	if (M_file.fail()){
		fprintf(stderr, "Couln't open message file -- %s\n", argv[2]);
		exit(1);
	}
	M_file >> m;
	M_file.close();
	printf("Read message: %s\n", m.c_str());

	MES_SIZE = m.size()/2;
	unsigned char message[MES_SIZE+1];

	//    Convert hex strings to actual hex values   // 
	
	for (i = 0; i < KEY_SIZE; i++){
		temp = k.substr(i*2,2);
		key[i]=strtol(temp.c_str(),NULL,16);
	}
	
	key[KEY_SIZE] = '\0';
	printf("Convert key: %s\n", key);
	for (i = 0; i < MES_SIZE; i++){
		temp = m.substr(i*2,2);
		message[i]=strtol(temp.c_str(),NULL,16);
	}
	message[MES_SIZE] = '\0';
	printf("Convert message: %s\n", message);

	//   set IV to all 0's since no need for MAC   //
	unsigned char IV[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,'\0'};

	printf("IV: %s\n", IV);

	//   open Tag file to write to OR to read from  //
	T_file.open(argv[3]);
	if (T_file.fail()){
		fprintf(stderr, "Couln't open file for tag -- %s\n", argv[3]);
		exit(1);
	}



	// Ask user for a choice of mode and task //

	cout << "Select CBC-MAC or Hash-and-MAC" << endl
		<<"Type 1 for CBC-MAC or 2 for Hash-and-MAC : ";
	cin >> mode;
	cout << "Type 1 Tag Generation or type 2 for for Verification : ";
	cin >> task;

	if (task == 1){

		char temp[3];

		if (mode == 1){
	// CBC-MAC tag generation mode //
			CBCGen(key,message,MES_SIZE,IV,tag);
		}else{
	// Hash-and-MAC tag generation mode //
			HashMacGen(key,message,MES_SIZE,tag);
		}

		for (i = 0; i < 16; i++){
			sprintf( temp, "%02x", IV[i] );
			T_file << temp;
		}
		printf("Tag:     "); 
		for (i = 0; i < 16; i++){
			printf("%02x",tag[i]);
			sprintf( temp, "%02x", tag[i] );
			T_file << temp;
		}
		cout << endl;
		T_file.close();

	}else if (task == 2){
		
		bool isValid;
		string t;

		T_file >> t;
		T_file.close();
		printf("Read tag: %s\n", t.c_str());

		if (t.size() != 64){
			cout <<"***Tag is NOT valid***"<<endl;
			return 0;
		}

		for (i = 0; i < 16; i++){
			temp = t.substr(i*2+32,2);
			tag[i]=strtol(temp.c_str(),NULL,16);
		}
		tag[16] = '\0';
		printf("Convert tag: %s\n", key);

		if (mode ==1){
			isValid = CBCVer(key, message, MES_SIZE,IV,tag);
		}else{
			isValid = HashMacVer(key, message, MES_SIZE,tag);
		}
		printf ("%s\n", (isValid ? "***Tag is valid***" : "***Tag is NOT valid***"));
	}
	return 0;
}
