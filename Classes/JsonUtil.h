#ifndef _JsonUtil_H_
#define _JsonUtil_H_

#include "cocos2d.h"
USING_NS_CC;

class JsonUtil {
public:
	static JsonUtil* getInstance();

	void parseMonsterDataFormFile(std::string filePath);
	ValueMap getMonsterData();

	void parseWeaponBaseFormFile(std::string filePath);
	ValueMap getWeaponBase();

	void parseRoleBaseFormFile(std::string filePath);
	ValueMap getRoleBase();

private:
	static JsonUtil* _JsonUtil;
	ValueMap _monData;
	ValueMap _wpbData;
	ValueMap _roleData;
};

#endif