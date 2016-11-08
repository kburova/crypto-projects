#include<cstdio>
#include<iostream>
#include<cstring>
#include<openssl/bn.h>
#include<openssl/rand.h>
#include"rsa.h"

using namespace std;

int main(int argc, char** argv)
{
	RSA_obj rsa;
	string fileName;

	fileName = "PK";

	rsa.RSAEncrypt(fileName);
	return 0;
}
