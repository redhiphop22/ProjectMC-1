// ClientSelector.cpp : ���� �����Դϴ�.
//

#include "pch.h"
#include "ClientSelector.h"

// ClientSelector

IMPLEMENT_DYNCREATE(ClientSelector, ClientBase)

ClientSelector::ClientSelector()
	: ClientBase(IDD_SELECTOR)
{
}

ClientSelector::~ClientSelector()
{
}

void ClientSelector::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ClientSelector, CFormView)
	ON_BN_CLICKED(IDC_SELECTOR_SINGLE, &ClientSelector::OnBnClickedSelectorSingle)
	ON_BN_CLICKED(IDC_SELECTOR_MULTI, &ClientSelector::OnBnClickedSelectorMulti)
END_MESSAGE_MAP()


// ClientSelector �����Դϴ�.

#ifdef _DEBUG
void ClientSelector::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void ClientSelector::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// ClientSelector �޽��� ó�����Դϴ�.
void ClientSelector::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}

void ClientSelector::OnBnClickedSelectorSingle()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	GetApp()->InitClient(COLTROLLER_TYPE::SINGLE);
}


void ClientSelector::OnBnClickedSelectorMulti()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	GetApp()->InitClient(COLTROLLER_TYPE::MULTI);
}
