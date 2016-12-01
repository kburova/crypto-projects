#ifndef RSA_H
#define RSA_H

#include<string>
#include<openssl/bn.h>

using namespace std;

class RSA_obj
{
private:
// variables
	unsigned long n;						// Secutity parameter in bits
	BIGNUM N;								//
	BIGNUM key;
public:	
// functions
	RSA_obj();
	void RSAEncrypt(string, string, string&);
	void RSADecrypt(string, string, string&);
	void openKeyFile(string&);
};

void keyGen(string&, string&);


#endif
