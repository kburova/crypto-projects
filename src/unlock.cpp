#include <iostream>
#include <fstream>
#include <sstream>
#include "lock.h"
#include "sig.h"
#include "aes.h"
#include "mac.h"
#include "rsa.h"

dirToUnlock::dirToUnlock(string& d, string& pk, string& sk, string &sg){

    char *s;
    struct stat buf;
    ifstream file;

    // INITIALIZE POINTERS IN CLASS
    dirName = d;
    PKfile = pk;
    SKfile = sk;
    SigFile = sg;

    //locking party
    lPKfile = d + "/LockPK.txt" ;
    lSigFile = d + "/LockSig.txt";

    //CA
    caSigFile = d + "/CAsig.txt";
    caPKfile = d + "/CApk.txt";

    // Store signature
    readSigFile(sg, Sig);
    readSigFile(lSigFile, lSig);
    readSigFile(caSigFile, caSig);
}


void dirToUnlock::verifySharedKeys() {

    string message, signature, keyFile, keySigFile;

    keyFile = dirName + "/SharedKeys.enc";
    keySigFile = dirName + "/SharedKeys.enc.sig";

    dirToStr(keyFile,message);
    readSigFile(keySigFile,signature);

    if ( verify(lPKfile,signature,message) ){
        cout << "SharedKeys.enc is verified..." << endl;
    }else{
        cerr << "SharedKeys.enc is NOT verified..."<<endl;
        exit(1);
    }

    string tmp = "rm " + keySigFile;
    //system(tmp.c_str());
}


void dirToUnlock::decryptKeysFile() {

    RSA_obj enc;
    ofstream encKeys;
    string message, cypher;
    string keyFile;

    keyFile = dirName + "/SharedKeys.enc";
    fileForKeys = dirName + "/SharedKeys";

    encKeys.open(fileForKeys);
    if (encKeys.fail() ){
        fprintf(stderr, "Failed to open for shared keys\n");
        exit(1);
    }

    // decrypt file
    dirToStr(keyFile, cypher);
    enc.RSADecrypt(SKfile, cypher, message);
    cout << SKfile << endl;
    cout << "Message: "<<message<<endl;

    //create SharedKeys file
    encKeys << message;
    encKeys.close();

}
void dirToUnlock::decryptFiles(){


}
void dirToUnlock::dirToStr(string &d, string &m){

    ifstream PK;
    stringstream ss;
    PK.open(d);
    if (PK.fail()){
        perror("Error: Couldn't open certificate");
        exit(1);
    }

    ss.clear();
    ss << PK.rdbuf();
    m = ss.str();

    PK.close();
}
void dirToUnlock::reportIdentity(){
    ifstream lpk, capk;
    string line;

    printf("Locker party identity is...");
    lpk.open(lPKfile);
    if (lpk.fail()){
        cerr << "Couldn't open locking party certificate"<<endl;
        exit(1);
    }
    while(getline(lpk,line)){
        if (line =="Identity:"){
            getline(lpk,line);
            printf("%s\n", line.c_str());
        }
    }
    lpk.close();

    printf("CA identity is...");
    capk.open(caPKfile);
    if (capk.fail()){
        cerr << "Couldn't open CA certificate"<<endl;
        exit(1);
    }
    while(getline(capk,line)){
        if (line =="Identity:"){
            getline(capk,line);
            printf("%s\n", line.c_str());
        }
    }
    capk.close();
    cout <<endl;
}
void dirToUnlock::verifyPKeys(){

    bool client = false;
    bool locker = false;
    bool CA = false;
    string message;

    printf("Verifying CA...");
    dirToStr(caPKfile, message);
    CA = verify(caPKfile, caSig , message);
    if (!CA) {
        cerr << "CA is not verified"<< endl;
    }

    printf("Verifying unlocking party...\n");
    dirToStr(PKfile, message);
    client = verify(caPKfile, Sig ,message);
    if (!client) {
        cerr << "Unlocking party is not verified"<< endl;
    }

    printf("Verifying locking party...\n");
    dirToStr(lPKfile, message);
    locker = verify(caPKfile, lSig, message);
    if (!locker){
        cerr << "Locking party is not verified"<< endl;
    }

    if (! (client && locker && CA) ){
        cerr << "Aborting..." << endl;
        exit(1);
    }else{
        cout << "All parties verified successfully..." <<endl<<endl;
    }

    // need to delete files here instead
//    string temp = "cp " + PKfile + " " + dirName;
//    system(temp.c_str());
//    temp = "cp " + SigFile + " " + dirName;
//    system(temp.c_str());
}

void dirToUnlock::readSigFile(string& fileName, string & Sig) {
    ifstream in;

    in.open(fileName.c_str(), in.in);
    if (in.fail()) {
        fprintf(stderr, "Couldn't open signature file -- %s\n", fileName.c_str());
        exit(1);
    }
    in >> Sig;

    in.close();
}

