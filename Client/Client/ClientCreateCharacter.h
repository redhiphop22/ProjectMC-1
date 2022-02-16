#pragma once

#include "ClientBase.h"

// ClientCreateCharacter �� ���Դϴ�.

class ClientCreateCharacter : public ClientBase
{
	DECLARE_DYNCREATE(ClientCreateCharacter)

public:
	ClientCreateCharacter();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~ClientCreateCharacter();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_CREATE_CHARACTER };
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

private:
	CString m_nick;

public:
	virtual void			OnInitialUpdate();

	afx_msg void			OnBnClickedCreateCharacterCreate();
	afx_msg void			OnBnClickedCreateCharacterNickDuplication();

	void					NameDuplicationResult(const protocol::CHARACTER_NAME_DUPLICATION_S2C* msg);
	

};


