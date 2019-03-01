#pragma once
#include <imgui.h>

typedef unsigned int uint32_t;

class HitboxOverlay
{
public:
	HitboxOverlay();
	void Update();
	void SetChar1ScreenPos(float x, float y);

private:
	void BeforeDraw();
	void Draw();
	void AfterDraw();

	void RenderLine(const ImVec2& from, const ImVec2& to, uint32_t color, float thickness = 1.0f);
	void RenderCircle(const ImVec2& position, float radius, uint32_t color, float thickness = 1.0f, uint32_t segments = 16);
	void RenderCircleFilled(const ImVec2& position, float radius, uint32_t color, uint32_t segments = 16);
	void RenderRect(const ImVec2& from, const ImVec2& to, uint32_t color, float rounding = 0.0f, uint32_t roundingCornersFlags = ImDrawCornerFlags_All, float thickness = 1.0f);
	void RenderRectFilled(const ImVec2& from, const ImVec2& to, uint32_t color, float rounding = 0.0f, uint32_t roundingCornersFlags = ImDrawCornerFlags_All);


	ImGuiWindowFlags m_windowFlags =
		ImGuiWindowFlags_AlwaysAutoResize
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoMove;
			//| ImGuiWindowFlags_NoBringToFrontOnFocus
			//| ImGuiWindowFlags_NoFocusOnAppearing
			//| ImGuiWindowFlags_NoInputs;
	// ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs

	float m_rectAx = 100.0f;
	float m_rectAy = 100.0f;
	float m_rectBx = 200.0f;
	float m_rectBy = 200.0f;

	ImVec2 m_lineFrom;
	ImVec2 m_lineTo;
};
