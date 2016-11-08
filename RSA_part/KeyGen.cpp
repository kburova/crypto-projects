#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <ctime>
#include <cmath>
#include <openssl/bn.h>
using namespace std;

void keyGen(string & PK_file, string &SK_file) {
    bool notFound = true;

    FILE  *PK, *SK;
    unsigned long int secParam, limit;
    int i, j;
    BIGNUM p,q, groupMem,gcd, one, rem;
    BN_init(&p);
    BN_init(&q);
    BN_init(&groupMem);
    BN_init(&gcd);
    BN_init(&one);
    BN_init(&rem);
    BIGNUM *temp1 = BN_new();
    BIGNUM *temp2 = BN_new();
    BIGNUM *n = BN_new();
    BIGNUM *e = BN_new();
    BIGNUM *d = BN_new();
    BIGNUM *n_minus1 = BN_new();
    BIGNUM *N = BN_new();
    BIGNUM *orderOfGroup = BN_new();

    BN_CTX *x = BN_CTX_new();
    BN_one(&one);

    PK = fopen(PK_file.c_str(), "w");
    if (PK == NULL) {
        fprintf(stderr, "Couldn't open Public Key file %s\n", PK_file.c_str());
        exit(1);
    }

    SK = fopen(SK_file.c_str(), "w");
    if (SK == NULL) {
        fprintf(stderr, "Couldn't open Secret Key file %s\n", SK_file.c_str()) ;
        exit(1);
    }

    cout << "Enter security parameter n: "<<endl;
    cin >> secParam;


    cout << "Generating p and q..." << endl;

    BN_set_word(n, secParam);
    BN_set_word(n_minus1, secParam - 1);

    /*** Generating random n-bit primes p and q ***/
    limit = 3 * pow(secParam, 2);

    while(notFound) {
        //for (i = 0; i < 3*limit; i++) {
            BN_rand(&p, secParam, 0, 1);
        char *a = (char*)malloc(sizeof(char)*300);
        for (i= 0; i<300;i++) a[i] = '\0';
        a = BN_bn2dec(&p);
        printf ("p: %s\n", a);
            /***      Check primality:
             *   1. Choose uniform element from Group of order N-1
             *   2. Check if really a group member by checking GCD of 1
             *   3. Compute mod N for that number raised to the power of N-1
             *   4. If it's 1, N is probably rime                                ***/

            for (j = 0; j < 16; j++) {
                while (true) {
                    BN_rand_range(&groupMem, &p);
                    if (BN_gcd(&gcd, &p, &groupMem, x) == 0) {
                        fprintf(stderr, " Couldn't compute GCD for p and possible group member\n");
                        exit(1);
                    }
                    /*** if gcd == 1 for rand element and N, then it's in a group ***/
                    if (BN_cmp(&one, &gcd) == 0) {
                        break;
                    }
                }
                char *a = (char*)malloc(sizeof(char)*300);
                for (i= 0; i<300;i++) a[i] = '\0';
                a = BN_bn2dec(&groupMem);
                printf ("groupMem: %s\n", a);

                BN_mod_exp(&rem, &groupMem, n_minus1, n, x);
                if (BN_cmp(&one, &rem) != 0) {
                    break;
                }
            }
            if (j >= 16){
                printf("Found possible prime p...\n");
                notFound = false;
            }
        }
//    notFound = true;
//    while(notFound) {
//        //for (i = 0; i < 3*limit; i++) {
//            BN_rand(&q, secParam, 0, 1);
//            BN_gcd(&gcd, &q, &p, x);
//            if (BN_cmp(&one, &gcd) != 0) {
//                continue;
//            }
//            /***      Check primality:
//             *   1. Choose uniform element from Group of order N-1
//             *   2. Check if really a group member by checking GCD of 1
//             *   3. Compute mod N for that number raised to the power of N-1
//             *   4. If it's 1, N is probably rime
//             *   5. Also check here of GCD for q and p is also 1 (Carmichael numbers MAY fail check) ***/
//            for (j = 0; j < 16; j++) {
//                while (true) {
//                    BN_rand_range(&groupMem, &q);
//                    if (BN_gcd(&gcd, &q, &groupMem, x) == 0) {
//                        fprintf(stderr, " Couldn't compute GCD for p and possible group member\n");
//                        exit(1);
//                    }
//                    /*** if gcd == 1 for rand element and N, then it's in a group ***/
//                    if (BN_cmp(&one, &gcd) == 0) {
//                        break;
//                    }
//                }
//                BN_mod_exp(&rem, &groupMem, n_minus1, n, x);
//                if (BN_cmp(&one, &rem) != 0 ) {
//                    break;
//                }
//            }
//            if (j >= secParam/2){
//                printf("Found possible prime q...\n");
//                notFound = false;
//            }
//        }
//
//        /***  calculate N = p x q, and the order of the group (p-1)(q-1) ***/
//
//        BN_mul(N, &p, &q, x);
//        BN_sub(temp1, &p, &one);
//        BN_sub(temp2, &q, &one);
//
//        BN_mul(orderOfGroup, temp1, temp2, x);
//
//        unsigned long int fermatPrime = 65537;
//        BN_set_word(e, fermatPrime);
//        if (BN_gcd(&gcd, e, orderOfGroup, x) == 0) {
//            fprintf(stderr, " Couldn't compute GCD for e and Order of the group\n");
//            exit(1);
//        }
//        if (BN_cmp(&one, &gcd) == 0) {
//            printf("e > 1 is chosen...\n");
//        }
//
//        if (BN_mod_inverse(d, e, orderOfGroup, x) != NULL) {
//            printf("d is calculated...\n");
//            notFound = false;
//        } else {
//            printf("Couldn't calculate d, restart process...\n");
//
//        }
//
//
//    char *a = (char*)malloc(sizeof(char)*300);
//    for (i= 0; i<300;i++) a[i] = '\0';
//    a = BN_bn2dec(&p);
//    printf ("p: %s\n", a);
//
//    char *b = (char*)malloc(sizeof(char)*300);
//    for (i= 0; i<300;i++) b[i] = '\0';
//    b = BN_bn2dec(&q);
//    printf ("q: %s\n", b);
//    char *c = (char*)malloc(sizeof(char)*300);
//    for (i= 0; i<300;i++) c[i] = '\0';
//    c = BN_bn2dec(e);
//    printf ("e: %s\n", c);
//
//    char *f = (char*)malloc(sizeof(char)*300);
//    for (i= 0; i<300;i++) f[i] = '\0';
//    f = BN_bn2dec(orderOfGroup);
//    printf ("order of the Group: %s\n", f);
//
//    char *l = (char*)malloc(sizeof(char)*300);
//    for (i= 0; i<30;i++) l[i] = '\0';
//    l = BN_bn2dec(d);
//    printf ("d: %s\n", l);
//
//    printf ("p bits: %d\n", BN_num_bits(&p) );
//    printf ("q bits: %d\n", BN_num_bits(&q) );
//
//    printf("Writing Public Key to a file...\n");
//    BN_print_fp(PK, N);
//    BN_print_fp(PK, e);
//    printf("Writing Secret Key to a file...\n");
//    BN_print_fp(SK, N);
//    BN_print_fp(SK, d);
//
//    fclose(PK);
//    fclose(SK);
//
//    BN_CTX_free(x);
//    BN_free(n_minus1);
//    BN_free(n);
//    BN_free(N);
//    BN_free(e);
//    BN_free(d);
//    BN_free(temp1);
//    BN_free(temp2);

}

