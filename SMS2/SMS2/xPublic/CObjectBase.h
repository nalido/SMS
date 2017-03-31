//CObjectBase.h: interface for the CObjectBase class.
#pragma once
#include "..\\stdafx.h"

#define MAX_ARRAY_NUMBER 1000  //��󱣴���Ϣ����

namespace xPublic{
	typedef struct struct_SENDFILEPARAM//���ݸ�����ͼ���̵߳Ľṹ��������������ջ�������ָ�룬��������ip
	{
		CString		sPicName;
		CString		sPicName2;
		BYTE*		pictureBuff;
		int			pictureLen;	//12.5 �������� ���͵����ݴ�С�����ڶ���ʶ�������ݡ�
		int			nDelType;	//0=��������ɾ����1=ֻɾ��struct��2=ɾ��structͬʱɾ��byte
		BYTE		platePos;	//0�󣬷���Ϊ��
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