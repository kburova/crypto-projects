#include <iostream>
#include <fstream>


#include "lock.h"


dirToLock::dirToLock(string& d, string& PK, string& SK, string &S, string &uPK, string &uS){

    char *s;
    struct stat buf;
    ifstream file;

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

    // START OPENING ALL FILES AND STORE INFORMATION

    file.open(PK);
    if (file.fail()){
        fprintf(stderr, "Couldn't open key file -- %s\n", PK.c_str());
        exit(1);
    }

    //......
}

void dirToLock::lock(){

}

