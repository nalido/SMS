#pragma once
#include "..\\stdafx.h"
#include <afxtempl.h>
#include <afxmt.h>
#include <map>
using namespace std;
namespace xPublic{
;/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
extern DWORD ip2dword(const char *ip); //字符数组IP地址转换为DWORD-IP地址
extern BOOL  ip2char(DWORD dwIP, char *ip, int nBytesOfAddr);//DWORD-IP地址转换为字符数组IP地址
extern CStringA ip2string(DWORD dwIP, WORD wPort);//IP地址格式化
extern CStringA ip2string(SOCKADDR_IN &sockAddrIN);
extern CStringA getHostName(void);//获得本地计算机名称
extern CStringA getHostIP(void);//获得本地IP

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
	函数功能: 关闭连接，虚函数，可用于连接关闭时的消息显示
	输入参数: 无
	输出参数: 无
	返 回 值: 无
	********************************************************************/
	virtual BOOL Close();

	/********************************************************************
	函数功能: 停止服务，虚函数
	输入参数: 无
	输出参数: 无
	返 回 值: 无
	********************************************************************/
	virtual void ShutDown();

	/********************************************************************
	函数功能: 是否连接成功
	输入参数: 无
	输出参数: 无
	返 回 值: TRUE 已连接；FALSE 未连接
	********************************************************************/
	BOOL IsConnected(void);

	/********************************************************************
	函数功能: 设置套接字，一般用于服务器监听到套接字后，创建新的客户端并赋值
	输入参数: SOCKET sock：套接字
			  const char *pszHostIPPort ：客户端IP&端口192.168.1.101:8008
			  BOOL noDelay：发送延时标记
	输出参数: 无
	返 回 值: TRUE 已连接；FALSE 未连接
	********************************************************************/
	BOOL SetClientSocket(SOCKET sock, const char *pszHostIPPort, BOOL noDelay = TRUE);

	/********************************************************************
	函数功能: 初始化接收线程，如果上层指针和回调接收函数有效的话则创建接收线程
	输入参数: 无
	输出参数: 无
	返 回 值: TRUE 创建成功；FALSE 创建失败
	********************************************************************/
	BOOL InitReciveThread(void);

	/********************************************************************
	函数功能: 客户端连接
	输入参数: const char *pszHostAddr：服务IP地址
			  WORD nHostPort：服务端口
	输出参数: 无
	返 回 值: TRUE 连接成功；FALSE 连接失败
	********************************************************************/
	BOOL Connect(const char *pszHostAddr, WORD nHostPort, WORD nLocalPort);

	/********************************************************************
	函数功能: 发送数据
	输入参数: void * lpBuf：要发送的数据
	int len：要发送的数据长度
	输出参数: 无
	返 回 值: TRUE 发送成功；FALSE 发送失败
	********************************************************************/
	BOOL Send(const void * lpBuf, int len);
	BOOL Send3(const void * lpBuf, int len); //二次识别定制的发送函数
	BOOL Send2(const BYTE * lpBuf, int len, int max);

	/********************************************************************
	函数功能: 接收指定长度的数据
	输入参数: void * lpBuf：要接收的数据
			  int len：要接收的数据长度
			  long usec：接收超时时间，默认为1000ms
	输出参数: 无
	返 回 值: TRUE 接收成功；FALSE 接收失败
	********************************************************************/
	BOOL Receive( void* lpBuf, int len, long sec = 1);
	BOOL Receive2(void* lpBuf, int len, long sec = 1);

	CStringA m_sHostIPPort; //客户端IP&端口192.168.1.101:8008

protected:
	/********************************************************************
	函数功能: 设置发送缓存和发送超时
	输入参数: SOCKET sock：套接字
	输出参数: 无
	返 回 值: TRUE 设置成功；FALSE 设置失败
	********************************************************************/
	BOOL SetSendBuffer(SOCKET sock);

	/********************************************************************
	函数功能: 设置接收缓存
	输入参数: SOCKET sock：套接字
	输出参数: 无
	返 回 值: TRUE 设置成功；FALSE 设置失败
	********************************************************************/
	BOOL SetRecvBuffer(SOCKET sock);

	/********************************************************************
	函数功能: 开启通讯链路心跳，在无数据通讯情况下拔网线能检测出连接状态
	输入参数: SOCKET sock：套接字
	输出参数: 无
	返 回 值: TRUE 设置成功；FALSE 设置失败
	********************************************************************/
	BOOL SetKeepAlive(SOCKET sock);

	/********************************************************************
	函数功能: 数据接收线程
	输入参数: LPVOID lparam：当前类指针
	输出参数: 无
	返 回 值: 返回值没有特别的意义，在此我们将返回值设为0
	********************************************************************/
	static unsigned _stdcall OnReveiveProc(LPVOID lparam);

	BOOL		m_bIsConnect;	//连接状态
	SOCKET		m_socket;		//套接字
	HANDLE		m_hSockThread;	//线程句柄
	HANDLE		m_hCloseEvent;	//线程关闭事件
	HANDLE		m_hParent;		//父窗口指针
	ONTCPRECIVE	m_OnTCPRecive;	//接收回调的函数
	ONTCPCLOSED	m_OnTCPClosed;
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
}//namespace

