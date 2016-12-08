#include <iostream>
#include <openssl/bn.h>
#include <fstream>
#include <sstream>
#include "rsa.h"
#include "sig.h"

using namespace std;

void signCertificate(const string & PK_file, const string & CAsig){

    ifstream PK;
    ofstream SF;
    stringstream ss;
    string messageToSig;
    string sigFile;
            //= "/Users/kseniaburova/Documents/school/Fall2016/cs483/Prog2/files/signature.txt";
    string signature = "";

    cout << "Enter signature file: " <<endl;
    cin >> sigFile;

    PK.open(PK_file.c_str());
    if (PK.fail()){
        perror("Error: Could open certificate");
        exit(1);
    }
    SF.open(sigFile.c_str());
    if (SF.fail()){
        perror("Error: Could open file for signature");
        exit(1);
    }

    ss.clear();
    ss << PK.rdbuf();
    messageToSig = ss.str();
    PK.close();

    /*** call Signature function to sign public key file ***/
    sig(CAsig, messageToSig, signature);
    SF << signature;
    cout <<"Sig: "<< signature <<endl;
    SF.close();

}
