#include<fstream>
#include<iostream>
#include<string>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include"aes.h"

using std::string;
using std::cout;
using std::endl;
using std::strcpy;
using std::strcmp;
using std::ifstream;
using std::ofstream;

void parameter_check(int, char**);
void print_usage();
void print_help();
int open_check(ifstream&, char*);

int main(int argc, char** argv)
{
	char *key;				// pointer to the read in key
	char *input;			// pointer to the input text string
	char *output;			// pointer to the output text string
	ifstream in;			// filestream for input
	ofstream out;			// filestream for output
//	FILE* out;				// filestream for output
	int fileSize,keySize;			// size of the file in bytes
	AES aes;					// creats the AES object

	parameter_check(argc, argv);

    fileSize = open_check(in, argv[3]);

    // check for proper key size
    if(fileSize*4 != 256 && fileSize*4 != 192 && fileSize*4 != 128)
    {
        cout << "\nError: Improper key size\n\n";
        exit(0);
    }

    key = (char*)malloc(sizeof(char)*fileSize+1);
    in.getline(key,fileSize+1);
    in.close();

	aes.setKey(key);


	fileSize = open_check(in, argv[4]);
	input = (char*)malloc(sizeof(char)*fileSize+1);
	in.getline(input,fileSize+1);
	in.close();
	
	if(strcmp(argv[1], "-cbc")==0)
	{
		if(strcmp(argv[2], "-e")==0)
		{
			output = aes.CBCencrypt(input);
		}
		else
		{
			output = aes.CBCdecrypt(input);
		}
	}
	else
	{
		if(strcmp(argv[2], "-e")==0)
		{
			output = aes.CTRencrypt(input);
		}
		else
		{
			output = aes.CTRdecrypt(input);
		}
	}

	out.open(argv[5], out.out);
	if(out.fail())
	{
		cout << "Error: " << endl;
		exit(0);
	}

	out << output;
	out.close();

	return 0;
}

/*****************************************************************************
parameter_check: checks the command line argumets for user error.
parameters:
		argc:  number of arguments passes
		argv:  array of c-string arguments
returns:
		none:
*****************************************************************************/

void parameter_check(int argc, char** argv)
{
	// checks for proper number of arguments
	if(argc != 6)
	{
		// checks for help option
		if(argc == 2)
		{
			if(strcmp(argv[1],"--help")==0)
			{
				print_help();
				exit(0);
			}
		}
		cout << "\nError: Not enough arguments\n";
		print_usage();
		exit(0);
	}

	// checks for proper modes
	if(strcmp(argv[1],"-cbc")!= 0 && strcmp(argv[1],"-ctr")!=0)
	{
		cout << "\nError: Improper MODE\n";
		print_usage();	
		exit(0);
	}

	// checks for proper options
	if(strcmp(argv[2],"-e")!= 0 && strcmp(argv[2],"-d")!=0)
	{
		cout << "\nError: Improper OPTION\n";
		print_usage();	
		exit(0);
	}
}

/*****************************************************************************
open_check: opens a file and checks to see if it both exists and is non-empty
Parameters:
		in: ifstream ref to be linked to the file
		file: Name of the file to be opened
Returns:
		size: file size in bytes
*****************************************************************************/

int open_check(ifstream& in, char* file)
{
	string check;

	in.open(file);

	if(in.fail())
	{
		cout << "\nError: File doesn't exist\n\n";
		exit(0);
	}

	in >> check;
	in.seekg (0, in.beg);

	if(!check.size())
	{
		cout << "\nError: File is empty\n\n";
		exit(0);
	}
	return check.size();
}

/*****************************************************************************
print_usage: prints the proper command line usage to the screen
Parameters:
		none:
Returns:
		none:
*****************************************************************************/

void print_usage()
{
	cout << "\nUseage: aesDriver [MODE] [OPTION] [KEY_FILE] [IMPUT_FILE] [OUTPUT_FILE]\n";
	cout << "Use \"aesDriver --help\" for list of cmd arguments\n\n";
	return;
}

/*****************************************************************************
print_help: prints the help menu to the screen
Parameters:
		none:
Returns:
		none:
*****************************************************************************/

void print_help()
{
	cout << "\nUseage: aesDriver [MODE] [OPTION] [KEY_FILE] [IMPUT_FILE] [OUTPUT_FILE]\n\n";
	cout << "MODES:\n";
	cout << "\t-cbc\t\tEncrypt or decrypt using CBC mode\n";
	cout << "\t-ctr\t\tEncrypt or decrypt using CTR mode\n";
	cout << "OPTIONS:\n";
	cout << "\t-e\t\tEncrypt a message\n";
	cout << "\t-d\t\tDecrypt a cypher\n\n";
	cout << "\t--help\t\tDisplay help menu\n\n";
	cout << "Example: aesDriver -cbc -e key.txt message.txt out.txt\n\n";
}
