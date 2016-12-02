#include"sig.h"
#include<fstream>
#include<string>
#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<cstring>

using namespace std;

void print_usage();
void print_help();
bool process_args(int, char**, char&);

int main(int argc, char** argv)
{
   char mode;
   string key_file;
   string message_file;
   string signature_file;
   string message;
   string signature;
   ifstream in;
   ofstream out;
   

   if(process_args(argc, argv, mode))
   {
      exit(0);
   }

   key_file = argv[2];
   message_file = argv[3];
   signature_file = argv[4];

   in.open(message_file.c_str(), in.in);
   if(in.fail())
   {
      cout << "File \"" << message_file << "\" does not exist" << endl;
      exit(0);
   }

   if(mode == 's')
   {
      in >> message;
      in.close();
      sig(key_file, message, signature);

      out.open(signature_file.c_str(), out.out);
      out << signature;
      out.close();
   }
   else
   {
      in >> message;
      in.close();
      in.open(signature_file.c_str(), in.in);
      if(in.fail())
      {
         cout << "File \"" << signature_file << "\" does not exist" << endl;
         exit(0);
      }
      in >> signature;
      in.close();

      if(verify(key_file, signature, message)) {
         cout << "Signature Verified" << endl;
      }else{
         cout << "Signature did not verify" << endl;
      }
   }

   return 0;
}

bool process_args(int argc, char** argv, char& m)
{
   if(argc == 2)
   {
      if(strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
      {
         print_help();
         return true;
      }
   }
   if(argc != 5)
   {
      cout << "Too few arguments:" << endl << endl;
      print_usage();
      return true;
   }
   if(strcmp(argv[1], "-s") == 0)
   {
      m = 's';
   }else if(strcmp(argv[1], "-v") == 0){
      m = 'v';
   }else{
      cout << "Invalid Mode:" << endl << endl;
      print_usage();
      return true;
   }
   return false;
}

void print_usage()
{
   cout << "Usage: mode [-s|-v] key_File message_file signature_file" << endl << endl;
}

void print_help()
{
   print_usage();
   cout << "MODES:" << endl;
   cout << "\t-s\tPlaces program into signature mode." << endl;
   cout << "\t-v\tPlaces program into varification mode." << endl;
}
