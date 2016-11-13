#include <iostream>
#include <fstream>
#include "rsa.h"

using namespace std;

int main(int argc, char** argv){

	int task;
	RSA_obj rsa;
	string PK_file, SK_file, text_file, cipher_file;
	fstream PK, SK, Text, Cipher;
	cout << "Choose a task: \n"
		<< "1  -  Key Generation\n"
		<< "2  -  RSA Encryption\n"
		<< "3  -  RSA Decryption\n" << endl;
	cin >> task;

	switch (task){
		case 1:
			cout << "Enter path to Public Key File" <<endl;
			cin >> PK_file;
			cout << "Enter path to Secret Key File" <<endl;
			cin >> SK_file;
			cout<<endl;
			keyGen(PK_file, SK_file);
			break;
		case 2:
			cout << "Enter path to Public Key File" <<endl;
			cin >> PK_file;
			cout << "Enter path to Plain Text File" <<endl;
			cin >> text_file;
			cout << "Enter path to Cipher Text File" <<endl;
			cin >> cipher_file;
			cout<<endl;
			rsa.RSAEncrypt(PK_file, text_file, cipher_file);
			break;
		case 3:
			cout << "Enter path to Secret Key File" <<endl;
			cin >> SK_file;
			cout << "Enter path to Plain Text File" <<endl;
			cin >> text_file;
			cout << "Enter path to Cipher Text File" <<endl;
			cin >> cipher_file;
			cout<<endl;
			rsa.RSADecrypt(SK_file, cipher_file, text_file);
		break;
	}


	return 0;
}

