
#include "stdafx.h"
#include "CObjectBase.h"

namespace xPublic{

	void DELETE_PARAM(SENDFILEPARAM *p)
	{
		if (p != NULL)
		{
			delete p; p = NULL;
		}
	}

	CObjectBase::CObjectBase()
	{
	}

	CObjectBase::~CObjectBase()
	{
		DeleteAll();
	}

	void CObjectBase::DeleteAll()
	{
		m_cs.Lock();
		try
		{
			for (int i = 0; i < m_arList.GetSize(); i++)
			{
				SENDFILEPARAM *p = m_arList.GetAt(i);
				DELETE_PARAM(p);
			}
			m_arList.RemoveAll();
		}
		catch (...)
		{
		}
		m_cs.Unlock();
	}

	void CObjectBase::AddTail(SENDFILEPARAM *pParam)
	{
		m_cs.Lock();
		try
		{
			m_arList.Add(pParam);
			if (m_arList.GetSize() >= MAX_ARRAY_NUMBER)
			{
				SENDFILEPARAM *p = m_arList.GetAt(0);
				DELETE_PARAM(p);
				m_arList.RemoveAt(0);
#ifdef _OUTPUT_LOG
				recog3_trace("hvmviewer.log", "相机发送图片缓存达到100条");
#endif
			}
		}
		catch (...)
		{
		}
		m_cs.Unlock();
	}

	BOOL CObjectBase::AddTail(CString str)
	{
		SENDFILEPARAM *pParam = new SENDFILEPARAM;
		if (NULL != pParam)
		{
			pParam->strMsg = str;
			AddTail(pParam);
			return TRUE;
		}
		return FALSE;
	}

	void CObjectBase::InsertAt(int index, SENDFILEPARAM *pParam)
	{
		m_cs.Lock();
		try
		{
			m_arList.InsertAt(index, pParam);
			if (m_arList.GetSize() >= MAX_ARRAY_NUMBER)
			{
				SENDFILEPARAM *p = m_arList.GetAt(0);
				DELETE_PARAM(p);
				m_arList.RemoveAt(0);
#ifdef _OUTPUT_LOG
				recog3_trace("hvmviewer.log", "相机发送图片缓存达到100条");
#endif
			}
		}
		catch (...)
		{
		}
		m_cs.Unlock();
	}

	void CObjectBase::DeleteAt(int index, BOOL bDelete)
	{
		m_cs.Lock();
		try
		{
			if (index >= 0 && index < m_arList.GetSize())
			{
				SENDFILEPARAM *p = m_arList.GetAt(index);
				if (bDelete)
				{
					DELETE_PARAM(p);
				}
				m_arList.RemoveAt(index);
			}
		}
		catch (...)
		{
		}
		m_cs.Unlock();
	}

	SENDFILEPARAM* CObjectBase::GetHead()
	{
		SENDFILEPARAM *r = NULL;
		m_cs.Lock();
		try
		{
			if (m_arList.GetSize() > 0)
			{
				r = m_arList.GetAt(0);
				m_arList.RemoveAt(0);
			}
		}
		catch (...)
		{
		}
		m_cs.Unlock();
		return r;
	}

	SENDFILEPARAM* CObjectBase::GetAt(int index)
	{
		SENDFILEPARAM *r = NULL;
		m_cs.Lock();
		try
		{
			if (index >= 0 && index < m_arList.GetSize())
			{
				r = m_arList.GetAt(index);
			}
		}
		catch (...)
		{
		}
		m_cs.Unlock();
		return r;
	}

	int CObjectBase::GetCount()
	{
		return m_arList.GetSize();
	}

}//xPublic