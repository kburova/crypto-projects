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
    struct dirent *de;
    vector <string> dirFiles;

    key *personPK;
    BIGNUM *personSig;

    key *lockerPK;
    key *lockerSK;
    BIGNUM *lockerSig;

public:

    dirToLock(string& d, string& PK, string& SK, string &S, string &uPK, string &uS);
    void lock();

};


class dirToUnlock{

protected:
    string dirName;

public:
    void unlock();

};

#endif //LOCK_H_H
