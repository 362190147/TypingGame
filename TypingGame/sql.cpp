#include <tchar.h>
#include "stdio.h"
#include "sql.h"
#ifdef X64
#include <mysql.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Secur32.lib")
//#pragma comment(lib, "vs14/mysqlclient.lib")
#pragma comment(lib, "libmysql.lib")

#ifdef _DEBUG
#define LOG(...)  fprintf(fp,__VA_ARGS__)
#else
#define LOG(...) 
#endif // DEBUG
MYSQL* pmysql = NULL;
MYSQL* connectsql = NULL;
int sqlconnect(){
	char*host = "www.yumesekai.top";
	char*user = "typing";
	char*password = "typing";
	char*database = "typinggame";
	pmysql = mysql_init(NULL);
	mysql_set_character_set(pmysql, "utf8");
	connectsql = mysql_real_connect(pmysql, host, user, password, database, 0, NULL, 0);
#ifdef _DEBUG
	FILE *fp = fopen("test.txt", "w");
#endif
	if (connectsql == NULL)
	{

		LOG("connect to %s:%s error\n", host, database);
		return -1;
	}
	else {
		LOG("connect to %s:%s success\n", host, database);
	
	}
#ifdef _DEBUG
	fclose(fp);
#endif
}

void sqlClose() {

	mysql_close(pmysql);
	mysql_library_end();
}


//保存最高记录
int setScore(char *title, char*name ,int score)
{
	char sqlbuff[255];
	if (!connectsql) return -1;
#ifdef _DEBUG
	FILE *fp = fopen("test.txt", "a");
#endif	
	sprintf(sqlbuff, "SELECT * FROM score where title='%s';",title);
	LOG(sqlbuff);
	int ret_query = mysql_query(pmysql, sqlbuff);
	if (ret_query == 0)
	{
		
		MYSQL_RES * my_result = mysql_store_result(pmysql);
		int num = mysql_num_fields(my_result);
		MYSQL_ROW row;
		if (mysql_num_rows(my_result)==0) {
			sprintf(sqlbuff, "insert into score(title, name, highscore) values('%s','%s', %d);",title, name, score);
			LOG(sqlbuff);
			ret_query = mysql_query(pmysql, sqlbuff);
		}else {
			sprintf(sqlbuff, "UPDATE score(title, name, highscore) values('%s','%s', %d)where title='%s' AND highscore<%d;", title, name, score, title, score);
			LOG(sqlbuff);
			ret_query = mysql_query(pmysql, sqlbuff);
		}
		LOG("\n");
		mysql_free_result(my_result);
		
	}
	else
	{
		LOG("query  fail\n");
	}
#ifdef _DEBUG
	fclose(fp);
#endif
	return 0;
}

//获取最高记录
int getScore(char *title) {
	int score = 0;
	char sqlbuff[255];
	if (!connectsql) return -1;
#ifdef _DEBUG
	FILE *fp = fopen("test.txt", "a");
#endif	
	sprintf(sqlbuff, "SELECT highscore FROM score where title='%s';", title);
	LOG(sqlbuff);
	int ret_query = mysql_query(pmysql, sqlbuff);
	if (ret_query == 0)
	{

		MYSQL_RES * my_result = mysql_store_result(pmysql);
		int num = mysql_num_fields(my_result);
		MYSQL_ROW row;
	
		while ((row = mysql_fetch_row(my_result)) != NULL)
		{
			LOG("%s\t",row[0]);
			score=atoi(row[0]);
		}
		LOG("\n");
		// 释放获取的结果
		mysql_free_result(my_result);

	}
	else
	{
		LOG("query  fail\n");
	}
#ifdef _DEBUG
	fclose(fp);
#endif
	//mysql_close(pmysql);
	//mysql_library_end();
	return score;
}
#endif // WIN32