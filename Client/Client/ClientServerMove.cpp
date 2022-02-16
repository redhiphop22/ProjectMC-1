// ClientServerMove.cpp : ���� �����Դϴ�.
//

#include "pch.h"
#include "ClientServerMove.h"

// ClientServerMove

IMPLEMENT_DYNCREATE(ClientServerMove, ClientBase)

ClientServerMove::ClientServerMove()
	: ClientBase(IDD_CLIENT_SERVERMOVE)
	, m_uid(_T("1"))
	, m_id(_T("test1"))
{
}

ClientServerMove::~ClientServerMove()
{
}

void ClientServerMove::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SERVERMOVE_UID, m_uid);
	DDX_Text(pDX, IDC_SERVERMOVE_ID, m_id);
}

BEGIN_MESSAGE_MAP(ClientServerMove, CFormView)
	ON_BN_CLICKED(IDC_SERVERMOVE_MOVE, &ClientServerMove::OnBnClickedServermoveMove)
END_MESSAGE_MAP()


// ClientServerMove �����Դϴ�.

#ifdef _DEBUG
void ClientServerMove::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void ClientServerMove::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// ClientServerMove �޽��� ó�����Դϴ�.
void ClientServerMove::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}

void ClientServerMove::OnBnClickedServermoveMove()
{
	UpdateData(TRUE);

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol::CreateSERVER_CONNECT_C2S(fbb,
		atoll(m_uid.GetBuffer()),
		fbb.CreateString(m_id.GetBuffer())
	);
	fbb.Finish(body);

	SEND_PACKET(SERVER_CONNECT_C2S, fbb);
}
