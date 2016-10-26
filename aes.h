#include<string>
#include<openssl/aes.h>

#ifndef AES_H
#define AES_H

class AES
{
private:
	unsigned char IV[16];
	unsigned char key[33];
	AES_KEY AESKey;
public:
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
