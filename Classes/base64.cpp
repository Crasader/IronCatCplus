#include "base64.h"
#include <iostream>

static const std::string base64_chars ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


static inline bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64::base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while((i++ < 3))
			ret += '=';

	}

	return ret;

}

std::string base64::base64_decode(std::string const& encoded_string) {
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i ==4) {
			for (i = 0; i <4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i) {
		for (j = i; j <4; j++)
			char_array_4[j] = 0;

		for (j = 0; j <4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}

	return ret;
}





void CConvert::_enBase64Help(unsigned char chasc[3],unsigned char chuue[4])
{
	int i, k=2;
	unsigned char t = 0;
	for(i=0; i<3; i++)
	{
		*(chuue+i) = *(chasc+i)>>k;
		*(chuue+i) |= t;
		t = *(chasc+i)<<(8-k);
		t >>= 2;
		k += 2;
	}
	*(chuue+3) = *(chasc+2)&63;

	for ( i=0; i<4; i++ ) {
		if ( (*(chuue+i)<=128) && (*(chuue+i)<=25) ) {
			*(chuue+i) += 65; // 'A'-'Z'
		} else if ( (*(chuue+i)>=26) && (*(chuue+i)<=51) ) {
			*(chuue+i) += 71; // 'a'-'z'
		} else if ( (*(chuue+i)>=52) && (*(chuue+i)<=61) ) {
			*(chuue+i) -= 4; // 0-9
		} else if ( *(chuue+i)==62 ) {
			*(chuue+i) = 43; // +
		} else if ( *(chuue+i)==63 ) {
			*(chuue+i) = 47; // /
		}
	}
}

void CConvert::_deBase64Help(unsigned char chuue[4],unsigned char chasc[3]) {
	int i,k=2;
	unsigned char t=0;

	for( i=0; i<4; i++) {
		if ( (*(chuue+i)>=65) && (*(chuue+i)<=90))
			*(chuue+i) -= 65; // 'A'-'Z' -> 0-25
		else if ( (*(chuue+i)>=97)&&(*(chuue+i)<=122) )
			*(chuue+i) -= 71; // 'a'-'z' -> 26-51
		else if ( (*(chuue+i)>=48)&&(*(chuue+i)<=57) )
			*(chuue+i) += 4; // '0'-'9' -> 52-61
		else if ( *(chuue+i)==43 )
			*(chuue+i) = 62; // + -> 62
		else if ( *(chuue+i)==47 )
			*(chuue+i) = 63; // / -> 63
		else if ( *(chuue+i)==61 )
			*(chuue+i) = 0;  // = -> 0  Note: 'A'和'='都对应了0
	}
	for ( i=0; i<3; i++ ) {
		*(chasc+i) = *(chuue+i) << k;
		k += 2;
		t = *(chuue+i+1) >> (8-k);
		*(chasc+i) |= t;
	}
}

string CConvert::enBase64( const char* inbuf, size_t inbufLen ) {
	string outStr;
	unsigned char in[8];
	unsigned char out[8];
	out[4] = 0;
	size_t blocks = inbufLen / 3;
	for ( size_t i=0; i<blocks; i++ ) {
		in[0] = inbuf[i*3];
		in[1] = inbuf[i*3+1];
		in[2] = inbuf[i*3+2];
		_enBase64Help(in,out);
		outStr += out[0];
		outStr += out[1];
		outStr += out[2];
		outStr += out[3];
	}
	if ( inbufLen % 3 == 1 ) {
		in[0] = inbuf[inbufLen-1];
		in[1] = 0;
		in[2] = 0;
		_enBase64Help(in,out);
		outStr += out[0];
		outStr += out[1];
		outStr += '=';
		outStr += '=';
	} else if ( inbufLen % 3 == 2 ) {
		in[0] = inbuf[inbufLen-2];
		in[1] = inbuf[inbufLen-1];
		in[2] = 0;
		_enBase64Help(in,out);
		outStr += out[0];
		outStr += out[1];
		outStr += out[2];
		outStr += '=';
	}
	return string(outStr);
}

string CConvert::enBase64( const string &inbuf)
{
	return CConvert::enBase64( inbuf.c_str(), inbuf.size() );
}

int CConvert::deBase64( string src, char* outbuf ) {

	// Break when the incoming base6 4 coding is wrong
	if((src.size() % 4 )!= 0 )
	{
		return 0;
	}

	unsigned char in[4];
	unsigned char out[3];

	size_t blocks = src.size()/4;
	for ( size_t i=0; i<blocks; i++ ) {
		in[0] = src[i*4];
		in[1] = src[i*4+1];
		in[2] = src[i*4+2];
		in[3] = src[i*4+3];
		_deBase64Help(in,out);
		outbuf[i*3]   = out[0];
		outbuf[i*3+1] = out[1];
		outbuf[i*3+2] = out[2];
	}
	int length = src.size() / 4 * 3;
	if ( src[src.size()-1] == '=' ) {
		length--;
		if ( src[src.size()-2] == '=' ) {
			length--;
		}
	}
	return length;
}
string CConvert::deBase64( string src)
{
	char * buf=new char[src.length()*2];
	int len=deBase64(src,buf);
	buf[len]='\0';
	string result=string(buf,len);
	delete [] buf;
	return result;
}