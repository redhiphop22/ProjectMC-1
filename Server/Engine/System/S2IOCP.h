#pragma once

namespace s2 {

class S2IOCP
{
public:
	S2IOCP();
	virtual ~S2IOCP();

	bool					Create(uint32_t ip, uint16_t port, S2IOCPWorkerMgr* workerMgr, S2IOCPSessionMgr* sessionMgr, int32_t backLog = SOMAXCONN);
	void					Destroy();

private:
	HANDLE					m_IOCP = NULL;
	SOCKET					m_listenSocket = INVALID_SOCKET;
};

}