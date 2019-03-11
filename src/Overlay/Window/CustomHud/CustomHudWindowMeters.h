#pragma once
#include "Overlay/Window/Window.h"

#include "Game/MeterInfo.h"

class CustomHudWindowMeters : public Window
{
public:
	CustomHudWindowMeters(const std::string& windowTitle, bool windowClosable,
		ImGuiWindowFlags windowFlags)
		: Window(windowTitle, windowClosable, windowFlags) {}

	~CustomHudWindowMeters() override = default;
	void SetMeterData(const MeterInfo& meterData) { m_meterData = &meterData; }
	void SetScale(const ImVec2& scale)
	{
		m_skillBarSize = ImVec2(200.0f * scale.x, 35.0f * scale.y);
		m_crossBarSize = ImVec2(400.0f * scale.x, 40.0f * scale.y);
	}
protected:
	void BeforeDraw() override;
	void Draw() override;
	void AfterDraw() override;
private:
	void CalculateCrossBarBlazeAvailableColor(ImVec4& colorCrossBar) const;
	void DrawCrossBar(const float crossPercentage, const ImVec4 & color) const;
	void DrawSkillBar(const float skillPercentage, const ImVec4 & color) const;

	const MeterInfo* m_meterData = nullptr;
	bool m_isRightSide = false;
	ImVec2 m_skillBarSize = { 200.0f , 35.0f };
	ImVec2 m_crossBarSize = { 400.0f , 40.0f };
};
