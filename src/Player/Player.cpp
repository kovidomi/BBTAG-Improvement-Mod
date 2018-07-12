#include "Player.h"

Player::Player() //: m_char1(CharHandle()), m_char2(CharHandle())
{
}

const CharHandle & Player::Char1() const
{
	return m_char1;
}

const CharHandle & Player::Char2() const
{
	return m_char2;
}

const MeterInfo * Player::Meters() const
{
	return m_meters;
}

void Player::SetMeterPtr(const void * addr)
{
	m_meters = (MeterInfo*)addr;
}
