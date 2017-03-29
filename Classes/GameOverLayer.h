#ifndef __GameOverLayer_H__
#define __GameOverLayer_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "GameConstant.h"
#include "Singleton.h"
#include "PayCBDelegate.h"
#include "Delegates.h"
USING_NS_CC;
using namespace cocos2d::ui;




class GameOverBaseLayer :public Layer
{
public:
	virtual void uiButtonClickListener(Ref* pSender);
protected:
	virtual bool init(TollgateSettlement tsm);
	void onExit() override;
	void onEnter() override;
	void overlayerUpdate(float deltaTime);
	vector<GameTaskDesc> updateTaskDesc();

protected:
	void releaseGamePool(); //�ͷ���Ϸ�����
	void releaseImageResources(); //�ͷ���Դ
	void restoreGuide();
	void uploadEfficiency(); //����ս��ֵ
protected:
	//CC_SYNTHESIZE(int, _tollgateExp, TollgateExp); //�ؿ�����
	//CC_SYNTHESIZE(int,_tollgateGold,TollgateGold) //�ؿ����
	CC_SYNTHESIZE(int, _tollgateIndex, TollgateIndex); //�ؿ�λ��
	CC_SYNTHESIZE(int, _levelPositionIndex, LevelPositionIndex); //����λ��
	CC_SYNTHESIZE(TollgateSettlement::SettlementType, _overType, OverType); //��������

	int _tempTollgateIndex, _tempLevelPosintIndex;  //�����ؿ�������λ��  �����汾��ʹ��
};


class GameOver_Win :public GameOverBaseLayer
{
public:
	CREATE_ENTITY_FUNC(GameOver_Win, TollgateSettlement); 
	virtual void uiButtonClickListener(Ref* pSender) override;
	void onEnter() override;
	void onExit() override;
	void gameoverUpdate(float deltaTime);
	void otherClosed();
	void tabChanged(Ref *pSender, Widget::TouchEventType eventType);
	void checkChanged(Ref*pSender, CheckBox::EventType eventType);
private:
	bool init(TollgateSettlement tsm) override;
	void initAwd(Node* parent,TollgateSettlement tsm); //��ʼ������
	void initTollgateAwdView(TollgateSettlement tsm); //��ʼ���ؿ��̶�����
	void initExpAwdData(const vector<ItemChance>& awardings); //��ʼ�����⻰��������
	void initExpAwdView(Node* parent); //��ʼ�����⽱������
	void initNormalView(TollgateSettlement tsm); //��ʼ����ͨ����
	void initExtraView(TollgateSettlement tsm); //��ʼ�������ý���

	void showGuide();
	float displayTime;
	float currentDisTime;//��ǰ��ʱ��
	int everySec; //ÿ�����ǵĵ���
	//TextBMFont* exLabel; //�������ֵ
	//TextBMFont* goldLabel; //�����ֵ
	

	float currentPercent; //��ǰ�ٷֱ�
	float targetPercent; //Ŀ��ٷֱ�
	int currentLevel;//��ǰ�ȼ�
	int targetLevel;//�������ĵȼ�
	map<GameItem, int> awdmaps; //�����б�
	map<GameItem, int> exawddatas; //���⽱��
	std::map<GameItem, TextBMFont*> tollgateAwdmap; //�ؿ��̶������Ŀؼ�����
	//std::unordered_map<GameItem, TextBMFont*> exAwdmap; //���⽱���ؼ�����
	

	Layout* panel_normal, *panel_extra;
	ListView* normalList; //�ؿ��̶�����
	ListView* normalWinMaybeList; // �ؿ���ֵ���Ķ���
	ListView*  listAwarding;//�ؿ��л�ý���������
};

class GameOver_WinGfit :public GameOverBaseLayer
{

public:
	GameOver_WinGfit();
	~GameOver_WinGfit();
	CREATE_ENTITY_FUNC(GameOver_WinGfit, TollgateSettlement);
	virtual void uiButtonClickListener(Ref* pSender) override;
	void onEnter() override;
	void onExit() override;
	
private:
	bool init(TollgateSettlement tsm) override;
	void initListView(TollgateSettlement tsm);
	
