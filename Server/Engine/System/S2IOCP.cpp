#include "S2Engine.h"

namespace s2 {

S2IOCP::S2IOCP()
{
}

S2IOCP::~S2IOCP()
{
}

bool S2IOCP::Create(uint32_t ip, uint16_t port, S2IOCPWorkerMgr* workerMgr, S2IOCPSessionMgr* sessionMgr, int32_t backLog)
{
	// �Ϸ����� ó���ϴ� ��ü(CP : Completion Port) ����
	m_IOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if(NULL == m_IOCP)
	{
		return false;
	}

	// 1. ���ϻ���  
	m_listenSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );//WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(SOCKET_ERROR == m_listenSocket)
	{
		return false;
	}

	// �������� ��ü����
	sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(sockaddr_in));
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.S_un.S_addr = htonl(ip);

	// 2. ���ϼ���
	if(SOCKET_ERROR == bind(m_listenSocket, (struct sockaddr*)&serverAddr, sizeof(sockaddr_in)))
	{
		return false;
	}

	//BOOL on = TRUE;
	//if(SOCKET_ERROR == setsockopt(m_listenSocket, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, (char*)&on, sizeof(on)))
	//{
	//	return false;
	//}

	// 3. ���Ŵ�⿭����
	if(SOCKET_ERROR == listen(m_listenSocket, backLog))
	{
		return false;
	}

	CreateIoCompletionPort((HANDLE)m_listenSocket, m_IOCP, 0, 0);

	// Worker Init
	if(false == workerMgr->Init(m_IOCP))
	{
		return false;
	}

	// Session IOCP Accept
	if(false == sessionMgr->Init(m_IOCP, m_listenSocket))
	{
		return false;
	}	
    
    return true;
}

void S2IOCP::Destroy()
{
    if(INVALID_SOCKET != m_listenSocket)
    {
        closesocket(m_listenSocket);
		m_listenSocket = INVALID_SOCKET;
    }
}

}