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
	int i, j;									// loop iterattor
	int padSize;								// pad Size
	int lBlockSize;							// size of last block
	int cypherSize;							// size of the cypher string
	int testSize;								// tests the size of the output string
	char* mPtr;									// ptr inside msessage cpy
	char* mCpy;									// copy of the message
	char* mBlock;								// copy of the message block t/b encrypted
	char* cypher;								// cypher to be writen
	char* cPtr;									// pointer inside of cypher
	char* cCatch;								// catches the string returned by bin2hex
	unsigned long mBlockSize;				// message block size
	unsigned long mSize;						// size of the message
	unsigned long numBlocks;				// number of block the cypher will contain
	unsigned char* mCon;						// message converter
	unsigned char* mConPtr;					// pointer inside of mCon
	unsigned char* randomness;				// randomness for the cypher
	string temp;								// extracts the message from the file
	ifstream in;								// stream to the message file
	ofstream out;								// stream to the cypher file
	BIGNUM m;									// message block in BIGNUM form;
	BIGNUM c;									// cypher block in BIGNUM form;
	BIGNUM t;									// temp BIGNUM
	BN_CTX* ctx;								// not realy sure what this is for;

	// initalize BIGNUM objs
	BN_init(&m);
	BN_init(&c);
	BN_init(&t);
	ctx = BN_CTX_new();

	// retreave all the key information and store them in the class
	openKeyFile(keyFile);

	// determine size of the cypher block string;
	cCatch = BN_bn2hex(&N);
	cypherSize = strlen(cCatch);
	OPENSSL_free(cCatch);

	// retreave the message
	in.open(messageFile.c_str(), in.in);
	if(in.fail())
	{
		cout << "Error: file does not exist." << endl;
		exit(0);
	}
	in >> temp;

	// determine the size of everything
	mSize = temp.size()/2;
	mBlockSize = (n/16) - 3;
	numBlocks = (mSize/mBlockSize)+1;
	lBlockSize = (mSize-((mSize/mBlockSize)*mBlockSize))*2;
	padSize = mBlockSize-(mSize-((mSize/mBlockSize)*mBlockSize));

/**************************************************
	cout << "mSize\t\t" << mSize << endl;
	cout << "mBlockSize\t" << mBlockSize << endl;
	cout << "numBlocks\t" << numBlocks << endl;
	cout << "lBlockSize\t" << lBlockSize << endl;
	cout << "padSize\t\t" << padSize << endl;
	cout << temp << endl << endl;
/**************************************************/

	// allocate all the dynamic memory
	mCpy = (char*)malloc(temp.size()+1);
	mBlock = (char*)malloc(mBlockSize*2 +1);
	mCon = (unsigned char*)malloc((n/8)+1);
	cypher = (char*)malloc((cypherSize*numBlocks)+1);
	randomness = (unsigned char*)malloc(mBlockSize);

	// set string itterators
	mPtr = mCpy;
	cPtr = cypher;
	mConPtr = mCon;

	strcpy(mCpy,temp.c_str());

	// copy the message blocks, get some randomness, and build the string that
	// gets converted to a BIGNUM.  Do the math on it and convert it back to a
	// hex string
	for(i=0; i<numBlocks-1; i++)

	{
		memcpy(mBlock, mPtr, mBlockSize*2);
		mBlock[mBlockSize*2]=0;
		mPtr += mBlockSize*2;
//		cout << mBlock << endl;

		RAND_bytes(randomness, n/16);

		// 0||2||randomness||0
		*mConPtr = 0;
		mConPtr++;
		*mConPtr = 2;
		mConPtr++;
		memcpy(mConPtr, randomness, n/16);
		mConPtr += n/16;
		*mConPtr = 0;
		mConPtr++;

		// 0||2||randomness||0||messageBlock
		for(j = mBlockSize-1; j >= 0; j--)
		{   
			mConPtr[j] = (unsigned char) strtol(mBlock+(j*2),NULL,16);
			mBlock[j*2] = 0;
		}   
		mConPtr = mCon;

		// convert to BN and do the math then back to hex string;
		BN_bin2bn(mCon, n/8, &m);
		BN_mod_exp(&c, &m, &key, &N, ctx);
		cCatch = BN_bn2hex(&c);
		testSize = cypherSize - strlen(cCatch);

		// make sure all blocks are the same size
		if(testSize)
		{
			for(j=0;j<testSize;j++)
			{
				cPtr[j] = 0;
			}
			memcpy(cPtr+testSize, cCatch, cypherSize-testSize);
			cPtr += cypherSize;
		}
		else
		{
			memcpy(cPtr, cCatch, cypherSize);
			cPtr += cypherSize;
		}
		OPENSSL_free(cCatch);
/***********************************************
		cout << "length  " << strlen(cCatch) << endl;
		cout << cCatch << endl;
/***********************************************/
/***********************************************
		for(int z=0; z< n/8; z++)
		{
			printf("%X", mCon[z]/16);
			printf("%X", mCon[z]%16);
		}
		cout << endl;
		BN_print_fp(stdout, &m);
		cout << endl;
/***********************************************/
	}
	memcpy(mBlock, mPtr, lBlockSize);
	mBlock[lBlockSize]=0;
//	cout << mBlock << endl << endl;

	RAND_bytes(randomness, mBlockSize);

	*mConPtr = 0;
	mConPtr++;
	*mConPtr = 2;
	mConPtr++;
	memcpy(mConPtr, randomness, n/16);
	mConPtr += n/16;
	*mConPtr = 0;
	mConPtr++;

	for(j = (lBlockSize/2)-1; j >= 0; j--)
	{   
		mConPtr[j] = (unsigned char) strtol(mBlock+(j*2),NULL,16);
		mBlock[j*2] = 0;
	}   
	mConPtr += lBlockSize/2;
	memset(mConPtr,0,padSize);
	mCon[(n/8)-1]=padSize;

	BN_bin2bn(mCon, n/8, &m);
	BN_mod_exp(&c, &m, &key, &N, ctx);
	cCatch = BN_bn2hex(&c);
	testSize = cypherSize - strlen(cCatch);

	// make sure all blocks are the same size
	if(testSize)
	{
		for(j=0;j<testSize;j++)
		{
			cPtr[j] = 0;
		}
		memcpy(cPtr+testSize, cCatch, cypherSize-testSize);
		cPtr += cypherSize;
	}
	else
	{
		memcpy(cPtr, cCatch, cypherSize);
		cPtr += cypherSize;
	}
	OPENSSL_free(cCatch);
/***********************************************
	for(int z=0; z< n/8; z++)
	{
		printf("%X", mCon[z]/16);
		printf("%X", mCon[z]%16);
	}
	cout << endl;
	BN_print_fp(stdout, &m);
	cout << endl;
/***********************************************/

	out.open(cypherFile.c_str(), out.out);
	out << cypher;
	out.close();



	free(mCon);
	free(randomness);
	free(mBlock);
	free(mCpy);
	free(cypher);
	BN_free(&m);
	BN_free(&c);
	BN_free(&t);
	BN_CTX_free(ctx);
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
