// ClientConnect.cpp : ���� �����Դϴ�.
//

#include "pch.h"
#include "ClientConnect.h"
// ClientConnect

IMPLEMENT_DYNCREATE(ClientConnect, ClientBase)

ClientConnect::ClientConnect()
	: ClientBase(IDD_CLIENT_CONNECT)
	, m_port(36101)
{

}

ClientConnect::~ClientConnect()
{
}

void ClientConnect::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONNECT_IP, m_ip);
	DDX_Text(pDX, IDC_CONNECT_PORT, m_port);
	DDV_MinMaxUInt(pDX, m_port, 0, 65535);
}

BEGIN_MESSAGE_MAP(ClientConnect, CFormView)
	ON_BN_CLICKED(IDC_CONNECT_CONNECT, &ClientConnect::OnBnClickedConnectConnect)
END_MESSAGE_MAP()


// ClientConnect �����Դϴ�.

#ifdef _DEBUG
void ClientConnect::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void ClientConnect::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// ClientConnect �޽��� ó�����Դϴ�.
void ClientConnect::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	m_ip.SetAddress(127, 0, 0, 1);
}

void ClientConnect::OnBnClickedConnectConnect()
{
	UpdateData(TRUE);

	BYTE bIP[ 4 ] ;
	m_ip.GetAddress( bIP[0], bIP[1], bIP[2], bIP[3] );
	UINT32 ui32IP = bIP[0] | (bIP[1] << 8) | (bIP[2] << 16) | (bIP[3] << 24);

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CClientApp::user_list_t userList;
	GetApp()->CreateUser(1, false, ui32IP, m_port, 1, 0, "", userList);

	CONTROLLER_S.CONNECT_RESULT_C2S(userList[0], nullptr, 0);
}