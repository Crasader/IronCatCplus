#pragma once
#include "cocos2d.h"
USING_NS_CC;
/************************************************************************/
/* ֧��ί��                                                             */
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
	*@param PayTag  ֧����ǩ���ɹ���� 
	*֧���ص�
	**/
	virtual void payCB(PayTag tag) = 0;
};
class LoginDelegate
{
public:
	/**
	*	��¼�ɹ��ص�
	**/
	virtual void loginSuccess(const std::string& userid,const std::string & token) = 0;
};

class EfficiencyDelegate
{
public:
	struct EfficiencyValue
	{
		int rankno; //����
		int effValue; //ս��ֵ
		std::string userName; //�û���
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
	*@param effvalues  ��ѯ�õ���ս��ֵ�������� �����������������
	*֧���ص�
	**/
	virtual void queryBack(const std::vector<EfficiencyValue>& effvalues) = 0;
	virtual void querySelfBack(const EfficiencyValue& evalue) = 0;
};
