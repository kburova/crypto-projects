#include<cstdio>
#include<cstring>
#include<string>
#include<openssl/bn.h>
#include<openssl/sha.h>
#include <iostream>
#include"sig.h"
#include"rsa.h"

using std::printf;
using std::strlen;
using std::string;

void sig(string PK_file, string message, string& signature)
{
   RSA_obj sig;

   hhash(message);
   sig.RSAEncrypt(PK_file, message, signature);

   return;
}

bool verify(string SK_file, string signature, string message)
{
   RSA_obj sig;
   string newMessage;

   hhash(message);
   sig.RSADecrypt(SK_file, signature, newMessage);

   if(message == newMessage) return true;

   return false;
}

void hhash(string& message)
{
   unsigned char md[SHA256_DIGEST_LENGTH];
   SHA256_CTX context;
   char hash[HASH_STRING_LENGTH];
   int i, j;

   SHA256_Init(&context);
   SHA256_Update(&context, (unsigned char*)message.c_str(), message.size());
   SHA256_Final(md, &context);

   for(i=0,j=0; i<SHA256_DIGEST_LENGTH; i++,j+=2)
   {
      sprintf(hash+j,"%02X",md[i]);
   }
   message = hash;

   return;
}
