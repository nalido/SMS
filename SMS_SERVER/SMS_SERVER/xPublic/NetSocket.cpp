
// TCPSocket.cpp : implementation file
// Implementation of AsyncSocket derived class.
// This class only works with versions of windows that recognize the TransmitPackets API function call
//

#include "stdafx.h"
#include "NetSocket.h"
#include <winsock2.h>
#include <mstcpip.h>  

namespace xPublic{
;/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
CStringA ip2string(SOCKADDR_IN &sockAddrIN)
{
	CStringA str;
	char *pAddr = inet_ntoa(sockAddrIN.sin_addr);
	str.Format("%s:%d", pAddr,ntohs(sockAddrIN.sin_port));
	//str.Format("%s", pAddr);
	return str;
}
//��ñ��ؼ��������
CStringA getHostName(void)
{
	char szHostName[256] = {};
	int  nRetCode = gethostname(szHostName, sizeof(szHostName));
	CStringA sHostName = szHostName;
	return sHostName;
}

//��ñ���IP
CStringA getHostIP(void)
{
	CStringA sIpAddress;
	struct hostent FAR *lpHostEnt = gethostbyname(getHostName());
	if ( NULL != lpHostEnt )
	{
		//��ȡIP
		LPSTR lpAddr = lpHostEnt->h_addr_list[0];
		if ( lpAddr )
		{
			struct in_addr inAddr;
			memmove(&inAddr,lpAddr,4);
			//ת��Ϊ��׼��ʽ
			sIpAddress = inet_ntoa(inAddr);
		}
	}
	return sIpAddress;
}

//TCPͨѶ��
CTCPClient::CTCPClient(HANDLE hParent, ONTCPRECIVE onRecive, ONTCPCLOSED onClosed)
{
	m_hParent		= hParent;
	m_OnTCPRecive	= onRecive;
	m_OnTCPClosed	= onClosed;
	m_bIsConnect	= FALSE;		 //���ӱ�־
	m_socket		= INVALID_SOCKET;//�׽���
	m_hSockThread	= NULL;			 //�߳̾��
	m_hCloseEvent	= NULL;			 //�̹߳ر��¼�
	m_sHostIPPort	= "127.0.0.1:8008";   //�ͻ���IP&�˿�192.168.1.101:8008

	//��ʼ��socket��Դ
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2),&wsa);
}

CTCPClient::~CTCPClient()
{
	ShutDown();

	//�ͷ���Դ
	WSACleanup();
}

/********************************************************************
��������: �ر����ӣ��麯�������������ӹر�ʱ����Ϣ��ʾ
�������: ��
�������: ��
�� �� ֵ: ��
********************************************************************/
BOOL CTCPClient::Close()
{
	BOOL bConnected = m_bIsConnect;
	m_bIsConnect = FALSE;

	//�ر�Socket
	if ( m_socket != INVALID_SOCKET )
	{
		try
		{
			closesocket(m_socket);
			m_socket  = INVALID_SOCKET;

			if ( bConnected && NULL != m_OnTCPClosed )
			{
				m_OnTCPClosed(m_hParent,this);
			}
		}catch(...)
		{
			TRACE("CTCPClient::Close\r\n");
		}
		return TRUE;
	}
	return FALSE;
}

/********************************************************************
��������: ֹͣ�����麯��
�������: ��
�������: ��
�� �� ֵ: ��
********************************************************************/
void CTCPClient::ShutDown()
{
	//�����˳��¼�
	if ( NULL != m_hCloseEvent )
	{
		::SetEvent(m_hCloseEvent);
	}

	//�ر�Socket
	Close();

	// �ر��߳�
	if ( NULL != m_hSockThread )
	{
		if ( ::WaitForSingleObject(m_hSockThread, 10000) == WAIT_TIMEOUT )
		{
			::TerminateThread(m_hSockThread, 0);
			TRACE("Terminate CTCPClient Thread\r\n");
		}//end while

		::CloseHandle(m_hSockThread); m_hSockThread = NULL;
	}

	// �رչر��¼�
	if ( NULL != m_hCloseEvent )
	{
		::CloseHandle(m_hCloseEvent); m_hCloseEvent = NULL;
	}	
}

