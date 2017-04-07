// SerialPort.h: interface for the CSerialPort class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "..\\stdafx.h"
namespace xPublic{
;/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class CThreadBase  
{
	typedef void (CALLBACK *ONWORK)(LPVOID lParam, HANDLE hCloseEvent);
public:
	//构造时决定线程处理方式(hParent和onWork都有效时通过回调处理，否则使用虚函数处理)
	//一般情况，一个类要使用多个线程采用回调处理，若是单线程可使用虚函数，也可以回调
	CThreadBase(HANDLE hParent = NULL, ONWORK onWork = NULL);
	virtual ~CThreadBase();

	/********************************************************************
	函数功能: 线程处理事件，虚函数，通知上层处理
	输入参数: HANDLE hCloseEvent：退出事件
	输出参数: 无
	返 回 值: 无
	********************************************************************/
	virtual void DoWork(HANDLE hCloseEvent);

	/********************************************************************
	函数功能: 停止线程
	输入参数: 无
	输出参数: 无
	返 回 值: 无
	********************************************************************/
	void StopThread();

	/********************************************************************
	函数功能: 启动线程
	输入参数: 无
	输出参数: 无
	返 回 值: TRUE 成功；FALSE 失败
	********************************************************************/
	BOOL StartThread();

	/********************************************************************
	函数功能: 关闭通知,一般用于提前通知关闭事件
	输入参数: 无
	输出参数: 无
	返 回 值: TRUE 成功；FALSE 失败
	********************************************************************/
	void SetClose(void);

	/********************************************************************
	函数功能: 等待，必须在启动线程后才有效
	输入参数: DWORD dwTimeOut：等待时间,允许为0
	输出参数: 无
	返 回 值: TRUE 成功；FALSE 失败
	********************************************************************/
	BOOL WaitTimeOut(DWORD dwTimeOut =10);

protected:
	/********************************************************************
	函数功能：线程函数
	输入参数：lParam：指向当前类的句柄
	输出参数：无
	返 回 值：0，无实际意义
	/********************************************************************/
	static unsigned _stdcall WorkProc(LPVOID lParam);

protected:
	HANDLE	m_hWorkThread;	//线程句柄
	HANDLE	m_hCloseEvent;	//线程关闭事件
	HANDLE	m_hParent;		//父窗口指针
	ONWORK	m_OnWork;		//处理回调的函数
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
}//namespace
