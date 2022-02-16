#include "pch.h"
#include "NetworkTCP.h"
#include "ControllerBase.h"
#include "MessageProcessor.h"

NetworkTCP::NetworkTCP(MessageProcessor* message)
	: m_messageProcessor(message)
{	
}

NetworkTCP::~NetworkTCP()
{
	DisConnect();
}

bool NetworkTCP::Connect(uint32_t ui32IP, uint16_t ui16Port)
{
	// ���ú� �̺�Ʈ ����
	m_recvEvent = ::WSACreateEvent();

	// ���� ����
	m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == m_socket)
	{
		return false;
	}

	// ���� ����
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family		= AF_INET;
	serverAddr.sin_addr.s_addr	= ui32IP;
	serverAddr.sin_port			= ::htons(ui16Port);

	int32_t result = ::connect(m_socket, (sockaddr*)&serverAddr, sizeof(serverAddr));
	if(SOCKET_ERROR == result)
	{
		uint32_t errorCode = ::GetLastError();
		if(WSAEWOULDBLOCK != errorCode)
		{
			return false;
		}
	}

	// ���ú� ���� ��� �ʱ�ȭ
	m_receivedPacketSize = 0;
	memset(m_receiveBuffer, 0, sizeof(m_receiveBuffer));

	return true;
}

void NetworkTCP::DisConnect()
{
	if (INVALID_HANDLE_VALUE != m_recvEvent)
	{
		::WSACloseEvent(m_recvEvent);
		m_recvEvent = INVALID_HANDLE_VALUE;
	}

	if (INVALID_SOCKET != m_socket)
	{
		::shutdown(m_socket, SD_BOTH);
		::closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}

	m_receivedPacketSize = 0;
}

int32_t NetworkTCP::SelectEvent()
{
	if(INVALID_SOCKET == m_socket)
		return 0;

	int32_t i32Rv = 0; 
	WSANETWORKEVENTS event;
	memset(&event, 0, sizeof(event));

	// ó���� �̺�Ʈ�� Ȯ���Ѵ�.
	if(SOCKET_ERROR != ::WSAEventSelect(m_socket, m_recvEvent, FD_WRITE | FD_READ | FD_CLOSE))
	{
		if(SOCKET_ERROR != ::WSAEnumNetworkEvents(m_socket, m_recvEvent, &event))
		{
			if(event.lNetworkEvents & FD_READ)
			{
				i32Rv = _OnReceive();
			}
			if(event.lNetworkEvents & FD_CLOSE)
			{
				DisConnect();
				i32Rv = TCP_DISCONNECT_TYPE_OTHER;
			}

			return i32Rv;
		}
	}

	return i32Rv;
}

int32_t NetworkTCP::_OnReceive()
{
	// ��Ŷ�� �޴´�.
	int32_t receivedByte = ::recv(m_socket, _GetRecvBuffer(), PACKET_BUFFER_SIZE, 0);

	if (SOCKET_ERROR == receivedByte)
	{
		TCHAR strMessage[ MAX_PATH ];
		int32_t i32Error = ::WSAGetLastError();

		_stprintf_s( strMessage, MAX_PATH, _T("[S2ClientSocket::_OnReceive] lastError = %d"), i32Error );
		TRACE( strMessage );

		DisConnect();
		return TCP_DISCONNECT_TYPE_READ_ERROR;		
	}
	
	if (0 == receivedByte)
	{
		return receivedByte;	// ClosedSocket
	}

	
	// ���� ������ ũ�� ����
	m_receivedPacketSize += receivedByte;

	if (CLIENT_SOCKET_BUFFER_SIZE < m_receivedPacketSize)
	{
		TRACE( _T("[WARNING] S2ClientSocket::_OnReceive() - Buffer overflow(%d bytes)!\n"), m_receivedPacketSize);
		DisConnect();
		return TCP_DISCONNECT_TYPE_READ_ERROR;
	}

	// ���ۻ��� �Ľ� ���� ��ġ 
	int32_t i32StartIdx = 0;

	// ������ ��� ��Ŷ�� �Ľ��Ѵ�.
	for(;;)
	{
		// ��Ŷ �������� ���� �����÷ο� �˻�
		int32_t i32PacketSize;
		i32PacketSize = PacketParsing( m_receiveBuffer+i32StartIdx, m_receivedPacketSize-i32StartIdx );

		if (0 > i32PacketSize || PACKET_BUFFER_SIZE < i32PacketSize)
		{
			TRACE( _T("[WARNING] S2ClientSocket::_OnReceive() - Break packet size(%d bytes)!\n"), i32PacketSize);
			DisConnect();
			return TCP_DISCONNECT_TYPE_READ_ERROR;
		}
		if( 0 == i32PacketSize ) break;

		i32StartIdx += i32PacketSize;
		if( i32StartIdx >= m_receivedPacketSize )
		{
			break;
		}
	}


	// �Ľ� �� ���� �����͸� �������Ѵ�.
	m_receivedPacketSize -= i32StartIdx;
	if (0 < i32StartIdx && 0 < m_receivedPacketSize)
	{
		memmove(m_receiveBuffer, m_receiveBuffer + i32StartIdx, m_receivedPacketSize);
	}

	return receivedByte;
}

