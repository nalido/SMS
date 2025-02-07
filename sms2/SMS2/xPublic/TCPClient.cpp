
// TCPSocket.cpp : implementation file
// Implementation of AsyncSocket derived class.
// This class only works with versions of windows that recognize the TransmitPackets API function call
//

#include "stdafx.h"
#include "TCPClient.h"
#include <winsock2.h>
#include <mstcpip.h>  

namespace xPublic{
;/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/********************************************************************
函数功能: 字符数组IP地址转换为DWORD-IP地址
输入参数: const char *ip：字符数组IP地址
输出参数: 无
返 回 值: DWORD-IP地址
********************************************************************/
DWORD ip2dword(const char *ip)
{
	return ntohl(inet_addr(ip));
}

/********************************************************************
函数功能: DWORD-IP地址转换为字符数组IP地址
输入参数: DWORD dwIP：DWORD-IP地址
		  int nLen：字符数组大小
输出参数: const char *ip：字符数组IP地址
返 回 值: DWORD-IP地址
********************************************************************/
BOOL ip2char(DWORD dwIP, char *ip, int nBytesOfAddr)
{
	if ( ip )
	{
		struct in_addr   in;
		in.S_un.S_addr = htonl(dwIP);
		char * pAddr = inet_ntoa(in);
		strcpy_s(ip, nBytesOfAddr, pAddr);
		return TRUE;
	}
	return FALSE;
}

/********************************************************************
函数功能: IP地址格式化
输入参数: DWORD dwIP：IP地址
		  WORD wPort：端口
输出参数: 无
返 回 值: 格式化字符串
********************************************************************/
CStringA ip2string(DWORD dwIP, WORD wPort)
{
	CStringA str;
	{
		struct in_addr   in;
		in.S_un.S_addr = htonl(dwIP);
		char * pAddr = inet_ntoa(in);
		str.Format("%s:%d", pAddr,wPort);
	}
	return str;
}
CStringA ip2string(SOCKADDR_IN &sockAddrIN)
{
	CStringA str;
	char *pAddr = inet_ntoa(sockAddrIN.sin_addr);
	str.Format("%s:%d", pAddr,ntohs(sockAddrIN.sin_port));
	return str;
}

//获得本地计算机名称
CStringA getHostName(void)
{
	char szHostName[256] = {};
	int  nRetCode = gethostname(szHostName, sizeof(szHostName));
	CStringA sHostName = szHostName;
	return sHostName;
}

//获得本地IP
CStringA getHostIP(void)
{
	CStringA sIpAddress;
	struct hostent FAR *lpHostEnt = gethostbyname(getHostName());
	if ( NULL != lpHostEnt )
	{
		//获取IP
		LPSTR lpAddr = lpHostEnt->h_addr_list[0];
		if ( lpAddr )
		{
			struct in_addr inAddr;
			memmove(&inAddr,lpAddr,4);
			//转换为标准格式
			sIpAddress = inet_ntoa(inAddr);
		}
	}
	return sIpAddress;
}


//TCP通讯类
CTCPClient::CTCPClient(HANDLE hParent, ONTCPRECIVE onRecive, ONTCPCLOSED onClosed)
{
	m_hParent		= hParent;
	m_OnTCPRecive	= onRecive;
	m_OnTCPClosed	= onClosed;
	m_bIsConnect	= FALSE;		 //连接标志
	m_socket		= INVALID_SOCKET;//套接字
	m_hSockThread	= NULL;			 //线程句柄
	m_hCloseEvent	= NULL;			 //线程关闭事件
	m_sHostIPPort	= "127.0.0.1:8008"; //客户端IP&端口192.168.1.101:8008

	//初始化socket资源
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2),&wsa);
}

CTCPClient::~CTCPClient()
{
	ShutDown();

	//释放资源
	WSACleanup();
}

/********************************************************************
函数功能: 关闭连接，虚函数，可用于连接关闭时的消息显示
输入参数: 无
输出参数: 无
返 回 值: 无
********************************************************************/
BOOL CTCPClient::Close()
{
	BOOL bConnected = m_bIsConnect;
	m_bIsConnect = FALSE;

	//关闭Socket
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
	return bConnected;
}