	ListView*  listAwarding;//�ؿ��л�ý���������
};

class GameOver_Lose :public GameOverBaseLayer
{
public:
	CREATE_ENTITY_FUNC(GameOver_Lose, TollgateSettlement);
	virtual void uiButtonClickListener(Ref* pSender) override;
	void onEnter() override;
	void onExit() override;
private:
	bool init(TollgateSettlement tsm) override;
	void initLayer(Node* parent);
private:
	GameTaskID punishid;
	
};
class GameOverPauseContentView;
class GameOver_Pause :public GameOverBaseLayer
{
friend class GameOver_QuitAlert;
public:
	CREATE_ENTITY_FUNC(GameOver_Pause, TollgateSettlement);
	virtual void uiButtonClickListener(Ref* pSender) override;
private:
	bool init(TollgateSettlement tsm) override;
	void runCloseAmin(Node* rootNode,const std::function<void()> & closeFun);
	//void initListView(ListView* lv, const vector<GameTaskDesc>& taskdescs);
	
protected:
	GameOverPauseContentView* contentview;
};

#define CREATE_CONTENT_FUNC(__TYPE__,__ARG1__,__ARG2__) \
	static __TYPE__* create(__ARG1__ argu1, __ARG2__ argu2) \
{ \
	__TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
if (pRet && pRet->init(argu1, argu2)) \
{ \
	pRet->autorelease(); \
	return pRet; \
} \
	else \
{ \
	delete pRet; \
	pRet = NULL; \
	return NULL; \
} \
}

class GameOverPauseContentView :public Layout
{
protected:
	virtual bool init(const TollgatePosition& tp, Node* parentNode);
	vector<GameTaskDesc> updateTaskDesc();
};

class GameOverPauseContentView_Gift :public GameOverPauseContentView
{
public:
	CREATE_CONTENT_FUNC(GameOverPauseContentView_Gift,TollgatePosition,Node*);
protected:
	bool init(const TollgatePosition& tp, Node* parentNode) override;
	
};

class GameOverPauseContentView_Normal :public GameOverPauseContentView
{
public:
	CREATE_CONTENT_FUNC(GameOverPauseContentView_Normal, TollgatePosition, Node*);
protected:
	bool init(const TollgatePosition& tp, Node* parentNode) override;
	void initListView(ListView* lv, const TollgatePosition& tp,const vector<GameTaskDesc>& taskdescs);
};

class GameOver_QuitAlert :public GameOverBaseLayer
{
public:
	CREATE_ENTITY_FUNC(GameOver_QuitAlert, TollgateSettlement);
	virtual void uiButtonClickListener(Ref* pSender) override;
private:
	bool init(TollgateSettlement tsm) override;
	void runCloseAmin(Node* rootNode, const std::function<void()> & closeFun);

};



//���ܶһ�����
class GameSkillExchangeLayer :public Layer,public PayCBDelegate,public GameItemDelegate
{
public:
	GameSkillExchangeLayer();
	~GameSkillExchangeLayer();
	CREATE_ENTITY_FUNC(GameSkillExchangeLayer,SkillInfo::SkillType);
	void payCB(PayTag tag) override;
	void skillButtonClickListener(Ref* pSender);
	void runCloseAmin(Node* rootNode, const std::function<void()> & closeFun);
	void onEnter() override;
	void onExit() override;
	void itemChanged(const GameItem& gid) override;
protected:
	bool init(SkillInfo::SkillType skType);
	void freshUI(); //ˢ������
	CC_SYNTHESIZE(SkillInfo::SkillType, _skType, SkillType);
	TextBMFont *fishAlllabel, *skilllAllabel; //��Һͼ��ܵ�����
};

class GameOverFactory :public Singleton<GameOverFactory>
{
public:
	GameOverFactory();
	~GameOverFactory();
	GameOverBaseLayer* createOverLayer(const TollgateSettlement& tsm);
};


#endif
