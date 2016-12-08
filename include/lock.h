#ifndef LOCK_H_H
#define LOCK_H_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <openssl/bn.h>
#include <set>

using namespace std;

class dirToLock{

protected:

    DIR *dir;
    string dirName;
    string fileForKeys;
    struct dirent *de;
    vector <string> dirFiles;

    //unlocking party
    string uPKfile;
    string personSig;
    string uSigFile;

    //locking party
    string PKfile;
    string SKfile;
    string SigFile;
    string lockerSig;

    //CAfiles
    string caSigFile;
    string caPKfile;
    string caSig;

    BIGNUM *encKey;
    BIGNUM *macKey;

public:

    dirToLock(string& d, string& PK, string& SK, string &S, string &uPK, string &uS, string &caPK, string &caSIG);
    void readSigFile(string& fileName, string & Sig);
    void dirToStr(string &d, string & m);
    void verifyPKeys();
    void generateAESKeys();
    void encryptFiles();
    void encryptKeysFile();
    void signFile(string & file);
    string strToHexStr(string & m);
    string hexStrToStr(string & hex);
};


class dirToUnlock{

protected:

    DIR *dir;
    string dirName;
    string fileForKeys;
    struct dirent *de;
    set <string> encFiles;
    set <string> tagFiles;

    //unlocking party
    string PKfile;
    string SKfile;
    string SigFile;
    string Sig;

    //locking party
    string lPKfile;
    string lSigFile;
    string lSig;

    //CA
    string caSigFile;
    string caPKfile;
    string caSig;

    BIGNUM *encKey;
    BIGNUM *macKey;

public:
    dirToUnlock(string& d, string& PK, string& SK, string &S);
    void reportIdentity();
    void readSigFile(string& fileName, string & Sig);
    void dirToStr(string &d, string & m);
    void verifyPKeys();
    void readAESKeys();
    void decryptFiles();
    void decryptKeysFile();
    void verifySharedKeys();
    string strToHexStr(string & m);
    string hexStrToStr(string & hex);
};

#endif //LOCK_H_H
