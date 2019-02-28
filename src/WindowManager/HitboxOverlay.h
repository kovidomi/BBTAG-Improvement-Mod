#pragma once
#include <imgui.h>

class HitboxOverlay
{
public:
	HitboxOverlay();
	void Update();
private:
	void BeforeDraw();
	void Draw();
	void AfterDraw();

	ImGuiWindowFlags m_windowFlags =
		ImGuiWindowFlags_AlwaysAutoResize
			| ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoBringToFrontOnFocus
			| ImGuiWindowFlags_NoFocusOnAppearing
			| ImGuiWindowFlags_NoInputs;
	ImVec2 m_resolution;
};
