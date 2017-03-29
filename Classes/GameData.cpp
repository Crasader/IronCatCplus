
#include "GameData.h"
#if CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM==CC_PLATFORM_WIN32
#include "tables.h"
#include "platform/CCCommon.h"
#include "platform/CCFileUtils.h"
#include "base/base64.h"
//#include "cocos2d/external/tinyxml2/tinyxml2.h"
#include "platform/CCPlatformConfig.h"
#include "external/tinyxml2/tinyxml2.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#include "dirent.h"
#include <sys/stat.h>
#elif CC_TARGET_PLATFORM==CC_PLATFORM_IOS
#include "dirent.h"
#include <sys/stat.h>
#endif

/*-------------------------------
 把DatIn开始的长度位Len位的二进制
 复制到DatOut后
--------------------------------*/
static void BitsCopy(bool *DatOut,bool *DatIn,int Len)     // 数组复制 OK 
{
    int i=0;
    for(i=0;i<Len;i++)
    {
        DatOut[i]=DatIn[i];
    }
}

/*-------------------------------
 字节转换成位函数 
 每8次换一个字节 每次向右移一位
 和1与取最后一位 共64位 
--------------------------------*/
static void ByteToBit(bool *DatOut,char *DatIn,int Num)       // OK
{
    int i=0;
    for(i=0;i<Num;i++)
    {
        DatOut[i]=(DatIn[i/8]>>(i%8))&0x01;   
    }                                       
}

/*-------------------------------
 位转换成字节函数
 字节数组每8次移一位
 位每次向左移 与上一次或   
---------------------------------*/
static void BitToByte(char *DatOut,bool *DatIn,int Num)        // OK
{
    int i=0;
    for(i=0;i<(Num/8);i++)
    {
        DatOut[i]=0;
    } 
    for(i=0;i<Num;i++)
    {
        DatOut[i/8]|=DatIn[i]<<(i%8);    
    }        
}


/*----------------------------------
 二进制密文转换为十六进制
 需要16个字符表示
-----------------------------------*/
static void BitToHex(char *DatOut,bool *DatIn,int Num)
{
    int i=0;
    for(i=0;i<Num/4;i++)
    {
        DatOut[i]=0;
    }
    for(i=0;i<Num/4;i++)
    {
        DatOut[i] = DatIn[i*4]+(DatIn[i*4+1]<<1)
                    +(DatIn[i*4+2]<<2)+(DatIn[i*4+3]<<3);
        if((DatOut[i]%16)>9)
        {
            DatOut[i]=DatOut[i]%16+'7';       //  余数大于9时处理 10-15 to A-F
        }                                     //  输出字符 
        else
        {
            DatOut[i]=DatOut[i]%16+'0';       //  输出字符       
        }
    }
    
}

/*---------------------------------------------
 十六进制字符转二进制
----------------------------------------------*/
static void HexToBit(bool *DatOut,char *DatIn,int Num)
{
    int i=0;                        // 字符型输入 
    for(i=0;i<Num;i++)
    {
        if((DatIn[i/4])>'9')         //  大于9 
        {
            DatOut[i]=((DatIn[i/4]-'7')>>(i%4))&0x01;               
        }
        else
        {
            DatOut[i]=((DatIn[i/4]-'0')>>(i%4))&0x01;     
        } 
    }    
}

// 表置换函数  OK
static void TablePermute(bool *DatOut,bool *DatIn,const char *Table,int Num)  
{
    int i=0;
    static bool Temp[256]={0};
    for(i=0;i<Num;i++)                // Num为置换的长度 
    {
        Temp[i]=DatIn[Table[i]-1];  // 原来的数据按对应的表上的位置排列 
    }
    BitsCopy(DatOut,Temp,Num);       // 把缓存Temp的值输出 
} 

// 子密钥的移位
static void LoopMove(bool *DatIn,int Len,int Num) // 循环左移 Len数据长度 Num移动位数
{
    static bool Temp[256]={0};    // 缓存   OK
    BitsCopy(Temp,DatIn,Num);       // 将数据最左边的Num位(被移出去的)存入Temp 
    BitsCopy(DatIn,DatIn+Num,Len-Num); // 将数据左边开始的第Num移入原来的空间
    BitsCopy(DatIn+Len-Num,Temp,Num);  // 将缓存中移出去的数据加到最右边 
} 