char *	NetworkTCP::_GetRecvBuffer()
{
	return m_receiveBuffer + m_receivedPacketSize;
}


int32_t NetworkTCP::SendPacket(protocol::MESSAGE message, flatbuffers::FlatBufferBuilder& fbb)
{
	packet_size_t size = static_cast<packet_size_t>(fbb.GetSize()) + PACKET_HEAD_SIZE;
	packet_protocol_t protocol = static_cast<packet_protocol_t>(message);
	packet_seed_t seed = 0;

	memcpy(m_sendBuffer,		&size,					sizeof(packet_size_t));
	memcpy(&m_sendBuffer[2],	&protocol,				sizeof(packet_protocol_t));
	memcpy(&m_sendBuffer[4],	&seed,					sizeof(packet_seed_t));
	memcpy(&m_sendBuffer[6],	fbb.GetBufferPointer(),	fbb.GetSize());

	return _SendPacketMessage(m_sendBuffer, size);
}

int32_t NetworkTCP::SendPacket(char* pBuffer, int32_t i32Size)
{
	return _SendPacketMessage(pBuffer, i32Size);
}

int32_t NetworkTCP::_SendPacketMessage(const char* pBuffer, int32_t i32Size)
{
	// ������ �������� ������ �ʴ´�.
	if (INVALID_SOCKET == m_socket)	return 0;

	// ��Ŷ�� �� ���������� �ݺ��Ѵ�.
	int32_t i32SendIdx = 0;
	while( i32Size > i32SendIdx)
	{
		int32_t sendedByte = ::send(m_socket, pBuffer + i32SendIdx, i32Size - i32SendIdx, 0);

		// ���� ���� 
		if (SOCKET_ERROR == sendedByte)
		{
			int32_t i32Error = ::WSAGetLastError();
			
			return -1;
		}

		// ������ ��������.
		if (0 == sendedByte)
		{
			return 0;
		}

		// ���� ����Ʈ��ŭ �ε��� ����
		i32SendIdx += sendedByte;
	}

	return i32SendIdx;
}

