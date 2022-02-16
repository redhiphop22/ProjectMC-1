#pragma once

#include "ClientBase.h"

// ClientServerMove �� ���Դϴ�.

class ClientServerMove : public ClientBase
{
	DECLARE_DYNCREATE(ClientServerMove)

public:
	ClientServerMove();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~ClientServerMove();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_SERVERMOVE };
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

protected:
	CString					m_uid;
	CString					m_id;

public:
	virtual void			OnInitialUpdate();
	afx_msg void			OnBnClickedServermoveMove();
};


