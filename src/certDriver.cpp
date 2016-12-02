#include <iostream>
#include <fstream>
//#include <rsa.h>
#include "/Users/kseniaburova/Documents/school/Fall2016/cs483/Prog2/src/certGen.cpp"


using namespace std;

int main(int argc, char* argv[]) {

    string PK_file, SK_file, identity;


    cout << "Enter path to Public Key File" << endl;
    cin >> PK_file;
    cout << "Enter path to Secret Key File" << endl;
    cin >> SK_file;
    cout << endl;
    certGen(PK_file, SK_file, identity);
}


