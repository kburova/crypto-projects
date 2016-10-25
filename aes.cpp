#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<iostream>
#include<openssl/aes.h>
#include<openssl/rand.h>
#include"aes.h"

using std::strcpy;
using std::cout;
using std::endl;
using std::strtol;

AES::AES()
{
	int i;

	key[0] = 1;
	for(i=1; i<16; i++)
	{
		key[i] = 0;
	}
	message[0] = 1;


	for(i=1; i<16; i++)
	{
		message[i] = 0;
	}
	strcpy((char*)message, "ABCDabcdEFGHefg");

	cypher[0] = 0;
}

void AES::setKey(int size, char* textKey)
{
	int i;				// loop itterator
	long temp;
	keySize = size*4;
	key[size/2] = 0;
	
	for(i = size/2-1; i >= 0; i--)
	{
		temp = strtol(textKey+(i*2),NULL,16);
		key[i] = (unsigned char) temp;
		textKey[i*2] = 0;
	}

	cout << key << endl;

	for (i=0; i<16; i++)
	{
		printf("%X", key[i]/16);
		printf("%X", key[i]%16);
	}
	printf("\n");
	return;	
}

void AES::CBCencrypt()
{
	int i;
	cout << "You are in CBCencrypt()" << endl;

	// creates an IV
	RAND_bytes(IV, sizeof(IV));

	if(AES_set_encrypt_key(key, keySize, &AESKey))
	{
		cout << "ERROR: " << perror;
	}
	
	AES_encrypt(message, cypher, &AESKey);

	for (i=0; i<16; i++)
	{
		printf("%X", cypher[i]/16);
		printf("%X", cypher[i]%16);
	}
	printf("\n");

	AES_set_decrypt_key(key, 128, &AESKey);

	AES_decrypt(cypher, message2, &AESKey);

	for (i=0; i<16; i++)
	{
		printf("%X", message2[i]/16);
		printf("%X", message2[i]%16);
	}
	printf("\n");

	printf("%s\n",message2);
	return;
}

void AES::CBCdecrypt()
{
	cout << "You are in CBCdecrypt()" << endl;
	return;
}

void AES::CTRencrypt()
{
	cout << "You are in CTRencrypt()" << endl;
	return;
}

void AES::CTRdecrypt()
{
	cout << "You are in CTRdecrypt()" << endl;
	return;
}
