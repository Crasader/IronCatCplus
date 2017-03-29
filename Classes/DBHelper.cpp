#include "DBHelper.h"
#include "Util.h"


DBHelper::DBHelper() :pDB(nullptr), errMsg(nullptr), stmt(nullptr), result(-1)
{


}
DBHelper::~DBHelper()
{

}

string DBHelper::getSavePath()
{
	string fullDBPath = FileUtils::sharedFileUtils()->getWritablePath() + "GameData.db";
	CCLOG("fullPath : %s", fullDBPath.c_str());
	return fullDBPath;
}

//创建数据库  
void DBHelper::initDB()
{
	if (pDB)return; //已经打开了
	//打开一个数据库，如果该数据库不存在，则创建一个数据库文件  
	
	result = sqlite3_open_v2(this->getSavePath().c_str(), &pDB,SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX, NULL);

	if (result != SQLITE_OK)
		CCLOG("open database failed，errcode:%d ，reason:%s\n", result, sqlite3_errmsg(pDB));
}


//tableIsExist的回调函数  
int isExisted(void * para, int n_column, char ** column_value, char ** column_name)
{
	bool *isExisted_ = (bool*)para;
	*isExisted_ = (**column_value) != '0';
	return 0;
}



//判断表格是否存在  
bool DBHelper::tableIsExist(string name)
{
	if (pDB != NULL)
	{
		//判断表是否存在  
		bool tableIsExisted;
		sqlstr = "select count(type) from sqlite_master where type='table' and name ='" + name + "'";
		result = sqlite3_exec(pDB, sqlstr.c_str(), isExisted, &tableIsExisted, &errMsg);
		return tableIsExisted;
	}
	return false;
}



//在数据库中判断名为name的表示否存在，如果不存在则创建这张表  
//@示例语句string sqls = "create table user(id integer,username text,password text)";  
void DBHelper::createTable(string sql, string name)
{
	if (!tableIsExist(name))
	{
		//创建表，设置ID为主键，且自动增加  
		result = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errMsg);
		if (result != SQLITE_OK)
			CCLOG("create table failed,errcode:%d ,reason:%s\n", result, errMsg);
	}
}


//删除表格  
//@示例语句sqlstr="drop table name";  
void DBHelper::deleteTable(string sql, string name)
{
	if (tableIsExist(name))
	{
		result = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errMsg);
		if (result != SQLITE_OK)
			CCLOG("delete table failed，errcode:%d ，reason:%s\n", result, errMsg);
	}
}


//插入数据  
//@示例语句sqlstr=" insert into MyTable_1( name ) values ( '擎天柱' ) ";  
void DBHelper::insertData(string sql){
	result = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errMsg);
	if (result != SQLITE_OK)
		CCLOG("insert data failed，errcode:%d ，reason:%s\n", result, errMsg);
}


//删除数据  
//@示例语句sqlstr="delete from MyTable_1 where ID = 2";  
void DBHelper::deleteData(string sql)
{
	result = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errMsg);
	if (result != SQLITE_OK)
		CCLOG("delete datebase failed，errcode:%d ，reason:%s\n", result, errMsg);
}


//修改数据  
//@示例语句        sqlstr="update MyTable_1 set name='威震天' where ID = 3";  
void DBHelper::updateData(string sql)
{
	result = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errMsg);
	if (result != SQLITE_OK)
		CCLOG("update data failed，errcode:%d ，reason:%s\n", result, errMsg);
}


//getDataCount的回调函数  
int loadRecordCount(void * para, int n_column, char ** column_value, char ** column_name)
{
	int *count = (int*)para;
	*count = n_column;
	return 0;
}
//获取记录的条数  
//@示例语句string sqlsssss = "select count(*) from user";  
//@示例语句  取得表格字段的语句string sqlsssss = "select * from user";  
int DBHelper::getDataCount(string sql)
{
	int count = 0;
	sqlite3_exec(pDB, sql.c_str(), loadRecordCount, &count, &errMsg);
	return count;
}


