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
	CharHandle& Char1();
	CharHandle& Char2();
	const MeterInfo* Meters() const;

	void SetMeterPtr(const void* addr);
	bool IsNullPtrMeters() const;
};