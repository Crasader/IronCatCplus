#include "WeaponAction.h"
#include "PoolManager.h"
#include "CGlobal.h"
#include "FightRole.h"
#include "GameCompanion.h"
#include "BulletBomb.h"
#include "BulletIce.h"
#include "BulletTorpedo.h"
#include "BulletElectromagnetic.h"
#include "Boss.h"

WeaponAction::WeaponAction() :weaponType(BulletInfo::BulletType::Bullet_Null), bulletFile("")
{
}


WeaponAction::~WeaponAction()
{
}
/********************************普通子弹Action***********************************************************/
WeaponNormalAction::WeaponNormalAction() :WeaponAction()
{
	weaponType = BulletInfo::Bullet_Normal;
	bulletFile = "game/bullets/bulletNormal.png";
}
void WeaponNormalAction::shotAction(Entity* emitterHost, Vec2 targetPos/* = Vec2::ZERO*/)
{
	//后期可以从配置文件中读取
	BulletInfo binfo;
	binfo.bType = weaponType;
	binfo.attNum = emitterHost->getAttNum();
	binfo.parentType = emitterHost->getMyType();
	binfo.bulletRes = bulletFile;
	switch (binfo.parentType)
	{
	case Type_Player:
	{
			auto host = dynamic_cast<FightRole*>(emitterHost);
			if (host)
			{
				//主角子弹伤害浮动
				binfo.attNum += cocos2d::random(-10, 10);
				binfo.attNum = std::max(0, binfo.attNum);

				binfo.weaponID = host->getCurrentWeaponSid();
				float speedMax = 2200;
				binfo.mSpeed = Vec2(speedMax * host->getWeaponDir().x, speedMax * host->getWeaponDir().y);
				//binfo.mAccelerate = Vec2(0, 1);
			}
	}

		break;
	case Type_Boss:
	{
		//boss 的普通子弹
		binfo.bulletRes = StringUtils::format("game/bullets/monster_normal_%d.png", 1);
		binfo.mSpeed = emitterHost->getCurrentSpeed();
		//binfo.mAccelerate = targetPos*emitterHost->getAccelerate().length();
	}
		break;
	case Type_Enemy:
	{
		//随机射击位置

		binfo.bulletRes = StringUtils::format("game/bullets/monster_normal_%d.png", emitterHost->getLevel()>5?2:1);
		targetPos.normalize();
		if (targetPos == Vec2::ZERO)
		{
			binfo.mSpeed = Vec2(0, emitterHost->getCurrentSpeed().y*2);
			binfo.mSpeed = Vec2(0, emitterHost->getCurrentSpeed().y*2);
		}
		else
		{
			binfo.mSpeed = targetPos*emitterHost->getCurrentSpeed().length()*2;
			binfo.mAccelerate = targetPos*emitterHost->getAccelerate().length()*2;
		}

	}

		break;
	case Type_Companion:
	{
		binfo.weaponID = -1;
		float speedMax = 1800;
		binfo.mSpeed = speedMax*targetPos;
		binfo.mAccelerate = targetPos*2;
		
	}
		break;
	default:
		break;
	}
	

	BulletBase* bullet =  BulletPool::getInstance()->acquireObject(binfo);
	switch (binfo.parentType)
	{
	
	case Type_Player:
	{
		CCGlobal::getInstance()->getGamingScene()->pArrPlayerBullet->addObject(bullet);
		bullet->setPosition(dynamic_cast<FightRole*>(emitterHost)->getShotPos());
		//加特林和普通枪
		
		auto weapon = GameConfig::getInstance()->getWeaponDataBySid(dynamic_cast<FightRole*>(emitterHost)->getCurrentWeaponSid());
		if (weapon&&weapon->getType() == 6) //6代表加特林
		{
			GameMusicTools::getInstance()->playSoundEffect(EFFECT_GATELY_GUN_SHOT); //加特林
		}
		else
		{
			GameMusicTools::getInstance()->playSoundEffect(EFFECT_GUN_SHOT_1); //普通枪
		}
		
	}
		break;
	case Type_Boss:
	{
		CCGlobal::getInstance()->getGamingScene()->pArrEnemyBullet->addObject(bullet);
		bullet->setPosition(targetPos);
		auto monster = dynamic_cast<MonsterBase*>(emitterHost);
		if (monster)
		{
			monster->addLifeObserver(bullet);
		}
	}
		break;
	case Type_Enemy:
	{
		if (emitterHost->getName() == "monsterguide1")
		{
			bullet->setName("bulletguide");
		}

		CCGlobal::getInstance()->getGamingScene()->pArrEnemyBullet->addObject(bullet);
		bullet->setPosition(emitterHost->getPosition());
		auto monster = dynamic_cast<MonsterBase*>(emitterHost);
		if (monster)
		{
			monster->addLifeObserver(bullet);
		}
	}
		break;
	case Type_Companion:
	{
		CCGlobal::getInstance()->getGamingScene()->pArrPlayerBullet->addObject(bullet);
		bullet->setPosition(dynamic_cast<GameCompanion*>(emitterHost)->getShotPos());
	}
		break;
	default:
		break;
	}
	CCGlobal::getInstance()->getGamingScene()->gamelogicLayer->addChild(bullet, emitterHost->getLevel() + emitterHost->getAttNum());
	
	bullet->start();
	
}
/********************************普通子弹连发Action***********************************************************/
WeaponNumberalAction::WeaponNumberalAction() :WeaponAction()
{
	weaponType = BulletInfo::Bullet_Normal;
	bulletFile = "game/bullets/bulletNormal.png";
	bulletNum = 3; // 默认三颗子弹
}

