#pragma once

#include "ClientBase.h"

// ClientBotRun �� ���Դϴ�.

class ClientBotRun : public ClientBase
{
	DECLARE_DYNCREATE(ClientBotRun)

public:
	ClientBotRun();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~ClientBotRun();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BOT_RUN };
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

	CListCtrl				m_userView;
	CEdit					m_notice;
public:
	virtual void			OnInitialUpdate();
	virtual void			OnEnter();
	virtual void			OnExit();

	void					OnViewUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


