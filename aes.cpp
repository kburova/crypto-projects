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

	key[0] = 0;
	for(i=0;i<16;i++) inputBlock[i] = 0;
	for(i=0;i<16;i++) outputBlock[i] = 0;
}

void AES::setKey(int size, char* textKey)
{
	int i;				// loop itterator
	keySize = size*4;
	key[size/2] = 0;

	// move backwards through the string starting 2 bytes from the end
	// of the string treating it as its own string. Convert the 2 byte
	// string to a hex int and store in the key. Set the value in 
	// textKey = 0 so the next string will also be 2 bytes.
	for(i = size/2-1; i >= 0; i--)
	{
		key[i] = (unsigned char) strtol(textKey+(i*2),NULL,16);
		textKey[i*2] = 0;
	}

	return;	
}

char* AES::CBCencrypt(int size, char* input)
{
	int i,j;								// loop itterators
	int fieldPad;						// invers string pad size
	char temp[33];						// converter string
	char tempOut[33];					// testing string
	char* inputPtr;					// points to cur loc in input string
	char* output;						// return string
	char* outputPtr;					// points to cur loc in output string

	// determine the number of blocks and the size of the padding
	padSize = 16-((size%32)/2);
	numBlocks = (size/32)+1;
	fieldPad = size%32;

	output = (char*)malloc(((numBlocks+1)*16)+1);
	outputPtr = output;

	// creates an IV
	RAND_bytes(IV, sizeof(IV));


	// insert the IV into the cypher
	for(i=0; i<16 ;i++)
	{
   	 sprintf(outputPtr+(i*2), "%02X", IV[i]);
	}
	outputPtr += 32;

	AES_set_encrypt_key(key, keySize, &AESKey);

	inputPtr = input;
	// cpy the message into inputBlocks and encrypt them
	for(i=0; i<numBlocks-1; i++)
	{
		memcpy(temp,inputPtr,32);
		inputPtr += 32;
		temp[32] = 0;
	
		// convert the string into hex
		for(j = 15; j >= 0; j--)
		{
			inputBlock[j] = (unsigned char) strtol(temp+(j*2),NULL,16);
			temp[j*2] = 0;
		}
		// if this is the first block
		if(!i)
		{
			for(j=0; j<16; j++)
			{
				inputBlock[j] = inputBlock[j] ^ IV[j];
			}
		}
		else
		{
			for(j=0; j<16; j++)
			{
				inputBlock[j] = inputBlock[j] ^ outputBlock[j];
			}
		}
		AES_encrypt(inputBlock, outputBlock, &AESKey);
		// append the outputBlock to the output stinng
		for(j=0; j<16 ;j++)
		{
			sprintf(outputPtr+(j*2), "%02X", outputBlock[j]);
		}
		outputPtr += 32;
	}
	
	// create the last block with padding.
	memcpy(temp,inputPtr, fieldPad);
	temp[32] = 0;
	
	// copy the data
	for(i = fieldPad/2-1; i >= 0; i--)
	{
		inputBlock[i] = (unsigned char) strtol(temp+(i*2),NULL,16);
		temp[i*2] = 0;
	}
	inputBlock[15] = padSize;
	
	// pad the rest
	for(i = 14; i > 15-padSize; i--)
	{
		inputBlock[i] = 0;
	}

	for(j=0; j<16; j++)
	{
		inputBlock[j] = inputBlock[j] ^ outputBlock[j];
	}
	AES_encrypt(inputBlock, outputBlock, &AESKey);
	
	// append the outputBlock to the output stinng
	for(j=0; j<16 ;j++)
	{
		sprintf(outputPtr+(j*2), "%02X", outputBlock[j]);
	}
	outputPtr += 32;
	
*outputPtr = 0;

/****************  TESTING *********************
unsigned char inputBlock2[16];
	AES_set_decrypt_key(key, 128, &AESKey);
	AES_decrypt(outputBlock, inputBlock2, &AESKey);
//	print_hex(inputBlock2,16);
	for(i=0; i<16; i++)
	{
		inputBlock2[i] = inputBlock2[i] ^ IV[i];
	}

//	print_hex(inputBlock2,16);

	for(i=0; i<16 ;i++)
	{
   	 sprintf(tempOut+(i*2), "%02X", inputBlock2[i]);
	}

//	cout << tempOut << endl;
//	print_hex(outputBlock, 16);
***************************************************/
	return output;
}

char* AES::CBCdecrypt(int size, char* input)
{
	cout << "You are in CBCdecrypt()" << endl;
	char* output;


//	AES_set_decrypt_key(key, 128, &AESKey);
//	AES_decrypt(outputBlock, inputBlock2, &AESKey);
//	print_hex(inputBlock2,16);
//	printf("%s\n",inputBlock2);

	return output;
}

char* AES::CTRencrypt(int size, char* input)
{
	char* output;
	cout << "You are in CTRencrypt()" << endl;
	return output;
}

char* AES::CTRdecrypt(int size, char* input)
{
	char* output;
	cout << "You are in CTRdecrypt()" << endl;
	return output;
}
