// ClientLogin.cpp : ���� �����Դϴ�.
//

#include "pch.h"
#include "ClientLogin.h"

// ClientLogin

IMPLEMENT_DYNCREATE(ClientLogin, ClientBase)

ClientLogin::ClientLogin()
	: ClientBase(IDD_CLIENT_CONNECT)
{

}

ClientLogin::~ClientLogin()
{
}

void ClientLogin::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ClientLogin, CFormView)
END_MESSAGE_MAP()


// ClientLogin �����Դϴ�.

#ifdef _DEBUG
void ClientLogin::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void ClientLogin::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// ClientLogin �޽��� ó�����Դϴ�.
void ClientLogin::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}