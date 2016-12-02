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
	string identity;
public:	
// functions
	RSA_obj();
	void RSAEncrypt(string, string, string&);
	void RSADecrypt(string, string, string&);
	void openPublicKeyFile(string&);
	void openSecretKeyFile(string&);
};

void keyGen(const string & PK_file,const string & SK_file,const string & identity, const string & CAsig = "");
int passFermatTest(BIGNUM *prime);
int signCertificate(const string & PK_file, const string & CAsig);

#endif
