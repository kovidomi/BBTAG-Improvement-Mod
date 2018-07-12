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
	const CharHandle& Char1() const;
	const CharHandle& Char2() const;
	const MeterInfo* Meters() const;

	void SetMeterPtr(const void* addr);
};