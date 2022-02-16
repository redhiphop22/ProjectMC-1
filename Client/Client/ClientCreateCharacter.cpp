// ClientCreateCharacter.cpp : ���� �����Դϴ�.
//

#include "pch.h"
#include "ClientCreateCharacter.h"
// ClientCreateCharacter

IMPLEMENT_DYNCREATE(ClientCreateCharacter, ClientBase)

ClientCreateCharacter::ClientCreateCharacter()
	: ClientBase(IDD_CLIENT_CREATE_CHARACTER)
	, m_nick(_T(""))
{
}

ClientCreateCharacter::~ClientCreateCharacter()
{
}

void ClientCreateCharacter::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CREATE_CHARACTER_NICK, m_nick);
}

BEGIN_MESSAGE_MAP(ClientCreateCharacter, CFormView)
	ON_BN_CLICKED(IDC_CREATE_CHARACTER_CREATE, &ClientCreateCharacter::OnBnClickedCreateCharacterCreate)
	ON_BN_CLICKED(IDC_CREATE_CHARACTER_NICK_DUPLICATION, &ClientCreateCharacter::OnBnClickedCreateCharacterNickDuplication)
END_MESSAGE_MAP()


// ClientCreateCharacter �����Դϴ�.

#ifdef _DEBUG
void ClientCreateCharacter::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void ClientCreateCharacter::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// ClientCreateCharacter �޽��� ó�����Դϴ�.
void ClientCreateCharacter::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}

void ClientCreateCharacter::OnBnClickedCreateCharacterCreate()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	auto user = GetUser();
	user->m_nick = m_nick.GetBuffer();

	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	GetUser()->MakeCHARACTER_CREATE_C2S(fbb);
	SEND_PACKET(CHARACTER_CREATE_C2S, fbb);
}


void ClientCreateCharacter::OnBnClickedCreateCharacterNickDuplication()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol::CreateCHARACTER_NAME_DUPLICATION_C2S(fbb,
		fbb.CreateString(m_nick.GetBuffer())
	);
	fbb.Finish(body);
	
	SEND_PACKET(CHARACTER_NAME_DUPLICATION_C2S, fbb);
}

void ClientCreateCharacter::NameDuplicationResult(const protocol::CHARACTER_NAME_DUPLICATION_S2C* msg)
{
	auto result = msg->result();
}