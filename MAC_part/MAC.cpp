#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include "cryptopp/cryptlib.h"
#include <fstream>
#include <sstream>
#include "aes.cpp"
using namespace std;

void CBCGen(char** argv){

        fstream file;
        file.open(argv[1]);
        string k, m;
        
        unsigned char * key;
        unsigned char * message;
        
	// Read the key from the file //       
        file >> k;
        int size = k.size();
        key = (unsigned char *)malloc( sizeof(unsigned char)* size); 
        
        for (int i = 0; i < size; i = i + 2){
            unsigned char temp = atoi((k.substr(i,2)).c_str());
            cout << hex << temp;
        }

	// Read the message from the file //


        //aesEcb(key, temp);

}
bool HashMacVer(){
	return 0;
}
void HashMacGen(char** argv){

}
bool CBCVer(){
	return 1;
}


int main(int argc, char** argv){

	int mode, task;

	cout << "Select CBC-MAC or Hash-and-MAC" << endl
		<<"Type 1 for CBC-MAC or 2 for Hash-and-MAC : ";
	cin >> mode;
	cout << "Type 1 Tag Generation or type 2 for for Verification : ";
	cin >> task;

	if (mode == 1 and task == 1)
		CBCGen(argv);
	else if ( mode == 1 and task == 2)
		CBCVer();
	else if ( mode == 2 and task == 1)
		HashMacGen(argv);
	else
		HashMacVer();

	return 0;
}