// 按位异或
static void Xor(bool *DatA,bool *DatB,int Num)           // 异或函数
{
    int i=0;
    for(i=0;i<Num;i++)
    {
        DatA[i]=DatA[i]^DatB[i];                  // 异或 
    }
} 

// 输入48位 输出32位 与Ri异或
static void S_Change(bool DatOut[32],bool DatIn[48])     // S盒变换
{
    int i,X,Y;                                    // i为8个S盒 
    for(i=0,Y=0,X=0;i<8;i++,DatIn+=6,DatOut+=4)   // 每执行一次,输入数据偏移6位 
    {                                              // 每执行一次,输出数据偏移4位
        Y=(DatIn[0]<<1)+DatIn[5];                          // af代表第几行
        X=(DatIn[1]<<3)+(DatIn[2]<<2)+(DatIn[3]<<1)+DatIn[4]; // bcde代表第几列
        ByteToBit(DatOut,&S_Box[i][Y][X],4);      // 把找到的点数据换为二进制    
    }
}

// F函数
static void F_Change(bool DatIn[32],bool DatKi[48])       // F函数
{
    static bool MiR[48]={0};             // 输入32位通过E选位变为48位
    TablePermute(MiR,DatIn,E_Table,48); 
    Xor(MiR,DatKi,48);                   // 和子密钥异或
    S_Change(DatIn,MiR);                 // S盒变换
    TablePermute(DatIn,DatIn,P_Table,32);   // P置换后输出
}



static void SetKey(char KeyIn[8])               // 设置密钥 获取子密钥Ki 
{
    int i=0;
    static bool KeyBit[64]={0};                // 密钥二进制存储空间 
    static bool *KiL=&KeyBit[0],*KiR=&KeyBit[28];  // 前28,后28共56
    ByteToBit(KeyBit,KeyIn,64);                    // 把密钥转为二进制存入KeyBit 
    TablePermute(KeyBit,KeyBit,PC1_Table,56);      // PC1表置换 56次
    for(i=0;i<16;i++)
    {
        LoopMove(KiL,28,Move_Table[i]);       // 前28位左移 
        LoopMove(KiR,28,Move_Table[i]);          // 后28位左移 
         TablePermute(SubKey[i],KeyBit,PC2_Table,48); 
         // 二维数组 SubKey[i]为每一行起始地址 
         // 每移一次位进行PC2置换得 Ki 48位 
    }        
}

static void PlayDes(char MesOut[8],char MesIn[8])  // 执行DES加密
{                                           // 字节输入 Bin运算 Hex输出 
    int i=0;
    static bool MesBit[64]={0};        // 明文二进制存储空间 64位
    static bool Temp[32]={0};
    static bool *MiL=&MesBit[0],*MiR=&MesBit[32]; // 前32位 后32位 
    ByteToBit(MesBit,MesIn,64);                 // 把明文换成二进制存入MesBit
    TablePermute(MesBit,MesBit,IP_Table,64);    // IP置换 
    for(i=0;i<16;i++)                       // 迭代16次 
    {
        BitsCopy(Temp,MiR,32);            // 临时存储
        F_Change(MiR,SubKey[i]);           // F函数变换
        Xor(MiR,MiL,32);                  // 得到Ri 
        BitsCopy(MiL,Temp,32);            // 得到Li 
    }                    
    TablePermute(MesBit,MesBit,IPR_Table,64);
    BitToHex(MesOut,MesBit,64);
}

static void KickDes(char MesOut[8],char MesIn[8])       // 执行DES解密
{                                                // Hex输入 Bin运算 字节输出 
    int i=0;
    static bool MesBit[64]={0};        // 密文二进制存储空间 64位
    static bool Temp[32]={0};
    static bool *MiL=&MesBit[0],*MiR=&MesBit[32]; // 前32位 后32位
    HexToBit(MesBit,MesIn,64);                 // 把密文换成二进制存入MesBit
    TablePermute(MesBit,MesBit,IP_Table,64);    // IP置换 
    for(i=15;i>=0;i--)
    {
        BitsCopy(Temp,MiL,32);
        F_Change(MiL,SubKey[i]);
        Xor(MiL,MiR,32);
        BitsCopy(MiR,Temp,32);
    }    
    TablePermute(MesBit,MesBit,IPR_Table,64);
    BitToByte(MesOut,MesBit,64);        
}




