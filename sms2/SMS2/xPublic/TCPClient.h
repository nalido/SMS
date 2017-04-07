#pragma once
#include "..\\stdafx.h"
#include <afxtempl.h>
#include <afxmt.h>
#include <map>
using namespace std;
namespace xPublic{
;/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
extern DWORD ip2dword(const char *ip); //�ַ�����IP��ַת��ΪDWORD-IP��ַ
extern BOOL  ip2char(DWORD dwIP, char *ip, int nBytesOfAddr);//DWORD-IP��ַת��Ϊ�ַ�����IP��ַ
extern CStringA ip2string(DWORD dwIP, WORD wPort);//IP��ַ��ʽ��
extern CStringA ip2string(SOCKADDR_IN &sockAddrIN);
extern CStringA getHostName(void);//��ñ��ؼ��������
extern CStringA getHostIP(void);//��ñ���IP

class CTCPClient;
typedef void (CALLBACK *ONTCPCLOSED)(LPVOID lParam, CTCPClient *pSocket);
typedef void (CALLBACK *ONTCPRECIVE)(LPVOID lParam, CTCPClient *pSocket);

// TCP stream socket class
class CTCPClient
{	
public:
	CTCPClient(HANDLE hParent = NULL, ONTCPRECIVE onRecive = NULL, ONTCPCLOSED onClosed = NULL);
	virtual ~CTCPClient();

	/********************************************************************
	��������: �ر����ӣ��麯�������������ӹر�ʱ����Ϣ��ʾ
	�������: ��
	�������: ��
	�� �� ֵ: ��
	********************************************************************/
	virtual BOOL Close();

	/********************************************************************
	��������: ֹͣ�����麯��
	�������: ��
	�������: ��
	�� �� ֵ: ��
	********************************************************************/
	virtual void ShutDown();

	/********************************************************************
	��������: �Ƿ����ӳɹ�
	�������: ��
	�������: ��
	�� �� ֵ: TRUE �����ӣ�FALSE δ����
	********************************************************************/
	BOOL IsConnected(void);

	/********************************************************************
	��������: �����׽��֣�һ�����ڷ������������׽��ֺ󣬴����µĿͻ��˲���ֵ
	�������: SOCKET sock���׽���
			  const char *pszHostIPPort ���ͻ���IP&�˿�192.168.1.101:8008
			  BOOL noDelay��������ʱ���
	�������: ��
	�� �� ֵ: TRUE �����ӣ�FALSE δ����
	********************************************************************/
	BOOL SetClientSocket(SOCKET sock, const char *pszHostIPPort, BOOL noDelay = TRUE);

	/********************************************************************
	��������: ��ʼ�������̣߳�����ϲ�ָ��ͻص����պ�����Ч�Ļ��򴴽������߳�
	�������: ��
	�������: ��
	�� �� ֵ: TRUE �����ɹ���FALSE ����ʧ��
	********************************************************************/
	BOOL InitReciveThread(void);

	/********************************************************************
	��������: �ͻ�������
	�������: const char *pszHostAddr������IP��ַ
			  WORD nHostPort������˿�
	�������: ��
	�� �� ֵ: TRUE ���ӳɹ���FALSE ����ʧ��
	********************************************************************/
	BOOL Connect(const char *pszHostAddr, WORD nHostPort, WORD nLocalPort);

	/********************************************************************
	��������: ��������
	�������: void * lpBuf��Ҫ���͵�����
	int len��Ҫ���͵����ݳ���
	�������: ��
	�� �� ֵ: TRUE ���ͳɹ���FALSE ����ʧ��
	********************************************************************/
	BOOL Send(const void * lpBuf, int len);
	BOOL Send3(const void * lpBuf, int len); //����ʶ���Ƶķ��ͺ���
	BOOL Send2(const BYTE * lpBuf, int len, int max);

	/********************************************************************
	��������: ����ָ�����ȵ�����
	�������: void * lpBuf��Ҫ���յ�����
			  int len��Ҫ���յ����ݳ���
			  long usec�����ճ�ʱʱ�䣬Ĭ��Ϊ1000ms
	�������: ��
	�� �� ֵ: TRUE ���ճɹ���FALSE ����ʧ��
	********************************************************************/
	BOOL Receive( void* lpBuf, int len, long sec = 1);
	BOOL Receive2(void* lpBuf, int len, long sec = 1);

	CStringA m_sHostIPPort; //�ͻ���IP&�˿�192.168.1.101:8008

protected:
	/********************************************************************
	��������: ���÷��ͻ���ͷ��ͳ�ʱ
	�������: SOCKET sock���׽���
	�������: ��
	�� �� ֵ: TRUE ���óɹ���FALSE ����ʧ��
	********************************************************************/
	BOOL SetSendBuffer(SOCKET sock);

	/********************************************************************
	��������: ���ý��ջ���
	�������: SOCKET sock���׽���
	�������: ��
	�� �� ֵ: TRUE ���óɹ���FALSE ����ʧ��
	********************************************************************/
	BOOL SetRecvBuffer(SOCKET sock);

	/********************************************************************
	��������: ����ͨѶ��·��������������ͨѶ����°������ܼ�������״̬
	�������: SOCKET sock���׽���
	�������: ��
	�� �� ֵ: TRUE ���óɹ���FALSE ����ʧ��
	********************************************************************/
	BOOL SetKeepAlive(SOCKET sock);

	/********************************************************************
	��������: ���ݽ����߳�
	�������: LPVOID lparam����ǰ��ָ��
	�������: ��
	�� �� ֵ: ����ֵû���ر�����壬�ڴ����ǽ�����ֵ��Ϊ0
	********************************************************************/
	static unsigned _stdcall OnReveiveProc(LPVOID lparam);

	BOOL		m_bIsConnect;	//����״̬
	SOCKET		m_socket;		//�׽���
	HANDLE		m_hSockThread;	//�߳̾��
	HANDLE		m_hCloseEvent;	//�̹߳ر��¼�
	HANDLE		m_hParent;		//������ָ��
	ONTCPRECIVE	m_OnTCPRecive;	//���ջص��ĺ���
	ONTCPCLOSED	m_OnTCPClosed;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
}//namespace

