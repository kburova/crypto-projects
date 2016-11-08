#ifndef RSA_H
#define RSA_H

#include<string>

using namespace std;

class RSA_obj
{
public:
// variables
	
// functions
	void RSAEncrypt(string);
	void RSADecrypt(string);
	void openKeyFile(string&);
};

void keyGen(string&, string&);


#endif
