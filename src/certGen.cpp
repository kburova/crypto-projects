#include <iostream>
#include <openssl/bn.h>
#include "rsa.h"
#include "sig.h"

using namespace std;

int signCertificate(const string & PK_file, const string & CAsig){

    FILE *CA;

    /*** open file that has signature to sign things ***/

    CA = fopen(PK_file.c_str(), "r");
    if (CA == NULL) {
        fprintf(stderr, "Couldn't open file that holds Secret Key for signing %s\n", PK_file.c_str());
        exit(1);
    }

}
