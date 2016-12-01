#include"sig.h"
#include<string>
#include<iostream>

using namespace std;

int main()
{
   string message = "AAAABBBBCCCCDDDDEEEEFFFF0000999988887777666655554444333322221111";
   string signature;
   string PK_file = "files/PK";
   string SK_file = "files/SK";

   sig(PK_file, message, signature);
   cout << signature << endl;

   if(verify(SK_file, signature, message))
   {
      cout << "IT WORKED!!!" << endl;
   } else {
      cout << "SOMETING WENT WRONG!" << endl;
   }

   return 0;
}
