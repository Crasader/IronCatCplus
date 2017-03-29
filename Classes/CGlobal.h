#pragma once
#include "Singleton.h"
#include "cocos2d.h"
#include "GamingScene.h"
#include "GlobalPath.h"
#include "GameMusicTools.h"
#include "PayCBDelegate.h"
USING_NS_CC;
class GamingScene;
class CCGlobal:public Singleton<CCGlobal>
{
public:
	CCGlobal();
	virtual ~CCGlobal();
	CC_SYNTHESIZE(GamingScene*, _gameScene, GamingScene);
	CC_SYNTHESIZE(float, enemySpeedFactor, EnemySpeedFactor);
	CC_SYNTHESIZE(GameState, _gameState, GameState);
	CC_SYNTHESIZE(int,currentEfficiency,CurrentEfficiency); //�����ȡ�ĵ�ǰս��ֵ
	CC_SYNTHESIZE(std::vector<EfficiencyDelegate::EfficiencyValue>, globalEfficiencies, GlobalEfficiencies); //�����ȡ�ĵ�ǰ����ս������

	CC_SYNTHESIZE(std::string,myuserid,SelfId);
	CC_SYNTHESIZE(std::string,mytoken,SelfToken);
	CC_SYNTHESIZE(std::string,channel,Channel);
	CC_SYNTHESIZE(std::string,versionId,Version);
	CC_SYNTHESIZE(std::string,areaType,AreaType); //����
};