GameData* GameData::instance = nullptr;
/**
 * implements of GameData
 */

string GameData::_filePath = string("");
bool GameData::_isFilePathInitialized = false;

// root name of xml
#define USERDEFAULT_ROOT_NAME    "GameDataRoot"

#define XML_FILE_NAME "GameData.udat"
#define PUBLIC_KEY "NVLDOWQ1"
/*-------------------------------------------------------
      Data Encryption Standard  56位密钥加密64位数据 
                  2011.10
--------------------------------------------------------*/
GameData::GameData(void)
{

}


GameData::~GameData(void)
{
}

GameData* GameData::getInstance()
{
	initXMLFilePath();

	// only create xml file one time
	// the file exists after the program exit
	if ((! isXMLFileExist()) && (! createXMLFile()))
	{
		return nullptr;
	}

	if(!instance)
	{
		instance = new GameData;
	}
	return instance;
}
void GameData::destroyInstance()
{
	CC_SAFE_DELETE(instance);
}
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)



/**
 * define the functions here because we don't want to
 * export xmlNodePtr and other types in "CCUserDefault.h"
 */

static tinyxml2::XMLElement* getXMLNodeForKey(const char* pKey, tinyxml2::XMLElement** rootNode, tinyxml2::XMLDocument **doc)
{
    tinyxml2::XMLElement* curNode = nullptr;

    // check the key value
    if (! pKey)
    {
        return nullptr;
    }

    do 
    {
 		tinyxml2::XMLDocument* xmlDoc = new tinyxml2::XMLDocument();
		*doc = xmlDoc;

        std::string xmlBuffer = FileUtils::getInstance()->getStringFromFile(GameData::getInstance()->getXMLFilePath());
		
		//SetKey(YourKey);                                       // 设置密钥

		//KickDes(decMessage,MesHex);                     // 解密输出到MyMessage
		//xmlBuffer.length();
		
		char decMessage[8];
		char MesHex[17]={0};         // 16个字符数组用于存放 64位16进制的密文
		SetKey(PUBLIC_KEY); 
		std::string decryptBuffer;
		for(int i=0,len=xmlBuffer.length();i<len;i+=16)
		{
			if(len-i<16)
			{
				sprintf(MesHex,"%s",xmlBuffer.substr(i,len-i).c_str());

			}
			else
			{
				sprintf(MesHex,"%s",xmlBuffer.substr(i,16).c_str());
			}
			KickDes(decMessage,MesHex);
			decryptBuffer.append(decMessage,8);
		}

		

		if (decryptBuffer.empty())
		{
			CCLOG("can not read xml file");
			break;
		}
		xmlDoc->Parse(decryptBuffer.c_str(), decryptBuffer.size());

		// get root node
		*rootNode = xmlDoc->RootElement();
		if (nullptr == *rootNode)
		{
			CCLOG("read root node error");
			break;
		}
		// find the node
		curNode = (*rootNode)->FirstChildElement();
		while (nullptr != curNode)
		{
			const char* nodeName = curNode->Value();
			if (!strcmp(nodeName, pKey))
			{
				break;
			}

			curNode = curNode->NextSiblingElement();
		}
	} while (0);

	
	return curNode;
}