Vec2 WeaponNumberalAction::rotateAngle(Vec2 center, Vec2 startPoint, float angle)
{
	Vec2 retPos;
	retPos.x = (startPoint.x - center.x)*cos(angle*M_PI / 180) - (startPoint.y - center.y)*sin(angle*M_PI / 180) + center.x;
	retPos.y = (startPoint.x - center.x)*sin(angle*M_PI / 180) + (startPoint.y - center.y)*cos(angle*M_PI / 180) + center.y;
	return retPos;
}

void WeaponNumberalAction::shotAction(Entity* emitterHost, Vec2 targetPos)
{
	//散弹
	if (emitterHost->getMyType() == Type_Player)
	{
		GameMusicTools::getInstance()->playSoundEffect(EFFECT_NUMBERAL_GUN_SHOT); //普通枪
	}
	
	for (int i = 0; i < bulletNum; i++)
	{
		BulletInfo binfo;
		binfo.bType = weaponType;
		binfo.attNum = emitterHost->getAttNum();
		binfo.parentType = emitterHost->getMyType();
		binfo.bulletRes = bulletFile;
		switch (binfo.parentType)
		{
		case Type_Player:
		{
							binfo.bulletRes = "game/bullets/bulletNormal_3.png";
							auto host = dynamic_cast<FightRole*>(emitterHost);
							if (host)
							{
								//主角子弹伤害浮动
								binfo.attNum += cocos2d::random(-10, 10);
								binfo.attNum = std::max(0, binfo.attNum);

								binfo.weaponID = host->getCurrentWeaponSid();
								float speedMax = 2200;
								//这儿的速度方向需要根据子弹的数量来计算
								binfo.mSpeed = Vec2(speedMax * host->getWeaponDir().x, speedMax * host->getWeaponDir().y);
							
								//binfo.mAccelerate = Vec2(0, 1);
							}
		}

			break;
		case Type_Boss:
		{
						  //boss 的普通子弹
						  binfo.mSpeed = emitterHost->getCurrentSpeed();
					
						  //binfo.mAccelerate = targetPos*emitterHost->getAccelerate().length();
		}
			break;
		case Type_Enemy:
		{
						   //随机射击位置

						   binfo.bulletRes = StringUtils::format("game/bullets/monster_normal_%d.png", emitterHost->getLevel() > 5 ? 2 : 1);
						   targetPos.normalize();
						   if (targetPos == Vec2::ZERO)
						   {
							   binfo.mSpeed = Vec2(0, emitterHost->getCurrentSpeed().y * 2);
							   binfo.mSpeed = Vec2(0, emitterHost->getCurrentSpeed().y * 2);
						   }
						   else
						   {
							   binfo.mSpeed = targetPos*emitterHost->getCurrentSpeed().length() * 2;
							   binfo.mAccelerate = targetPos*emitterHost->getAccelerate().length() * 2;
						   }
						 

		}

			break;
		default:
			break;
		}
		binfo.mSpeed = rotateAngle(Vec2::ZERO, binfo.mSpeed, (std::floor(bulletNum / 2.0) - i) / (std::floor(bulletNum / 2.0)) * 8);

		BulletBase* bullet = BulletPool::getInstance()->acquireObject(binfo);
		switch (binfo.parentType)
		{

		case Type_Player:
		{
							CCGlobal::getInstance()->getGamingScene()->pArrPlayerBullet->addObject(bullet);
							bullet->setPosition(dynamic_cast<FightRole*>(emitterHost)->getShotPos());
							
		}
			break;
		case Type_Boss:
		{
						  CCGlobal::getInstance()->getGamingScene()->pArrEnemyBullet->addObject(bullet);
						  bullet->setPosition(targetPos);
						  auto monster = dynamic_cast<MonsterBase*>(emitterHost);
						  if (monster)
						  {
							
							  monster->addLifeObserver(bullet);
						  }
		}
			break;
		case Type_Enemy:
		{
						   CCGlobal::getInstance()->getGamingScene()->pArrEnemyBullet->addObject(bullet);
						   bullet->setPosition(emitterHost->getPosition());
						   auto monster = dynamic_cast<MonsterBase*>(emitterHost);
						   if (monster)
						   {
						
							   monster->addLifeObserver(bullet);
						   }
		}
			break;
		default:
			break;
		}
		CCGlobal::getInstance()->getGamingScene()->gamelogicLayer->addChild(bullet, emitterHost->getLevel() + emitterHost->getAttNum());

		bullet->start();



	}


}




