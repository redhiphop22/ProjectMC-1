#include "S2Engine.h"

namespace s2 {

S2IOCPWorker::S2IOCPWorker(int32_t idx)
	: m_idx(idx)
{
}

S2IOCPWorker::~S2IOCPWorker()
{
}

bool S2IOCPWorker::Create()
{
	return ThreadCreate();
}

void S2IOCPWorker::Destroy()
{
	S2Thread::Destroy();
}

bool S2IOCPWorker::OnThreadUpdate()
{
	bool working = false;

	DWORD			transferSize = 0;
	ULONG_PTR		completionKey = NULL;
	LPOVERLAPPED	overLapped = nullptr;

	//Check IOPort 
	bool result = ::GetQueuedCompletionStatus(m_IOCP, &transferSize, &completionKey, &overLapped, 1);
		
	if(false == result || NULL == overLapped)
	{
		if(NULL != completionKey)
		{
			//이녀석은 삭제처리 합니다. 
			working = true;

			S2IOCPSession* session = reinterpret_cast<S2IOCPSession*>(completionKey);
			if(nullptr != session && session->IsActive())
			{
				session->OnIOCPCompletionClose();
			}
		}
	}
	else
	{
		S2IOCPSession::OVERLAPPED_EX* overLappedEx = static_cast<S2IOCPSession::OVERLAPPED_EX*>(overLapped);
		if(S2IOCPSession::SESSION_ASYNCFLAG_TYPE::RECEIVE == overLappedEx->m_flags )
		{
			S2IOCPSession* session = reinterpret_cast<S2IOCPSession*>(completionKey);
			if(nullptr != session && session->IsActive())
			{
				if(0 == transferSize)
				{
					session->OnIOCPCompletionClose();
				}
				else
				{
					session->OnIOCPCompletionRecv(transferSize, this);
				}
			}
		}
		else if(S2IOCPSession::SESSION_ASYNCFLAG_TYPE::SEND == overLappedEx->m_flags )
		{
			//처리 
			S2IOCPSession* session = reinterpret_cast<S2IOCPSession*>(completionKey);
			if(nullptr != session && session->IsActive())
			{
				session->OnIOCPCompletionSend();
			}
		}
		else if(S2IOCPSession::SESSION_ASYNCFLAG_TYPE::ACCEPT == overLappedEx->m_flags )
		{
			S2IOCPSession::OVERLAPPED_ACCEPT* overLappedAccept = static_cast<S2IOCPSession::OVERLAPPED_ACCEPT*>(overLapped);
			overLappedAccept->m_session->OnIOCPCompletionAccept();
		}
	}

	return working;
}

}