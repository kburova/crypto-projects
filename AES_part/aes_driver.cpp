#include<fstream>
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include"aes.h"

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
	char* output;			// pointer to the output text string
	ifstream in;			// filestream for input
	FILE* out;				// filestream for output
	int fileSize;			// size of the file in bytes
	AES aes;					// creats the AES object

	parameter_check(argc, argv);

	fileSize = open_check(in, argv[3]);

	// check for proper key size
	if(fileSize*4 != 256 && fileSize*4 != 192 && fileSize*4 != 128)
	{
		cout << "file size = " << fileSize << endl;
		cout << "key size = " << fileSize * 4 << endl;
		cout << "\nError: Improper key size\n\n";
		exit(0);
	}

	key = (char*)malloc(sizeof(char)*fileSize+1);
	in.getline(key,fileSize+1);
	in.close();

	aes.setKey(fileSize, key);

	fileSize = open_check(in, argv[4]);

	input = (char*)malloc(sizeof(char)*fileSize+1);
	in.getline(input,fileSize+1);
	in.close();
	
	if(strcmp(argv[1], "-cbc")==0)
	{
		if(strcmp(argv[2], "-e")==0)
		{
			output = aes.CBCencrypt(fileSize, input);
		}
		else
		{
			output = aes.CBCdecrypt(fileSize, input);
		}
	}
	else
	{
		if(strcmp(argv[2], "-e")==0)
		{
			output = aes.CTRencrypt(fileSize, input);
		}
		else
		{
			output = aes.CTRdecrypt(fileSize, input);
		}
	}

	out = fopen(argv[5], "w");
	if(out == NULL)
	{
		cout << "Error: " << perror << endl;
		exit(0);
	}
	fprintf(out,"%s\n",output);
//	fputs(output, out);
	fclose(out);

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
	int size = 0;

	in.open(file);

	if(in.fail())
	{
		cout << "\nError: File doesn't exist\n\n";
		exit(0);
	}

	in.seekg (0, in.end);
	size = in.tellg();
	in.seekg (0, in.beg);

	if(!size)
	{
		cout << "\nError: File is empty\n\n";
		exit(0);
	}

	return size-1;
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
	cout << "\nUseage: aes_driver.exe [MODE] [OPTION] [KEY_FILE] [IMPUT_FILE] [OUTPUT_FILE]\n";
	cout << "Use \"aes_driver.exe --help\" for list of cmd arguments\n\n";
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
	cout << "\nUseage: aes_driver.exe [MODE] [OPTION] [KEY_FILE] [IMPUT_FILE] [OUTPUT_FILE]\n\n";
	cout << "MODES:\n";
	cout << "\t-cbc\t\tEncrypt or decrypt using CBC mode\n";
	cout << "\t-ctr\t\tEncrypt or decrypt using CTR mode\n";
	cout << "OPTIONS:\n";
	cout << "\t-e\t\tEncrypt a message\n";
	cout << "\t-d\t\tDecrypt a cypher\n\n";
	cout << "\t--help\t\tDisplay help menu\n\n";
	cout << "Example: aes_driver.exe -cbc -e key.txt message.txt out.txt\n\n";
}