/********************************************************************
函数功能: 停止服务，虚函数
输入参数: 无
输出参数: 无
返 回 值: 无
********************************************************************/
void CTCPClient::ShutDown()
{
	//设置退出事件
	if ( NULL != m_hCloseEvent )
	{
		::SetEvent(m_hCloseEvent);
	}

	//关闭Socket
	Close();

	// 关闭线程
	if ( NULL != m_hSockThread )
	{
		if ( ::WaitForSingleObject(m_hSockThread, 10000) == WAIT_TIMEOUT )
		{
			::TerminateThread(m_hSockThread, 0);
			TRACE("Terminate CTCPClient Thread\r\n");
		}//end while

		::CloseHandle(m_hSockThread); m_hSockThread = NULL;
	}

	// 关闭关闭事件
	if ( NULL != m_hCloseEvent )
	{
		::CloseHandle(m_hCloseEvent); m_hCloseEvent = NULL;
	}	
}

/********************************************************************
函数功能: 是否连接成功
输入参数: 无
输出参数: 无
返 回 值: TRUE 已连接；FALSE 未连接
********************************************************************/
BOOL CTCPClient::IsConnected(void)
{
	return m_bIsConnect;
}

/********************************************************************
函数功能: 设置套接字，一般用于服务器监听到套接字后，创建新的客户端并赋值
输入参数: SOCKET sock：套接字
		  const char *pszHostIPPort ：客户端IP&端口192.168.1.101:8008
		  BOOL noDelay：发送延时标记
输出参数: 无
返 回 值: TRUE 已连接；FALSE 未连接
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
函数功能: 设置发送缓存和发送超时
输入参数: SOCKET sock：套接字
输出参数: 无
返 回 值: TRUE 设置成功；FALSE 设置失败
********************************************************************/
BOOL CTCPClient::SetSendBuffer(SOCKET sock)
{	
	/*
	//设置发送缓冲区大小为32k
	int sendbuf_len = 16 * 1024; //实际缓冲区大小的一半。
	if( setsockopt( sock, SOL_SOCKET, SO_SNDBUF, (char*)&sendbuf_len, sizeof(int) ) < 0 )
	{
		TRACE(_T("Unable to setsockopt SNDBUF\n")); //perror("getsockopt: ");
        Close();
		return FALSE;
	}
	*/
	//*
	//设置发送超时限制
	int nNetTimeout = 10000;//10秒
	if ( setsockopt (sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&nNetTimeout, sizeof(int) ) < 0 )
	{
		TRACE(_T("Unable to setsockopt SNDTIMEOUT\n"));
        Close();
		return FALSE;
	}//*/
	return TRUE;
}

/********************************************************************
函数功能: 设置接收缓存
输入参数: SOCKET sock：套接字
输出参数: 无
返 回 值: TRUE 设置成功；FALSE 设置失败
********************************************************************/
BOOL CTCPClient::SetRecvBuffer(SOCKET sock)
{	
	//设置接收缓冲区大小为32k
	int recvbuf_len = 16 * 1024; //实际缓冲区大小的一半。
	if( setsockopt( sock, SOL_SOCKET, SO_RCVBUF, (char*)&recvbuf_len, sizeof(int) ) < 0 )
	{
		TRACE(_T("Unable to setsockopt SNDBUF\n")); //perror("getsockopt: ");
        Close();
		return FALSE;
	}
	return TRUE;
}