/********************************************************************
��������: �Ƿ����ӳɹ�
�������: ��
�������: ��
�� �� ֵ: TRUE �����ӣ�FALSE δ����
********************************************************************/
BOOL CTCPClient::IsConnected(void)
{
	return m_bIsConnect;
}

/********************************************************************
��������: �����׽��֣�һ�����ڷ������������׽��ֺ󣬴����µĿͻ��˲���ֵ
�������: SOCKET sock���׽���
		  const char *pszHostIPPort ���ͻ���IP&�˿�192.168.1.101:8008
		  BOOL noDelay��������ʱ���
�������: ��
�� �� ֵ: TRUE �����ӣ�FALSE δ����
********************************************************************/
BOOL CTCPClient::SetClientSocket(SOCKET sockClient, const char *pszHostIPPort, BOOL noDelay /*TRUE*/)
{
	ShutDown();
	m_socket      = sockClient;
	m_sHostIPPort = pszHostIPPort;

	if ( noDelay )
	{
		int optval = 1; 
		if( setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, sizeof(optval)) < 0 )
		{
			TRACE(_T("Unable to setsockopt TCP_NODELAY\n")); //perror("getsockopt: ");
			Close();
			return FALSE;
		}
	}

	if ( !InitReciveThread() )
	{
		TRACE(_T("Unable to Initialize received thread\n")); //perror("getsockopt: ");
		Close();
		return FALSE;
	}

	m_bIsConnect = SetKeepAlive(m_socket);
	return m_bIsConnect;
}

/********************************************************************
��������: ���÷��ͻ���ͷ��ͳ�ʱ
�������: SOCKET sock���׽���
�������: ��
�� �� ֵ: TRUE ���óɹ���FALSE ����ʧ��
********************************************************************/
BOOL CTCPClient::SetSendBuffer(SOCKET sock)
{	
	//���÷��ͻ�������СΪ32k
	int sendbuf_len = 16 * 1024; //ʵ�ʻ�������С��һ�롣
	if( setsockopt( sock, SOL_SOCKET, SO_SNDBUF, (char*)&sendbuf_len, sizeof(int) ) < 0 )
	{
		TRACE(_T("Unable to setsockopt SNDBUF\n")); //perror("getsockopt: ");
        Close();
		return FALSE;
	}

	//���÷��ͳ�ʱ����
	int nNetTimeout = 10000;//10��
	if ( setsockopt (sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&nNetTimeout, sizeof(int) ) < 0 )
	{
		TRACE(_T("Unable to setsockopt SNDTIMEOUT\n"));
        Close();
		return FALSE;
	}
	return TRUE;
}

/********************************************************************
��������: ���ý��ջ���
�������: SOCKET sock���׽���
�������: ��
�� �� ֵ: TRUE ���óɹ���FALSE ����ʧ��
********************************************************************/
BOOL CTCPClient::SetRecvBuffer(SOCKET sock)
{	
	//���ý��ջ�������СΪ32k
	int recvbuf_len = 16 * 1024; //ʵ�ʻ�������С��һ�롣
	if( setsockopt( sock, SOL_SOCKET, SO_RCVBUF, (char*)&recvbuf_len, sizeof(int) ) < 0 )
	{
		TRACE(_T("Unable to setsockopt SNDBUF\n")); //perror("getsockopt: ");
        Close();
		return FALSE;
	}
	return TRUE;
}

/********************************************************************
��������: ����ͨѶ��·��������������ͨѶ����°������ܼ�������״̬
�������: SOCKET sock���׽���
�������: ��
�� �� ֵ: TRUE ���óɹ���FALSE ����ʧ��
********************************************************************/
BOOL CTCPClient::SetKeepAlive(SOCKET sock)
{	
	BOOL bKeepAlive = TRUE;  
	int nRet = setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&bKeepAlive, sizeof(bKeepAlive));  
	if (nRet == SOCKET_ERROR)  
	{  
		TRACE(L"setsockopt keepAlive failed: %d\n", WSAGetLastError());  
		Close(); 
		return FALSE;
	}  
	// set KeepAlive parameter  
	tcp_keepalive alive_in;  
	tcp_keepalive alive_out;  
	alive_in.keepalivetime		= 1000;	//�೤ʱ�䣨 ms ��û�����ݾͿ�ʼ send ������
	alive_in.keepaliveinterval  = 1000;	//ÿ���೤ʱ�䣨 ms �� send һ��������
	alive_in.onoff              = TRUE;	//�Ƿ��� keepalive
	unsigned long ulBytesReturn = 0;  
	nRet = WSAIoctl(sock, SIO_KEEPALIVE_VALS, &alive_in, sizeof(alive_in),&alive_out, sizeof(alive_out), &ulBytesReturn, NULL, NULL);  
	if (nRet == SOCKET_ERROR)  
	{  
		TRACE(L"WSAIoctl failed: %d\n", WSAGetLastError());  
		Close(); 
		return FALSE;
	}
	return TRUE;
}

