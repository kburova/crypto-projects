#include<fstream>
#include<cstdio>
#include<iostream>
#include<cstring>
#include<openssl/bn.h>
#include<openssl/rand.h>
#include"rsa.h"

using namespace std;

void RSA_obj::RSAEncrypt(string fileName)
{
	cout << "Entering RSAEncrypt" << endl;
	openKeyFile(fileName);

	return;
}

void RSA_obj::RSADecrypt(string fileName)
{

	return;
}

void RSA_obj::openKeyFile(string& fileName)
{
	ifstream in;
	string temp;

	in.open(fileName.c_str(), in.in);
	if(in.fail())
	{
		cout << "Error: File does not exist" << endl;
		exit(0);
	}
	else
		cout << "success" << endl;
	
	
	return;
}
