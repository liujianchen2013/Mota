#ifndef __GAMESAVE_H__
#define __GAMESABE_H__
#include <functional>
#include "cocos2d.h"
#include "Layer/GameLayer.h"
#include "Singleton.h"
#include "Tool/sqlite3.h"

//sqliteC++·â×°
class CppSqlite 
{
public:
	CppSqlite();
	bool openDB(std::string dbFileName);
	bool execSql(std::string sqlStr, int(*callback)(void*, int, char**, char**) = nullptr, void* object = nullptr);
	void closeDB();

private:
	sqlite3 * m_db;
};

//ÓÎÏ·´æµµºÍ¶Áµµ
class GameSave : public Singleton<GameSave>
{
public:
	void init(GameLayer* gameLayer);
	bool readData();
	bool saveData();

private:
	bool readMapData();
	bool saveMapData();
	static int selectMapCallback(void* key, int c, char** value, char** cols);
	static int selectHeroCallback(void* key, int c, char** value, char** cols);
	bool readHeroData();
	bool saveHeroData();
	GameLayer* m_gameLayer;
	CppSqlite m_sqlDB;
};

#define gGameSave GameSave::Instance()


#endif //__GAMESABE_H__