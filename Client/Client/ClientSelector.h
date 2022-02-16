#pragma once

#include "ClientBase.h"

// ClientSelector �� ���Դϴ�.

class ClientSelector : public ClientBase
{
	DECLARE_DYNCREATE(ClientSelector)

public:
	ClientSelector();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~ClientSelector();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SELECTOR };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	virtual void			OnInitialUpdate();
	afx_msg void OnBnClickedSelectorSingle();
	afx_msg void OnBnClickedSelectorMulti();
};


