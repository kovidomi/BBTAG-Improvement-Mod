#pragma once
#include "Overlay/Window/Window.h"

#include "Game/MeterInfo.h"
#include "Game/Player.h"

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
	void SetRightSide(bool isRightSide) { m_isRightSide = isRightSide; }
protected:
	void BeforeDraw() override;
	void Draw() override;
	void AfterDraw() override;
private:
	void  DrawAlignedToLeft(const ImVec4& colorSkillBar, const ImVec4& colorCrossBar) const;
	void  DrawAlignedToRight(const ImVec4& colorSkillBar, const ImVec4& colorCrossBar) const;
	void  CalculateCrossBarBlazeAvailableColor(ImVec4& colorCrossBar) const;
	int	  BlazeValueToSingleDigit() const;
	// To fill up the skill gauge several times like the original HUD does
	int   SkillValueToSingleBar() const;
	float SkillValueToSingleBarPercent() const;
	int   SkillValueToSingleDigit() const;
	float CrossValueToBarPercent() const;
	bool  IsAstralAvailable(Player & opponentPlayer) const;
	bool  SetAstralAvailableColors() const;
	void  DrawCrossBar(float crossPercentage, const ImVec4 & color) const;
	void  DrawSkillBar(float skillPercentage, const ImVec4 & color) const;

	const MeterInfo* m_meterData = nullptr;
	bool m_isRightSide = false;
	ImVec2 m_skillBarSize = { 200.0f , 35.0f };
	ImVec2 m_crossBarSize = { 400.0f , 40.0f };
};
