#include <iostream>
#include <fstream>
#include <sstream>
#include "lock.h"
#include "sig.h"


dirToLock::dirToLock(string& d, string& PK, string& SK, string &S, string &uPK, string &uS){

    char *s;
    struct stat buf;
    ifstream file;

    // INITIALIZE POINTERS IN CLASS
    dirName = d;
    PKfile = PK;
    SKfile = SK;
    uPKfile = uPK;



    // open directory and save all files (names) to vector for ease of implementation

    s = (char *) malloc(sizeof(char)*(strlen(d.c_str())+258));
    dir = opendir( d.c_str() );
    if (dir == NULL) {
        perror("Couldn't open directory to read files");
        exit(1);
    }
    for (de = readdir(dir); de != NULL; de = readdir(dir)){

        sprintf(s, "%s/%s", d.c_str(), de->d_name);
        lstat(s, &buf);

        if (! S_ISDIR(buf.st_mode) ) {
            dirFiles.push_back(s);
        }
    }

    // START OPENING ALL FILES AND STORE KEYS INFORMATION INTO CLASS
    readKeyFile(PK, lockerPK);
    readKeyFile(SK, lockerSK);
    readSigFile(S, lockerSig);

    readKeyFile(uPK, personPK);
    readSigFile(uS, personSig);
}

void dirToLock::generateAESKeys() {

    string fileForKeys = dirName + "/SharedKeys";
    BIGNUM *encKey = BN_new();
    BIGNUM *macKey = BN_new();

    BN_rand(encKey, 256, 0, 1);
    BN_rand(macKey, 256, 0, 1);
}
void dirToLock::dirToStr(string &d, string &m){

    ifstream PK;
    stringstream ss;
    PK.open(d);
    if (PK.fail()){
        perror("Error: Could open certificate");
        exit(1);
    }

    ss.clear();
    ss << PK.rdbuf();
    m = ss.str();

    PK.close();
}
void dirToLock::verifyPKeys(){

    bool client = false;
    bool locker = false;
    string message;

    dirToStr(uPKfile, message);
    client = verify(uPKfile, personSig ,message);
    if (!client) {
        cerr << "Requesting party is not verified"<< endl;
    }
    dirToStr(PKfile, message);
    locker = verify(PKfile, lockerSig, message);
    if (!locker){
        cerr << "Locking party is not verified"<< endl;
    }

    if (! (client && locker) ){
        cerr << "Aborting..." << endl;
        exit(1);
    }else{
        cout << "Both party verified successfully..." <<endl;
    }
}
void dirToLock::lock(){

}

void dirToLock::readKeyFile(string& fileName, key &k) {
    int i, size;
    ifstream in;
    string temp;
    unsigned char *convert1;
    char *convert2;

    in.open(fileName.c_str(), in.in);
    if(in.fail()) {
        fprintf(stderr, "Couldn't open key file -- %s\n", fileName.c_str());
        exit(1);
    }

    while (getline(in, temp)) {
        /*** get N ***/
        if (temp == "N:") {

            getline(in,temp);
            convert1 = (unsigned char *) malloc(temp.size() / 2);
            convert2 = (char *) malloc(temp.size() + 1);
            strcpy(convert2, temp.c_str());
            if (temp.size() % 2) {
                size = (temp.size() + 1) / 2;
                for (i = size - 1; i > 0; i--) {
                    convert1[i] = (unsigned char) strtol(convert2 + (i * 2 - 1), NULL, 16);
                    convert2[i * 2 - 1] = 0;
                }
                convert1[0] = (unsigned char) strtol(convert2, NULL, 16);
            } else {
                size = temp.size() / 2;
                for (i = size - 1; i >= 0; i--) {
                    convert1[i] = (unsigned char) strtol(convert2 + (i * 2), NULL, 16);
                    convert2[i * 2] = 0;
                }
            }
            k.N = BN_new();
            BN_bin2bn(convert1, size, k.N);

        }else if (temp == "Key:") {
            /*** get Key ***/
            getline(in,temp);
            strcpy(convert2, temp.c_str());
            if (temp.size() % 2) {
                size = (temp.size() + 1) / 2;
                for (i = size - 1; i > 0; i--) {
                    convert1[i] = (unsigned char) strtol(convert2 + (i * 2 - 1), NULL, 16);
                    convert2[i * 2 - 1] = 0;
                }
                convert1[0] = (unsigned char) strtol(convert2, NULL, 16);
            } else {
                size = temp.size() / 2;
                for (i = size - 1; i >= 0; i--) {
                    convert1[i] = (unsigned char) strtol(convert2 + (i * 2), NULL, 16);
                    convert2[i * 2] = 0;
                }
            }
            k.k = BN_new();
            BN_bin2bn(convert1, size, k.k);

        }else if (temp == "Bits:") {
            /*** get n: ***/
            getline(in,temp);
            k.n = strtol(temp.c_str(), NULL, 16);

        }else if (temp == "Identity:"){

            /*** read an identity ***/
            getline(in,temp);
            k.identity = temp;
        }
    }
    in.close();
    //free(convert1);
    //free(convert2);
}

void dirToLock::readSigFile(string& fileName, string & Sig) {
    ifstream in;

    in.open(fileName.c_str(), in.in);
    if (in.fail()) {
        fprintf(stderr, "Couldn't open signature file -- %s\n", fileName.c_str());
        exit(1);
    }
    in >> Sig;

    in.close();
}