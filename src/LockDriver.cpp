#include <iostream>
#include <fstream>
#include "lock.h"

using namespace std;

int main(int argc, char** argv){


    string dir, PK, SK, SIG, userPK, userSIG, caPK,caSIG;
    bool action;

    cout << "Do you lock or unlock directory? Type 1 (lock) or 0 (unlock)" << endl;
    cin >> action;

    if (action) {
        //ifstream in("/Users/kseniaburova/Documents/school/Fall2016/cs483/Prog2/input");
        ifstream in("input2");
        cin.rdbuf(in.rdbuf());

        cout << "Enter path for directory to lock" << endl;
        cin >> dir;
        cout << "Enter path for locking party Public Key" << endl;
        cin >> PK;
        cout << "Enter path for locking party Secret Key" << endl;
        cin >> SK;
        cout << "Enter path for locking party signature" << endl;
        cin >> SIG;

        cout << "Enter path for unlocking party Public Key" << endl;
        cin >> userPK;
        cout << "Enter path for unlocking party signature" << endl;
        cin >> userSIG;

        cout << "Enter path for CA's Public Key" << endl;
        cin >> caPK;
        cout << "Enter path for CA's signature" << endl;
        cin >> caSIG;
        cout << endl;

        dirToLock dirObj(dir, PK, SK, SIG, userPK, userSIG, caPK, caSIG);
        dirObj.verifyPKeys();
        dirObj.generateAESKeys();
        dirObj.encryptFiles();

    }else{
        //ifstream in("/Users/kseniaburova/Documents/school/Fall2016/cs483/Prog2/unlockInput");
        ifstream in("unlockInput2");
        cin.rdbuf(in.rdbuf());
        cout << "Enter path for directory to unlock" << endl;
        cin >> dir;
        cout << "Enter path for unlocking party Public Key" << endl;
        cin >> PK;
        cout << "Enter path for unlocking party Secret Key" << endl;
        cin >> SK;
        cout << "Enter path for unlocking party signature" << endl;
        cin >> SIG;
        cout<<endl;

        dirToUnlock dirObj(dir, PK, SK, SIG);
        dirObj.verifyPKeys();
        dirObj.verifySharedKeys();
        dirObj.decryptKeysFile();
        dirObj.decryptFiles();
    }

    return 0;
}