/********************************************************************
函数功能: 开启通讯链路心跳，在无数据通讯情况下拔网线能检测出连接状态
输入参数: SOCKET sock：套接字
输出参数: 无
返 回 值: TRUE 设置成功；FALSE 设置失败
********************************************************************/
BOOL CTCPClient::SetKeepAlive(SOCKET sock)
{	
	//return TRUE;
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
	alive_in.keepalivetime		= 1000;	//多长时间（ ms ）没有数据就开始 send 心跳包
	alive_in.keepaliveinterval  = 1000;	//每隔多长时间（ ms ） send 一个心跳包
	alive_in.onoff              = TRUE;	//是否开启 keepalive
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
函数功能: 初始化接收线程，如果上层指针和回调接收函数有效的话则创建接收线程
输入参数: 无
输出参数: 无
返 回 值: TRUE 创建成功；FALSE 创建失败
********************************************************************/
BOOL CTCPClient::InitReciveThread(void)
{
	if ( NULL != m_hParent && NULL != m_OnTCPRecive )
	{
		if ( NULL == m_hCloseEvent ) m_hCloseEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);//手动，初始化false
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
函数功能: 客户端连接
输入参数: const char *pszHostAddr：服务IP地址
		  WORD nHostPort：服务端口
输出参数: 无
返 回 值: TRUE 连接成功；FALSE 连接失败
********************************************************************/
BOOL CTCPClient::Connect(const char *pszHostAddr, WORD nHostPort, WORD nLocalPort)
{
	BOOL bRet = FALSE;
	Close();
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	m_sHostIPPort.Format("%s:%d", pszHostAddr,nHostPort);

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

			//对于客户端需要绑定端口是，必须这样设置，否则重连时会失败，要等好几秒才能连接成功
			//以下设置是套接口关闭时夭折连接，TCP将丢弃保留在套接口发送缓冲区中的任何数据并发送一个RST给对方
			//而不是通常的四分组终止序列，这避免了TIME_WAIT状态；
			//Linger on close if unsent data is present
			linger optval;
			optval.l_linger = 0;//等待时间
			optval.l_onoff  = 1;
			int nRet = setsockopt(m_socket, SOL_SOCKET, SO_LINGER, (char*)&optval, sizeof(optval));  
			bBind = (nRet!=SOCKET_ERROR && 0==bind(m_socket,(struct sockaddr *)&clientAddr,sizeof(clientAddr))); 
		}

		
		//连接服务
		if (   bBind
			&& 0 == connect(m_socket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) 
			&& SetSendBuffer(m_socket)
			//&& SetRecvBuffer(m_socket)
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
函数功能: 数据接收线程
输入参数: LPVOID lparam：当前类指针
输出参数: 无
返 回 值: 返回值没有特别的意义，在此我们将返回值设为0
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
			::WaitForSingleObject(pThis->m_hCloseEvent,10);
			continue;
		}

		FD_ZERO(&fds_read);
		FD_SET(pThis->m_socket, &fds_read);

		//如果等于0表示时间超时，大于0表示有数据，否则错误发生
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

/*
	UINT headLen = sizeof(COM_HEAD);
	COM_HEAD head;
		if ( fds_rets > 0 && pThis->Receive(&head,headLen) > 0 )
		{
			if ( head.type < CP_CONN_ASK )
			{
				pThis->kb_insert(head);
				//pWnd->OnReceive(pWnd, &head);
			}
			else if ( head.nVal == 0 )
			{
				pThis->OnReceive(pWnd, &head);
			}
			else
			{
				UINT  nRecvLen = head.nVal;
				CHAR *pRecvBuf = (CHAR *)malloc(nRecvLen+headLen);
				if ( NULL != pRecvBuf )
				{
					//memset(pRecvBuf, 0, nRecvLen+headLen);
					memcpy(pRecvBuf, &head, headLen);

					//读取包数据，分配内存
					nRecvLen = pThis->Receive(pRecvBuf+headLen, nRecvLen);

					//通知数据接收
					pThis->OnReceive(pWnd, pRecvBuf);

					//释放内存
					free(pRecvBuf); 
				}//end_if_recvbuf
			}//end_if_head
		}//end_if_fds_rets
*/

/********************************************************************
函数功能: 发送数据
输入参数: void * lpBuf：要发送的数据
		  int len：要发送的数据长度
输出参数: 无
返 回 值: TRUE 发送成功；FALSE 发送失败
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

		int ret = ::select(0,NULL,&fds_write, NULL,&timeout);//大于0表示允许发送，等于0表示超时，小于0为断开连接
		if (ret > 0 )
		{
			int thisSend = len - sendlen;
			thisSend = ::send( m_socket, ( char* )lpBuf+sendlen, thisSend, 0 );
			if( thisSend <= 0 ){
				//ShowLastError();
				Close();
				return FALSE;
			}

			sendlen += thisSend;
		}
		else if ( ret < 0 )
		{
			//ShowLastError();
			Close();
			return FALSE;
		}
	}
	return ( sendlen == len );
}
BOOL CTCPClient::Send3(const void * lpBuf, int len)
{
	if ( !IsConnected() ) return FALSE;

	fd_set  fds_write;
	TIMEVAL	timeout;
	timeout.tv_sec  = 1;
	timeout.tv_usec = 0;

	int sendlen = 0 ;
	//int fst = 0;
	//while( sendlen <  len )
	{ 
		FD_ZERO(&fds_write);
		FD_SET(m_socket, &fds_write);

		int ret = ::select(0,NULL,&fds_write, NULL,&timeout);//大于0表示允许发送，等于0表示超时，小于0为断开连接
		if (ret > 0 )
		{

			sendlen +=::send( m_socket, ( char* )lpBuf, 100  , 0 );
			if(sendlen<0) 
				return false;

			Sleep(200);
			int nSendMax = 1000;
			::send( m_socket, ( char* )lpBuf, 100  , 0 );
			int nPack = (len-100)/nSendMax;

			
			int i = 0;
			for(i=0; i<nPack; i++)
			{
				sendlen +=::send( m_socket, ( char* )lpBuf+ i*nSendMax+100, nSendMax, 0 );
				if(sendlen<0) return false;
				Sleep(2);
			}
			
			int nlastlen = (len-100) % nSendMax;
			if( nlastlen != 0 )
			{
				sendlen +=::send( m_socket, ( char* )lpBuf+ i*nSendMax+100, nlastlen, 0 );
				if(sendlen<0) return false;
			}

			TRACE(_T("Pack Send %d\n"), i);

			TRACE(_T("SendLen  %d\n"), sendlen);

		/*	int thisSend = len - sendlen;


			thisSend = ::send( m_socket, ( char* )lpBuf+sendlen, thisSend, 0 );
			if( thisSend <= 0 ){
				//ShowLastError();
				Close();
				return FALSE;
			}

			sendlen += thisSend;*/
		}
		else if ( ret < 0 )
		{
			//ShowLastError();
			Close();
			return FALSE;
		}
	}
	return ( sendlen == len );
}


