#include<cstdio>
#include<fstream>
#include<string>
#include<cstring>
#include<cstdlib>
#include<iostream>
#include<openssl/aes.h>
#include<openssl/rand.h>
#include"aes.h"

using std::ofstream;
using std::string;
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
	char* inputPtr;					// points to cur loc in input string
	char* output;						// return string
	char* outputPtr;					// points to cur loc in output string


	// determine the number of blocks and the size of the padding
	padSize = 16-((size%32)/2);
	numBlocks = (size/32)+1;
	fieldPad = size%32;

	output = (char*)malloc(((numBlocks+1)*32)+1);

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

	return output;
}

char* AES::CBCdecrypt(int size, char* input)
{
	int i, j;							// loop itterators
	int padSize;						// extracted pad size
	int inputSize;						// Size of the cypher message
	int outputSize;					// Size of the resulting message
	int numBlocks;						// number of blocks
	char* inputPtr;					// ptr to the current pos in input string
	char* output;						// output string
	char* outputPtr;					// ptr to the current pos in the output string
	char temp[33];						// temp string
	char storeC[32];					// holds untill next block is deciphered

	AES_set_decrypt_key(key, keySize, &AESKey);

	inputPtr = input;
	inputSize = strlen(input);
	numBlocks = inputSize / 32;

	memcpy(temp, inputPtr, 32);
	temp[32] = 0;
	inputPtr += (inputSize - 32);
	
	// Extract the IV
	for(i=15; i>=0; i--)
	{
		IV[i] = (unsigned char) strtol(temp+(i*2),NULL,16);
		temp[i*2] = 0;
	}

	// copy the data
	memcpy(temp, inputPtr, 32);
	temp[32] = 0;
	inputPtr -= 32;

	// cast it to a hex value
	for(j=15; j>=0; j--)
	{
		inputBlock[j] = (unsigned char) strtol(temp+(j*2),NULL,16);
		temp[j*2] = 0;
	}

	// untill there is only one block left
	for(i=numBlocks-1; i > 0; i--)
	{
		// decrypt the data
		AES_decrypt(inputBlock, outputBlock, &AESKey);

		// and copy the next block of data
		memcpy(temp, inputPtr, 32);
		inputPtr -= 32;

		// and convert it to its hex value
		for(j=15; j>=0; j--)
		{
			inputBlock[j] = (unsigned char) strtol(temp+(j*2),NULL,16);
			temp[j*2] = 0;
		}
		// now we can XOR it with the output returned from the decypher
		for(j=0; j<16; j++)
		{
			outputBlock[j] = outputBlock[j] ^ inputBlock[j];
		}

		// if this is the padded block
		if(i == numBlocks-1)
		{
			padSize = (int)outputBlock[15];
			if(padSize == 16)
			{
				outputSize = ((numBlocks-2) * 32)+1;
				output = (char*)malloc(outputSize);
			}
			else
			{
				outputSize = ((numBlocks-2)*32)+(32-(padSize*2))+1;
				output = (char*)malloc(outputSize);
			}
			output[outputSize-1] = 0;
			outputPtr = output + ((outputSize/32)*32);
			// copy the padded block into the return string
			for(j = 0; j < 16-padSize; j++)
			{
				sprintf(outputPtr+(j*2), "%02X", outputBlock[j]);
			}
			outputPtr -= 32;
		}
		else
		{
			// store the result in the return string		
			for(j=0; j<16 ;j++)
			{
				sprintf(storeC+(j*2), "%02X", outputBlock[j]);
			}
			memcpy(outputPtr, storeC, 32);
			outputPtr -= 32;
		}
	}

	return output;
}

