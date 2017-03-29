#pragma once
#include "cocos2d.h"
#include "platform/CCPlatformConfig.h"
#include <string>
#include "base/CCData.h"
USING_NS_CC;
using namespace std;
class GameData
{
public:
	
	static GameData* getInstance();
     
private:
	static GameData* instance;
	GameData(void);
	~GameData(void); 
public:
	

  /**
    @brief Get bool value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is false.
    * @js NA
    */
    bool    getBoolForKey(const char* pKey);
    /**
     * @js NA
     */
    bool    getBoolForKey(const char* pKey, bool defaultValue);
    /**
    @brief Get integer value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is 0.
    * @js NA
    */
    int     getIntegerForKey(const char* pKey);
    /**
     * @js NA
     */
    int     getIntegerForKey(const char* pKey, int defaultValue);
    /**
    @brief Get float value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is 0.0f.
    * @js NA
    */
    float    getFloatForKey(const char* pKey);
    /**
     * @js NA
     */
    float    getFloatForKey(const char* pKey, float defaultValue);
    /**
    @brief Get double value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is 0.0.
    * @js NA
    */
    double  getDoubleForKey(const char* pKey);
    /**
     * @js NA
     */
    double  getDoubleForKey(const char* pKey, double defaultValue);
    /**
    @brief Get string value by key, if the key doesn't exist, a default value will return.
    You can set the default value, or it is "".
    * @js NA
    */
    std::string getStringForKey(const char* pKey);
    /**
     * @js NA
     */
    std::string getStringForKey(const char* pKey, const std::string & defaultValue);
    /**
     @brief Get binary data value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is null.
     * @js NA
     * @lua NA
     */
    Data getDataForKey(const char* pKey);
    /**
     * @js NA
     * @lua NA
     */
    Data getDataForKey(const char* pKey, const Data& defaultValue);

    // set value methods

    /**
     @brief Set bool value by key.
     * @js NA
     */
    void    setBoolForKey(const char* pKey, bool value);
    /**
     @brief Set integer value by key.
     * @js NA
     */
    void    setIntegerForKey(const char* pKey, int value);
    /**
     @brief Set float value by key.
     * @js NA
     */
    void    setFloatForKey(const char* pKey, float value);
    /**
     @brief Set double value by key.
     * @js NA
     */
    void    setDoubleForKey(const char* pKey, double value);
    /**
     @brief Set string value by key.
     * @js NA
     */
    void    setStringForKey(const char* pKey, const std::string & value);
    /**
     @brief Set binary data value by key.
     * @js NA
     * @lua NA
     */
    void    setDataForKey(const char* pKey, const Data& value);
    /**
     @brief Save content to xml file
     * @js NA
     */
    void    flush();

   
    static void destroyInstance();

    
    /**
     * @js NA
     */
    const static std::string& getXMLFilePath();
    /**
     * @js NA
     */
    static bool isXMLFileExist();

private:
 
    
    static bool createXMLFile();
    static void initXMLFilePath();
    
    static std::string _filePath;
    static bool _isFilePathInitialized;

	

};

class WriteIo
{
public:
	bool isFileExist(const std::string& pFileName);
	std::string getWritablePath();
	bool saveFile(const char* pContentString, const std::string& pFileName);
	bool deleteFile(const std::string& pFileName);
	bool mkDirM(const std::string& pDirName);
	 bool mkDir(const std::string& pDirName);
	 std::string convertPathFormatToUnixStyle(const std::string& path);
	 static WriteIo* getInstance();
private:
	static WriteIo *writeInstance;
	WriteIo();
	~WriteIo();

};