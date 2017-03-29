#pragma once
#include "cocos2d.h"
USING_NS_CC;
/************************************************************************/
/* 支付委托                                                             */
/************************************************************************/
class PayCBDelegate
{
public:
	enum PayTag
	{
		Pay_Fail,
		Pay_OK,

	};
public:
	/**
	*@param PayTag  支付标签，成功与否 
	*支付回调
	**/
	virtual void payCB(PayTag tag) = 0;
};
class LoginDelegate
{
public:
	/**
	*	登录成功回调
	**/
	virtual void loginSuccess(const std::string& userid,const std::string & token) = 0;
};

class EfficiencyDelegate
{
public:
	struct EfficiencyValue
	{
		int rankno; //排名
		int effValue; //战斗值
		std::string userName; //用户名
		std::string userid;
		EfficiencyValue()
		{
			rankno = -1;
			effValue = 0;
			userName = "";
			userid = "";
		}
	};
public:
	/**
	*@param effvalues  查询得到的战斗值排名数据 排名最多数量有限制
	*支付回调
	**/
	virtual void queryBack(const std::vector<EfficiencyValue>& effvalues) = 0;
	virtual void querySelfBack(const EfficiencyValue& evalue) = 0;
};
