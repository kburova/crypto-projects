#include <iostream>
#include <fstream>
#include "lock.h"

using namespace std;

int main(int argc, char** argv){


    string dir, PK, SK, SIG, userPK, userSIG;
    ifstream in("/Users/kseniaburova/Documents/school/Fall2016/cs483/Prog2/input");
    cin.rdbuf(in.rdbuf());

    cout << "Enter path for directory to lock" << endl;
    cin >> dir;
    cout << "Enter path for locking party Public Key" << endl;
    cin >> PK;
    cout << "Enter path for locking party Secret Key" << endl;
    cin >> SK;
    cout << "Enter path for locking party signature" << endl;
    cin >> SIG;

    cout << "Enter path for user's Public Key" << endl;
    cin >> userPK;
    cout << "Enter path for user's signature" << endl;
    cin >> userSIG;

//    cout << PK <<endl
//         << SK <<endl
//         << SIG << endl
//         << userPK <<endl
//         << userSIG <<endl;

    cout <<endl;
    dirToLock dirObj(dir, PK, SK, SIG, userPK, userSIG);
    dirObj.verifyPKeys();
    dirObj.generateAESKeys();
    dirObj.encryptFiles();

    return 0;
}
