#include "HitboxOverlay.h"

#include "WorldToScreen.h"

#include "../Game/JonbReader.h"
#include "../interfaces.h"
#include "imgui_internal.h"

// Literally a magic number as of now
// TODO: Figure out how to calculate properly on all resolutions
const float M = 0.3685f;

void HitboxOverlay::Update()
{
	BeforeDraw();

	ImGui::Begin("##HitboxOverlay", nullptr, m_windowFlags);

	Draw();

	ImGui::End();

	AfterDraw();
}

void HitboxOverlay::BeforeDraw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
}

void HitboxOverlay::Draw()
{
	//ImGui::SliderFloat4("Box", &m_rectAx, 0.0f, 1600.0f, "%.0f");

	for (int i = 0; i < g_gameVals.entityCount; i++)
	{
		CharInfo* pEntity = (CharInfo*)g_gameVals.pEntityList[i];
		D3DXVECTOR2 entityWorldPos = CalculatePlayerWorldPosition(pEntity);
		DrawCollisionAreas(pEntity, entityWorldPos);
	}

	//D3DXVECTOR2 playerOneCharOneWorldPos = CalculatePlayerWorldPosition(g_interfaces.player1.GetChar1().GetData());
	//DrawCollisionAreas(g_interfaces.player1.GetChar1().GetData(), playerOneCharOneWorldPos);

	//D3DXVECTOR2 playerOneCharTwoWorldPos = CalculatePlayerWorldPosition(g_interfaces.player1.GetChar2().GetData());
	//DrawCollisionAreas(g_interfaces.player1.GetChar2().GetData(), playerOneCharTwoWorldPos);

	//D3DXVECTOR2 playerTwoCharOneWorldPos = CalculatePlayerWorldPosition(g_interfaces.player2.GetChar1().GetData());
	//DrawCollisionAreas(g_interfaces.player2.GetChar1().GetData(), playerTwoCharOneWorldPos);

	//D3DXVECTOR2 playerTwoCharTwoWorldPos = CalculatePlayerWorldPosition(g_interfaces.player2.GetChar2().GetData());
	//DrawCollisionAreas(g_interfaces.player2.GetChar2().GetData(), playerTwoCharTwoWorldPos);

	//D3DXVECTOR3 playerScreenPos = CalculateScreenPosition(D3DXVECTOR3(playerWorldPos.x, playerWorldPos.y, 0.0f));
	//DrawOriginLine(ImVec2(playerScreenPos.x, playerScreenPos.y));
}

void HitboxOverlay::AfterDraw()
{
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
}

D3DXVECTOR2 HitboxOverlay::CalculatePlayerWorldPosition(const CharInfo* charObj)
{
	return D3DXVECTOR2(
		floor(charObj->position_x / 1000 * M),
		floor(charObj->position_y / 1000 * M)
	);
}

D3DXVECTOR3 HitboxOverlay::CalculateScreenPosition(D3DXVECTOR3 worldPos)
{
	D3DXVECTOR3 result;
	WorldToScreen(g_interfaces.pD3D9ExWrapper, &worldPos, &result);

	result.x = floor(result.x);
	result.y = floor(result.y);

	return result;
}

void HitboxOverlay::DrawOriginLine(ImVec2 screenPos)
{
	const unsigned int colorOrange = 0xFFFF9900;

	ImVec2 lineFrom = ImVec2(screenPos.x, screenPos.y);
	ImVec2 lineTo = ImVec2(screenPos.x, screenPos.y - 500);

	RenderLine(lineFrom, lineTo, colorOrange, 2);
}

void HitboxOverlay::DrawCollisionAreas(const CharInfo* charObj, const D3DXVECTOR2 playerWorldPos)
{
	std::vector<JonbEntry> entries = JonbReader::getJonbEntries(charObj);

	for (JonbEntry entry : entries)
	{
		float offsetX =  floor(entry.offsetX * M);
		float offsetY = -floor(entry.offsetY * M);
		float width =    floor(entry.width * M);
		float height =  -floor(entry.height * M);

		if (!charObj->facingLeft)
		{
			offsetX = -offsetX;
			width = -width;
		}

		D3DXVECTOR3 rectFrom = CalculateScreenPosition(
			D3DXVECTOR3(
				playerWorldPos.x + offsetX,
				playerWorldPos.y + offsetY,
				0.0f
			)
		);

		D3DXVECTOR3 rectTo = CalculateScreenPosition(
			D3DXVECTOR3(
				playerWorldPos.x + offsetX + width,
				playerWorldPos.y + offsetY + height,
				0.0f
			)
		);

		const unsigned int colorBlue = 0xFF0033CC;
		const unsigned int colorRed = 0xFFFF0000;
		unsigned int rectColor = entry.type == JonbChunkType_Hurtbox ? colorBlue : colorRed;

		RenderRect(ImVec2(rectFrom.x, rectFrom.y), ImVec2(rectTo.x, rectTo.y), rectColor, 0, ImDrawCornerFlags_All, 3);
	}
}


void HitboxOverlay::RenderLine(const ImVec2& from, const ImVec2& to, uint32_t color, float thickness)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float r = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float b = (color) & 0xff;

	window->DrawList->AddLine(from, to, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), thickness);
}

void HitboxOverlay::RenderCircle(const ImVec2& position, float radius, uint32_t color, float thickness, uint32_t segments)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float r = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float b = (color) & 0xff;

	window->DrawList->AddCircle(position, radius, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), segments, thickness);
}

void HitboxOverlay::RenderCircleFilled(const ImVec2& position, float radius, uint32_t color, uint32_t segments)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float r = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float b = (color) & 0xff;

	window->DrawList->AddCircleFilled(position, radius, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), segments);
}

void HitboxOverlay::RenderRect(const ImVec2& from, const ImVec2& to, uint32_t color, float rounding, uint32_t roundingCornersFlags, float thickness)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xFF;
	float r = (color >> 16) & 0xFF;
	float g = (color >> 8) & 0xFF;
	float b = (color) & 0xFF;

	window->DrawList->AddRect(from, to, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), rounding, roundingCornersFlags, thickness);
}

void HitboxOverlay::RenderRectFilled(const ImVec2& from, const ImVec2& to, uint32_t color, float rounding, uint32_t roundingCornersFlags)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xFF;
	float r = (color >> 16) & 0xFF;
	float g = (color >> 8) & 0xFF;
	float b = (color) & 0xFF;

	window->DrawList->AddRectFilled(from, to, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), rounding, roundingCornersFlags);
}