int32_t NetworkTCP::PacketParsing(char* buffer, int32_t bufferSize)
{
	// ��� ����� ���ڶ�
	if(bufferSize < PACKET_HEAD_SIZE)
		return 0;

	packet_size_t size = *((packet_size_t*)buffer);

	// ���� ��Ŷ�� �� �޾���
	if(bufferSize < size)
		return 0;

	packet_protocol_t protocol = *((packet_protocol_t*)(buffer+sizeof(packet_size_t)));
	m_messageProcessor->PushData((User*)this, protocol, buffer+PACKET_HEAD_SIZE, size-PACKET_HEAD_SIZE);

	//packet_protocol_t protocol = *((packet_protocol_t*)(buffer+sizeof(packet_size_t)));
	//CONTROLLER.PacketParsing((User*)this, protocol, buffer+PACKET_HEAD_SIZE, size-PACKET_HEAD_SIZE);

	//S2MOPacketBase PacketBase( PROTOCOL_VALUE_NONE );
	//PacketBase.UnPacking_Head( pBuffer );

	//T_PACKET_SIZE TSize = PacketBase.GetPacketSize();
	//if( TSize > i32Size )		return 0;
	//	
	//char* pContents = pBuffer + S2MO_PACKET_HEAD_SIZE;

	//switch( PacketBase.GetProtocol() )
	//{
	//case PROTOCOL_BASE_CONNECT_ACK:
	//	{
	//		m_pApp->ConnectServerAck( pContents );
	//	}
	//	break;
	//case PROTOCOL_BASE_NOTICE_ACK:
	//	{
	//		m_pApp->NoticeAck( pContents );
	//	}
	//	break;
	//case PROTOCOL_LOGIN_INPUT_ACK:
	//	{
	//		m_pApp->LoginAck( pContents );
	//	}
	//	break;
	//case PROTOCOL_USER_INFO_LOAD_ACK:
	//	{
	//		m_pApp->UserInfoLoadAck( pContents );
	//	}
	//	break;
	//case PROTOCOL_USER_CHANGE_NICK_ACK:
	//	{
	//		m_pApp->UserChangeNickAck( pContents );
	//	}
	//	break;
	//case PROTOCOL_USER_SHOP_LIST_ACK:
	//	{
	//		m_pApp->UserShopListAck( pContents );
	//	}
	//	break;
	//case PROTOCOL_ROOM_CREATE_ACK:
	//	{
	//		m_pApp->RoomCreateAck( pContents );
	//	}
	//	break;
	//case PROTOCOL_ROOM_ENTER_ACK:
	//	{
	//		m_pApp->RoomEnterAck( pContents );
	//	}
	//	break;
	//case PROTOCOL_ROOM_SLOT_INFO_ACK:
	//	{
	//		m_pApp->RoomSlotInfoAck( pContents );
	//	}
	//	break;
	//case PROTOCOL_ROOM_LEAVE_ACK:
	//	{
	//		m_pApp->RoomLeaveAck( pContents );
	//	}
	//	break;
	//case PROTOCOL_ROOM_ENTER_TRANS_ACK:
	//	{
	//		m_pApp->RoomEnterTransAck( pContents );
	//	}
	//	break;
	//case PROTOCOL_ROOM_LEAVE_TRANS_ACK:
	//	{
	//		m_pApp->RoomLeaveTransAck( pContents );
	//	}
	//	break;
	//case PROTOCOL_ROOM_SLOT_READY_ACK:
	//	break;
	//case PROTOCOL_ROOM_STATE_CHANGE_ACK:
	//	{
	//		m_pApp->RoomStateChangeAck( pContents );
	//	}
	//	break;
	////case PROTOCOL_ROOM_MAP_VOTE_TRANS_ACK:
	////	{
	////		m_pApp->RoomMapVoteAck( pContents );
	////	}
	////	break;
	//case PROTOCOL_ROOM_SLOT_STATE_CHANGE_ACK:
	//	{
	//		m_pApp->RoomSlotStateChangeAck( pContents );
	//	}
	//	break;
	//case PROTOCOL_ROOM_BATTLE_ENTER_ACK:
	//	{
	//		m_pApp->RoomBattleEnterAck( pContents );
	//	}
	//	break;
	//case PROTOCOL_ROOM_BATTLE_READY_ACK:
	//	{
	//	}
	//	break;
	//case PROTOCOL_ROOM_BATTLE_START_ACK:
	//	{
	//		m_pApp->RoomBattleStartAck( pContents );
	//	}
	//	break;
	//case PROTOCOL_ROOM_BATTLE_END_ACK:
	//	{
	//		m_pApp->RoomBattleEndAck( pContents );
	//	}
	//	break;
	//case PROTOCOL_ROOM_LIST_ACK:
	//	{
	//		m_pApp->RoomList( pContents );
	//	}
	//	break;
	//}

	return size;
}