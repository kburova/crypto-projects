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
    encKey = BN_new();
    macKey = BN_new();

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
    system(tmp.c_str());
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
   // cout << SKfile << endl;
    //create SharedKeys file
    encKeys <<  hexStrToStr(message);
    encKeys.close();

    readAESKeys();

    string temp = "rm " + fileForKeys;
    system(temp.c_str());
    temp = "rm " + keyFile;
    system(temp.c_str());
    temp = "rm " + lPKfile;
    system(temp.c_str());
    temp = "rm " + lSigFile;
    system(temp.c_str());
    temp = "rm " + caPKfile;
    system(temp.c_str());
    temp = "rm " +caSigFile;
    system(temp.c_str());
}
void dirToUnlock::decryptFiles(){

    char *eKey;
    char *s;
    char *out;
    char * messForDec;
    struct stat buf;
    ifstream enc,tag;
    ofstream orig;
    string temp;
    string mkey( BN_bn2hex(macKey) );
    eKey =  BN_bn2hex(encKey);
    AES aes;
    aes.setKey(eKey);

    // open directory
    s = (char *) malloc(sizeof(char)*(strlen(dirName.c_str())+258));
    dir = opendir( dirName.c_str() );
    if (dir == NULL) {
        perror("Couldn't open directory to read files");
        exit(1);
    }

    for (de = readdir(dir); de != NULL; de = readdir(dir)){

        sprintf(s, "%s/%s", dirName.c_str(), de->d_name);
        lstat(s, &buf);

        //avoid encrypting ./ and ../
        if (! S_ISDIR(buf.st_mode) ) {
            string f(de->d_name);
            if ( f.substr(f.size()-4) == ".enc" && f.substr(0,f.size()-4) != "SharedKeys"){
                encFiles.insert(f);
            }else if ( f.substr(f.size()-4) == ".tag" ){
                tagFiles.insert(f);
            }
        }
    }
    closedir(dir);

    //1. Iterate through all files and store .enc in .tag in appropriate data structures.
    for ( string encFile : encFiles){
        //2. For each .enc file find .tag one and verify
        //      If verified - decrypt and remove .enc and .tag files
        string origFile = encFile.substr( 0,encFile.size() - 4);
        string tagFile = origFile + ".tag";
        //cout << "Enc file: " << origFile <<" tagFile: "<<tagFile<<endl;
            if (tagFiles.find( origFile + ".tag" ) == tagFiles.end() ){
                fprintf(stderr,"There are no matching tag file for %s", encFile.c_str());
                exit(1);
            }
        temp = dirName + "/" + encFile;
        enc.open(temp.c_str());
        if (enc.fail()) {
            fprintf(stderr, "Couldn't open encrypted file-- %s\n", encFile.c_str());
            exit(1);
        }
        temp = dirName + "/" + tagFile;
        tag.open(temp.c_str());
        if (tag.fail()) {
            fprintf(stderr, "Couldn't open tag file -- %s\n", tagFile.c_str());
            exit(1);
        }
        temp = dirName + "/" + origFile;
        orig.open(temp.c_str());
        if (orig.fail()){
            fprintf(stderr, "Couldn't open file for original content -- %s\n", origFile.c_str());
            exit(1);
        }

        string message, t, mTag = "";
        enc >> message;
        tag >> t;

        messForDec = (char*)malloc(sizeof(char) * message.size() + 1);
        memcpy(messForDec, message.c_str(), message.size());
        messForDec[message.size()] = '\0';

        // Verify tag
        for (int i = 0; i < t.size()/2 ; i++){
            temp = t.substr(i*2,2);
            mTag += strtol(temp.c_str(),NULL,16);
        }
        int size = message.size();
        if (HashMacVer( mkey,message, size, mTag)){
            printf("%s and %s are verified\n", tagFile.c_str() , encFile.c_str());
        }else{
            printf("%s and %s are NOT verified\n", tagFile.c_str() , encFile.c_str());
            exit(1);
        }

        cout << "decrypt " <<origFile << endl;
        // decrypt file
        out = aes.CBCdecrypt(messForDec);
        orig << out;

        free(messForDec);
        free(out);

        tag.close();
        enc.close();
        orig.close();

        temp = "rm " + dirName+ "/" + origFile + ".tag";
        system(temp.c_str());
        temp = "rm " + dirName+ "/" + origFile + ".enc";
        system(temp.c_str());
    }

    free (eKey);
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
    cout << endl;
}
void dirToUnlock::verifyPKeys(){

    bool client = false;
    bool locker = false;
    bool CA = false;
    string message,temp;

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

    reportIdentity();
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

string dirToUnlock::strToHexStr(string & m){

    char temp[3];
    stringstream ss;
    string hexString = "";

    ss.clear();
    for (int i = 0; i < m.size(); i++){
        sprintf( temp, "%02x",  (unsigned char)(unsigned int)m[i] );
        ss << temp;
    }
    hexString = ss.str();

    return hexString;
}

string dirToUnlock::hexStrToStr(string & hex){

    string temp, m = "";

    for (int i = 0; i < hex.size()/2; i++){
        temp = hex.substr(i*2,2);
        m += strtol(temp.c_str(),NULL,16);
    }

    return m;
}

void  dirToUnlock::readAESKeys() {

    int i;                            // loop itterator
    int size;                        // size of the conversion string
    ifstream in;                    // file stream
    string temp;                    // extracts data from file
    unsigned char *convert1;    // used to convert string to hex
    char *convert2;                // used to convert string to hex
    string fileName = dirName +"/SharedKeys";
    in.open(fileName.c_str(), in.in);
    if (in.fail()) {
        cout << "Error: File does not exist" << endl;
        exit(0);
    }

    while (getline(in, temp)) {

        if (temp == "---Encryption key---") {
            getline(in, temp);
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
            BN_bin2bn(convert1, size, encKey);

        } else if (temp == "---Macing key---") {

            getline(in, temp);
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
            BN_bin2bn(convert1, size, macKey);
        }
        //free(convert1);
        //free(convert2);
    }
    cout << "Shared Keys read..."<<endl;
    in.close();
}