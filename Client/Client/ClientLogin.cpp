// ClientLogin.cpp : ���� �����Դϴ�.
//

#include "pch.h"
#include "ClientLogin.h"

// ClientLogin

IMPLEMENT_DYNCREATE(ClientLogin, ClientBase)

ClientLogin::ClientLogin()
	: ClientBase(IDD_CLIENT_LOGIN)
	, m_id(_T("idtest"))
	, m_pw(_T("pwtest"))
{
}

ClientLogin::~ClientLogin()
{
}

void ClientLogin::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LOGIN_ID, m_id);
	DDX_Text(pDX, IDC_LOGIN_PW, m_pw);
}

BEGIN_MESSAGE_MAP(ClientLogin, CFormView)
	ON_BN_CLICKED(IDC_LOGIN_LOGIN, &ClientLogin::OnBnClickedLoginLogin)
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

void ClientLogin::OnBnClickedLoginLogin()
{
	UpdateData(TRUE);

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol::CreateLOGIN_C2S(fbb,
		fbb.CreateString(m_id.GetBuffer()),
		fbb.CreateString(m_pw.GetBuffer())
	);
	fbb.Finish(body);
	
	SEND_PACKET(LOGIN_C2S, fbb);
}
