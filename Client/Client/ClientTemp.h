#pragma once

#include "ClientBase.h"

// ClientLogin �� ���Դϴ�.

class ClientLogin : public ClientBase
{
	DECLARE_DYNCREATE(ClientLogin)

public:
	ClientLogin();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~ClientLogin();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_LOGIN };
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

private:

public:
	virtual void			OnInitialUpdate();
};


