#pragma once

namespace s2 {

class S2IOCPWorker;

class S2IOCPSession
{
public:
	enum class SESSION_STATE
	{
		NONE,
		WAITING,
		DISCONNECT,
		CONNECT,
		LOGIN,
	};

	enum class SESSION_ASYNCFLAG_TYPE
	{
		ACCEPT = 1,
		SEND = 2,
		RECEIVE	= 3,
	};

	struct OVERLAPPED_EX : OVERLAPPED
	{
		SESSION_ASYNCFLAG_TYPE	m_flags;
	};
	struct OVERLAPPED_ACCEPT : OVERLAPPED_EX
	{
		S2IOCPSession* m_session;
	};

public:
	S2IOCPSession(int32_t idx);
	virtual ~S2IOCPSession();

	virtual bool			Create(HANDLE IOCP, SOCKET listenSocket);
	virtual void			Destroy();

	bool					RegisterAccept();

	bool					IsActive()									{	return SESSION_STATE::DISCONNECT < m_state; }

	bool					SendPacket(packet_protocol_t protocol, uint8_t* packet, int32_t size);

	bool					WaitPacketReceive();

	bool					OnIOCPCompletionAccept();
	bool					OnIOCPCompletionClose();
	bool					OnIOCPCompletionRecv(int32_t size, S2IOCPWorker* worker);
	bool					OnIOCPCompletionSend();
	bool					OnCompletionLogin();

	virtual bool			OnAccept()														{	return false;	};
	virtual bool			OnClose()														{	return false;	};
	virtual bool			OnRecvPacket(int32_t idx, const char* packet, int32_t size)		{	return false;	};
	virtual bool			OnSendPacket(const char* packet, int32_t size);

protected:

	const static uint16_t SEND_BUFFER_MAX = 8096;	// �ֱ� MTU 9000
	const static uint16_t RECV_BUFFER_MAX = 8096;

	int32_t					m_idx = -1;

	HANDLE					m_IOCP = NULL;
	SOCKET					m_listenSocket = INVALID_SOCKET;

	SOCKET					m_socket = INVALID_SOCKET;
	SESSION_STATE			m_state = SESSION_STATE::NONE;

	int32_t					m_recvPackSize = 0;
	char					m_recvBuffer[RECV_BUFFER_MAX];

	int32_t					m_sendPacketSize = 0;
	char					m_sendBuffer[SEND_BUFFER_MAX];

	sockaddr_in				m_localAddr;
	sockaddr_in				m_remoteAddr;

	OVERLAPPED_ACCEPT		m_overLappedAccept;
	OVERLAPPED_EX			m_overLappedRecv;
	OVERLAPPED_EX			m_overLappedSend;

	S2SpinLock				m_spinLock;
};

}