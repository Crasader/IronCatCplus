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
	封装 sqlite3操作
	************************************************************/
	//用来创建一个db数据库 db为数据库的名字  
	

	//用来判断表格是否存在  
	// name:表示表名  
	bool tableIsExist(string name);


	//用来创建一个表名为name的表格，创建时会先匹配时否有该表的存在如果存在则不创建  
	//创建表  
	void createTable(string sql, string name);

	//用来删除一张表名为name的表格，删除时会先匹配是否有该表的存在如果不存在则不执行删除操作  
	//删除表名  
	void deleteTable(string sql, string name);

	//用来向表中插入一条数据  
	//插入一条数据  
	void insertData(string sql);

	//用来向表中删除一条数据  
	//删除一条数据  
	void deleteData(string sql);

	//用来向表中修改一条数据  
	// 修改一条数据  
	void updateData(string sql);

	//获取一个记录的条数  
	// 获得记录的条数  
	int getDataCount(string sql);

	//读取一条记录的信息  
	/*
	*  此方法是查询方法,相当之重要,pSender最好是个vector
	*/
	void getDataInfo(string sql, void *pSend);


	bool pendingOderExists(const std::string &order);
	///////////////////////////////////////
	//游戏里面直接封装

	void createTable();

public:
	void init();
	//增  任务奖励
	void insertAwardTask(const TollgatePosition& tp, const std::set<GameTaskInfo>& taskinfoset);
	//改 任务奖励
	void updateAwardTask(const TollgatePosition& tp, const GameTaskInfo& ginfo);
	void updateAwardTask(const TollgatePosition& tp, const std::set<GameTaskInfo>& taskinfoset);
	//删 任务奖励
	void deleteAwardTask(const TollgatePosition& tp);
	//查 任务奖励
	std::set<GameTaskInfo> queryAwardTask(const TollgatePosition& tp);
	map<TollgatePosition, std::set<GameTaskInfo>> queryAwardTask();


	void insertHeroPiece(const TollgatePosition& tp,const GameItem& gid);
	void updateHeroPiece(const TollgatePosition& tp, const vector<GameItem>& gids);
	vector<GameItem> queryHeroPiece(const TollgatePosition& tp);
	

	//添加未完成的订单
	void insertPendingOrder(const PendingOrderInfo& orderinfo);
	//通过订单号来删除
	void deletePendingOder(const std::string& order);
	//该条订单尝试查询
	void orderTried(const std::string& order);
	//从数据库中获取一条未完成的订单
	PendingOrderInfo getOnePendingOrder(std::string orderid = "");

	
private:
	void readPDuplicate();
	void writePDuplicates();
	//  打开数据库  
	void initDB();

	//关闭打开的数据库  
	void closeDB();

	 std::string taskInfoToStr(const std::set<GameTaskInfo>& taskinfoset);
	 std::set<GameTaskInfo> strToTaskInfos(const std::string& taskStr);

	 std::vector<ItemChance> strToGameItems(const std::string& itemstr);
	 std::string gameItemsToStr(const std::vector<ItemChance>& items);
	//锁
	std::mutex mutex;

	sqlite3 *pDB = NULL;//数据库指针  
	char * errMsg = NULL;//错误信息  
	std::string sqlstr;//SQL指令  
	int result;//sqlite3_exec返回值  
	sqlite3_stmt *stmt;

};

#endif // !_DBUTIL_H_