//getDataInfo的回调函数  
int loadRecord(void * para, int n_column, char ** column_value, char ** column_name)
{
	CCLOG("n_column:%d", n_column);

	/*MyTest* testVO = (MyTest*)para;
		
	testVO->setId(Value(column_value[0]).asInt());
	testVO->setLevel(Value(column_value[1]).asInt());
	testVO->setLastscore(Value(column_value[2]).asInt());
	testVO->setBestscore(Value(column_value[3]).asInt());
	testVO->setStar(Value(column_value[4]).asInt());*/
	/* 可能有5个字段 */
	// id level lastscore bestscore star  
	//    CCLOG("c[0]:%s,c[1]:%s,c[2]:%s,c[3]:%s,c[4]:%s",column_name[0],column_name[1],column_name[2],column_name[3],column_name[4]);  
	//      
	CCLOG("id=%s,level=%s,lastscore=%s,bestscore=%s,star=%s", column_value[0], column_value[1], column_value[2], column_value[3], column_value[4]);
	return 0;
}
//获取一条记录的信息 其中的pSend是一个实体类我们以后可以自定义一个继承了CCObject的类来代替他保存数据库中取出来的数据  
/*
*  这里最好扩展下，让  pSend  是一个vector
*/
void DBHelper::getDataInfo(string sql, void *pSend)
{
	sqlite3_exec(pDB, sql.c_str(), loadRecord, pSend, &errMsg);
}

void DBHelper::updateAwardTask(const TollgatePosition& tp, const GameTaskInfo& ginfo)
{
	this->initDB();
	sqlstr = "update awardTask set taskinfos=? where starID=? and tollgateIndex=?";
	result = sqlite3_prepare_v2(pDB, sqlstr.c_str(), -1, &stmt, NULL);
	if (result != SQLITE_OK)
		CCLOG("savePartnerData,errcode:%d ，reason:%s\n", result, sqlite3_errmsg(pDB));

	int count = getDataCount(StringUtils::format("select count(*) from awardTask where starID=%d and tollgateIndex=%d", tp.star, tp.tollgate));
	if (count == 0) //没有数据
	{
		std::set<GameTaskInfo> sets;
		sets.insert(ginfo);
		insertAwardTask(tp, sets);

	}
	else //有数据
	{
		std::set<GameTaskInfo>  sets = queryAwardTask(tp);
		if (sets.find(ginfo) != sets.end())
		{
			//已经存在了，不重复添加
		}
		else
		{
			sets.insert(ginfo);
			std::string taskinfoStr = taskInfoToStr(sets);
			sqlite3_bind_text(stmt, 1, taskinfoStr.c_str(),-1,NULL);
			sqlite3_bind_int(stmt, 2, tp.star);
			sqlite3_bind_int(stmt, 3, tp.tollgate);
			result = sqlite3_step(stmt);
			if (result != SQLITE_DONE)
				log("insertPlayerData，errcode:%d ，reason:%s\n", result, errMsg);
			if (stmt) sqlite3_reset(stmt);
		}
	}
	closeDB();



}
std::set<GameTaskInfo> DBHelper::queryAwardTask(const TollgatePosition& tp)
{
	mutex.lock();
	this->initDB();
	sqlstr = "select taskinfos from awardTask where  starID=? and tollgateIndex=?";
	result = sqlite3_prepare_v2(pDB, sqlstr.c_str(), -1, &stmt, NULL);

	if (result != SQLITE_OK)
		log("query data,errcode:%d ,reason:%s\n", result, sqlite3_errmsg(pDB));
	sqlite3_bind_int(stmt, 1, tp.star);
	sqlite3_bind_int(stmt, 2, tp.tollgate);
	std::set<GameTaskInfo> tasksets;
	int result = sqlite3_step(stmt);
	if (result == SQLITE_ROW)
	{
		//获取玩家基本数据
		const unsigned char* cc =sqlite3_column_text(stmt, 0);
		unsigned char* c= const_cast<unsigned char*>(cc);
		
		std::string taskinfosStr =c?(char*)c:"";
		tasksets = strToTaskInfos(taskinfosStr);
		
	}
	if (stmt) sqlite3_reset(stmt);
	closeDB();
	mutex.unlock();
	return tasksets;

}

