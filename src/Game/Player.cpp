#include "Player.h"

Player::Player() //: m_char1(CharHandle()), m_char2(CharHandle())
{
}

CharHandle & Player::GetChar1()
{
	return m_char1;
}

CharHandle & Player::GetChar2()
{
	return m_char2;
}

const MeterInfo * Player::GetMeters() const
{
	return m_meters;
}

void Player::SetMeterPtr(const void * addr)
{
	m_meters = (MeterInfo*)addr;
}

bool Player::IsNullPtrMeters() const
{
	return m_meters == 0;
}
