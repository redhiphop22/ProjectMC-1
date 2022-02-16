#pragma once

#include "resource.h"
#include "ControllerSingle.h"
#include "Client.h"
#include "User.h"

#define SEND_PACKET(p, f)	CONTROLLER_S.p(GetUser(), f)

class ClientBase : public CFormView
{
	DECLARE_DYNCREATE(ClientBase)

public:
	ClientBase();
	ClientBase(UINT nIDTemplate);           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~ClientBase();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

	CClientApp*		GetApp();
	User*			GetUser();

public:
	virtual void	OnEnter() {};
	virtual void	OnExit() {};
};