map<TollgatePosition, std::set<GameTaskInfo>> DBHelper::queryAwardTask()
{
	mutex.lock();
	map<TollgatePosition, std::set<GameTaskInfo>> maps;

	this->initDB();
	sqlstr = "select starID,tollgateIndex,taskinfos from awardTask";
	result = sqlite3_prepare_v2(pDB, sqlstr.c_str(), -1, &stmt, NULL);

	if (result != SQLITE_OK)
		log("query all data,errcode:%d,reason:%s\n", result, sqlite3_errmsg(pDB));

	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		TollgatePosition tp;
		
		tp.star = sqlite3_column_int(stmt, 0);
		tp.tollgate = sqlite3_column_int(stmt, 1);
		const unsigned char* cc = sqlite3_column_text(stmt, 2);
		unsigned char* c = const_cast<unsigned char*>(cc);
		std::string s = c ? (char*)c : "";
		maps.insert(make_pair(tp, strToTaskInfos(s)));
	}
	if (stmt) sqlite3_reset(stmt);
	this->closeDB();
	mutex.unlock();
	return maps;

}
void DBHelper::deleteAwardTask(const TollgatePosition& tp)
{
	mutex.lock();
	this->initDB();
	sqlstr = "delete from awardTask where starID=? and tollgateIndex=?";

	result = sqlite3_prepare_v2(pDB, sqlstr.c_str(), -1, &stmt, NULL);
	if (result != SQLITE_OK)
		log("delete,errcode:%d ,reason:%s\n", result, sqlite3_errmsg(pDB));
	sqlite3_bind_int(stmt, 1, tp.star);
	sqlite3_bind_int(stmt, 2, tp.tollgate);
	result = sqlite3_step(stmt);
	
	if (result != SQLITE_DONE)
		log("insertPlayerData，errcode:%d ,reason:%s\n", result, errMsg);
	if (stmt) sqlite3_reset(stmt);

	this->closeDB();
	mutex.unlock();
}


void DBHelper::insertAwardTask(const TollgatePosition& tp, const std::set<GameTaskInfo>& taskinfoset)
{
	mutex.lock();
	this->initDB();

	sqlstr = "insert into awardTask(starID,tollgateIndex,taskinfos) values (?,?,?)";
	//预编译
	result = sqlite3_prepare_v2(pDB, sqlstr.c_str(), -1, &stmt, NULL);
	if (result != SQLITE_OK)
		log("insertPlayerData，errcode:%d ,reason:%s\n", result, sqlite3_errmsg(pDB));
	sqlite3_bind_int(stmt, 1, tp.star);
	sqlite3_bind_int(stmt, 2, tp.tollgate);
	std::string taskinfoStr = taskInfoToStr(taskinfoset);

	
	sqlite3_bind_text(stmt, 3, taskinfoStr.c_str(), -1, NULL);
	result = sqlite3_step(stmt);
	if (result != SQLITE_DONE)
		log("insertPlayerData，errcode:%d ,reason:%s\n", result, errMsg);
	if (stmt) sqlite3_reset(stmt);

	this->closeDB();
	mutex.unlock();
}
void DBHelper::updateAwardTask(const TollgatePosition& tp, const std::set<GameTaskInfo>& taskinfoset)
{
	mutex.lock();
	this->initDB();
	
	sqlstr = "update awardTask set taskinfos=? where starID=? and tollgateIndex=?";
	//预编译
	result = sqlite3_prepare_v2(pDB, sqlstr.c_str(), -1, &stmt, NULL);
	if (result != SQLITE_OK)
		log("update,errcode:%d ,reason:%s\n", result, sqlite3_errmsg(pDB));
	sqlite3_bind_int(stmt, 2, tp.star);
	sqlite3_bind_int(stmt, 3, tp.tollgate);
	std::string taskinfoStr = taskInfoToStr(taskinfoset);


	sqlite3_bind_text(stmt, 1, taskinfoStr.c_str(), -1, NULL);
	
	result = sqlite3_step(stmt);
	if (result != SQLITE_DONE)
		log("insertPlayerData,errcode:%d,reason:%s\n", result, sqlite3_errmsg(pDB));
	if (stmt) sqlite3_reset(stmt);

	this->closeDB();
	mutex.unlock();
}

