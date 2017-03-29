#pragma once
#include <string>
using namespace std;
class base64
{
public:


	static std::string base64_encode(unsigned char const* , unsigned int len);
	static std::string base64_decode(std::string const& s);

};

class CConvert
{
public:


	//static std::string base64_encode(unsigned char const* , unsigned int len);
	//static std::string base64_decode(std::string const& s);
	static	void _enBase64Help(unsigned char chasc[3],unsigned char chuue[4]);
	static	void _deBase64Help(unsigned char chuue[4],unsigned char chasc[3]) ;
	static	string enBase64( const char* inbuf, size_t inbufLen );

	static	string deBase64( string src);
	static	string enBase64( const string &inbuf);
	static	int deBase64( string src, char* outbuf ) ;
};