#include "S2Engine.h"
#include "S2Time.h"
#include <mmsystem.h>

#define SERVER_BEGIN_TIME	10000

S2ThreadTimer* s_pTimer = nullptr;

S2ThreadTimer::S2ThreadTimer()
{
}

S2ThreadTimer::~S2ThreadTimer()
{
	Destroy();
}

bool S2ThreadTimer::Create()
{
	return ThreadCreate();
}

void S2ThreadTimer::Destroy()
{
	S2Thread::Destroy();
}

bool S2ThreadTimer::OnThreadUpdate()
{
	SYSTEMTIME stSystemTime;

	while(IsThreadRunning())
	{
		// ���Ÿ�� ����޴� ���� ����Ͻú��� ������Ʈ		
		//::GetLocalTime(&stSystemTime);
		//m_dt32Local = stSystemTime;

		// ���Ÿ�� ������� ǥ�� ����Ͻú��� ������Ʈ
		//SYSTEMTIME UTCTime;
		//TIME_ZONE_INFORMATION TZInfo;
		//::GetSystemTime(&UTCTime);
		//::GetTimeZoneInformation(&TZInfo);
		//TZInfo.DaylightBias = 0;			// Ÿ���� �������� ���Ÿ���� �������� �ʵ��� ������ ����
		//::SystemTimeToTzSpecificLocalTime(&TZInfo, &UTCTime, &stSystemTime);
		//m_dt32Stand = stSystemTime;

		//// 4. ����
		//Waiting(1);
	}
	return false;
}

bool S2Time::Init()
{
	s_pTimer = new S2ThreadTimer;
	return s_pTimer->Create();
}

void S2Time::Destroy()
{
	SAFE_DELETE(s_pTimer);
}

date32_t S2Time::LocalTime()
{
	return s_pTimer->GetLocalTime();
}

date32_t S2Time::StandTime()
{
	return 0;//s_pTimer->GetStandTime();
}
