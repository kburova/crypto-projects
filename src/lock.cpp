#include <iostream>
#include <fstream>
#include <sstream>
#include "lock.h"
#include "sig.h"
#include "aes.h"
#include "mac.h"
#include "rsa.h"

dirToLock::dirToLock(string& d, string& PK, string& SK, string &S, string &uPK, string &uS, string &caPK, string &caSIG){

    char *s;
    struct stat buf;
    ifstream file;

    // INITIALIZE POINTERS IN CLASS
    dirName = d;
    PKfile = PK;
    SKfile = SK;
    uPKfile = uPK;
    SigFile = S;
    uSigFile = uS;
    caSigFile = caSIG;
    caPKfile = caPK;

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

        //avoid encrypting ./ and ../
        if (! S_ISDIR(buf.st_mode) ) {
            dirFiles.push_back(de->d_name);
        }
    }
    // Store signatures
        //locker
    readSigFile(S, lockerSig);
        //requester
    readSigFile(uS, personSig);
        //CA
    readSigFile(caSIG, caSig);
    closedir(dir);
}

void dirToLock::generateAESKeys() {

    //create new file for storing keys
    FILE * SharedKeys;
    string encFile;

    fileForKeys = dirName + "/SharedKeys";

    SharedKeys = fopen(fileForKeys.c_str(), "w");
    if (SharedKeys == NULL){
        fprintf(stderr, "Failed to open file to write - %s\n", fileForKeys.c_str());
        exit(1);
    }

    printf("Generating 'SharedKeys'...\n");
    encKey = BN_new();
    macKey = BN_new();

    BN_rand(encKey, 256, 0, 1);
    BN_rand(macKey, 256, 0, 1);

    fprintf(SharedKeys, "---Encryption key---\n");
    BN_print_fp(SharedKeys, encKey);
    fprintf(SharedKeys, "\n");
    fprintf(SharedKeys, "---Macing key---\n");
    BN_print_fp(SharedKeys, macKey);

    fclose(SharedKeys);

    //encrypt file
    printf("Encrypting 'SharedKeys'...\n");
    encryptKeysFile();

    //sign encrypted file
    printf("Signing 'SharedKeys.enc'...\n\n");
    encFile = fileForKeys + ".enc";
    signFile(encFile);


    string temp = "rm " + fileForKeys;
    system(temp.c_str());
}

void dirToLock::signFile(string & file) {

    string message,signature;
    ofstream keysSig;

    keysSig.open(file + ".sig");
    if (keysSig.fail() ){
        fprintf(stderr, "Failed to open file for signature\n");
        exit(1);
    }

    // sign file
    dirToStr(file, message);
    sig(SKfile, message, signature);

    keysSig << signature;
    keysSig.close();
}


void dirToLock::encryptKeysFile() {

    RSA_obj enc;
    ofstream encKeys;
    string message, cypher, hexMessage;

    encKeys.open(fileForKeys + ".enc");
    if (encKeys.fail() ){
        fprintf(stderr, "Failed to open file for encrypted shared keys\n");
        exit(1);
    }

    // encrypt file
    dirToStr(fileForKeys, message);
    hexMessage = strToHexStr(message);
    enc.RSAEncrypt(uPKfile, hexMessage, cypher);

    //create .enc file
    encKeys << cypher;
    encKeys.close();

}
void dirToLock::encryptFiles(){

    ifstream from;
    ofstream to, tFile;
    string inp;
    string temp, tag;
    char *eKey ;
    char *output;
    char *input;
    eKey = BN_bn2hex(encKey);
    AES aes;
    aes.setKey(eKey);
    string mKey( BN_bn2hex(macKey) );

    for (string file : dirFiles) {

        printf("Encryptitng '%s' ...\n", file.c_str());
        file = dirName + "/" +file;

        from.open(file.c_str());
        if (from.fail()) {
            fprintf(stderr, "Couldn't open file for encryption -- %s\n", file.c_str());
            exit(1);
        }
        temp = file + ".enc";
        to.open(temp.c_str());
        if (to.fail()) {
            fprintf(stderr, "Couldn't open file for encrypted text -- %s\n", temp.c_str());
            exit(1);
        }
        temp = file +".tag";
        tFile.open(temp);
        if (tFile.fail()){
            fprintf(stderr, "Couldn't open file for tag -- %s\n", temp.c_str());
            exit(1);
        }

        // convert c++ string to c string for encryption part
        from >> inp;
        input = (char*) malloc(sizeof(char) * inp.size() + 1);
        memcpy(input, inp.c_str(), inp.size());
        input[inp.size()] = '\0';

        //Encrytion of the file
        output = aes.CBCencrypt(input);
        to << output;
        to.close();

        //Taging encrypted file
        string mInput(output);
        int size = mInput.size();

        HashMacGen(mKey, mInput, size , tag);

        char tmp[3];

        printf("Generating tag...\n");
        for (int i = 0; i < 16; i++){
            sprintf( tmp, "%02x",  (unsigned char)(unsigned int)tag[i] );
            tFile << tmp;
        }

        tFile.close();
        from.close();
        free(input);
        free(output);

        string temp = "rm "+ file;
        system( temp.c_str() );
    }

}
void dirToLock::dirToStr(string &d, string &m){

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
void dirToLock::verifyPKeys(){

    bool client = false;
    bool locker = false;
    bool CA = false;
    string message;

    printf("Verifying CA...\n");
    dirToStr(caPKfile, message);
    CA = verify(caPKfile, caSig , message);
    if (!CA) {
        cerr << "CA is not verified"<< endl;
    }

    printf("Verifying requesting party...\n");
    dirToStr(uPKfile, message);
    client = verify(caPKfile, personSig ,message);
    if (!client) {
        cerr << "Requesting party is not verified"<< endl;
    }

    printf("Verifying locking party...\n");
    dirToStr(PKfile, message);
    locker = verify(caPKfile, lockerSig, message);
    if (!locker){
        cerr << "Locking party is not verified"<< endl;
    }

    if (! (client && locker && CA) ){
        cerr << "Aborting..." << endl;
        exit(1);
    }else{
        cout << "All parties verified successfully..." <<endl;
    }


    //SYSTEM CALLS !!! TO COPY FILES OVER
    string temp = "cp " + PKfile + " " + dirName + "/LockPK.txt";
    system(temp.c_str());
    temp = "cp " + SigFile + " " + dirName + "/LockSig.txt";
    system(temp.c_str());
    temp = "cp " + caPKfile + " " + dirName + "/CApk.txt";
    system(temp.c_str());
    temp = "cp " + caSigFile + " " + dirName + "/CAsig.txt";
    system(temp.c_str());
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

string dirToLock::strToHexStr(string & m){

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
string dirToLock::hexStrToStr(string & hex){

    string temp, m = "";

    for (int i = 0; i < hex.size()/2; i++){
        temp = hex.substr(i*2,2);
        m += strtol(temp.c_str(),NULL,16);
    }

    return m;
}