std::string DBHelper::taskInfoToStr(const std::set<GameTaskInfo>& taskinfoset)
{
	std::string taskinfoStr;
	int olen = 0;
	for (auto &taskinfo : taskinfoset)
	{
		taskinfo.taskid;
		taskinfoStr.append(StringUtils::format("%d", taskinfo.taskid) + ":");
		taskinfo.taskargs;
		int len = 0;
		for (auto& taks : taskinfo.taskargs)
		{
			taskinfoStr.append(StringUtils::format("%d", taks));
			if (len != taskinfo.taskargs.size() - 1)
			{
				taskinfoStr.append(",");
			}

			len++;
		}
		olen++;
		if (olen!=taskinfoset.size())
		taskinfoStr.append("|");
		
	}
	return taskinfoStr;

}

 std::set<GameTaskInfo> DBHelper::strToTaskInfos(const std::string& taskStr)
{
	std::set<GameTaskInfo> tasksets;
	std::vector<std::string> taskinfoArr = splits(taskStr, "|");

	for (auto &taskinfos : taskinfoArr)
	{

		vector<std::string>ta = splits(taskinfos, ":");
		if (ta.size() == 2)
		{
			GameTaskInfo ginfo;
			ginfo.taskid = GameTaskID(atoi(ta.at(0).c_str()));
			//std::vector<int> taskarg;
			for (auto taskArgs : splits(ta.at(1), ","))
			{
				ginfo.taskargs.push_back(atoi(taskArgs.c_str()));
			}

			tasksets.insert(ginfo);
		}

	}
	return tasksets;

}


 void DBHelper::insertHeroPiece(const TollgatePosition& tp, const GameItem& gid)
 {
	 mutex.lock();
	 this->initDB();

	 sqlstr = "insert into PieceConfig(starID,tollgateIndex,piece) values (?,?,?)";
	 //预编译
	 result = sqlite3_prepare_v2(pDB, sqlstr.c_str(), -1, &stmt, NULL);
	 if (result != SQLITE_OK)
		 log("PieceConfig，errcode:%d ,reason:%s\n", result, sqlite3_errmsg(pDB));
	 sqlite3_bind_int(stmt, 1, tp.star);
	 sqlite3_bind_int(stmt, 2, tp.tollgate);
	 std::string taskinfoStr = StringUtils::format("%d",gid);


	 sqlite3_bind_text(stmt, 3, taskinfoStr.c_str(), -1, NULL);
	 result = sqlite3_step(stmt);
	 if (result != SQLITE_DONE)
		 log("PieceConfig，errcode:%d ,reason:%s\n", result, sqlite3_errmsg(pDB));
	 if (stmt) sqlite3_reset(stmt);

	 this->closeDB();
	 mutex.unlock();
 }
 void DBHelper::updateHeroPiece(const TollgatePosition& tp, const vector<GameItem>& gids)
 {
	 bool retFlag = false;
	 mutex.lock();
	 this->initDB();
	 sqlstr = "update PieceConfig set piece=? where starID=? and tollgateIndex=?";
	 //预编译
	 result = sqlite3_prepare_v2(pDB, sqlstr.c_str(), -1, &stmt, NULL);
	 if (result != SQLITE_OK)
		 log("update,errcode:%d ,reason:%s\n", result, sqlite3_errmsg(pDB));
	 sqlite3_bind_int(stmt, 2, tp.star);
	 sqlite3_bind_int(stmt, 3, tp.tollgate);

	 std::string taskinfoStr;
	 int index = 0;
	 for (auto& gid : gids)
	 {
		 taskinfoStr.append(StringUtils::format("%d",gid));
		 index++;
		 if (index != gids.size())
		 {
			 taskinfoStr.append(",");
		 }
	 }


	 sqlite3_bind_text(stmt, 1, taskinfoStr.c_str(), -1, NULL);

	 result = sqlite3_step(stmt);
	 if (result != SQLITE_DONE)
		 log("PieceConfig,errcode:%d,reason:%s\n", result, sqlite3_errmsg(pDB));
	 if (stmt) sqlite3_reset(stmt);

	 this->closeDB();
	 mutex.unlock();
 }


 vector<GameItem> DBHelper::queryHeroPiece(const TollgatePosition& tp)
 {
	 vector<GameItem> items;

	 mutex.lock();
	 this->initDB();
	 sqlstr = "select piece from PieceConfig where  starID=? and tollgateIndex=?";
	 result = sqlite3_prepare_v2(pDB, sqlstr.c_str(), -1, &stmt, NULL);

	 if (result != SQLITE_OK)
		 log("query data,errcode:%d ,reason:%s\n", result, sqlite3_errmsg(pDB));
	 sqlite3_bind_int(stmt, 1, tp.star);
	 sqlite3_bind_int(stmt, 2, tp.tollgate);

	 int result = sqlite3_step(stmt);
	 if (result == SQLITE_ROW)
	 {
		 //获取玩家基本数据
		 const unsigned char* cc = sqlite3_column_text(stmt, 0);
		 unsigned char* c = const_cast<unsigned char*>(cc);

		 std::string taskinfosStr = c ? (char*)c : "";

		 vector<std::string> piecesV = splits(taskinfosStr, ",");
		 for (auto& pi : piecesV)
		 {
			 items.push_back(GameItem(atoi(pi.c_str())));
			
		 }
	 }

	 if (stmt) sqlite3_reset(stmt);
	 closeDB();
	 mutex.unlock();

	 return items;
 }



 std::vector<ItemChance> DBHelper::strToGameItems(const std::string& itemstr)
 {
	 std::vector<ItemChance> items;
	 std::vector<std::string> itemStr1s = splits(itemstr, "|");

	 for (auto& itemStr1 : itemStr1s)
	 {
		 std::vector<std::string> itemStr2s = splits(itemStr1, ".");
		 if (itemStr2s.size() == 2)
		 {
			 ItemChance it;
			 it.item = GameItem(atoi(itemStr2s.at(0).c_str()));
			 it.num = atoi(itemStr2s.at(1).c_str());
			
			 items.push_back(it);
		 }
	 }
	 return items;
 }
 std::string DBHelper::gameItemsToStr(const std::vector<ItemChance>& items)
 {
	 std::string itemStr;
	 int len = items.size();
	 int index = 0;
	 for (auto& it : items)
	 {
		 index++;
		 itemStr.append(StringUtils::format("%d", it.item)).append(".").append(StringUtils::format("%d",it.num));
		 if (index < len)
		 {
			 itemStr.append("|");
		 }
	 }

	 return itemStr;
 }

 bool DBHelper::pendingOderExists(const std::string &order)
 {
	 bool existsOrder = false;
	 mutex.lock();
	 this->initDB();
	 sqlstr = "select * from pendingOrder where  orderId=?";
	 result = sqlite3_prepare_v2(pDB, sqlstr.c_str(), -1, &stmt, NULL);

	 if (result != SQLITE_OK)
		 log("query data,errcode:%d ,reason:%s\n", result, sqlite3_errmsg(pDB));
	
	 sqlite3_bind_text(stmt, 1, order.c_str(), -1, NULL);
	 int result = sqlite3_step(stmt);
	 if (result == SQLITE_ROW)
	 {
		 existsOrder = true;
	 }

	 if (stmt) sqlite3_reset(stmt);
	 closeDB();
	 mutex.unlock();
	 return existsOrder;
 }

 void DBHelper::insertPendingOrder(const PendingOrderInfo& orderinfo)
 {
	 if (pendingOderExists(orderinfo.order)) return;
		
	 mutex.lock();
	 this->initDB();

	 sqlstr = "insert into pendingOrder(orderId,awdSid) values (?,?)";
	 //预编译
	 result = sqlite3_prepare_v2(pDB, sqlstr.c_str(), -1, &stmt, NULL);
	 if (result != SQLITE_OK)
		 log("pendingOrder,errcode:%d ,reason:%s\n", result, sqlite3_errmsg(pDB));

	 sqlite3_bind_text(stmt, 1, orderinfo.order.c_str(),-1,NULL);
	
	 
	 std::vector<ItemChance>items;
	 if (orderinfo.shopItem)items = orderinfo.shopItem->paylist;
	std::string ccc = gameItemsToStr(items);
	const char* cccccc = ccc.c_str();
	sqlite3_bind_text(stmt, 2, cccccc, -1, NULL);
	 result = sqlite3_step(stmt);
	 if (result != SQLITE_DONE)
		 log("pendingOrder，errcode:%d ,reason:%s\n", result, sqlite3_errmsg(pDB));
	 if (stmt) sqlite3_reset(stmt);

	 this->closeDB();
	 mutex.unlock();
 }
 void DBHelper::orderTried(const std::string& order)
 {
	 bool retFlag = false;
	 mutex.lock();
	 this->initDB();
	 sqlstr = "update pendingOrder set tryTimes=tryTimes+1 where orderId=?";
	 //预编译
	 result = sqlite3_prepare_v2(pDB, sqlstr.c_str(), -1, &stmt, NULL);
	 if (result != SQLITE_OK)
		 log("update,errcode:%d ,reason:%s\n", result, sqlite3_errmsg(pDB));
	 sqlite3_bind_text(stmt, 1, order.c_str(), -1, NULL);
	 result = sqlite3_step(stmt);
	 if (result != SQLITE_DONE)
		 log("pendingOrder,errcode:%d,reason:%s\n", result, sqlite3_errmsg(pDB));
	 if (stmt) sqlite3_reset(stmt);

	 this->closeDB();
	 mutex.unlock();
 }

 void DBHelper::deletePendingOder(const std::string& order)
 {
	 mutex.lock();
	 this->initDB();
	 sqlstr = "delete from pendingOrder where orderId=?";

	 result = sqlite3_prepare_v2(pDB, sqlstr.c_str(), -1, &stmt, NULL);
	 if (result != SQLITE_OK)
		 log("delete pendingOrder,errcode:%d ,reason:%s\n", result, sqlite3_errmsg(pDB));
	 
	 sqlite3_bind_text(stmt, 1, order.c_str(), -1, NULL);
	 result = sqlite3_step(stmt);

	 if (result != SQLITE_DONE)
		 log("pendingOrder,errcode:%d ,reason:%s\n", result, errMsg);
	 if (stmt) sqlite3_reset(stmt);

	 this->closeDB();
	 mutex.unlock();
 }

 PendingOrderInfo DBHelper::getOnePendingOrder(std::string orderid /*= ""*/)
 {
	 PendingOrderInfo orderinfo;

	 mutex.lock();
	 this->initDB();
	 if (orderid.empty())
	 {
		 sqlstr = "select orderId,awdSid,tryTimes from pendingOrder order by tryTimes desc limit 1";
	 }
	 else
	 {
		 sqlstr = "select orderId,awdSid,tryTimes from pendingOrder where orderId=?";
	 }
	 
	 result = sqlite3_prepare_v2(pDB, sqlstr.c_str(), -1, &stmt, NULL);

	 if (result != SQLITE_OK)
		 log("query pendingOrder data,errcode:%d,reason:%s\n", result, sqlite3_errmsg(pDB));
	 if (!orderid.empty())
	 {
		 sqlite3_bind_text(stmt, 1, orderid.c_str(), -1, NULL);
	 }
	

	 while (sqlite3_step(stmt) == SQLITE_ROW)
	 {
		 
		 const unsigned char* cunorder = sqlite3_column_text(stmt, 0);
		 unsigned char* unorder = const_cast<unsigned char*>(cunorder);
		 orderinfo.order = unorder ? (char*)unorder : "";
		
		 const unsigned char* cunsid = sqlite3_column_text(stmt, 1);
		 unsigned char* unsid = const_cast<unsigned char*>(cunsid);
		 std::string sid = unsid ? (char*)unsid : "";
		 ShopItem* item = new ShopItem;
		 item->paylist = strToGameItems(sid);
		 orderinfo.shopItem = item;
		 break;
	 }
	 if (stmt) sqlite3_reset(stmt);
	 this->closeDB();
	 mutex.unlock();

	 return orderinfo;
 }
