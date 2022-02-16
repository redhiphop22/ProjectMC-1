#pragma once

class IOCPSessionMgr : public s2::S2IOCPSessionMgr
{
public:
	IOCPSessionMgr();
	virtual ~IOCPSessionMgr();

	virtual bool			Create(int32_t sessionCount) override;

};