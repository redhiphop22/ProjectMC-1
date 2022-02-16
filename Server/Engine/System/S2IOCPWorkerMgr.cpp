#include "S2Engine.h"
#include "S2IOCPWorkerMgr.h"

namespace s2 {

S2IOCPWorkerMgr::S2IOCPWorkerMgr()
{
}

S2IOCPWorkerMgr::~S2IOCPWorkerMgr()
{
}

bool S2IOCPWorkerMgr::Create(int32_t workerCount)
{
	for(int32_t i = 0 ; i < workerCount ; ++i)
	{
		S2IOCPWorker* worker = new S2IOCPWorker(i);
		if(nullptr == worker)
		{
			return false;
		}
		if(false == worker->Create())
		{
			return false;
		}
		m_workerList.push_back(worker);;
	}
	return true;
}

void S2IOCPWorkerMgr::Destroy()
{
	for(auto& iter : m_workerList)
	{
		iter->Destroy();
		SAFE_DELETE(iter);
	}
}

bool S2IOCPWorkerMgr::Init(HANDLE IOCP)
{
	for(auto& iter : m_workerList)
	{
		iter->SetIOCP(IOCP);
	}
	return true;
}

}