static void setValueForKey(const char* pKey, const char* pValue)
{
 	tinyxml2::XMLElement* rootNode = NULL;
	tinyxml2::XMLDocument* doc = NULL;
	tinyxml2::XMLElement* node = NULL;
	// check the params
	if (! pKey || ! pValue)
	{
		return;
	}
	// find the node
	node = getXMLNodeForKey(pKey, &rootNode, &doc);
	// if node exist, change the content
	if (node)
	{
        if (node->FirstChild())
        {
            node->FirstChild()->SetValue(pValue);
        }
        else
        {
            tinyxml2::XMLText* content = doc->NewText(pValue);
            node->LinkEndChild(content);
        }
	}
	else
	{
		if (rootNode)
		{
			tinyxml2::XMLElement* tmpNode = doc->NewElement(pKey);//new tinyxml2::XMLElement(pKey);
			rootNode->LinkEndChild(tmpNode);
			tinyxml2::XMLText* content = doc->NewText(pValue);//new tinyxml2::XMLText(pValue);
			tmpNode->LinkEndChild(content);
		}	
	}

    // here get the string of the document ,encrypt it and store in the xml file
	


	if (doc)
	{
		tinyxml2::XMLPrinter mprinter;
		doc->Print(&mprinter);
		std::string docStr = mprinter.CStr();

		char MyMessage[9]={0};       // 初始明文 
		char MesHex[16]={0};         // 16个字符数组用于存放 64位16进制的密文
		SetKey(PUBLIC_KEY); 
		std::string decryptBuffer;
		for(int i=0,len=docStr.length();i<len;i+=8)
		{
			if(len-i<8)
			{
				sprintf(MyMessage,"%s",docStr.substr(i,len-i).c_str());

			}
			else
			{
				sprintf(MyMessage,"%s",docStr.substr(i,8).c_str());
			}
			PlayDes(MesHex,MyMessage);   // 执行DES加密
			decryptBuffer.append(MesHex,16);
		}

		WriteIo::getInstance()->saveFile(decryptBuffer.c_str(),GameData::getInstance()->getXMLFilePath().c_str());
		//doc->SaveFile(GameData::getInstance()->getXMLFilePath().c_str());
		delete doc;
	}
}



bool GameData::getBoolForKey(const char* pKey)
{
 return getBoolForKey(pKey, false);
}

bool GameData::getBoolForKey(const char* pKey, bool defaultValue)
{
    const char* value = nullptr;
	tinyxml2::XMLElement* rootNode;
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* node;
	node =  getXMLNodeForKey(pKey, &rootNode, &doc);
	// find the node
	if (node && node->FirstChild())
	{
        value = (const char*)(node->FirstChild()->Value());
	}

	bool ret = defaultValue;

	if (value)
	{
		ret = (! strcmp(value, "true"));
	}

    if (doc) delete doc;

	return ret;
}

int GameData::getIntegerForKey(const char* pKey)
{
    return getIntegerForKey(pKey, 0);
}

int GameData::getIntegerForKey(const char* pKey, int defaultValue)
{
	const char* value = nullptr;
	tinyxml2::XMLElement* rootNode;
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* node;
	node =  getXMLNodeForKey(pKey, &rootNode, &doc);
	// find the node
	if (node && node->FirstChild())
	{
        value = (const char*)(node->FirstChild()->Value());
	}

	int ret = defaultValue;

	if (value)
	{
		ret = atoi(value);
	}

	if(doc)
	{
		delete doc;
	}


	return ret;
}

float GameData::getFloatForKey(const char* pKey)
{
    return getFloatForKey(pKey, 0.0f);
}

float GameData::getFloatForKey(const char* pKey, float defaultValue)
{
    float ret = (float)getDoubleForKey(pKey, (double)defaultValue);
 
    return ret;
}

double  GameData::getDoubleForKey(const char* pKey)
{
    return getDoubleForKey(pKey, 0.0);
}

double GameData::getDoubleForKey(const char* pKey, double defaultValue)
{
	const char* value = nullptr;
	tinyxml2::XMLElement* rootNode;
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* node;
	node =  getXMLNodeForKey(pKey, &rootNode, &doc);
	// find the node
	if (node && node->FirstChild())
	{
        value = (const char*)(node->FirstChild()->Value());
	}

	double ret = defaultValue;

	if (value)
	{
		ret = atof(value);
	}

    if (doc) delete doc;

	return ret;
}

std::string GameData::getStringForKey(const char* pKey)
{
    return getStringForKey(pKey, "");
}

string GameData::getStringForKey(const char* pKey, const std::string & defaultValue)
{
    const char* value = nullptr;
	tinyxml2::XMLElement* rootNode;
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* node;
	node =  getXMLNodeForKey(pKey, &rootNode, &doc);
	// find the node
	if (node && node->FirstChild())
	{
        value = (const char*)(node->FirstChild()->Value());
	}

	string ret = defaultValue;

	if (value)
	{
		ret = string(value);
	}

    if (doc) delete doc;

	return ret;
}

Data GameData::getDataForKey(const char* pKey)
{
    return getDataForKey(pKey, Data::Null);
}

