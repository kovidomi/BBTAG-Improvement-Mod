#pragma once
#include "Overlay/Window/Window.h"

#include "Game/CharInfo.h"

class HealthWindow : public Window
{
public:
	HealthWindow(const std::string& windowTitle, bool windowClosable,
		ImGuiWindowFlags windowFlags)
		: Window(windowTitle, windowClosable, windowFlags) {}

	~HealthWindow() override = default;
	void SetCharObj(const CharInfo& charObj) { m_pCharObj = &charObj; }
	void SetScale(const ImVec2& scale) { m_healthBarSize = ImVec2(635.0f * scale.x, 45.0f * scale.y); }
	void SetRightSide(bool isRightSide) { m_isRightSide = isRightSide; }
protected:
	void Draw() override;
private:
	ImVec4 CalculateCurrentHealthBarColor(float currentHealthPercentage) const;
	void DrawHealthBar(float healthPercentage, const ImVec4& color) const;
	void DrawHealthBarRecoverable() const;
	void DrawHealthBarCurrent() const;
	void DrawCharacterName(const ImVec2& healthBarSize) const;
	const char* GetCharacterName() const;
	void SetCursorPosForCharacterName(const char * charname, const ImVec2& healthBarSize) const;

	const CharInfo* m_pCharObj = nullptr;
	bool m_isRightSide = false;
	ImVec2 m_healthBarSize = { 635.0f , 45.0f};
};