/********************************************************************
��������: ��ʼ�������̣߳�����ϲ�ָ��ͻص����պ�����Ч�Ļ��򴴽������߳�
�������: ��
�������: ��
�� �� ֵ: TRUE �����ɹ���FALSE ����ʧ��
********************************************************************/
BOOL CTCPClient::InitReciveThread(void)
{
	if ( NULL != m_hParent && NULL != m_OnTCPRecive )
	{
		if ( NULL == m_hCloseEvent ) m_hCloseEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);//�ֶ�����ʼ��false
		if ( NULL != m_hCloseEvent && NULL == m_hSockThread )
		{
			unsigned int nThreadID = 0;	
			m_hSockThread = (HANDLE)_beginthreadex(NULL,0,OnReveiveProc,this,0,&nThreadID);
		}
		return ( m_hSockThread != NULL );
	}
	else
	{
		return TRUE;
	}
}

/********************************************************************
��������: �ͻ�������
�������: const char *pszHostAddr������IP��ַ
		  WORD nHostPort������˿�
�������: ��
�� �� ֵ: TRUE ���ӳɹ���FALSE ����ʧ��
********************************************************************/
BOOL CTCPClient::Connect(const char *pszHostAddr, WORD nHostPort, WORD nLocalPort)
{
	BOOL bRet = FALSE;
	Close();
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
//	m_sHostIPPort.Format("%s:%d", pszHostAddr,nLocalPort);

	if ( m_socket != INVALID_SOCKET )
	{
		SOCKADDR_IN  serverAddr;
		ZeroMemory((char *)&serverAddr,sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(nHostPort);
		serverAddr.sin_addr.S_un.S_addr	= inet_addr(pszHostAddr);

		BOOL bBind = TRUE;
		if ( nLocalPort > 0 )
		{
			SOCKADDR_IN  clientAddr;
			ZeroMemory(&clientAddr,sizeof(clientAddr));
			clientAddr.sin_family			= AF_INET;
			clientAddr.sin_port				= htons(nLocalPort);
			clientAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

			//���ڿͻ�����Ҫ�󶨶˿��ǣ������������ã���������ʱ��ʧ�ܣ�Ҫ�Ⱥü���������ӳɹ�
			//�����������׽ӿڹر�ʱز�����ӣ�TCP�������������׽ӿڷ��ͻ������е��κ����ݲ�����һ��RST���Է�
			//������ͨ�����ķ�����ֹ���У��������TIME_WAIT״̬��
			//Linger on close if unsent data is present
			linger optval;
			optval.l_linger = 0;//�ȴ�ʱ��
			optval.l_onoff  = 1;
			int nRet = setsockopt(m_socket, SOL_SOCKET, SO_LINGER, (char*)&optval, sizeof(optval));  
			bBind = (nRet!=SOCKET_ERROR && 0==bind(m_socket,(struct sockaddr *)&clientAddr,sizeof(clientAddr))); 
		}

		
		//���ӷ���
		if (   bBind
			&& 0 == connect(m_socket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) 
			&& SetSendBuffer(m_socket)
			&& SetRecvBuffer(m_socket)
			&& SetKeepAlive(m_socket)
			)
		{
			bRet = InitReciveThread();
		}
	}

	m_bIsConnect = bRet;
	return bRet;
}

/********************************************************************
��������: ���ݽ����߳�
�������: LPVOID lparam����ǰ��ָ��
�������: ��
�� �� ֵ: ����ֵû���ر�����壬�ڴ����ǽ�����ֵ��Ϊ0
********************************************************************/
unsigned _stdcall CTCPClient::OnReveiveProc(LPVOID lparam)
{
	ASSERT(lparam != NULL);
	CTCPClient *pThis = (CTCPClient*)lparam;
	INT			fds_rets;
	FD_SET		fds_read;
	TIMEVAL		timeout;
	timeout.tv_sec  = 1;
	timeout.tv_usec = 0;

	while ( WAIT_TIMEOUT == ::WaitForSingleObject(pThis->m_hCloseEvent,0) )
	{
		if ( pThis->m_socket == INVALID_SOCKET )
		{
			::Sleep(10);
			continue;
		}

		FD_ZERO(&fds_read);
		FD_SET(pThis->m_socket, &fds_read);

		//�������0��ʾʱ�䳬ʱ������0��ʾ�����ݣ����������
		fds_rets = ::select(0, &fds_read, NULL, NULL, &timeout);
		if ( fds_rets > 0 )
		{
			if ( pThis->m_OnTCPRecive )
			{
				pThis->m_OnTCPRecive(pThis->m_hParent,pThis);
			}
			else
			{
				char rBuf[1025] = {};
				fds_rets = ::recv(pThis->m_socket,rBuf,1024, 0);
				if( fds_rets <= 0 )
					pThis->Close();
			}
			
		}
		else if ( fds_rets < 0 )
		{
			pThis->Close();
		}
		//end_if_fds_rets
	}
	
	return 0;
}

/********************************************************************
��������: ��������
�������: void * lpBuf��Ҫ���͵�����
		  int len��Ҫ���͵����ݳ���
�������: ��
�� �� ֵ: TRUE ���ͳɹ���FALSE ����ʧ��
********************************************************************/
BOOL CTCPClient::Send(const void * lpBuf, int len)
{
	if ( !IsConnected() ) return FALSE;

	fd_set  fds_write;
	TIMEVAL	timeout;
	timeout.tv_sec  = 1;
	timeout.tv_usec = 0;

	int sendlen = 0 ; 
	while( sendlen <  len )
	{ 
		FD_ZERO(&fds_write);
		FD_SET(m_socket, &fds_write);

		int ret = ::select(0,NULL,&fds_write, NULL,&timeout);//����0��ʾ�����ͣ�����0��ʾ��ʱ��С��0Ϊ�Ͽ�����
		if (ret > 0 )
		{
			int thisSend = len - sendlen;
			thisSend = ::send( m_socket, ( char* )lpBuf+sendlen, thisSend, 0 );
			if( thisSend <= 0 ){
				Close();
				return FALSE;
			}

			sendlen += thisSend;
		}
		else if ( ret < 0 )
		{
			Close();
			return FALSE;
		}
	}
	return ( sendlen == len );
}

/********************************************************************
��������: ����ָ�����ȵ�����
�������: void * lpBuf��Ҫ���յ�����
		  int len��Ҫ���յ����ݳ���
		  long usec�����ճ�ʱʱ�䣬Ĭ��Ϊ1000ms
�������: ��
�� �� ֵ: TRUE ���ճɹ���FALSE ����ʧ��
********************************************************************/
BOOL CTCPClient::Receive(void* lpBuf, int len, long usec /*=1000*/)
{
	int			iCount = 0;
	int			iRtnValue;
	FD_SET		fds_read;
	TIMEVAL		timeout;

	if ( m_socket == INVALID_SOCKET ) return FALSE;

	timeout.tv_sec  = 0;
	timeout.tv_usec = usec * 1000;

	while( iCount < len )
	{
		FD_ZERO(&fds_read);
		FD_SET(m_socket, &fds_read);

		iRtnValue = ::select(0, &fds_read, NULL, NULL, &timeout);
		if ( iRtnValue > 0 )
		{
			iRtnValue = ::recv(m_socket, ( char* )lpBuf+iCount, len-iCount, 0);
			if( iRtnValue <= 0 ){
				Close();
				return FALSE; //SOCKET_ERROR;
			}
			iCount += iRtnValue;
		}
		else if ( iRtnValue < 0 )//�׽��ִ���
		{
			Close();
			return FALSE;
		}
		else
		{
			return FALSE;//��ʱ
		}
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//						TCP����
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//TCPͨѶ��
CTCPServer::CTCPServer(HANDLE hParent, ONTCPACCEPT onAccept, ONTCPRECIVE onRecive, ONTCPCLOSED onClosed) //: baseclass()
{
	m_hParent		= hParent;
	m_OnTCPRecive	= onRecive;
	m_OnTCPClosed	= onClosed;
	m_OnTCPAccept	= onAccept;
	m_socket		= INVALID_SOCKET;//�׽���
	m_hSockThread	= NULL;		//�߳̾��
	m_hCloseEvent	= NULL;		//�̹߳ر��¼�

	//��ʼ��socket��Դ
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2),&wsa);
}

CTCPServer::~CTCPServer()
{
	ShutDown();

	DeleteAllClient();

	//�ͷ���Դ
	WSACleanup();
}

/********************************************************************
��������: �رշ�������
�������: ��
�������: ��
�� �� ֵ: ��
********************************************************************/
BOOL CTCPServer::Close()
{
	//�ر�Socket
	if ( m_socket != INVALID_SOCKET )
	{
		closesocket(m_socket);
		m_socket  = INVALID_SOCKET;
		return TRUE;
	}
	return FALSE;
}

/********************************************************************
��������: ֹͣ����
�������: ��
�������: ��
�� �� ֵ: ��
********************************************************************/
void CTCPServer::ShutDown()
{
	//�����˳��¼�
	if ( NULL != m_hCloseEvent )
	{
		::SetEvent(m_hCloseEvent);
	}

	//�ر�Socket
	Close();

	// �ر��߳�
	if ( NULL != m_hSockThread )
	{
		if ( ::WaitForSingleObject(m_hSockThread, 10000) == WAIT_TIMEOUT )
		{
			::TerminateThread(m_hSockThread, 0);
			TRACE("Terminate CTCPServer Thread\r\n");
		}//end while

		::CloseHandle(m_hSockThread); m_hSockThread = NULL;
	}

	// �رչر��¼�
	if ( NULL != m_hCloseEvent )
	{
		::CloseHandle(m_hCloseEvent); m_hCloseEvent = NULL;
	}

	CloseAllClient();
}

/********************************************************************
��������: ��������
�������: WORD nSocketPort������˿�
�������: ��
�� �� ֵ: TRUE �����ɹ���FALSE ����ʧ��
********************************************************************/
BOOL CTCPServer::Create(WORD nSocketPort)
{
	BOOL bRet = FALSE;
	Close();
	m_socket  = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if ( m_socket != INVALID_SOCKET )
	{
		SOCKADDR_IN  serverAddr;
		ZeroMemory(&serverAddr,sizeof(serverAddr));
		serverAddr.sin_family			= AF_INET;
		serverAddr.sin_port				= htons(nSocketPort);
		serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

		//�󶨼����׽���
		if (   0 == bind(m_socket,(struct sockaddr *)&serverAddr,sizeof(serverAddr)) 
			&& 0 == listen(m_socket,10) 
			)
		{
			if ( NULL == m_hCloseEvent ) m_hCloseEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);//�ֶ�����ʼ��false
			if ( NULL != m_hCloseEvent && NULL == m_hSockThread )
			{
				unsigned int nThreadID = 0;	
				m_hSockThread = (HANDLE)_beginthreadex(NULL,0,OnAcceptProc,this,0,&nThreadID);
				bRet = ( m_hSockThread != NULL );
			}
		}
	}

	return bRet;
}

