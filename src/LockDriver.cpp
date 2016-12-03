#include <iostream>
#include <fstream>
#include "lock.h"

using namespace std;

int main(int argc, char** argv){


    string dir, PK, SK, SIG, userPK, userSIG;

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

    dirToLock dirObj(dir, PK, SK, SIG, userPK, userSIG);




    return 0;
}