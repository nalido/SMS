#pragma once

#include "xPublic\CvvImage.h"
#include <opencv\cxcore.h>
#include <opencv2\core.hpp>
#include <opencv2\opencv.hpp>

// CMyPicCtrl

class CMyPicCtrl : public CStatic
{
	DECLARE_DYNAMIC(CMyPicCtrl)

public:
	CMyPicCtrl();
	virtual ~CMyPicCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	void InitPicSource(cv::Mat* mat);
	cv::Mat* m_pPic;
};


