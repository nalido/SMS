#pragma once
#include "..\\stdafx.h"
#include <afxtempl.h>
#include <afxmt.h>
#include <map>
using namespace std;
namespace xPublic{
;/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
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

	/********************************************************************
	��������: ����ָ�����ȵ�����
	�������: void * lpBuf��Ҫ���յ�����
			  int len��Ҫ���յ����ݳ���
			  long usec�����ճ�ʱʱ�䣬Ĭ��Ϊ1000ms
	�������: ��
	�� �� ֵ: TRUE ���ճɹ���FALSE ����ʧ��
	********************************************************************/
	BOOL Receive(void* lpBuf, int len, long usec = 1000);

	CStringA m_sHostIPPort; //�ͻ���IP&�˿�192.168.1.101

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


class CTCPServer
{	
	typedef void (CALLBACK *ONTCPACCEPT)(LPVOID lParam, CTCPClient *pClient, BOOL bNew);
public:
	CTCPServer(HANDLE hParent = NULL, ONTCPACCEPT onAccept = NULL, ONTCPRECIVE onRecive = NULL, ONTCPCLOSED onClosed = NULL);
	virtual ~CTCPServer();

	/********************************************************************
	��������: TCP���������¼����麯����һ���������������Ϣ
	�������: SOCKET sock���ͻ����׽���
			  const char *pszHostIPPort ���ͻ���IP&�˿�192.168.1.101:8008
	�������: ��
	�� �� ֵ: ��
	********************************************************************/
	//virtual void OnTCPAccept(CTCPClient *pClient);

	/********************************************************************
	��������: �رշ�������
	�������: ��
	�������: ��
	�� �� ֵ: ��
	********************************************************************/
	BOOL Close();

	/********************************************************************
	��������: ֹͣ����
	�������: ��
	�������: ��
	�� �� ֵ: ��
	********************************************************************/
	void ShutDown();

	/********************************************************************
	��������: ��������
	�������: WORD nSocketPort������˿�
	�������: ��
	�� �� ֵ: TRUE �����ɹ���FALSE ����ʧ��
	********************************************************************/
	BOOL Create(WORD nSocketPort);

protected:
	/********************************************************************
	��������: ɾ�����еĿͻ���
	�������: ��
	�������: ��
	�� �� ֵ: ��
	********************************************************************/
	void DeleteAllClient();
	void CloseAllClient();

	/********************************************************************
	��������: �������Ʋ��ҿͻ���
	�������: const char *pszHostIPPort���ͻ���IP&�˿�192.168.1.101:8008
	�������: ��
	�� �� ֵ: ���ؿͻ���
	********************************************************************/
	CTCPClient* FindClientByName(const char *pszName);

	/********************************************************************
	��������: ����µĿͻ��ˣ�������˼������ͻ���ʱ����
	�������: CTCPClient *pClient���¿ͻ���
	�������: ��
	�� �� ֵ: ��
	********************************************************************/
	void InsertNewClient(CTCPClient *pClient);

	/********************************************************************
	��������: �����׽����¼��߳�
	�������: LPVOID lparam����ǰ��ָ��
	�������: ��
	�� �� ֵ: ����ֵû���ر�����壬�ڴ����ǽ�����ֵ��Ϊ0
	********************************************************************/
	static unsigned _stdcall OnAcceptProc(LPVOID lparam);

	SOCKET		m_socket;		//�׽���
	HANDLE		m_hSockThread;	//�߳̾��
	HANDLE		m_hCloseEvent;	//�̹߳ر��¼�
	HANDLE		m_hParent;		//������ָ��
	ONTCPRECIVE	m_OnTCPRecive;	//���ջص��ĺ���
	ONTCPCLOSED	m_OnTCPClosed;
	ONTCPACCEPT	m_OnTCPAccept;	//�����ص��ĺ���
	CArray<CTCPClient*,CTCPClient*&>  m_arByHost;
	::CCriticalSection		   m_csHost;
};

//////////////////////////////////////////////////////////////////////
// CTCPTree ����
//////////////////////////////////////////////////////////////////////
class CTCPTree : public CTreeCtrl
{
// ����
public:
	CTCPTree();
	virtual ~CTCPTree();
	CTCPClient	*m_pSelectedNode;
	BOOL		 m_bDeleteItem;
// ��д
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()	
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
}//namespace

