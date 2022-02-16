// ClientLobby.cpp : ���� �����Դϴ�.
//

#include "pch.h"
#include "ClientLobby.h"

// ClientLobby

IMPLEMENT_DYNCREATE(ClientLobby, ClientBase)

ClientLobby::ClientLobby()
	: ClientBase(IDD_CLIENT_LOBBY)
{
}

ClientLobby::~ClientLobby()
{
}

void ClientLobby::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ClientLobby, CFormView)
END_MESSAGE_MAP()


// ClientLobby �����Դϴ�.

#ifdef _DEBUG
void ClientLobby::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void ClientLobby::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// ClientLobby �޽��� ó�����Դϴ�.
void ClientLobby::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}

void ClientLobby::OnEnter()
{
	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol::CreateCHARACTER_INFO_C2S(fbb);
	fbb.Finish(body);
	
	SEND_PACKET(CHARACTER_INFO_C2S, fbb);
}

void ClientLobby::SetCharacterInfo(const protocol::CHARACTER_INFO_S2C* msg)
{
}