#pragma once

class S2ThreadTimer : public s2::S2Thread
{
	date32_t m_dt32Local;			// ���Ÿ�� ����Ǵ� ���ý�(��ȯ��)
	date32_t m_dt32Stand;			// ���Ÿ�� �����ϴ� ǥ�ؽ�(��ȯ��)

public:
	S2ThreadTimer();
	~S2ThreadTimer();

	virtual bool Create();
	virtual void Destroy();

	date32_t GetLocalTime() { return m_dt32Local; }
	date32_t GetStandTime() { return m_dt32Stand; }

protected:
	virtual bool OnThreadUpdate() override;
};

extern S2ThreadTimer* s_pTimer;

namespace S2Time
{
	bool				Init();
	void				Destroy();
	inline date32_t	LocalTime();
	inline date32_t	StandTime();
};
