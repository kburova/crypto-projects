#ifndef MAC_H
#define MAC_H


typedef std::basic_string<unsigned char> u_string;
#define ABORT() (fprintf(stderr, "%s\nAborting in %s at %s:%d\n", ERR_error_string(ERR_get_error(), NULL), __PRETTY_FUNCTION__, __FILE__, __LINE__), abort(), 0)
using namespace std;


void encodeBlock(string & key, string & data, string & block);

void CBCGen( string & key,
             string & message,
             int & MES_SIZE,
             string & IV,
             string & tag);

void HashMacGen(string & Key,
                string & Message,
                int & MES_SIZE,
                string & Tag);

bool HashMacVer( string & key,
                 string & message,
                 int & MES_SIZE,
                 string & tag);

bool CBCVer( string & key,
             string & message,
             int & MES_SIZE,
             string & IV,
             string & tag);


#endif
