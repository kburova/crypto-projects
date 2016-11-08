#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <openssl/bn.h>
using namespace std;

int passFermatTest(BIGNUM *prime);

void keyGen(string & PK_file, string &SK_file) {
    bool notFound = true;
    unsigned long int fermatPrime = 65537;
    FILE *PK, *SK;
    char *c;
    unsigned long int secParam;
    int i, j;
    BIGNUM *p = BN_new();
    BIGNUM *q = BN_new();
    BIGNUM *gcd = BN_new();
    BIGNUM *one = BN_new();
    BIGNUM *temp1 = BN_new();
    BIGNUM *temp2 = BN_new();
    BIGNUM *e = BN_new();
    BIGNUM *d = BN_new();
    BIGNUM *N = BN_new();
    BIGNUM *orderOfGroup = BN_new();

    BN_CTX *x = BN_CTX_new();
    BN_one(one);

    PK = fopen(PK_file.c_str(), "w");
    if (PK == NULL) {
        fprintf(stderr, "Couldn't open Public Key file %s\n", PK_file.c_str());
        exit(1);
    }

    SK = fopen(SK_file.c_str(), "w");
    if (SK == NULL) {
        fprintf(stderr, "Couldn't open Secret Key file %s\n", SK_file.c_str());
        exit(1);
    }

    cout << "Enter security parameter n: " << endl;
    cin >> secParam;


    cout << "Generating p..." << endl;
    while (notFound) {
        BN_rand(p, secParam, 0, 1);
        for (i = 0; i < 20 ; i++) {
            if (!passFermatTest(p)) break;
        }
        if (i == 20) {
            notFound = false;
            printf("Prime p is found...\n");

            c = (char *) malloc(sizeof(char) * 300);
            for (i = 0; i < 300; i++) c[i] = '\0';
            c = BN_bn2dec(p);
            printf("p: %s\n\n", c);
            free(c);
        }
    }

    notFound = true;

    cout << "Generating q..." << endl;
    while (notFound) {
        BN_rand(q, secParam, 0, 1);
        if (BN_cmp(p,q) == 0) continue;

        for (i = 0; i < 20 ; i++) {
            if (!passFermatTest(q)) break;
        }
        if (i == 20) {
            notFound = false;
            printf("Prime q is found...\n");

            c = (char *) malloc(sizeof(char) * 300);
            for (i = 0; i < 300; i++) c[i] = '\0';
            c = BN_bn2dec(q);
            printf("q: %s\n\n", c);
            free(c);
        }
    }

    /***  calculate N = p x q,
          the order of the group (p-1)(q-1),
          set e as Fermat prime and check for GCD with order of the group,
          calculate d as inverse of e mod order of the group ***/

    BN_mul(N, p, q, x);
    BN_sub(temp1, p, one);
    BN_sub(temp2, q, one);

    BN_mul(orderOfGroup, temp1, temp2, x);
    BN_set_word(e, fermatPrime);

    if (BN_gcd(gcd, e, orderOfGroup, x) == 0) {
        fprintf(stderr, " Couldn't compute GCD for e and Order of the group\n");
        exit(1);
    }
    if (BN_cmp(one, gcd) == 0) {
        printf("e > 1 is chosen...\n");
    }

    if (BN_mod_inverse(d, e, orderOfGroup, x) != NULL) {
        printf("d is calculated...\n");
    } else {
        printf("Couldn't calculate d, restart the process...\n");
    }

    c = (char*)malloc(sizeof(char)*300);
    for (i= 0; i<300;i++) c[i] = '\0';
    c = BN_bn2dec(e);
    printf ("e: %s\n", c);
    free(c);

    c = (char*)malloc(sizeof(char)*300);
    for (i= 0; i<300;i++) c[i] = '\0';
    c = BN_bn2dec(orderOfGroup);
    printf ("order of the Group: %s\n", c);
    free(c);

    c = (char*)malloc(sizeof(char)*300);
    for (i= 0; i<300;i++) c[i] = '\0';
    c = BN_bn2dec(d);
    printf ("d: %s\n", c);
    free(c);

    printf("Writing Public Key to a file...\n");
    BN_print_fp(PK, N);
    fprintf(PK, "\n");
    BN_print_fp(PK, e);
    fprintf(PK, "\n%lx", secParam);

    printf("Writing Secret Key to a file...\n");

    BN_print_fp(SK, N);
    fprintf(SK, "\n");
    BN_print_fp(SK, d);
    fprintf(SK, "\n%lx", secParam);

    fclose(PK);
    fclose(SK);

    BN_CTX_free(x);
    BN_free(N);
    BN_free(e);
    BN_free(d);
    BN_free(temp1);
    BN_free(temp2);

}

int passFermatTest(BIGNUM *prime){

    // printf("Start fermat test...\n\n");
    BIGNUM *groupMember = BN_new();
    BIGNUM *GCD = BN_new();
    BIGNUM *one = BN_new();
    BIGNUM *rem = BN_new();
    BIGNUM *p_minus1 = BN_new();
    BN_CTX *x = BN_CTX_new();
    char *a;

    BN_one(one);
    BN_sub(p_minus1, prime, one);
    int i;

    /*** find a group Z_prime member ***/
    while (true) {
        BN_rand_range(groupMember, prime);
        if (BN_gcd(GCD, prime, groupMember, x) == 0) {
            fprintf(stderr, " Couldn't compute GCD for p and possible group member\n");
            exit(1);
        }
        if (BN_cmp(one, GCD) == 0) {
            break;
        }
    }

//    a = (char*)malloc(sizeof(char)*300);
//    for (i= 0; i<300;i++) a[i] = '\0';
//    a = BN_bn2dec(groupMember);
//    printf ("Check group member : %s\n", a);
//    free (a);

    BN_mod_exp(rem, groupMember, p_minus1, prime, x);

    BN_free(groupMember);
    BN_free(GCD);
    BN_free(p_minus1);
    BN_CTX_free(x);

    if (BN_cmp(one, rem) == 0) {
        BN_free(one);
        BN_free(rem);
        return 1;
    }
    BN_free(one);
    BN_free(rem);
    return 0;
}
