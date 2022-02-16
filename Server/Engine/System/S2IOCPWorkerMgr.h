#pragma once

namespace s2 {

class S2IOCPWorkerMgr
{
protected:
	using worker_list_t = std::vector<S2IOCPWorker*>;

public:
	S2IOCPWorkerMgr();
	virtual ~S2IOCPWorkerMgr();

	virtual bool			Create(int32_t workerCount);
	void					Destroy();

	bool					Init(HANDLE IOCP);

	worker_list_t&			GetWorkerList() {	return m_workerList;	}

protected:
	worker_list_t			m_workerList;
};

}