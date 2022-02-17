#include "Zone.h"
#include "IOCPSession.h"
//#include "UserProcessor.h"

IOCPSession::IOCPSession(int32_t idx)
	: s2::S2IOCPSession(idx)
{
}

IOCPSession::~IOCPSession()
{
}

void IOCPSession::CompletionLogin(uid_t UID)
{
	m_UID = UID;
	OnCompletionLogin();
}

bool IOCPSession::OnAccept()
{
	flatbuffers::FlatBufferBuilder fbb(FBB_BASIC_SIZE);
	auto body = protocol::CreateCONNECT_RESULT_S2C(fbb, common::RESULT_CODE_SUCESS);
	fbb.Finish(body);
	SendPacket(protocol::MESSAGE_CONNECT_RESULT_S2C, fbb);

	return true;
}

bool IOCPSession::OnClose()
{
	S2IOCPSession::Destroy();

	return true;
}

bool IOCPSession::OnRecvPacket(int32_t idx, const char* packet, int32_t size)
{
	return MESSAGE_PROCESSOR().SnedPacket_User(static_cast<int32_t>(MessageProcessor::MESSAGE_GROUP_USER::NETWORK), idx, this, packet, size);
}

bool IOCPSession::SendPacket(protocol::MESSAGE messageID, flatbuffers::FlatBufferBuilder& fbb)
{
	return S2IOCPSession::SendPacket(static_cast<s2::packet_protocol_t>(messageID), fbb.GetBufferPointer(), fbb.GetSize());
}