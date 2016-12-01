#ifndef SIG_H
#define SIG_H

#include<string>

using std::string;

#define HASH_STRING_LENGTH 65

void sig(string, string, string&);
bool verify(string, string, string);
void hash(string&);

#endif
