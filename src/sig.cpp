#include<cstdio>
#include<cstring>
#include<openssl/bn.h>
#include<openssl/sha.h>
#include"sig.h"

using std::printf;
using std::strlen;
void sig(char * message, char* d, char* N)
{
   
   unsigned long length;                  // length of the message
   unsigned char md[SHA256_DIGEST_LENGTH];
   SHA256_CTX context;
   char hash[HASH_STRING_LENGTH];
   int i, j;

   length = strlen(message);

   SHA256_Init(&context);
   SHA256_Update(&context, (unsigned char*)message, length);
   SHA256_Final(md, &context);

   for(i=0; i<SHA256_DIGEST_LENGTH; i++)
   {
      printf("%02X",md[i]);
   }
   printf("\n");
   for(i=0,j=0; i<SHA256_DIGEST_LENGTH; i++,j+=2)
   {
      sprintf(hash+j,"%02X",md[i]);
   }
   printf("%s\n",hash);
   
}

/*************************************************************

     void SHA256_Init(SHA256_CTX *context);
     void SHA256_Update(SHA256_CTX *context, const uint8_t *data, size_t len);
     void SHA256_Pad(SHA256_CTX *context);
     void SHA256_Final(uint8_t digest[SHA256_DIGEST_LENGTH], SHA256_CTX *context);
     void SHA256_Transform(uint32_t state[8], const uint8_t buffer[SHA256_BLOCK_LENGTH]);
     char * SHA256_End(SHA256_CTX *context, char *buf);
     char * SHA256_File(const char *filename, char *buf);
     char * SHA256_FileChunk(const char *filename, char *buf, off_t offset, off_t length);
     char * SHA256_Data(uint8_t *data, size_t len, char *buf);
**************************************************************/
