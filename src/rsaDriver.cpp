#include <iostream>
#include <fstream>
#include "rsa.h"

using namespace std;

int main(int argc, char** argv){

	int task;
	RSA_obj rsa;
	string PK_file, SK_file, text_file, cypher_file;
   string message;
   string cypher;
   ifstream PK, SK, in;
   ofstream out;
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
			cout << "Enter path to Cypher Text File" <<endl;
			cin >> cypher_file;
			cout<<endl;
         in.open(text_file.c_str(), in.in);
         if(in.fail())
         {
            cout << "Error: file does not exist." << endl;
            exit(0);
         }
         in >> message;

			rsa.RSAEncrypt(PK_file, message, cypher);
         out.open(cypher_file.c_str(), out.out);
         out << cypher;
         out.close();
			break;
		case 3:
			cout << "Enter path to Secret Key File" <<endl;
			cin >> SK_file;
			cout << "Enter path to Cypher Text File" <<endl;
			cin >> cypher_file;
			cout << "Enter path to Plain Text File" <<endl;
			cin >> text_file;
			cout<<endl;
         in.open(cypher_file.c_str(), in.in);
         if(in.fail())
         {
            cout << "Error: file does not exist." << endl;
            exit(0);
         }
         in >> cypher;

			rsa.RSADecrypt(SK_file, cypher, message);
         out.open(text_file.c_str(), out.out);
         out << message;
         out.close();
		break;
	}

	return 0;
}