/********************************激光Action***********************************************************/
WeaponLaserAction::WeaponLaserAction() :WeaponAction()
{
	weaponType = BulletInfo::Bullet_Laser;
	bulletFile = "game/animpacker/laser_light";
}
void WeaponLaserAction::shotAction(Entity* emitterHost, Vec2 targetPos)
{

	
	BulletInfo binfo;
	binfo.bType = weaponType;
	binfo.attNum = emitterHost->getAttNum();
	binfo.parentType = emitterHost->getMyType();
	binfo.bulletRes = bulletFile;
	binfo.mSpeed = Vec2(0, 100);

	BulletBase* bullet = BulletPool::getInstance()->acquireObject(binfo);
	
	switch (binfo.parentType)
	{
	case Type_Player:
		bullet->setName("bulletLaser");
		binfo.weaponID = dynamic_cast<FightRole*>(emitterHost)->getCurrentWeaponSid();
		binfo.mSpeed = dynamic_cast<FightRole*>(emitterHost)->getWeaponDir();
		CCGlobal::getInstance()->getGamingScene()->pArrPlayerBullet->addObject(bullet);
		bullet->setPosition(dynamic_cast<FightRole*>(emitterHost)->getShotPos()-emitterHost->getPosition());
		emitterHost->addChild(bullet, 1);
		bullet->start();
		break;
	default:
		break;
	}

	
}

/********************************炸弹Action***********************************************************/
WeaponBombAction::WeaponBombAction() :WeaponAction()
{
	weaponType = BulletInfo::Bullet_Bomb;
	bulletFile = "game/bullets/bulletBomb.png";
}

void WeaponBombAction::shotAction(Entity* emitterHost,Vec2 targetPos)
{
	BulletInfo binfo;
	binfo.bType = weaponType;
	binfo.attNum = emitterHost->getAttNum();
	binfo.parentType = emitterHost->getMyType();
	binfo.bulletRes = bulletFile;
	binfo.mSpeed = Vec2(0, 100);
	//bulletBomb
	switch (binfo.parentType)
	{
	case Type_Player:
	{
		auto host = dynamic_cast<FightRole*>(emitterHost);
		if (host)
		{
			//主角子弹伤害浮动
			binfo.attNum += cocos2d::random(-10, 10);
			binfo.attNum = std::max(0, binfo.attNum);

			float speedMax = 500;
			binfo.weaponID = host->getCurrentWeaponSid();
			binfo.mSpeed = Vec2(speedMax * host->getWeaponDir().x, speedMax * host->getWeaponDir().y);
					//binfo.mAccelerate = Vec2(0, 1);
		}
	}

		break;
	case Type_Enemy:
	{
					   //随机射击位置
		binfo.mSpeed = Vec2(0, emitterHost->getCurrentSpeed().y * 2);
		binfo.mAccelerate = Vec2(0, emitterHost->getAccelerate().y);
	}
		break;
	case Type_Boss:
	{
		auto host = dynamic_cast<Boss*>(emitterHost);
		if (host)
		{
			float speedMax = 500;
			Vec2 speedDir = (targetPos-host->getPosition());
			speedDir.normalize();

			binfo.mSpeed = Vec2(speedMax *speedDir.x, speedMax * speedDir.y);
			//binfo.mAccelerate = Vec2(0, 1);
		}
	}
		break;
	default:
		break;
	}


	BulletBase* bullet = BulletPool::getInstance()->acquireObject(binfo);

	if (dynamic_cast<BulletBomb*>(bullet))
	{
		dynamic_cast<BulletBomb*>(bullet)->setTargetPos(targetPos); //导弹是有目标位置的
	}
	switch (binfo.parentType)
	{
	case Type_Player:
	{
		CCGlobal::getInstance()->getGamingScene()->pArrPlayerBullet->addObject(bullet);
		bullet->setPosition(dynamic_cast<FightRole*>(emitterHost)->getShotPos());
	}
		break;
	case Type_Boss:
	{
		CCGlobal::getInstance()->getGamingScene()->pArrEnemyBullet->addObject(bullet);
		bullet->setPosition(emitterHost->getPosition());
		auto monster = dynamic_cast<MonsterBase*>(emitterHost);
		if (monster)
		{
			monster->addLifeObserver(bullet);
		}
	}
		break;
	case Type_Enemy:
	{
		CCGlobal::getInstance()->getGamingScene()->pArrEnemyBullet->addObject(bullet);
		bullet->setPosition(emitterHost->getPosition());
		auto monster = dynamic_cast<MonsterBase*>(emitterHost);
		if (monster)
		{
			monster->addLifeObserver(bullet);
		}
	}
		break;
	default:
		break;
	}

	CCGlobal::getInstance()->getGamingScene()->gamelogicLayer->addChild(bullet, emitterHost->getLevel() + emitterHost->getAttNum());
	
	bullet->start();
}



