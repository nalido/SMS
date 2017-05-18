#pragma once
#include "..\\stdafx.h"

#include <vector>
//利用vector数组结构构造一维字符串数组
typedef ::std::vector<CString> CStrs;
//利用vector数组结构构造二维字符串数组
typedef ::std::vector<CStrs> CDStrs;

namespace xPublic{
	class CMySQLEx
	{
	public:
		CMySQLEx();
		~CMySQLEx();

		MYSQL m_mysql; 
		CCriticalSection m_cs;//临界区锁
		BOOL m_isInit; //检验m_mysql是否被初始化过

		char* m_host;
		char* m_Db;
		char* m_user;
		char* m_passw;
		char* m_Charset;
		int m_port;
		BOOL m_canReconnect;

		/*
		函数功能：
			初始化数据库
			连接数据库
			设置字符集
		输入参数：
			host ：MYSQL服务器IP
			port:数据库端口
			Db：数据库名称
			user：数据库用户
			passwd：数据库用户的密码
			charset：希望使用的字符集
			Msg:返回的消息，包括错误消息
		输出参数：
			int ：1表示成功；0表示失败
		*/
		int Connect(char *host, int port, char * Db, char * user, char* passwd, char * charset, CString& Msg);

		//重连数据库
		int Reconnect(CString& Msg);

		/*
		函数功能：检验数据库是否连上
		输入参数：无
		输出参数：无
		返回值： TRUE 已连接， FALSE 未连接
		*/
		BOOL IsConnected();

		/*
		函数功能：断开数据库连接
		输入参数：无
		输出参数：无
		返回值： 无
		*/
		void Close();

		/*
		函数功能：根据SQL语句查询数据库，获取记录集
		输入参数；LPCTSTR lpszSQL SQL语句
		输出参数：CDStrs& Fields 得到的记录集, CString& Msg 错误信息
		返回参数：TRUE 查询成功， FALSE 查询错误
		*/
		BOOL  ExecuteQuery(LPCTSTR lpszSQL, CDStrs& Fields, CString& Msg);

		/*
		函数功能：根据SQL语句查询数据库，判断记录是否存在
		输入参数；LPCTSTR lpszSQL SQL语句
		输出参数：CString& Msg 错误信息
		返回参数：TRUE 存在此记录， FALSE 不存在此记录
		*/
		BOOL ExecuteQueryExist(LPCTSTR lpszSQL, CString& Msg);

		/*
		函数功能：执行SQL语句
		输入参数；LPCTSTR lpszSQL SQL语句
		输出参数：CString& Msg 错误信息
		返回参数：TRUE 执行成功， FALSE 执行错误
		*/
		BOOL ExecuteSQL(LPCTSTR lpszSQL, CString& Msg);

		/*
		函数功能：获取错误信息
		输入参数；
		输出参数：
		返回参数：
		*/
		CString GetError();

		/*
		函数功能：
		输入参数；
		输出参数：
		返回参数：
		*/
	};
}//xPublic

