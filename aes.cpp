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
cout << "post xor     ";
print_hex(inputBlock,16);
		}
		else
		{
			for(j=0; j<16; j++)
			{
				inputBlock[j] = inputBlock[j] ^ outputBlock[j];
			}
cout << "post xor     ";
print_hex(inputBlock,16);
		}
		AES_encrypt(inputBlock, outputBlock, &AESKey);
		// append the outputBlock to the output stinng
		for(j=0; j<16 ;j++)
		{
			sprintf(outputPtr+(j*2), "%02X", outputBlock[j]);
		}
cout << "cypher block ";
print_hex(outputBlock,16);
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
cout << "post xor     ";
print_hex(inputBlock,16);
	AES_encrypt(inputBlock, outputBlock, &AESKey);
	
	// append the outputBlock to the output stinng
	for(j=0; j<16 ;j++)
	{
		sprintf(outputPtr+(j*2), "%02X", outputBlock[j]);
	}
cout << "cypher block ";
print_hex(outputBlock,16);
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
cout << "cypher block " << temp << endl;
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
cout << "post xor     ";
print_hex(outputBlock, 16);
		// and copy the next block of data
		memcpy(temp, inputPtr, 32);
		inputPtr -= 32;
cout << "cypher block " << temp << endl;
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
//print_hex(outputBlock,16);
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