/********************************冰冻子弹Action***********************************************************/
WeaponIceAction::WeaponIceAction() :WeaponAction()
{
	weaponType = BulletInfo::Bullet_Ice;
	icePower = 0.5;
	bulletFile = "game/bullets/bulletIce.png";
}
void WeaponIceAction::shotAction(Entity* emitterHost, Vec2 targetPos/* = Vec2::ZERO*/)
{
	//后期可以从配置文件中读取
	BulletInfo binfo;
	binfo.bType = weaponType;
	binfo.attNum = emitterHost->getAttNum();
	binfo.parentType = emitterHost->getMyType();
	binfo.bulletRes = bulletFile;
	switch (binfo.parentType)
	{
	case Type_Player:
	{
		auto host = dynamic_cast<FightRole*>(emitterHost);
		if (host)
		{
			//主角子弹伤害浮动
			binfo.attNum += cocos2d::random(-10, 10);
			binfo.attNum = std::max(0, binfo.attNum);

			binfo.weaponID = host->getCurrentWeaponSid();
			float speedMax = 2000;
			binfo.mSpeed = Vec2(speedMax * host->getWeaponDir().x, speedMax * host->getWeaponDir().y);
							//binfo.mAccelerate = Vec2(0, 1);
		}
	}

		break;
	case Type_Companion:
	{
		binfo.weaponID = -1;
		float speedMax = 1000;
		binfo.mSpeed = speedMax*targetPos;
		binfo.mAccelerate = targetPos * 2;
	}
		break;
	}


	BulletBase* bullet = BulletPool::getInstance()->acquireObject(binfo);
	switch (binfo.parentType)
	{

	case Type_Player:
	{
		CCGlobal::getInstance()->getGamingScene()->pArrPlayerBullet->addObject(bullet);
		bullet->setPosition(dynamic_cast<FightRole*>(emitterHost)->getShotPos());
		//冰冻枪冰冻时间
		auto iceBullet = dynamic_cast<BulletIce*>(bullet);
		iceBullet->setIcePower(getIcePower());
		iceBullet->setSlownType(SlownDowm_Ice);

	}
		break;
	case Type_Companion:
	{
		CCGlobal::getInstance()->getGamingScene()->pArrPlayerBullet->addObject(bullet);
		bullet->setPosition(dynamic_cast<GameCompanion*>(emitterHost)->getShotPos());
		auto iceBullet = dynamic_cast<BulletIce*>(bullet);
		iceBullet->setIcePower(getIcePower());
		iceBullet->setSlownType(SlownDown_Spit);
	}
		break;
	}
	CCGlobal::getInstance()->getGamingScene()->gamelogicLayer->addChild(bullet, emitterHost->getLevel() + emitterHost->getAttNum());

	bullet->start();

}

/********************************惊天雷Action***********************************************************/


WeaponTorpedoAction::WeaponTorpedoAction() :WeaponAction()
{
	weaponType = BulletInfo::Bullet_Torpedo;
	bulletFile = "zdy_Animation";
}

