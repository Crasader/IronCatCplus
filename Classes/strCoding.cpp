//这是个类strCoding (strCoding.cpp文件)

#include "strCoding.h"



strCoding::strCoding(void)
{
}

strCoding::~strCoding(void)
{
}

//做为解Url使用
char strCoding:: CharToInt(char ch){
	if(ch>='0' && ch<='9')return (char)(ch-'0');
	if(ch>='a' && ch<='f')return (char)(ch-'a'+10);
	if(ch>='A' && ch<='F')return (char)(ch-'A'+10);
	return -1;
}
char strCoding::StrToBin(char *str){
	char tempWord[2];
	char chn;

	tempWord[0] = CharToInt(str[0]);                         //make the B to 11 -- 00001011
	tempWord[1] = CharToInt(str[1]);                         //make the 0 to 0  -- 00000000

	chn = (tempWord[0] << 4) | tempWord[1];                //to change the BO to 10110000

	return chn;
}



void strCoding::encrypt(EncryptMode mode,string &pout, string pin)
{
	if(pin.length()<1)return;
	int index1=0;
	int index2 =0;
	int index3 = 0;
	int index4 = 0;
	int index5 = 0;
	switch (mode)
	{
	case Mode_Encrypt:
		{
		
			for(int i=0,len=pin.length();i<len;i++)
			{
				char c = pin.at(i);
				char *addin = &c;
				index1 = i%24+1;
				index2 = i%7+1;
				index3 = i%3;
				index4 = i%11+1;
				index5 = i%5;
				//*addin += index1+index2*index2+index3*2+index4*3;
				*addin ^=0x7f;
				if(index2==0)
				{
					*addin ^=0x42;
				}
				if(index5!=0)
				{

					*addin ^=0x23;
				}

				
				pout.append(addin,1);
			
				
			}


		}
		break;
	case Mode_Decrypt:
		{
			
			for(int i=0,len=pin.length();i<len;i++)
			{
				char c = pin.at(i);
				char *addin = &c;
				index1 = i%24+1;
				index2 = i%7+1;
				index3 = i%3;
				index4 = i%11+1;
				index5 = i%5;
				//	*addin -= (index1+index2*index2+index3*2+index4*3);
				*addin ^=0x7f;
				if(index2==0)
				{
					*addin ^=0x42;
				}
				if(index5!=0)
				{

					*addin ^=0x23;
				}

				pout.append(addin,1);
			}


		}
		break;
	default:
		break;
	}
	
}
