#pragma once

#include "ClientBase.h"

// ClientConnect �� ���Դϴ�.

class ClientConnect : public ClientBase
{
	DECLARE_DYNCREATE(ClientConnect)

public:
	ClientConnect();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~ClientConnect();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_CONNECT };
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

	CIPAddressCtrl			m_ip;
	UINT					m_port;

public:
	virtual void			OnInitialUpdate();
	afx_msg void			OnBnClickedConnectConnect();
};