Data GameData::getDataForKey(const char* pKey, const Data& defaultValue)
{
    const char* encodedData = nullptr;
	tinyxml2::XMLElement* rootNode;
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* node;
	node =  getXMLNodeForKey(pKey, &rootNode, &doc);
	// find the node
	if (node && node->FirstChild())
	{
        encodedData = (const char*)(node->FirstChild()->Value());
	}
    
	Data ret = defaultValue;
    
	if (encodedData)
	{
        unsigned char * decodedData = nullptr;
        int decodedDataLen = base64Decode((unsigned char*)encodedData, (unsigned int)strlen(encodedData), &decodedData);
        
        if (decodedData) {
            ret.fastSet(decodedData, decodedDataLen);
        }
	}
    
    if (doc) delete doc;
    
	return ret;    
}


void GameData::setBoolForKey(const char* pKey, bool value)
{
    // save bool value as string

    if (true == value)
    {
        setStringForKey(pKey, "true");
    }
    else
    {
        setStringForKey(pKey, "false");
    }
}

void GameData::setIntegerForKey(const char* pKey, int value)
{
    // check key
    if (! pKey)
    {
        return;
    }

    // format the value
    char tmp[50];
    memset(tmp, 0, 50);
    sprintf(tmp, "%d", value);

    setValueForKey(pKey, tmp);
}

void GameData::setFloatForKey(const char* pKey, float value)
{
    setDoubleForKey(pKey, value);
}

void GameData::setDoubleForKey(const char* pKey, double value)
{
    // check key
    if (! pKey)
    {
        return;
    }

    // format the value
    char tmp[50];
    memset(tmp, 0, 50);
    sprintf(tmp, "%f", value);

    setValueForKey(pKey, tmp);
}

void GameData::setStringForKey(const char* pKey, const std::string & value)
{
    // check key
    if (! pKey)
    {
        return;
    }

    setValueForKey(pKey, value.c_str());
}

void GameData::setDataForKey(const char* pKey, const Data& value) {
    // check key
    if (! pKey)
    {
        return;
    }

    char *encodedData = 0;
    
    base64Encode(value.getBytes(), static_cast<unsigned int>(value.getSize()), &encodedData);
        
    setValueForKey(pKey, encodedData);
    
    if (encodedData)
        free(encodedData);
}


bool GameData::isXMLFileExist()
{
    FILE *fp = fopen(_filePath.c_str(), "r");
	bool bRet = false;

	if (fp)
	{
		bRet = true;
		fclose(fp);
	}

	return bRet;
}

void GameData::initXMLFilePath()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    if (! _isFilePathInitialized)
    {
        _filePath += FileUtils::getInstance()->getWritablePath() + XML_FILE_NAME;
        _isFilePathInitialized = true;
    }    
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	
	    if (! _isFilePathInitialized)
	    {
	        // GameData.xml is stored in /data/data/<package-path>/ before v2.1.2
	        _filePath += "/data/data/" + getPackageNameJNI() + "/" + XML_FILE_NAME;
	        _isFilePathInitialized = true;
	    }
#endif
}

// create new xml file
bool GameData::createXMLFile()
{
	bool bRet = false;  
	
    tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument(); 
    if (nullptr==pDoc)  
    {  
        return false;  
    }  
	tinyxml2::XMLDeclaration *pDeclaration = pDoc->NewDeclaration(nullptr);  
	if (nullptr==pDeclaration)  
	{  
		return false;  
	}  
	pDoc->LinkEndChild(pDeclaration); 
	tinyxml2::XMLElement *pRootEle = pDoc->NewElement(USERDEFAULT_ROOT_NAME);  
	if (nullptr==pRootEle)  
	{  
		return false;  
	}  
	pDoc->LinkEndChild(pRootEle);  

	tinyxml2::XMLPrinter mprinter;
	pDoc->Print(&mprinter);
	std::string docStr = mprinter.CStr();

	char MyMessage[9];       // 初始明文 
	char MesHex[16]={0};         // 16个字符数组用于存放 64位16进制的密文
	SetKey(PUBLIC_KEY); 
	
	std::string decryptBuffer;
	for(int i=0,len=docStr.length();i<len;i+=8)
	{
		if(len-i<8)
		{
			sprintf(MyMessage,"%s",docStr.substr(i,len-i).c_str());

		}
		else
		{
			sprintf(MyMessage,"%s",docStr.substr(i,8).c_str());
		}
		PlayDes(MesHex,MyMessage);   // 执行DES加密
		int declen = strlen(MesHex);
		decryptBuffer.append(MesHex,16);
	}

	
	bRet = WriteIo::getInstance()->saveFile(decryptBuffer.c_str(),_filePath.c_str());

	
	if(pDoc)
	{
		delete pDoc;
	}

	return bRet;
}

