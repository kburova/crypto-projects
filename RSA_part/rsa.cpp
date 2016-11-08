#include<fstream>
#include<cstdio>
#include<iostream>
#include<cstring>
#include<openssl/bn.h>
#include<openssl/rand.h>
#include"rsa.h"

using namespace std;

RSA_obj::RSA_obj()
{
	n = 0;
	BN_init(&N);
	BN_init(&key);
	BN_zero(&N);
	BN_zero(&key);
}

void RSA_obj::RSAEncrypt(string keyFile, string messageFile, string cypherFile)
{
	string temp;
	char* mPtr;
	char* mCpy;
	unsigned long mBlockSize;
	unsigned long mSize;
	unsigned long numBlocks;
	ifstream in;
	ofstream out;

	cout << "Entering RSAEncrypt" << endl;
	openKeyFile(keyFile);

	in.open(messageFile.c_str(), in.in);
	if(in.fail())
	{
		cout << "Error: file does not exist." << endl;
		exit(0);
	}
	in >> temp;

	mSize = temp.size()/2;
	mBlockSize = (n/16) - 3;
	numBlocks = (mSize/mBlockSize)+1;
	cout << "mSize " << mSize << endl;
	cout << "mBlockSize " << mBlockSize << endl;
	cout << "numBlocks " << numBlocks << endl;
	cout << temp << endl;

	return;
}

void RSA_obj::RSADecrypt(string keyFile, string cypherFile, string messageFile)
{

	return;
}

void RSA_obj::openKeyFile(string& fileName)
{
	int i;							// loop itterator
	int size;						// size of the conversion string
	ifstream in;					// file stream
	string temp;					// extracts data from file
	unsigned char *convert1;	// used to convert string to hex
	char *convert2;				// used to convert string to hex

	in.open(fileName.c_str(), in.in);
	if(in.fail())
	{
		cout << "Error: File does not exist" << endl;
		exit(0);
	}
	
	in >> temp;
	convert1 = (unsigned char*)malloc(temp.size()/2);
	convert2 = (char*)malloc(temp.size()+1);
	strcpy(convert2, temp.c_str());
	if(temp.size()%2)
	{
		size = (temp.size()+1)/2;
		for(i = size-1; i > 0; i--)
		{   
			convert1[i] = (unsigned char) strtol(convert2+(i*2-1), NULL, 16);
			convert2[i*2-1] = 0;
		}   
		convert1[0] = (unsigned char) strtol(convert2, NULL, 16);
	}
	else
	{
		size = temp.size()/2;
		for(i = size-1; i >= 0; i--)
		{   
			convert1[i] = (unsigned char) strtol(convert2+(i*2),NULL,16);
			convert2[i*2] = 0;
		}   
	}
	BN_bin2bn(convert1, size, &N);

	in >> temp;
	strcpy(convert2, temp.c_str());
	if(temp.size()%2)
	{
		size = (temp.size()+1)/2;
		for(i = size-1; i > 0; i--)
		{   
			convert1[i] = (unsigned char) strtol(convert2+(i*2-1), NULL, 16);
			convert2[i*2-1] = 0;
		}   
		convert1[0] = (unsigned char) strtol(convert2, NULL, 16);
	}
	else
	{
		size = temp.size()/2;
		for(i = size-1; i >= 0; i--)
		{   
			convert1[i] = (unsigned char) strtol(convert2+(i*2),NULL,16);
			convert2[i*2] = 0;
		}   
	}
	BN_bin2bn(convert1, size, &key);

	in >> temp;
	n = strtol(temp.c_str(), NULL, 16);
	cout << n << endl;

/*
	strcpy(convert2, temp.c_str());
	if(temp.size()%2)
	{
		size = (temp.size()+1)/2;
		for(i = size-1; i > 0; i--)
		{   
			convert1[i] = (unsigned char) strtol(convert2+(i*2-1), NULL, 16);
			convert2[i*2-1] = 0;
		}   
		convert1[0] = (unsigned char) strtol(convert2, NULL, 16);
	}
	else
	{
		size = temp.size()/2;
		for(i = size-1; i >= 0; i--)
		{   
			convert1[i] = (unsigned char) strtol(convert2+(i*2),NULL,16);
			convert2[i*2] = 0;
		}   
	}
	BN_bin2bn(convert1, size, &n);
*/

/********** TESTING ******************
	for(int z = 0; z< size; z++)
	{
		printf("%X", convert1[z]/16);
		printf("%X", convert1[z]%16);
	}
	printf("\n");
/********** TESTING ******************/

/********** TESTING ******************
	cout << "N: ";
	BN_print_fp(stdout, &N);
	cout << endl << endl << "key: ";
	BN_print_fp(stdout, &key);
	cout << endl << endl << "n: ";
	BN_print_fp(stdout, &n);
/********** TESTING ******************/
	free(convert1);
	free(convert2);

	return;
}