BOOL CTCPClient::Send2(const BYTE * lpBuf, int len, int max)
{
	int sendLen = 0;
	while (sendLen < len)
	{
		if ( len-sendLen > max )
		{
			if ( !Send(lpBuf+sendLen,max) )
				break;
			sendLen += max;
		}
		else
		{
			if ( !Send(lpBuf+sendLen,len-sendLen) )
				break;
			//sendLen = len;
			return TRUE;
		}
	}
	return FALSE;
}

/********************************************************************
函数功能: 接收指定长度的数据
输入参数: void * lpBuf：要接收的数据
		  int len：要接收的数据长度
		  long usec：接收超时时间，默认为1000ms
输出参数: 无
返 回 值: TRUE 接收成功；FALSE 接收失败
********************************************************************/
BOOL CTCPClient::Receive(void* lpBuf, int len, long sec /*=1*/)
{
	int			iCount = 0;
	int			iRtnValue;
	FD_SET		fds_read;
	TIMEVAL		timeout;

	if ( m_socket == INVALID_SOCKET ) return FALSE;

	timeout.tv_sec  = sec;
	timeout.tv_usec = 0;

	while( iCount < len )
	{
		FD_ZERO(&fds_read);
		FD_SET(m_socket, &fds_read);

		iRtnValue = ::select(0, &fds_read, NULL, NULL, &timeout);
		if ( iRtnValue > 0 )
		{
			iRtnValue = ::recv(m_socket, ( char* )lpBuf+iCount, len-iCount, 0);
			if( iRtnValue <= 0 ){
				//ShowLastError();
				Close();
				return FALSE; //SOCKET_ERROR;
			}
			iCount += iRtnValue;
		}
		else if ( iRtnValue < 0 )//套接字错误
		{
			//ShowLastError();
			Close();
			return FALSE;
		}
		else
		{
			//ShowLastError();
			Close();
			return FALSE;//超时
		}
	}

	return TRUE;
}

BOOL CTCPClient::Receive2(void* lpBuf, int len, long sec /*=1*/)
{
	int			iCount = 0;
	int			iRtnValue;
	FD_SET		fds_read;
	TIMEVAL		timeout;

	if ( m_socket == INVALID_SOCKET ) return FALSE;

	timeout.tv_sec  = sec;
	timeout.tv_usec = 0;//usec * 1000;

	while( iCount < len )
	{
		FD_ZERO(&fds_read);
		FD_SET(m_socket, &fds_read);

		iRtnValue = ::select(0, &fds_read, NULL, NULL, &timeout);
		if ( iRtnValue > 0 )
		{
			iRtnValue = ::recv(m_socket, ( char* )lpBuf+iCount, len-iCount, 0);
			if( iRtnValue <= 0 ){
				//ShowLastError();
				Close();
				return FALSE; //SOCKET_ERROR;
			}
			iCount += iRtnValue;
		}
		else if ( iRtnValue < 0 )//套接字错误
		{
			//ShowLastError();
			Close();
			return FALSE;
		}
		else
		{
			return TRUE;//超时
		}
	}

	return TRUE;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
}//namespace
