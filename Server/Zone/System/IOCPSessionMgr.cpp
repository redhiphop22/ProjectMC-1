#include "Zone.h"
#include "IOCPSessionMgr.h"
#include "IOCPSession.h"

IOCPSessionMgr::IOCPSessionMgr()
{
}

IOCPSessionMgr::~IOCPSessionMgr()
{
}

bool IOCPSessionMgr::Create(int32_t sessionCount)
{
	for(int32_t i = 0 ; i < sessionCount ; ++i)
	{
		IOCPSession* session = new IOCPSession(i);
		if(nullptr == session)
		{
			return false;
		}
		auto result = m_sessionList.insert(std::make_pair(i, std::move(session)));
		if(false == result.second)
		{
			return false;
		}
	}

	return true;
}
