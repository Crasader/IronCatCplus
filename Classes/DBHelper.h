#ifndef _DBUTIL_H_
#define _DBUTIL_H_

#include "cocos2d.h"
#include "GameConstant.h"
#include "Singleton.h"
#include "sqlite3/include/sqlite3.h"
USING_NS_CC;
using namespace std;

class DBHelper:public Singleton<DBHelper>
{
public:
	DBHelper();
	~DBHelper();
private:
	string getSavePath();
	/************************************************************
	��װ sqlite3����
	************************************************************/
	//��������һ��db���ݿ� dbΪ���ݿ������  
	

	//�����жϱ���Ƿ����  
	// name:��ʾ����  
	bool tableIsExist(string name);


	//��������һ������Ϊname�ı�񣬴���ʱ����ƥ��ʱ���иñ�Ĵ�����������򲻴���  
	//������  
	void createTable(string sql, string name);

	//����ɾ��һ�ű���Ϊname�ı��ɾ��ʱ����ƥ���Ƿ��иñ�Ĵ��������������ִ��ɾ������  
	//ɾ������  
	void deleteTable(string sql, string name);

	//��������в���һ������  
	//����һ������  
	void insertData(string sql);

	//���������ɾ��һ������  
	//ɾ��һ������  
	void deleteData(string sql);

	//����������޸�һ������  
	// �޸�һ������  
	void updateData(string sql);

	//��ȡһ����¼������  
	// ��ü�¼������  
	int getDataCount(string sql);

	//��ȡһ����¼����Ϣ  
	/*
	*  �˷����ǲ�ѯ����,�൱֮��Ҫ,pSender����Ǹ�vector
	*/
	void getDataInfo(string sql, void *pSend);


	bool pendingOderExists(const std::string &order);
	///////////////////////////////////////
	//��Ϸ����ֱ�ӷ�װ

	void createTable();

public:
	void init();
	//��  ������
	void insertAwardTask(const TollgatePosition& tp, const std::set<GameTaskInfo>& taskinfoset);
	//�� ������
	void updateAwardTask(const TollgatePosition& tp, const GameTaskInfo& ginfo);
	void updateAwardTask(const TollgatePosition& tp, const std::set<GameTaskInfo>& taskinfoset);
	//ɾ ������
	void deleteAwardTask(const TollgatePosition& tp);
	//�� ������
	std::set<GameTaskInfo> queryAwardTask(const TollgatePosition& tp);
	map<TollgatePosition, std::set<GameTaskInfo>> queryAwardTask();


	void insertHeroPiece(const TollgatePosition& tp,const GameItem& gid);
	void updateHeroPiece(const TollgatePosition& tp, const vector<GameItem>& gids);
	vector<GameItem> queryHeroPiece(const TollgatePosition& tp);
	

	//���δ��ɵĶ���
	void insertPendingOrder(const PendingOrderInfo& orderinfo);
	//ͨ����������ɾ��
	void deletePendingOder(const std::string& order);
	//�����������Բ�ѯ
	void orderTried(const std::string& order);
	//�����ݿ��л�ȡһ��δ��ɵĶ���
	PendingOrderInfo getOnePendingOrder(std::string orderid = "");

	
private:
	void readPDuplicate();
	void writePDuplicates();
	//  �����ݿ�  
	void initDB();

	//�رմ򿪵����ݿ�  
	void closeDB();

	 std::string taskInfoToStr(const std::set<GameTaskInfo>& taskinfoset);
	 std::set<GameTaskInfo> strToTaskInfos(const std::string& taskStr);

	 std::vector<ItemChance> strToGameItems(const std::string& itemstr);
	 std::string gameItemsToStr(const std::vector<ItemChance>& items);
	//��
	std::mutex mutex;

	sqlite3 *pDB = NULL;//���ݿ�ָ��  
	char * errMsg = NULL;//������Ϣ  
	std::string sqlstr;//SQLָ��  
	int result;//sqlite3_exec����ֵ  
	sqlite3_stmt *stmt;

};

#endif // !_DBUTIL_H_