const string& GameData::getXMLFilePath()
{
    return _filePath;
}

void GameData::flush()
{
}
#endif

WriteIo* WriteIo::writeInstance = nullptr;
WriteIo* WriteIo::getInstance()
{
	if(!writeInstance)
	{
		writeInstance = new WriteIo;
	}
	return writeInstance;
}
WriteIo::WriteIo()
{

}
WriteIo::~WriteIo()
{

}

bool WriteIo::saveFile(const char* pContentString, const std::string& pFileName)  
{  

	std::string path = pFileName;  
	FILE* file = fopen(path.c_str(), "w");    
	if (file)  
	{    
		fputs(pContentString, file);    
		fclose(file);   
		//log("save file [%s]",path.c_str());    
		return true;  
	}  
	else  
		log("fail to save file [%s]!",path.c_str());  
	return false;  
}  

bool WriteIo::deleteFile(const std::string& pFileName)
{
	if(isFileExist(pFileName))
	{
		std::remove(pFileName.c_str());
		return true;
	}
	return false;
	
}
//检测各级文件夹，不存在则创建  
bool WriteIo::mkDirM(const std::string& pDirName)  
{  
	std::string path = getWritablePath();  
	int np=pDirName.find('/',0);  
	while(np!=std::string::npos)  
	{  
		if(!mkDir(path+pDirName.substr(0,np)))  
			return false;  
		np=pDirName.find('/',np+1);  
	}  
	return mkDir(path+pDirName);  
}  

//创建文件夹  
bool WriteIo::mkDir(const std::string& pDirName)  
{  
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	DIR *pDir = NULL;  
	//打开该路径  
	pDir = opendir (pDirName.c_str());  
	if (! pDir)  
	{  
		//创建该路径  
		if(!mkdir(pDirName.c_str(), S_IRWXU | S_IRWXG | S_IRWXO))  
		{  
			log("fail to create dir [%s]",pDirName.c_str());  
			return false;  
		}  
		log("create dir [%s]",pDirName.c_str());  
	}  
#else  
	if ((GetFileAttributesA(pDirName.c_str())) == INVALID_FILE_ATTRIBUTES)  
	{  
		if(!CreateDirectoryA(pDirName.c_str(), 0))  
		{  
			log("fail to create dir [%s]",pDirName.c_str());  
			return false;  
		}  
		log("create dir [%s]",pDirName.c_str());  
	}  
#endif  

	return true;  
}  
//路径格式转为UnixStyle，"c:\xxx.txt" --> "c:/xxx.txt"  
std::string WriteIo::convertPathFormatToUnixStyle(const std::string& path)  
{   
	std::string ret = path; int len = ret.length();  
	for (int i = 0; i < len; ++i)   
	{   
		if (ret[i] == '\\')   
		{   
			ret[i] = '/';   
		}   
	}   
	return ret;  
}  
std::string WriteIo::getWritablePath()  
{  
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)  
	return CCFileUtils::getInstance()->getWritablePath();  
#else  
	char full_path[MAX_PATH + 1];  
	::GetModuleFileNameA(NULL, full_path,MAX_PATH + 1);  
	std::string ret((char*)full_path);  
	// remove xxx.exe  
	ret =  ret.substr(0, ret.rfind("\\") + 1);  
	ret = convertPathFormatToUnixStyle(ret);  
	return ret;  
#endif  
} 
bool WriteIo::isFileExist(const std::string& pFileName)  
{  
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)  
	return CCFileUtils::getInstance()->isFileExist(pFileName);  
#else  
	if(GetFileAttributesA(pFileName.c_str()) == INVALID_FILE_ATTRIBUTES)  
		return false;  
	return true;  
#endif  
} 
#endif