char* AES::CTRencrypt(int size, char* input)
{
	int i,j;								// loop itterators
	int fieldPad;						// inverse string pad size
	char temp[33];						// converter string
	char* inputPtr;					// points to cur loc in input string
	char* output;						// return string
	char* outputPtr;					// points to cur loc in output string

	// determine the number of blocks and the size of the padding
	padSize = 16-((size%32)/2);
	numBlocks = (size/32)+1;
	fieldPad = size%32;

	output = (char*)malloc(((numBlocks+1)*32)+1);
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
		// XOR the message with the IV
		for(j=0; j<16; j++)
		{
			inputBlock[j] = inputBlock[j] ^ IV[j];
		}
		// incrament IV
		for(j=15; j>=0; j--)
		{
			if(IV[j] == 255)
				IV[j] = 0;
			else
			{
				IV[j] += 1;
				break;
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
		inputBlock[j] = inputBlock[j] ^ IV[j];
	}
	AES_encrypt(inputBlock, outputBlock, &AESKey);
	
	// append the outputBlock to the output stinng
	for(j=0; j<16 ;j++)
	{
		sprintf(outputPtr+(j*2), "%02X", outputBlock[j]);
	}
	outputPtr += 32;
	*outputPtr = 0;

	return output;
}

char* AES::CTRdecrypt(int size, char* input)
{
	int i, j;							// loop itterators
	int padSize;						// extracted pad size
	int inputSize;						// Size of the cypher message
	int outputSize;					// Size of the resulting message
	int numBlocks;						// number of blocks
	char* inputPtr;					// ptr to the current pos in input string
	char* output;						// output string
	char* outputPtr;					// ptr to the current pos in the output string
	char temp[33];						// temp string
	char storeC[32];					// holds untill next block is deciphered
	unsigned char CTR[16];			// IV incramenter

	AES_set_decrypt_key(key, keySize, &AESKey);

	// get the size of the input and the number of blocks
	inputPtr = input;
	inputSize = strlen(input);
	numBlocks = inputSize / 32;

	// Extract the IV
	memcpy(temp, inputPtr, 32);
	temp[32] = 0;
	inputPtr += (inputSize - 32);
	for(i=15; i>=0; i--)
	{
		IV[i] = (unsigned char) strtol(temp+(i*2),NULL,16);
		temp[i*2] = 0;
	}
	memcpy(CTR,IV,16);

	// incrament CTR to extract last block
	if(255-CTR[15] < numBlocks-2)
	{
		CTR[15] = (CTR[15]+(numBlocks-2))%256;
		for(i=14; i>=0; i--)
		{
			if(CTR[i] == 255)
				CTR[i] = 0;
			else
			{
				CTR[i] += 1;
				break;
			}
		}
	}
	else
	{
		CTR[15] += numBlocks-2;
	}

	// copy the data
	memcpy(temp, inputPtr, 32);
	temp[32] = 0;
	inputPtr -= 32;
	// cast it to a hex value
	for(j=15; j>=0; j--)
	{
		inputBlock[j] = (unsigned char) strtol(temp+(j*2),NULL,16);
		temp[j*2] = 0;
	}

	// untill there is only one block left
	for(i=numBlocks-1; i > 0; i--)
	{
		// decrypt the data
		AES_decrypt(inputBlock, outputBlock, &AESKey);
		// now we can XOR it with the CTR
		for(j=0; j<16; j++)
		{
			outputBlock[j] = outputBlock[j] ^ CTR[j];
		}
		// Decrament the CTR
		for(j=15; j>=0; j--)
		{
			if(CTR[j] == 0)
				CTR[j] = 255;
			else
			{
				CTR[j] -= 1;
				break;
			}
		}
		// if this is the padded block
		if(i == numBlocks-1)
		{
			padSize = (int)outputBlock[15];
			if(padSize == 16)
			{
				outputSize = ((numBlocks-2) * 32)+1;
				output = (char*)malloc(outputSize);
			}
			else
			{
				outputSize = ((numBlocks-2)*32)+(32-(padSize*2))+1;
				output = (char*)malloc(outputSize);
			}
			output[outputSize-1] = 0;
			outputPtr = output + ((outputSize/32)*32);
			// copy the padded block into the return string
			for(j = 0; j < 16-padSize; j++)
			{
				sprintf(outputPtr+(j*2), "%02X", outputBlock[j]);
			}
			outputPtr -= 32;
		}
		else
		{
			// store the result in the return string		
			for(j=0; j<16 ;j++)
			{
				sprintf(storeC+(j*2), "%02X", outputBlock[j]);
			}
			memcpy(outputPtr, storeC, 32);
			outputPtr -= 32;
		}
		// and copy the next block of data
		memcpy(temp, inputPtr, 32);
		inputPtr -= 32;
		// and convert it to its hex value
		for(j=15; j>=0; j--)
		{
			inputBlock[j] = (unsigned char) strtol(temp+(j*2),NULL,16);
			temp[j*2] = 0;
		}
	}

	return output;
}
