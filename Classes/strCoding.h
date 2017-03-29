#pragma once
#include <iostream>
#include <string>
using namespace std;
enum EncryptMode
{
	Mode_Encrypt,
	Mode_Decrypt
};


class strCoding
{
public:
	strCoding(void);
	~strCoding(void);
	void encrypt(EncryptMode mode,string &pout, string pin);
	
private:
	char  CharToInt(char ch);
	char StrToBin(char *str);

};