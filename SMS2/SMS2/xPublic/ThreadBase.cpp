// SerialPort.cpp: implementation of the CSerialPort class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ThreadBase.h"
#include <process.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace xPublic{
;/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//����ʱ�����̴߳���ʽ(hParent��onWork����Чʱͨ���ص���������ʹ���麯������)
//һ�������һ����Ҫʹ�ö���̲߳��ûص��������ǵ��߳̿�ʹ���麯����Ҳ���Իص�
CThreadBase::CThreadBase(HANDLE hParent, ONWORK onWork)
{
	m_hParent	  = hParent;
	m_OnWork	  = onWork;
	m_hWorkThread = NULL;	//�߳̾��
	m_hCloseEvent = NULL;	//�̹߳ر��¼�
}

CThreadBase::~CThreadBase()
{
	StopThread();
}

/********************************************************************
��������: �̴߳����¼����麯����֪ͨ�ϲ㴦��
�������: HANDLE hCloseEvent���˳��¼�
�������: ��
�� �� ֵ: ��
********************************************************************/
void CThreadBase::DoWork(HANDLE hCloseEvent)
{
	while ( WaitTimeOut(1000) )
	{
	}
}

/********************************************************************
�������ܣ��̺߳���
���������lParam��ָ��ǰ��ľ��
�����������
�� �� ֵ��0����ʵ������
/********************************************************************/
unsigned _stdcall CThreadBase::WorkProc(LPVOID lParam)
{
	ASSERT(lParam != NULL);
	CThreadBase *pThis = (CThreadBase*)lParam;

	if ( NULL != pThis->m_hParent && NULL != pThis->m_OnWork )
	{
		pThis->m_OnWork(pThis->m_hParent, pThis->m_hCloseEvent);
	}
	else
	{
		pThis->DoWork(pThis->m_hCloseEvent);
	}
	return 0;
}

/********************************************************************
��������: ֹͣ�߳�
�������: ��
�������: ��
�� �� ֵ: ��
********************************************************************/
void CThreadBase::StopThread()
{
	// ���ùر��¼����ź�
	if ( NULL != m_hCloseEvent )
	{
		::SetEvent(m_hCloseEvent);
	}
	// �ر��߳�
	if ( NULL != m_hWorkThread )
	{
		if ( ::WaitForSingleObject(m_hWorkThread, 30000) == WAIT_TIMEOUT )
		{
			::TerminateThread(m_hWorkThread, 0);
		}
		::CloseHandle(m_hWorkThread); m_hWorkThread = NULL;
	}
	// �ر��¼�
	if ( NULL != m_hCloseEvent )
	{
		::CloseHandle(m_hCloseEvent); m_hCloseEvent = NULL;
	}
}

/********************************************************************
��������: �����߳�
�������: ��
�������: ��
�� �� ֵ: TRUE �ɹ���FALSE ʧ��
********************************************************************/
BOOL CThreadBase::StartThread()
{
	if ( NULL == m_hCloseEvent ) m_hCloseEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if ( NULL != m_hCloseEvent && NULL == m_hWorkThread )
	{
		unsigned int nThreadID;	
		m_hWorkThread = (HANDLE) _beginthreadex(NULL,0,WorkProc,this,0,&nThreadID);
	}
	return ( NULL != m_hWorkThread );
}

/********************************************************************
��������: �ر�֪ͨ,һ��������ǰ֪ͨ�ر��¼�
�������: ��
�������: ��
�� �� ֵ: TRUE �ɹ���FALSE ʧ��
********************************************************************/
void CThreadBase::SetClose()
{
	// ���ùر��¼����ź�
	if ( NULL != m_hCloseEvent )
	{
		::SetEvent(m_hCloseEvent);
	}
}

/********************************************************************
��������: �ȴ��������������̺߳����Ч
�������: DWORD dwTimeOut���ȴ�ʱ��,����Ϊ0
�������: ��
�� �� ֵ: TRUE �ɹ���FALSE ʧ��
********************************************************************/
BOOL CThreadBase::WaitTimeOut(DWORD dwTimeOut /*=10*/)
{
	return ( WAIT_TIMEOUT == ::WaitForSingleObject(m_hCloseEvent, dwTimeOut) );
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
}//namespace
