//CObjectBase.h: interface for the CObjectBase class.
#pragma once
#include "..\\stdafx.h"

#define MAX_ARRAY_NUMBER 1000  //最大保存信息数量

namespace xPublic{
	typedef struct struct_SENDFILEPARAM//传递给发送图像线程的结构体参数，包括接收缓冲区的指针，服务器的ip
	{
		CString		sPicName;
		CString		sPicName2;
		BYTE*		pictureBuff;
		int			pictureLen;	//12.5 新增变量 发送的数据大小，用于二次识别发送数据。
		int			nDelType;	//0=整个都不删除，1=只删除struct，2=删除struct同时删除byte
		BYTE		platePos;	//0左，非零为右
		CTime		snapTime;
		BYTE		needMove;

		struct_SENDFILEPARAM()
		{
			pictureBuff = NULL;
			pictureLen = 0;
			nDelType = 0;
			platePos = 0;
			snapTime = 0;
			needMove = 0;
		};
	}SENDFILEPARAM;

	class CObjectBase
	{
	public:
		CObjectBase();
		virtual ~CObjectBase();

		void DeleteAll();
		void AddTail(SENDFILEPARAM *pParam);
		BOOL AddTail(BYTE *pSrc, int nSrc);
		void InsertAt(int index, SENDFILEPARAM *pParam);
		void DeleteAt(int index, BOOL bDelete = TRUE);
		SENDFILEPARAM* GetHead();
		SENDFILEPARAM* GetAt(int index);
		int GetCount();

	public:
		CArray<SENDFILEPARAM*, SENDFILEPARAM*&> m_arList;
		::CCriticalSection						m_cs;
	};
}//xPublic