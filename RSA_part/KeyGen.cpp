#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <gmp.h>
#include <ctime>
using namespace std;
unsigned long mix(unsigned long a, unsigned long b, unsigned long c);

void keyGen(string & PK_file, string &SK_file) {
    fstream PK, SK;
    unsigned long int n;
    unsigned long seed =  mix(clock(), time(NULL), getpid());
    mpz_t p, q, temp, exp;
    mpz_inits(p,q,temp,exp);

    /*** Initialize state and seed for random function ***/
    gmp_randstate_t state;
    gmp_randinit_mt (state);
    gmp_randseed_ui (state, seed);

//    PK.open(PK_file.c_str());
//    if (PK.fail()){
//        fprintf(stderr, "Couldn't open Public Key file %s\n", PK_file.c_str());
//        exit(1);
//    }
//    SK.open(SK_file.c_str());
//    if (SK.fail()){
//        fprintf(stderr, "Couldn't open Secret Key file %s\n", SK_file.c_str());
//        exit(1);
//    }

    cout << "Enter securitu parameter n: "<<endl;
    cin >> n;
    cout << "Generating p and q..."<<endl;
    /*** Generating random n-bit primes p and q ***/


    mpz_urandomb (p, state, n);
    mpz_setbit (p, n-1);
   // printf("Size: ");
    //cout <<mpz_sizeinbase (p, 2)<<endl;
   // mpz_powm_ui (exp, p, n-1 , n);
    printf("p: %s\n", mpz_get_str (NULL, 10, p));
    cout << endl;

}


/*** Found algorithm on stackoverflow.com ***/
unsigned long mix(unsigned long a, unsigned long b, unsigned long c)
{
    a=a-b;  a=a-c;  a=a^(c >> 13);
    b=b-c;  b=b-a;  b=b^(a << 8);
    c=c-a;  c=c-b;  c=c^(b >> 13);
    a=a-b;  a=a-c;  a=a^(c >> 12);
    b=b-c;  b=b-a;  b=b^(a << 16);
    c=c-a;  c=c-b;  c=c^(b >> 5);
    a=a-b;  a=a-c;  a=a^(c >> 3);
    b=b-c;  b=b-a;  b=b^(a << 10);
    c=c-a;  c=c-b;  c=c^(b >> 15);
    return c;
}