void DBHelper::createTable()
{
	//创建玩家数据库
	if (!this->tableIsExist("awardTask"))
	{
		sqlstr = "create table awardTask(_id INTEGER PRIMARY KEY AUTOINCREMENT, starID INTEGER, tollgateIndex INTEGER, taskinfos TEXT)";
		this->createTable(sqlstr.c_str(), "awardTask");
	}
	if (!this->tableIsExist("PieceConfig")) //碎片配置，保存那些关卡获得了那些碎片
	{
		sqlstr = "create table PieceConfig(_id INTEGER PRIMARY KEY AUTOINCREMENT, starID INTEGER, tollgateIndex INTEGER, piece TEXT)";
		this->createTable(sqlstr.c_str(), "PieceConfig");
	}

	if (!this->tableIsExist("pendingOrder"))
	{
		sqlstr = "create table pendingOrder(_id INTEGER PRIMARY KEY AUTOINCREMENT, orderId TEXT, awdSid TEXT, tryTimes INTEGER default 0)";
		this->createTable(sqlstr.c_str(), "pendingOrder");
	}
	
}
void DBHelper::init()
{
	this->initDB();
	this->createTable();
	this->closeDB();
}

//关闭数据库  
void DBHelper::closeDB()
{
	if (pDB)sqlite3_close(pDB);
	pDB = NULL;
}