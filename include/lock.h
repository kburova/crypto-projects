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


class dirToLock{

protected:

    DIR *dir;
    string dirName;
    string fileForKeys;
    struct dirent *de;
    vector <string> dirFiles;

    //requesting party
    string uPKfile;
    string personSig;
    string uSigFile;
    //locking party
    string PKfile;
    string SKfile;
    string SigFile;
    string lockerSig;

    BIGNUM *encKey;
    BIGNUM *macKey;

public:

    dirToLock(string& d, string& PK, string& SK, string &S, string &uPK, string &uS);
    void readSigFile(string& fileName, string & Sig);
    void dirToStr(string &d, string & m);
    void verifyPKeys();
    void generateAESKeys();
    void encryptFiles();
    void encryptKeysFile();
    void signFile(string & file);

};


class dirToUnlock{

protected:
    string dirName;

public:
    void unlock();

};

#endif //LOCK_H_H
