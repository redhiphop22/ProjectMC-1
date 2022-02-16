#include "S2Engine.h"

namespace s2 {

S2IOCPSession::S2IOCPSession(int32_t idx)
	: m_idx(idx)
{
}

S2IOCPSession::~S2IOCPSession()
{
}

bool S2IOCPSession::Create(HANDLE IOCP, SOCKET listenSocket)
{
	m_IOCP = IOCP;
	m_listenSocket = listenSocket;

	return RegisterAccept();
}

void S2IOCPSession::Destroy()
{
	::closesocket(m_socket);
}

bool S2IOCPSession::RegisterAccept()
{
	m_recvPackSize = 0;
	m_sendPacketSize = 0;

	memset(&m_overLappedAccept, 0, sizeof(m_overLappedAccept));

	m_overLappedAccept.m_flags = S2IOCPSession::SESSION_ASYNCFLAG_TYPE::ACCEPT;
	m_overLappedAccept.m_session = this;
		
	//m_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	m_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if(INVALID_SOCKET == m_socket)
	{
		return false;
	}

	int32_t zero = 0;
	setsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, (char*)&zero, sizeof(zero));
    setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, (char*)&zero, sizeof(zero));

	if(false == AcceptEx(m_listenSocket, 
		m_socket, 
		m_recvBuffer,
		0, 
		sizeof(sockaddr_in) + 16,
		sizeof(sockaddr_in) + 16,
		(LPDWORD)(&m_recvPackSize),
		&m_overLappedAccept))
	{
		int32_t error = WSAGetLastError();
		if(WSA_IO_PENDING != error)
		{
			return false;
		}
	}

	m_state = SESSION_STATE::WAITING;

	return true;
}

bool S2IOCPSession::OnIOCPCompletionAccept()
{
	sockaddr_in *plocal = 0, *premote = 0;
	int locallen, remotelen;

	GetAcceptExSockaddrs(
		m_recvBuffer,
		0,
		sizeof(sockaddr_in) + 16,
		sizeof(sockaddr_in) + 16,
		(sockaddr **)&plocal, 
		&locallen,
		(sockaddr **)&premote, 
		&remotelen );

	memcpy(&m_localAddr, plocal, sizeof(sockaddr_in));
	memcpy(&m_remoteAddr, premote, sizeof(sockaddr_in));

	if(SOCKET_ERROR == setsockopt(m_socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (const char *)&m_listenSocket, sizeof(SOCKET)))
	{
		return false;
	}
 
	m_state = SESSION_STATE::CONNECT;
		
	// IOCP를 연결해준다.
	CreateIoCompletionPort((HANDLE)m_socket, m_IOCP, (ULONG_PTR)this, 0);

	m_recvPackSize = 0;
	if(false == WaitPacketReceive())
	{
		return false;
	}

	return OnAccept();
}

bool S2IOCPSession::OnIOCPCompletionClose()
{
	OnClose();

	if(S2IOCPSession::SESSION_STATE::LOGIN == m_state)
	{
		m_state = S2IOCPSession::SESSION_STATE::DISCONNECT;
	}
	else
	{
		RegisterAccept();
	}

	return true;
}

bool S2IOCPSession::OnIOCPCompletionRecv(int32_t size, S2IOCPWorker* worker)
{
	S2Scoped_SpinLock spinLock(m_spinLock);

	int32_t recvTotalSize = m_recvPackSize + size;
	int32_t readSize = 0;
	packet_size_t packetSize;

	auto checkPacket = [this, &packetSize, &recvTotalSize, &readSize]() -> bool {
		if(recvTotalSize < PACKET_HEAD_SIZE)
			return false;

		packetSize = *((packet_size_t*)&m_recvBuffer[readSize]);
		if(recvTotalSize < packetSize)
			return false;

		return true;
	};

	while(checkPacket())
	{
		OnRecvPacket(worker->GetIdx(), &m_recvBuffer[readSize], packetSize);

		recvTotalSize -= packetSize;
		readSize += packetSize;
	}
	
	if(0 < readSize && 0 < recvTotalSize)
	{
		memmove(m_recvBuffer, &m_recvBuffer[readSize], recvTotalSize);
	}
	m_recvPackSize = recvTotalSize;

	return WaitPacketReceive();
}

bool S2IOCPSession::OnIOCPCompletionSend()
{
	return true;
}

bool S2IOCPSession::OnCompletionLogin()
{
	m_state = SESSION_STATE::LOGIN;
	return true;
}

bool S2IOCPSession::OnSendPacket(const char* packet, int32_t size)
{
	std::memset(&m_overLappedSend, 0, sizeof(OVERLAPPED_EX));
	m_overLappedSend.m_flags = SESSION_ASYNCFLAG_TYPE::SEND;

	WSABUF buf;
	buf.len = size;
	buf.buf = (CHAR*)packet;

	DWORD sendBytes = 0;
	DWORD flags = 0;
	if(SOCKET_ERROR == WSASend(m_socket, &buf, 1, &sendBytes, flags, &m_overLappedSend, NULL))
	{
		DWORD lastError = ::GetLastError();
		switch( lastError )
		{
		case ERROR_IO_PENDING:
		case ERROR_SUCCESS:
			break;
		default:
			S2TRACE(  _T("[S2NetSession::SendPacketMessage] Packet Send Error : %d\n"), lastError);	
			return false;
		}
	}
	return true;
}

bool S2IOCPSession::WaitPacketReceive()
{
	std::memset(&m_overLappedRecv, 0, sizeof(OVERLAPPED_EX));
	m_overLappedRecv.m_flags = SESSION_ASYNCFLAG_TYPE::RECEIVE;
	
	WSABUF buf;
	buf.len = RECV_BUFFER_MAX-m_recvPackSize;
	buf.buf = (CHAR*)&m_recvBuffer[m_recvPackSize];

	DWORD recvBytes = 0;
	DWORD flags = 0;
	if(SOCKET_ERROR == WSARecv(m_socket, &buf, 1, &recvBytes, &flags, &m_overLappedRecv, NULL))
	{
		DWORD lastError = ::GetLastError();
		switch(lastError)
		{
		case ERROR_IO_PENDING:
			break;
		default:
			S2LOG_ERROR("Pcket Receive Error : %d", lastError);
			return false;
		}
	}
	return true;
}

bool S2IOCPSession::SendPacket(packet_protocol_t protocol, uint8_t* packet, int32_t size)
{
	packet_size_t* packetSize = (packet_size_t*)&m_sendBuffer[0];
	packet_protocol_t* packetProtocol = (packet_protocol_t*)&m_sendBuffer[2];
	packet_seed_t* packetSeed = (packet_seed_t*)&m_sendBuffer[4];

	*packetSize = size + PACKET_HEAD_SIZE;
	*packetProtocol = protocol;
	*packetSeed = 0;

	memcpy(&m_sendBuffer[6], packet, size);
	
	return OnSendPacket(m_sendBuffer, *packetSize);
}

}