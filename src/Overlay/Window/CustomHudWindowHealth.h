#pragma once
#include "Window.h"

#include "Game/CharInfo.h"

enum PlayerSide_
{
	PlayerSide_OneLeft,
	PlayerSide_TwoRight
};

class CustomHudWindowHealth : public Window
{
public:
	CustomHudWindowHealth(const std::string& windowTitle, bool windowClosable,
		ImGuiWindowFlags windowFlags)
		: Window(windowTitle, windowClosable, windowFlags) {}

	~CustomHudWindowHealth() override = default;
	void SetCharObj(const CharInfo& charObj) { m_pCharObj = &charObj; }
	void SetSide(PlayerSide_ side) { side == PlayerSide_TwoRight ? m_isRightSide = true : m_isRightSide = false; }
	void SetScale(const ImVec2& scale) { m_barSize = ImVec2(635.0f * scale.x, 45.0f * scale.y); }
protected:
	void Draw() override;
private:
	ImVec4 CalculateCurrentHealthBarColor(float currentHealthPercentage) const;
	void DrawHealthBar(const float healthPercentage, const ImVec4& color) const;
	void DrawHealthBarRecoverable() const;
	void DrawHealthBarCurrent() const;
	void DrawCharacterName(const ImVec2& healthBarSize) const;
	const char* GetCharacterName() const;
	void SetCursorPosForCharacterName(const char * charname, const ImVec2& healthBarSize) const;

	const CharInfo* m_pCharObj = nullptr;
	bool m_isRightSide = false;
	ImVec2 m_barSize = { 635.0f , 45.0f};
};
