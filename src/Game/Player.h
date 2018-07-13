#pragma once
#include "CharHandle.h"
#include "MeterInfo.h"

class Player
{
	CharHandle m_char1;
	CharHandle m_char2;
	MeterInfo* m_meters;

public:
	Player();
	CharHandle& GetChar1();
	CharHandle& GetChar2();
	const MeterInfo* GetMeters() const;

	void SetMeterPtr(const void* addr);
	bool IsMetersNullPtr() const;
};