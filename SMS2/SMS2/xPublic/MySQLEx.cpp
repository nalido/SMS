#include "stdafx.h"
#include "MySQLEx.h"


namespace xPublic{
	CMySQLEx::CMySQLEx()
	{
		m_isInit = false;
		m_host = "";
		m_Db = "";
		m_user = "";
		m_passw = "";
		m_Charset = "";
		m_port = 3306;
		m_canReconnect = FALSE;
	}


	CMySQLEx::~CMySQLEx()
	{
		Close();
	}


	int CMySQLEx::Connect(char *host, int port, char * Db, char * user, char* passwd, char * charset, CString& Msg)
	{
		m_host = host;
		m_Db = Db;
		m_user = user;
		m_passw = passwd;
		m_Charset = charset;
		m_port = port;
		m_canReconnect = TRUE;

		if (mysql_init(&m_mysql) == NULL)
		{
			Msg = "initial mysql handle error!";
			return 0;
		}
		m_isInit = true;

		if (!mysql_real_connect(&m_mysql, host, user, passwd, Db, port, NULL, 0))
		{
			CString e = mysql_error(&m_mysql);
			Msg = "Failed to connect to database:" + e;
			return 0;
		}

		if (mysql_set_character_set(&m_mysql, charset) != 0)
		{
			CString e = mysql_error(&m_mysql);
			Msg = "Failed to set character_set:" + e;
			return 0;
		}

		Msg = "successed to connect to database.";
		return 1;
	}

	int CMySQLEx::Reconnect(CString& Msg)
	{
		if (0)//(m_canReconnect)
		{
			Connect(m_host, m_port, m_Db, m_user, m_passw, m_Charset, Msg);
		}
		else
		{
			Msg = "连接参数未初始化！";
			return 0;
		}
		Msg = "重连成功！";
		return 1;
	}
	
	BOOL CMySQLEx::IsConnected()
	{
		CString strMsg;
		return ExecuteQueryExist("SELECT DATE_FORMAT(NOW(),'%Y%m%e-%H:%i:%s')", strMsg);
	}

	
	void CMySQLEx::Close()
	{
		if (m_isInit) mysql_close(&m_mysql);
		m_isInit = false;
	}

	
	BOOL CMySQLEx::ExecuteQuery(LPCTSTR lpszSQL, CDStrs& Fields, CString& Msg)
	{
		if (!m_isInit)
		{
			Msg = "No connection exist.";
			return false;
		}

		if (mysql_real_query(&m_mysql, lpszSQL, strlen(lpszSQL)))
		{
			CString strSQL = lpszSQL;
			Msg = "Failed to ExecuteQuery.\r\n" + strSQL;
			return false;
		}

		MYSQL_ROW row;
		MYSQL_RES *res;
		res = mysql_store_result(&m_mysql);
		if (res == NULL)
		{
			Msg = "Failed to store result.";
			return false;
		}
		int r = mysql_num_rows(res);
		int c = mysql_num_fields(res);
		for (int i = 0; i < r; i++)
		{
			row = mysql_fetch_row(res);
			CStrs strs;
			for (int y = 0; y < c; y++)
			{
				// one row
				strs.push_back(row[y]);
			}
			//one result table
			Fields.push_back(strs);
		}
		mysql_free_result(res);
		return true;
	}

	
	BOOL CMySQLEx::ExecuteQueryExist(LPCTSTR lpszSQL, CString& Msg)
	{
		Msg = "Exist!";
		if (!m_isInit)
		{
			Msg = "No connection exist.";
			return false;
		}

		if (mysql_real_query(&m_mysql, lpszSQL, strlen(lpszSQL)))
		{
			CString strSQL = lpszSQL;
			Msg = "Failed to ExecuteQueryExist.\r\n" + strSQL;
			return false;
		}

		MYSQL_RES *res;
		res = mysql_store_result(&m_mysql);
		BOOL isExist = FALSE;
		if (res) isExist = TRUE;
		mysql_free_result(res);

		return isExist;
	}

	
	BOOL CMySQLEx::ExecuteSQL(LPCTSTR lpszSQL, CString& Msg)
	{
		if (!m_isInit)
		{
			Msg = "No connection exist.";
			return false;
		}
		try
		{
			if (mysql_real_query(&m_mysql, lpszSQL, strlen(lpszSQL)))
			{
				CString strSQL = lpszSQL;
				Msg = "Failed to ExecuteSQL.\r\n" + strSQL;
				return false;
			}
		}
		catch (...)
		{
		}
		return true;
	}

}//xPublic