void WeaponTorpedoAction::shotAction(Entity* emitterHost, Vec2 targetPos)
{
	if (!emitterHost)return;
	BulletInfo binfo;
	binfo.bType = weaponType;
	binfo.attNum = emitterHost->getAttNum();
	binfo.parentType = emitterHost->getMyType();
	binfo.bulletRes = bulletFile;

	switch (binfo.parentType)
	{
	case Type_Player:
	{
		auto host = dynamic_cast<FightRole*>(emitterHost);
		if (host)
		{

		binfo.weaponID = host->getCurrentWeaponSid();
		float speedMax = 2000;
		binfo.mSpeed = Vec2(speedMax * host->getWeaponDir().x, speedMax * host->getWeaponDir().y);
		//binfo.mAccelerate = Vec2(0, 1);
		}
	}

		break;
	}


	BulletBase* bullet = BulletPool::getInstance()->acquireObject(binfo);
	switch (binfo.parentType)
	{

	case Type_Player:
	{
		CCGlobal::getInstance()->getGamingScene()->pArrPlayerBullet->addObject(bullet);
		bullet->setPosition(dynamic_cast<FightRole*>(emitterHost)->getShotPos());

		dynamic_cast<RoleBulletTorpedo*>(bullet)->setTargetPos(targetPos);
	}
		break;

	}
	CCGlobal::getInstance()->getGamingScene()->gamelogicLayer->addChild(bullet, emitterHost->getLevel() + emitterHost->getAttNum());

	bullet->start();
}


/********************************电磁炮Action***********************************************************/


WeaponElectromagneticAction::WeaponElectromagneticAction() :WeaponAction()
{
	weaponType = BulletInfo::Bullet_Electromagnetic;
	bulletFile = "game/animpacker/bullet_electronic"; //默认是帧动画
}

void WeaponElectromagneticAction::shotAction(Entity* emitterHost, Vec2 targetPos)
{
	if (!emitterHost)return;
	BulletInfo binfo;
	binfo.bType = weaponType;
	binfo.attNum = emitterHost->getAttNum();
	binfo.parentType = emitterHost->getMyType();
	binfo.bulletRes = bulletFile;
	switch (binfo.parentType)
	{
	case Type_Player:
	{
		auto host = dynamic_cast<FightRole*>(emitterHost);
		if (host)
		{
							//主角子弹伤害浮动
			binfo.attNum += cocos2d::random(-10, 10);
			binfo.attNum = std::max(0, binfo.attNum);

			binfo.weaponID = host->getCurrentWeaponSid();
			float speedMax = 800;
			binfo.mSpeed = Vec2(speedMax * host->getWeaponDir().x, speedMax * host->getWeaponDir().y);
							//binfo.mAccelerate = Vec2(0, 1);
		}
	}

		break;
	case Type_Companion:
	{
		//binfo.bulletRes = "game/bullets/bullet_electronic.png"; //默认是帧动画
		binfo.weaponID = -1;
		float speedMax = 1000;
		binfo.mSpeed = speedMax*targetPos;
		binfo.mAccelerate = targetPos * 2;
	}
		break;
	}


	BulletBase* bullet = BulletPool::getInstance()->acquireObject(binfo);
	switch (binfo.parentType)
	{

	case Type_Player:
	{
		CCGlobal::getInstance()->getGamingScene()->pArrPlayerBullet->addObject(bullet);
		bullet->setPosition(dynamic_cast<FightRole*>(emitterHost)->getShotPos());
		//电磁炮分裂次数
		auto electromagnetic = dynamic_cast<BulletElectromagnetic*>(bullet);
		if (electromagnetic)
		{
			electromagnetic->setDivideTimes(1);
			electromagnetic->setSlownType(SlownDown_Electornic);
		}

	}
		break;
	case Type_Companion:
	{
		CCGlobal::getInstance()->getGamingScene()->pArrPlayerBullet->addObject(bullet);
		bullet->setPosition(dynamic_cast<GameCompanion*>(emitterHost)->getShotPos());
			//电磁炮分裂次数
		auto electromagnetic = dynamic_cast<BulletElectromagnetic*>(bullet);
		if (electromagnetic)
		{
			electromagnetic->setDivideTimes(1);
			electromagnetic->setSlownType(SlownDown_CW_Electornic);
		}
	}
		break;

	}
	CCGlobal::getInstance()->getGamingScene()->gamelogicLayer->addChild(bullet, emitterHost->getLevel() + emitterHost->getAttNum());

	bullet->start();
}