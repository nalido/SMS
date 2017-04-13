//CObjectBase.h: interface for the CObjectBase class.
#pragma once
#include "..\\stdafx.h"

#define MAX_ARRAY_NUMBER 1000  //最大保存信息数量

namespace xPublic{
	typedef struct struct_SENDFILEPARAM//传递给发送图像线程的结构体参数，包括接收缓冲区的指针，服务器的ip
	{
		CString		strMsg;

		struct_SENDFILEPARAM()
		{
			strMsg = "";
		};
	}SENDFILEPARAM;

	class CObjectBase
	{
	public:
		CObjectBase();
		virtual ~CObjectBase();

		void DeleteAll();
		void AddTail(SENDFILEPARAM *pParam);
		BOOL AddTail(CString str);
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