#pragma once
#include "Overlay/Window/Window.h"

#include "Game/CharInfo.h"

class UniqueMetersWindow : public Window
{
public:
	UniqueMetersWindow(const std::string& windowTitle, bool windowClosable,
		ImGuiWindowFlags windowFlags)
		: Window(windowTitle, windowClosable, windowFlags) {}
	~UniqueMetersWindow() override = default;
	void SetCharObj(const CharInfo& charObj) { m_pCharObj = &charObj; }
	void SetScale(const ImVec2& scale) { m_uniqueBarSize = ImVec2(140.0f * scale.x, 20.0f * scale.y); }
	void SetRightSide(bool isRightSide) { m_isRightSide = isRightSide; }
	bool CharacterHasUniqueMeterToDraw() const;
protected:
	void Draw() override;
private:
	float GetUniqueMeterBarPercentage() const;
	std::string GetUniqueMeterNumberString() const;
	void DrawAlignedToLeft() const;
	void DrawAlignedToRight() const;
	bool ShouldShowUniqueMeterNumber() const;
	bool ShouldShowUniqueMeterBar() const;
	ImVec4 GetBarColor() const;

	const CharInfo* m_pCharObj = nullptr;
	bool m_isRightSide = false;
	ImVec2 m_uniqueBarSize = { 140.0f , 20.0f };
};
