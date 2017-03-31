#include "stdafx.h"
#include "MySQLEx.h"


namespace xPublic{
	CMySQLEx::CMySQLEx()
	{
		m_isInit = false;
	}


	CMySQLEx::~CMySQLEx()
	{
		Close();
	}

	int CMySQLEx::Connect(char *host, int port, char * Db, char * user, char* passwd, char * charset, CString& Msg)
	{
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

	
	BOOL CMySQLEx::IsConnected()
	{
		if (m_mysql.status != MYSQL_STATUS_READY) return true;
		else return false;
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

		return mysql_field_count(&m_mysql) ? true : false;
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