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

	string key;
	string message;
	string cypher;

	key = "PK";
	message = "message.txt";
	cypher = "cypher.txt";
	

	rsa.RSAEncrypt(key, message, cypher);
	return 0;
}
