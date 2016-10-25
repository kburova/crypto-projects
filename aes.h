#include<string>
#include<openssl/aes.h>

#ifndef AES_H
#define AES_H

using std::string;

class AES
{
private:
	unsigned char IV[16];
	AES_KEY AESKey;
public:
	unsigned char key[33];
	int keySize;
	unsigned char message[16];
	unsigned char message2[16];
	unsigned char cypher[16];
	AES();
	void setKey(int, char*);
	void CBCencrypt();
	void CBCdecrypt();
	void CTRencrypt();
	void CTRdecrypt();
};



#endif
