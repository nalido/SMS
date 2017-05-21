#include "stdafx.h"
#include "MySQLEx.h"
#include "..\MainFrm.h"


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
		if (m_canReconnect)
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
		CDStrs Time;
		BOOL ret = ExecuteQuery("SELECT DATE_FORMAT(NOW(),'%Y/%m/%d-%H:%i:%s')", Time, strMsg);
		if (Time.size() > 0) g_strCurrentTime = Time[0][0];
		else
		{
			CTime t = CTime::GetCurrentTime();
			g_strCurrentTime = t.Format("%Y/%m/%d-%H:%M:%S");
		}
		return ret;
	}


	void CMySQLEx::Close()
	{
		if (m_isInit) mysql_close(&m_mysql);
		m_isInit = false;
	}


	BOOL CMySQLEx::ExecuteQuery(LPCTSTR lpszSQL, CDStrs& Fields, CString& Msg)
	{
		m_cs.Lock();
		Msg = "ExecuteQuery Successed";
		if (!m_isInit)
		{
			Msg = "No connection exist.";
			m_cs.Unlock();
			return false;
		}

		if (mysql_real_query(&m_mysql, lpszSQL, strlen(lpszSQL)))
		{
			CString strSQL = lpszSQL;
			Msg = "Failed to ExecuteQuery.\r\n" + strSQL;
			m_cs.Unlock();
			return false;
		}

		MYSQL_ROW row;
		MYSQL_RES *res;
		res = mysql_store_result(&m_mysql);
		if (res == NULL)
		{
			Msg = "Failed to store result.";
			m_cs.Unlock();
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

		m_cs.Unlock();
		return true;
	}


	BOOL CMySQLEx::ExecuteQueryExist(LPCTSTR lpszSQL, CString& Msg)
	{
		m_cs.Lock();
		Msg = "Exist!";
		if (!m_isInit)
		{
			Msg = "No connection exist.";
			m_cs.Unlock();
			return false;
		}

		if (mysql_real_query(&m_mysql, lpszSQL, strlen(lpszSQL)))
		{
			CString strSQL = lpszSQL;
			Msg = "Failed to ExecuteQueryExist.\r\n" + strSQL;
			m_cs.Unlock();
			return false;
		}

		MYSQL_RES *res;
		res = mysql_store_result(&m_mysql);
		BOOL isExist = FALSE;

		int r = mysql_num_rows(res);
		//int c = mysql_num_fields(res);
		if (r>0)
			isExist = TRUE;
		mysql_free_result(res);
		m_cs.Unlock();
		return isExist;
	}


	BOOL CMySQLEx::ExecuteSQL(LPCTSTR lpszSQL, CString& Msg)
	{
		m_cs.Lock();
		Msg = "ExecuteSQL successed";
		if (!m_isInit)
		{
			Msg = "No connection exist.";
			m_cs.Unlock();
			return false;
		}
		try
		{
			if (mysql_real_query(&m_mysql, lpszSQL, strlen(lpszSQL)))
			{
				CString strSQL = lpszSQL;
				Msg = "Failed to ExecuteSQL.\r\n" + strSQL;
				m_cs.Unlock();
				return false;
			}
		}
		catch (...)
		{
		}
		m_cs.Unlock();
		return true;
	}

	CString CMySQLEx::GetError()
	{
		return mysql_error(&m_mysql);
	}

}//xPublic