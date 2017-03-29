#include "JsonUtil.h"
#include "jsonlib/reader.h"
#include "tinyxml2/tinyxml2.h"

JsonUtil* JsonUtil::_JsonUtil = NULL;

JsonUtil* JsonUtil::getInstance() {
	if (_JsonUtil == NULL) {
		_JsonUtil = new JsonUtil();
	}
	return _JsonUtil;
}

void JsonUtil::parseMonsterDataFormFile(std::string filePath) {
	_monData.clear();
	Json::Reader reader;
	Json::Value root;

	std::string monStr = FileUtils::getInstance()->getStringFromFile(filePath);

	if (reader.parse(monStr, root, false)) {
		ValueMap sample;
		for (unsigned i = 0; i < root.size(); i++) {
			int id = root[i]["id"].asInt();
			sample["id"] = id;
			sample["modelID"] = root[i]["modelID"].asInt();
			sample["hp"] = root[i]["hp"].asInt();
			sample["speed"] = root[i]["speed"].asInt();
			sample["weapon"] = root[i]["weapon"].asInt();
			sample["atk"] = root[i]["atk"].asInt();
			_monData[StringUtils::toString(id)] = sample;
		}
	}
}

ValueMap JsonUtil::getMonsterData() {
	return _monData;
}

void JsonUtil::parseWeaponBaseFormFile(std::string filePath) {
	_wpbData.clear();
	Json::Reader reader;
	Json::Value root;
	
	std::string wpStr = FileUtils::getInstance()->getStringFromFile(filePath);

	if (reader.parse(wpStr, root, false)) {
		ValueMap sample;
		for (unsigned i = 0; i < root.size(); i++) {
			sample["id"] = root[i]["id"].asInt();
			sample["name"] = root[i]["name"].asString();
			sample["modelID"] = root[i]["modelID"].asInt();
			sample["reloadTime"] = root[i]["reloadTime"].asDouble();
			sample["shotInterval"] = root[i]["shotInterval"].asDouble();
			sample["bulletNum"] = root[i]["bulletNum"].asInt();
			sample["atk"] = root[i]["atk"].asDouble();
			sample["level"] = 1;
			if (i < 3) {
				sample["isBuy"] = true;
			}
			else {
				sample["isBuy"] = false;
			}
			_wpbData[StringUtils::toString(i + 1)] = sample;
		}
	}
}

ValueMap JsonUtil::getWeaponBase() {
	return _wpbData;
}

void JsonUtil::parseRoleBaseFormFile(std::string filePath) {
	_roleData.clear();
	Json::Reader reader;
	Json::Value root;

	std::string roleStr = FileUtils::getInstance()->getStringFromFile(filePath);

	if (reader.parse(roleStr, root, false)) {
		ValueMap sample;
		for (unsigned i = 0; i < root.size(); i++) {
			sample["id"] = root[i]["id"].asInt();
			sample["name"] = root[i]["name"].asString();
			sample["level"] = root[i]["level"].asInt();
			sample["hp"] = root[i]["hp"].asInt();
			sample["exp"] = root[i]["exp"].asInt();
			sample["upgradeHp"] = root[i]["upgradeHp"].asInt();
			_roleData[StringUtils::toString(i + 1)] = sample;
		}
	}
}

ValueMap JsonUtil::getRoleBase() {
	return _roleData;
}