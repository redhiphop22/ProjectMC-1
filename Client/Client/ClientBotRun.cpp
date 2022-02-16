// ClientBotRun.cpp : ���� �����Դϴ�.
//

#include "pch.h"
#include "ClientBotRun.h"


// ClientBotRun

IMPLEMENT_DYNCREATE(ClientBotRun, ClientBase)

ClientBotRun::ClientBotRun()
	: ClientBase(IDD_BOT_RUN)
{
}

ClientBotRun::~ClientBotRun()
{
}

void ClientBotRun::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BOT_RUN_VIEW, m_userView);
	DDX_Control(pDX, IDC_BOT_RUN_NOTICE, m_notice);
}

BEGIN_MESSAGE_MAP(ClientBotRun, CFormView)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// ClientBotRun �����Դϴ�.

#ifdef _DEBUG
void ClientBotRun::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void ClientBotRun::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// ClientBotRun �޽��� ó�����Դϴ�.
void ClientBotRun::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	m_notice.SetWindowTextA("ReconnectTime : 3000ms");

	m_userView.InsertColumn(0, "ID", LVCFMT_LEFT, 100);
	m_userView.InsertColumn(1, "UID", LVCFMT_LEFT, 100);
	m_userView.InsertColumn(2, "State", LVCFMT_LEFT, 100);
}

void ClientBotRun::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	OnViewUpdate();

	ClientBase::OnTimer(nIDEvent);
}

void ClientBotRun::OnEnter()
{
	char temp[64];

	int32_t idx = 0;	
	auto& userList = GetApp()->GetUserList();
	for(auto user : userList)
	{	
		m_userView.InsertItem(idx, user->m_id.c_str());
		_ui64toa_s(user->m_uid, temp, 64, 10);
		m_userView.SetItemText(idx, 1, temp);
		m_userView.SetItemText(idx, 2, "");
		++idx;
	}
	SetTimer(0, 1, NULL);
}

void ClientBotRun::OnExit()
{
	KillTimer(0);
}

void ClientBotRun::OnViewUpdate()
{
	auto GetUserState = [](User::STATE state) -> char* {
		switch(state)
		{
		case User::STATE::CONNECT:		return "CONNECT";
		case User::STATE::LOBBY:		return "LOBBY";
		case User::STATE::DISCONNECT:	return "DISCONNECT";
		}
		return "";
	};
		
	int32_t idx = 0;	
	auto& userList = GetApp()->GetUserList();
	for(auto user : userList)
	{	
		m_userView.SetItemText(idx, 2, GetUserState(user->GetState()));
		++idx;
	}
}