/********************************************************************
��������: ɾ�����еĿͻ���
�������: ��
�������: ��
�� �� ֵ: ��
********************************************************************/
void CTCPServer::DeleteAllClient()
{
	m_csHost.Lock();
	try
	{
		for ( int i=0; i<m_arByHost.GetSize(); i++ )
		{
			CTCPClient *p = m_arByHost.GetAt(i);
			if ( NULL != p )
			{
				delete p; p = NULL;
			}
		}
		m_arByHost.RemoveAll();

	}catch(...)
	{
		TRACE("CTCPServer::DeleteAllClient\r\n");
	}
	m_csHost.Unlock();
}

void CTCPServer::CloseAllClient()
{
	m_csHost.Lock();
	try
	{
		for ( int i=0; i<m_arByHost.GetSize(); i++ )
		{
			CTCPClient *p = m_arByHost.GetAt(i);
			if ( NULL != p )
			{
				p->ShutDown();
			}
		}

	}catch(...)
	{
		TRACE("CTCPServer::DeleteAllClient\r\n");
	}
	m_csHost.Unlock();
}

/********************************************************************
��������: �������Ʋ��ҿͻ���
�������: const char *pszHostIPPort���ͻ���IP&�˿�192.168.1.101:8008
�������: ��
�� �� ֵ: ���ؿͻ���
********************************************************************/
CTCPClient* CTCPServer::FindClientByName(const char *pszHostIPPort)
{
	CTCPClient* r = NULL;

	m_csHost.Lock();
	try
	{
		for ( int i=0; i<m_arByHost.GetSize(); i++ )
		{
			CTCPClient *p = m_arByHost.GetAt(i);
			if ( NULL != p && 0==_stricmp(p->m_sHostIPPort,pszHostIPPort) )
			{
				r = p;
			}
		}
	}
	catch(...)
	{
		TRACE("CTCPServer::FindClientByName\r\n");
	}
	m_csHost.Unlock();

	return r;
}

