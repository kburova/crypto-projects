#include<string>
#include<openssl/aes.h>

#define print_hex(str,s)	for(int z = 0; z< s; z++) \
									{ \
										printf("%X", str[z]/16); \
										printf("%X", str[z]%16); \
									} \
									printf("\n")

#ifndef AES_H
#define AES_H

class AES
{
private:
	unsigned char IV[16];
	unsigned char key[33];
	unsigned char inputBlock[16];
	unsigned char outputBlock[16];
	int keySize;
	int numBlocks;
	int padSize;
	AES_KEY AESKey;

public:
	AES();
	void setKey(int, char*);
	char* CBCencrypt(char*);
	char* CBCdecrypt(char*);
	char* CTRencrypt(char*);
	char* CTRdecrypt(char*);
};

#endif
