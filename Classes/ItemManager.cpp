#include "ItemManager.h"
#include "GamingScene.h"
#include "MonsterBase.h"
#include "BulletBase.h"
#include "CGlobal.h"
#include "AOEEntity.h"
#include "GameTaskManager.h"


ItemManager* ItemManager::create(const Vector<ImageView*>& list) {
	auto itemMgr = new ItemManager();
	if (itemMgr && itemMgr->init(list)) {
		itemMgr->autorelease();
	}
	else {
		CC_SAFE_DELETE(itemMgr);
	}
	return itemMgr;
}

bool ItemManager::init(const Vector<ImageView*>& list) {
	
	skilList.clear();
	for (int i = 0; i < list.size(); i++) {
		itemBtnList.pushBack(list.at(i));
	}

	return true;
}

void ItemManager::initFromParent() {
	//create icon and init icon poss
	//for (int i = 0; i < itemBtnList.size(); i++) {
	//	ImageView* btn = itemBtnList.at(i);
	//	auto icon = Sprite::create(StringUtils::format("game/item/item_%d.png", i+1));
	//	getParent()->addChild(icon, 6);
	//	icon->setPosition(btn->getPosition());
	//	icon->setPositionX(icon->getPositionX() - 12);
	//}


	//craete itemBar


	auto skills = GameConfig::getInstance()->getAllSkill();

	int index=0;
	for (SkillInfo& skInfo : skills)
	{
		//这儿的技能信息需要和角色等级挂钩，所有一个计算公式
		skInfo.externalData = GameConfig::getInstance()->getSkillData(skInfo.skType, GameConfig::getInstance()->getCurrentLevel());
		auto skill = SkillFactory::getInstance()->createSkill(skInfo);
		if (skill)
		{
			skill->initBase(itemBtnList.at(index));
			this->addChild(skill);
			skilList.push_back(skill);
		}
		index++;
	}

}

void ItemManager::useItemByID(SkillInfo::SkillType itemID,int num) {
	if (!getParent()) {
		return;
	}

	for (auto skill:skilList)
	{
		if (skill->getSkillType() == itemID)
		{
			if (skill->getCurrentNum() + num < 0)
			{
				//技能点不足   赶紧买买买
				log("skill not enough,buy buy buy..........");

				SkillInfo skinfo = GameConfig::getInstance()->getSkillByType(skill->getSkillType());
				int current = GameConfig::getInstance()->getGameItem(skinfo.pricetag);
				if (GameData::getInstance()->getIntegerForKey(SAVEDATA_AUTOBUY_SKILL, -1) == 1 && current >= skinfo.price)
				{
					//购买
					GameConfig::getInstance()->changeSkillNum(skill->getSkillType(), 1);
					GameConfig::getInstance()->storeSkillNum();
					GameConfig::getInstance()->addGameItem(skinfo.pricetag, -skinfo.price);
					useItemByID(itemID, num);
				}
				else
				{
					auto fight = dynamic_cast<GamingScene*>(getParent());
					if (fight)
					{
						fight->setBuySkillType(skill->getSkillType());
						fight->changeState(GameState_Buy);
					}
				}

				
			}
			else
			{
				if (!skill->getCanUse()) return; //技能还在CD

				skill->changeSkillNum(num);
				if (num < 0)
				{
					GameTaskManager::getInstance()->punish(GameTaskId_UseSkillLimit, new int[1]{itemID}, 1);
					GameTaskManager::getInstance()->punish(GameTaskId_UseSkillMore, new int[1]{itemID}, 1);
					skill->useSkill();
				}
			}
	
		
			break;
		}
	}
	
}

void ItemManager::freshUI()
{
	for (auto skill : skilList)
	{
		skill->freshUI();
	}
}


Vec2 ItemManager::getSkillImagePosition(SkillInfo::SkillType itemID)
{
	if (itemID - 1 >= 0 && itemID - 1 < itemBtnList.size())
	{
		return itemBtnList.at(itemID - 1)->getParent()->convertToWorldSpace(itemBtnList.at(itemID - 1)->getPosition());
	}
	return Vec2::ZERO;
	
}