/********************************************************************
��������: ����µĿͻ��ˣ�������˼������ͻ���ʱ����
�������: CTCPClient *pClient���¿ͻ���
�������: ��
�� �� ֵ: ��
********************************************************************/
void CTCPServer::InsertNewClient(CTCPClient *pClient)
{
	m_csHost.Lock();
	try
	{
		m_arByHost.Add(pClient);
	}
	catch(...)
	{
		TRACE("CTCPServer::InsertNewClient\r\n");
	}
	m_csHost.Unlock();
}

/********************************************************************
��������: TCP���������¼����麯����һ���������������Ϣ
�������: SOCKET sock���ͻ����׽���
		  const char *pszHostIPPort ���ͻ���IP&�˿�192.168.1.101:8008
�������: ��
�� �� ֵ: ��
********************************************************************/
/*
void CTCPServer::OnTCPAccept(CTCPClient *pClient)
{
}
*/
/********************************************************************
��������: �����׽����¼��߳�
�������: LPVOID lparam����ǰ��ָ��
�������: ��
�� �� ֵ: ����ֵû���ر�����壬�ڴ����ǽ�����ֵ��Ϊ0
********************************************************************/
unsigned _stdcall CTCPServer::OnAcceptProc(LPVOID lparam)
{
	ASSERT(lparam != NULL);
	CTCPServer *pThis = (CTCPServer*)lparam;

	SOCKADDR_IN clientAddr;
	INT  addrlen = sizeof(clientAddr);

	while ( WAIT_TIMEOUT == ::WaitForSingleObject(pThis->m_hCloseEvent, 100) )
	{
		if ( pThis->m_socket == INVALID_SOCKET ) continue;

		//����
		//memset(&clientAddr, 0, addrlen);
		//ntohl(inet_addr(inet_ntoa(clientAddr.sin_addr))),ntohs(clientAddr.sin_port)
		SOCKET sock = accept(pThis->m_socket, (struct sockaddr *)&clientAddr, &addrlen);
		if ( INVALID_SOCKET == sock )
			continue;

		CStringA sHostIPPort = ip2string(clientAddr);
		CTCPClient  *pClient = pThis->FindClientByName(sHostIPPort);
		if ( NULL != pClient )
		{
			//�ͻ����Ѵ��ڣ�ֱ�������׽��ֲ�������
			if ( pClient->SetClientSocket(sock,sHostIPPort,FALSE) )
			{
				if ( pThis->m_OnTCPAccept )
				{
					pThis->m_OnTCPAccept(pThis->m_hParent, pClient, FALSE);
				}
				else
				{
					//pThis->OnTCPAccept(pClient);//�����ص�֪ͨ
				}
			}
			else
			{
				//�����׽���ʧ�ܲ���Ҫ��ɾ���׽��֣����ú����Ѿ�ɾ����
				//closesocket(sock); sock = INVALID_SOCKET;
			}
		}
		else
		{
			//�ͻ��˲����ڣ������ͻ���
			pClient = new CTCPClient(pThis->m_hParent,pThis->m_OnTCPRecive,pThis->m_OnTCPClosed);
			if ( NULL != pClient )
			{
				if ( pClient->SetClientSocket(sock,sHostIPPort,FALSE) )
				{
					pThis->InsertNewClient(pClient);
					if ( pThis->m_OnTCPAccept )
					{
						pThis->m_OnTCPAccept(pThis->m_hParent, pClient, TRUE);
					}
					else
					{
						//pThis->OnTCPAccept(pClient);//�����ص�֪ͨ
					}
				}
				else
				{
					//�����׽���ʧ�ܲ���Ҫ��ɾ���׽��֣����ú����Ѿ�ɾ����
					//closesocket(sock); sock = INVALID_SOCKET;
					delete pClient; pClient = NULL;
				}
			}
			else
			{
				//�����ͻ���ʧ�ܣ�ɾ���׽���
				closesocket(sock); sock = INVALID_SOCKET;
			}
		}//end_if_pClient
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////
// CTCPTree ����
//////////////////////////////////////////////////////////////////////
CTCPTree::CTCPTree()
{
	m_pSelectedNode = NULL;
	m_bDeleteItem   = FALSE;
}

CTCPTree::~CTCPTree()
{
}

BEGIN_MESSAGE_MAP(CTCPTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_GETDISPINFO, &CTCPTree::OnTvnGetdispinfo)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED,  &CTCPTree::OnTvnSelchanged)
//	ON_NOTIFY_REFLECT(NM_DBLCLK, &CTCPTree::OnNMDblclk)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTCPTree ��Ϣ�������
BOOL CTCPTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}

