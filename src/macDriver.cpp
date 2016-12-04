#include <iomanip>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <fstream>

#include "mac.h"
using namespace std;

int main(int argc, char** argv){

    ifstream M_file,K_file;
    fstream T_file;
    int i, j, MES_SIZE, KEY_SIZE, mode, task;
    string k, m, temp, IV = "", Key = "", Message = "", Tag = "";

    if (argc !=4 ){
        fprintf(stderr, "Usage: mac [key_file] [message_file] [tag_file]");
        exit(1);
    }

    /****  Read the key and message from the files   ****/

    K_file.open(argv[1]);
    if (K_file.fail()){
        fprintf(stderr, "Couln't open key file -- %s\n", argv[1]);
        exit(1);
    }
    if (K_file >> k) {
        KEY_SIZE = k.size()/2;
        if (KEY_SIZE != 16 and KEY_SIZE != 24 and KEY_SIZE != 32){
            fprintf(stderr,"Wrong key size: should be 128, 192 or 256 bits\n");
            exit(1);
        }
        printf("Read key...Success\n");
    }else{
        fprintf(stderr,"Read key...Fail\n");
        exit(1);
    }
    M_file.open(argv[2]);
    if (M_file.fail()){
        fprintf(stderr, "Couldn't open message file -- %s\n", argv[2]);
        exit(1);
    }
    if (M_file >> m) {
        MES_SIZE = m.size()/2;
        printf("Read message...Success\n");
    }else{
        fprintf(stderr,"Read message...Fail\n");
        exit(1);
    }

    /****  Convert hex strings to actual hex values   ****/

    for (i = 0; i < KEY_SIZE; i++){
        temp = k.substr(i*2,2);
        Key += strtol(temp.c_str(),NULL,16);
    }
    printf("Converted key: \"%s\"\n", Key.c_str());

    for (i = 0; i < MES_SIZE; i++){
        temp = m.substr(i*2,2);
        Message += strtol(temp.c_str(),NULL,16);
    }
    printf("Converted message: \"%s\"\n", Message.c_str());

    /****  set IV to all 0's since no need for MAC   ****/

    for (i = 0; i < 16; i++) IV += '\0';

    /****   open Tag file to write to OR to read from   ****/

    T_file.open(argv[3], fstream::out | fstream::trunc);
    if (T_file.fail()){
        fprintf(stderr, "Couln't open file for tag -- %s\n", argv[3]);
        exit(1);
    }

    /**** Ask user for a choice of mode and task   ****/

    cout << "Select CBC-MAC or Hash-and-MAC" << endl
         <<"Type 1 for CBC-MAC or 2 for Hash-and-MAC : ";
    cin >> mode;
    cout << "Type 1 Tag Generation or type 2 for for Verification : ";
    cin >> task;
    cout << endl;

    if (task == 1){

        char temp[3];

        if (mode == 1){
            CBCGen(Key,Message,MES_SIZE,IV,Tag);
        }else{
            HashMacGen(Key,Message,MES_SIZE,Tag);
        }

        printf("Tag:     ");
        for (i = 0; i < 16; i++){
            printf("%02x",(unsigned char)(unsigned int)Tag[i]);
            sprintf( temp, "%02x",  (unsigned char)(unsigned int)Tag[i] );
            T_file << temp;
        }
        cout << endl;

    }else if (task == 2){

        bool isValid;
        string t;

        T_file >> t;
        printf("Read tag: %s\n", t.c_str());

        if (t.size() != 32){
            cout <<"***Tag is NOT valid***"<<endl;
            return 0;
        }

        for (i = 0; i < 16; i++){
            temp = t.substr(i*2,2);
            Tag += strtol(temp.c_str(),NULL,16);
        }


        if (mode ==1){
            isValid = CBCVer(Key, Message, MES_SIZE,IV,Tag);
        }else{
            isValid = HashMacVer(Key, Message, MES_SIZE,Tag);
        }
        printf ("%s\n", (isValid ? "***Tag is valid***" : "***Tag is NOT valid***"));

    }

    K_file.close();
    M_file.close();
    T_file.close();

    return 0;
}

