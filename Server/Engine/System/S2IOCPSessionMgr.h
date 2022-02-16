#pragma once

namespace s2 {

class S2IOCPSessionMgr
{
protected:
	using session_list_t = std::unordered_map<int32_t, S2IOCPSession*>;

public:
	S2IOCPSessionMgr();
	virtual ~S2IOCPSessionMgr();

	virtual bool			Create(int32_t sessionCount);
	void					Destroy();

	bool					Init(HANDLE IOCP, SOCKET listenSocket);

protected:
	session_list_t			m_sessionList;
};

}