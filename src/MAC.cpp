#include <iomanip>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/aes.h>
#include <openssl/md5.h>

typedef std::basic_string<unsigned char> u_string;
#define ABORT() (fprintf(stderr, "%s\nAborting in %s at %s:%d\n", ERR_error_string(ERR_get_error(), NULL), __PRETTY_FUNCTION__, __FILE__, __LINE__), abort(), 0)
using namespace std;

/****   AES ECB unpadded mode, we process 1 block of 128 bit data   ****/
void encodeBlock(string & key, string & data, string & block)
{
	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);
	EVP_EncryptInit_ex (&ctx, EVP_aes_128_ecb(), NULL, (unsigned char*) key.c_str(), NULL);
	EVP_CIPHER_CTX_set_padding(&ctx, false);
	unsigned char buffer[1024], *pointer = buffer;
	int outlen;
	EVP_EncryptUpdate (&ctx, pointer, &outlen,(unsigned char*) data.c_str(), data.length()) or ABORT();
	pointer += outlen;
	EVP_EncryptFinal_ex(&ctx, pointer, &outlen) or ABORT();
	pointer += outlen;
	u_string temp = u_string(buffer, pointer-buffer);
	memcpy(&block, &temp, 17);
}

void CBCGen( string & key,
			 string & message,
			 int & MES_SIZE,
			 string & IV,
			 string & tag){

	int i,j;
	int lastBlockSize = MES_SIZE % 16;
	int d = MES_SIZE / 16;
	string returnBlock = "";
	returnBlock.resize(16);

	string Block = "";
	Block = IV;

	/****	XOR 4 first bytes of current state with message
		 	size, and last 12 with padding 0x0C or in other
			words, pad message size with 'padding size' char
			to block size and XOR with current state         ****/
	printf("Block  %d in: ",0);
	for (i = 0; i < 4; i++){
		Block[i] =(unsigned char)( Block[i] ^ ((MES_SIZE >> i*8) & 0xff));
		printf("%02x", (unsigned int)(unsigned char)Block[i]);
	}
	for (i = 4; i < 16; i++){
		Block[i] = (unsigned char)(Block[i] ^ 0x0C);
		printf("%02x",(unsigned int)(unsigned char) Block[i]);
	}
	cout<<endl;

	encodeBlock(key, Block, returnBlock);
	memcpy(&Block, &returnBlock, 17);

	printf("Block  %d out: ", 0);
	for (char r: Block) printf("%02x",(unsigned int)(unsigned char) r);
	cout << endl;
	cout << endl;

	/****	for each block XOR Current state with message
			and run result through Pseudorand F_k, output
			new current state						     ****/

	for ( i = 0; i < d; i++){

		printf("Block   %d in: ",i+1);
		for (char r: Block) printf("%02x",(unsigned int)(unsigned char) r);
		cout << endl;
		printf("Message %d in: ", i+1);
		for (j = 0; j < 16; j++) printf("%02x",(unsigned int)(unsigned char) message[i*16 + j]);
		cout << endl;
		printf("M XOR B %d in: ", i+1);

		for ( j = 0; j < 16; j++){
			//printf("%02x",(unsigned int)(unsigned char) Block[i*16 + j]);
			Block[j] = (unsigned char)(Block[j] ^ (message[i*16 + j] & 0xff));
			printf("%02x",(unsigned int)(unsigned char) Block[j]);
		}
		printf("\n");
		encodeBlock(key, Block, returnBlock);
		memcpy(&Block, &returnBlock, 17);


		printf("Block  %d out: ", i+1);
		for (char r: Block) printf("%02x",(unsigned int)(unsigned char) r);
		cout << endl;
		cout << endl;
	}

	/****   Process last block that has padding or that is padding only *****/
	printf("Block   %d in: ",i+1);
	for (char r: Block) printf("%02x",(unsigned int)(unsigned char) r);
	cout << endl;
	printf("Message %d in: ",i+1);
	for ( j = 0; j < lastBlockSize; j++){
		printf("%02x", (unsigned int)(unsigned char)message[i*16 + j]);
		Block[j] ^= message[i*16 + j];
	}
	for (j = lastBlockSize; j < 16; j++){
		int padSize = 16-lastBlockSize;
		printf("%02x", padSize);
		Block[j] ^= (padSize & 0xff);
		//printf("%02x", (unsigned int)(unsigned char)Block[i*16 + j]);
	}
	cout <<endl;
	encodeBlock(key, Block, returnBlock);
	memcpy(&Block, &returnBlock, 17);

	printf("Block  %d out: ", i+1);
	for (char r: Block) printf("%02x",(unsigned int)(unsigned char) r);
	cout << endl;
	tag = Block;
}

void HashMacGen(string & Key,
				string & Message,
				int & MES_SIZE,
				string & Tag){

	unsigned char hash[MD5_DIGEST_LENGTH];
	string Block = "";

	/*** calculate hash of message  and feed it into AES enc function ***/
	MD5((const unsigned char*)Message.c_str(), MES_SIZE, hash);

	//printf( "Hash:    ");
	//cout << "Hash size: " << sizeof(hash) << endl;
	//memcpy(&Block, &hash, 17);
	for (unsigned char c : hash){
		//printf("%02x", (unsigned int)c);
		Block+=c;
	}
//	cout << endl;
	encodeBlock(Key, Block, Tag);
	//printf ("Tag: %s\n", Tag.c_str());

}

bool HashMacVer( string & key,
				 string & message,
				 int & MES_SIZE,
				 string & tag){
	
	string tagToVerify;
	HashMacGen(key, message, MES_SIZE, tagToVerify);

	if (tag == tagToVerify)
		return 1;

	return 0;
}

bool CBCVer( string & key,
			string & message,
			int & MES_SIZE,
			string & IV,
			string & tag){

	string tagToVerify;
	CBCGen(key, message, MES_SIZE, IV, tagToVerify);

	if (tag == tagToVerify)
		return 1;

	return 0;
}
