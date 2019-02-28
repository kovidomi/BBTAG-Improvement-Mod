#include "HitboxOverlay.h"
#include "../interfaces.h"
#include "../Settings.h"

HitboxOverlay::HitboxOverlay()
{
	m_resolution = ImVec2(Settings::settingsIni.renderwidth, Settings::settingsIni.renderheight);
}

void HitboxOverlay::Update()
{
	BeforeDraw();

	ImGui::Begin("Hitbox Overlay", NULL, m_windowFlags);

	Draw();

	ImGui::End();

	AfterDraw();
}

void HitboxOverlay::BeforeDraw()
{
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(m_resolution);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
}

void HitboxOverlay::Draw()
{
	ImGui::Text("TEST TEST TEST TEST TEST ");
	ImGui::Text("TEST TEST TEST TEST TEST ");
	ImGui::Text("TEST TEST TEST TEST TEST ");
	ImGui::Text("TEST TEST TEST TEST TEST ");
}

void HitboxOverlay::AfterDraw()
{
	ImGui::PopStyleColor();
}
