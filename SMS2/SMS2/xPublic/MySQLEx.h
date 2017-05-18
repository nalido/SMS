#pragma once
#include "..\\stdafx.h"

#include <vector>
//����vector����ṹ����һά�ַ�������
typedef ::std::vector<CString> CStrs;
//����vector����ṹ�����ά�ַ�������
typedef ::std::vector<CStrs> CDStrs;

namespace xPublic{
	class CMySQLEx
	{
	public:
		CMySQLEx();
		~CMySQLEx();

		MYSQL m_mysql; 
		CCriticalSection m_cs;//�ٽ�����
		BOOL m_isInit; //����m_mysql�Ƿ񱻳�ʼ����

		char* m_host;
		char* m_Db;
		char* m_user;
		char* m_passw;
		char* m_Charset;
		int m_port;
		BOOL m_canReconnect;

		/*
		�������ܣ�
			��ʼ�����ݿ�
			�������ݿ�
			�����ַ���
		���������
			host ��MYSQL������IP
			port:���ݿ�˿�
			Db�����ݿ�����
			user�����ݿ��û�
			passwd�����ݿ��û�������
			charset��ϣ��ʹ�õ��ַ���
			Msg:���ص���Ϣ������������Ϣ
		���������
			int ��1��ʾ�ɹ���0��ʾʧ��
		*/
		int Connect(char *host, int port, char * Db, char * user, char* passwd, char * charset, CString& Msg);

		//�������ݿ�
		int Reconnect(CString& Msg);

		/*
		�������ܣ��������ݿ��Ƿ�����
		�����������
		�����������
		����ֵ�� TRUE �����ӣ� FALSE δ����
		*/
		BOOL IsConnected();

		/*
		�������ܣ��Ͽ����ݿ�����
		�����������
		�����������
		����ֵ�� ��
		*/
		void Close();

		/*
		�������ܣ�����SQL����ѯ���ݿ⣬��ȡ��¼��
		���������LPCTSTR lpszSQL SQL���
		���������CDStrs& Fields �õ��ļ�¼��, CString& Msg ������Ϣ
		���ز�����TRUE ��ѯ�ɹ��� FALSE ��ѯ����
		*/
		BOOL  ExecuteQuery(LPCTSTR lpszSQL, CDStrs& Fields, CString& Msg);

		/*
		�������ܣ�����SQL����ѯ���ݿ⣬�жϼ�¼�Ƿ����
		���������LPCTSTR lpszSQL SQL���
		���������CString& Msg ������Ϣ
		���ز�����TRUE ���ڴ˼�¼�� FALSE �����ڴ˼�¼
		*/
		BOOL ExecuteQueryExist(LPCTSTR lpszSQL, CString& Msg);

		/*
		�������ܣ�ִ��SQL���
		���������LPCTSTR lpszSQL SQL���
		���������CString& Msg ������Ϣ
		���ز�����TRUE ִ�гɹ��� FALSE ִ�д���
		*/
		BOOL ExecuteSQL(LPCTSTR lpszSQL, CString& Msg);

		/*
		�������ܣ���ȡ������Ϣ
		���������
		���������
		���ز�����
		*/
		CString GetError();

		/*
		�������ܣ�
		���������
		���������
		���ز�����
		*/
	};
}//xPublic

