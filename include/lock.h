#ifndef LOCK_H_H
#define LOCK_H_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <openssl/bn.h>

using namespace std;

// INCLUDE KEY STRUCTURE TO HOLD ALL 3 PARAMS FOR EACH RSA KEY AT ONE PLACE
struct key{

    BIGNUM *k;
    BIGNUM *N;
    unsigned long int n;
    string identity;
};

class dirToLock{

protected:

    DIR *dir;
    string dirName;
    string fileForKeys;
    struct dirent *de;
    vector <string> dirFiles;

    //requesting party
    string uPKfile;
    key personPK;
    string personSig;

    //locking party
    string PKfile;
    string SKfile;
    key lockerPK;
    key lockerSK;
    string lockerSig;

    BIGNUM *encKey;
    BIGNUM *macKey;

public:

    dirToLock(string& d, string& PK, string& SK, string &S, string &uPK, string &uS);
    void readKeyFile(string& fileName, key &k);
    void readSigFile(string& fileName, string & Sig);
    void dirToStr(string &d, string & m);
    void verifyPKeys();
    void generateAESKeys();
    void encryptFiles();

    void lock();

};


class dirToUnlock{

protected:
    string dirName;

public:
    void unlock();

};

#endif //LOCK_H_H