void CTCPTree::OnTvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);

	// Extract tree view item:
	TV_ITEM *pItem = &pTVDispInfo->item;

	// Pointer to object from tree item parameter.  We can cast this to a 
	CTCPClient *pObject = (CTCPClient *)(pItem->lParam);

	try
	{
		// The item's mask text bit will be set if we are being asked to supply
		// the item's text.
		if ( pItem->mask & TVIF_TEXT )
		{
			lstrcpy(pItem->pszText, pObject->m_sHostIPPort);
		}

		// The item's mask (not selected) image bit will be set if we are being
		// asked to supply the items image (index into image control).
		if ( pItem->mask & TVIF_IMAGE )
		{
			// Else give server not connected image:
			pItem->iImage = pObject->IsConnected() ? 4 : 0;
		}

		// The item's mask selected image bit will be set if we are being
		// asked to supply the items image (index into image control).
		if ( pItem->mask & TVIF_SELECTEDIMAGE )
		{
			// Else give server not connected image:
			pItem->iSelectedImage = pObject->IsConnected() ? 4 : 0;
		}
	}
	catch(...)
	{
		TRACE("CTCPTree::OnTvnGetdispinfo\r\n");
	}

	*pResult = 0;
}

void CTCPTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	//��ȡ���ָ��
	CTCPClient *pObject = (CTCPClient *)(pNMTreeView->itemNew.lParam);

	m_pSelectedNode = pObject;
	
	*pResult = 0;
}

/*
void CTCPTree::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
*/
void CTCPTree::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CTreeCtrl::OnPaint()
	//˫�������
	CRect   rect;
    CDC     MenDC;
    CBitmap MemMap;

    GetClientRect(&rect);
    MenDC.CreateCompatibleDC(&dc);
    MemMap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
    MenDC.SelectObject(&MemMap);
	MenDC.FillSolidRect(&rect,RGB(255,255,255));

    //����Ĭ�ϵ�OnPaint(),��ͼ�λ����ڴ�DC����
    DefWindowProc(WM_PAINT,(WPARAM)MenDC.m_hDC,(LPARAM)0);
    
    //���   
    dc.BitBlt(0, 0, rect.Width(), rect.Height(), &MenDC, 0, 0, SRCCOPY);
    MenDC.DeleteDC();
    MemMap.DeleteObject();
}

BOOL CTCPTree::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	return CTreeCtrl::OnEraseBkgnd(pDC);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
}//namespace
