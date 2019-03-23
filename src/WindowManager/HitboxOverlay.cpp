#include "HitboxOverlay.h"

#include "WorldToScreen.h"

#include "Core/interfaces.h"
#include "Game/JonbReader.h"
#include "imgui_internal.h"

void HitboxOverlay::Update()
{
	if (HasNullptrInData())
	{
		return;
	}

	BeforeDraw();

	ImGui::Begin("##HitboxOverlay", nullptr, m_windowFlags);

	Draw();

	ImGui::End();

	AfterDraw();
}

void HitboxOverlay::BeforeDraw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
}

void HitboxOverlay::Draw()
{
	for (int i = 0; i < g_gameVals.entityCount; i++)
	{
		CharInfo* pEntity = (CharInfo*)g_gameVals.pEntityList[i];
		const bool isCharacter = i < 4;
		const bool isEntityActive = pEntity->unknownStatus1 == 1 && pEntity->pJonbEntryBegin;

		if (isCharacter || isEntityActive)
		{
			const D3DXVECTOR2 entityWorldPos = CalculateObjWorldPosition(pEntity);
			DrawCollisionAreas(pEntity, entityWorldPos);
		}
	}
}

void HitboxOverlay::AfterDraw()
{
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
}

bool HitboxOverlay::HasNullptrInData()
{
	return !g_gameVals.pEntityList;
}

D3DXVECTOR2 HitboxOverlay::CalculateObjWorldPosition(const CharInfo* charObj)
{
	//float posX = charObj->position_x_dupe -
	//	charObj->positionOffsetX_1 +
	//	(charObj->facingLeft ? -charObj->positionOffsetX_2 : charObj->positionOffsetX_2) +
	//	(charObj->positionOffsetX_3 * 10) +
	//	charObj->position_x_dupe_offset_x;
	//	//- charObj->position_x_dupe_offset;

	//float posY = charObj->position_y_dupe + charObj->position_y_dupe_offset_y;

	float posX = charObj->position_x_dupe - charObj->positionOffsetX_1;
	float posY = charObj->position_y_dupe;// +charObj->position_y_dupe_offset_y;

	return D3DXVECTOR2(
		floor(posX / 1000 * m_scale),
		floor(posY / 1000 * m_scale)
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

D3DXVECTOR2 HitboxOverlay::RotatePoint(D3DXVECTOR2 center, float angleInRad, D3DXVECTOR2 point)
{
	if (!angleInRad)
	{
		return point;
	}

	// translate point back to origin:
	point.x -= center.x;
	point.y -= center.y;

	float s = sin(angleInRad);
	float c = cos(angleInRad);

	// rotate point
	float xNew = point.x * c - point.y * s;
	float yNew = point.x * s + point.y * c;

	// translate point back:
	point.x = xNew + center.x;
	point.y = yNew + center.y;
	return point;
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

	for (const JonbEntry &entry : entries)
	{
		float offsetX =  floor(entry.offsetX * m_scale);
		float offsetY = -floor(entry.offsetY * m_scale);
		float width =    floor(entry.width * m_scale);
		float height =  -floor(entry.height * m_scale);

		float rotationDeg = charObj->positionRotationDegrees / 1000.0f;

		if (!charObj->facingLeft)
		{
			offsetX = -offsetX;
			width = -width;
			if (rotationDeg)
			{
				rotationDeg = 360.0f - rotationDeg;
			}
		}

		float rotationRad = D3DXToRadian(rotationDeg);

		D3DXVECTOR2 rotatedRectFrom = RotatePoint(
			playerWorldPos,
			rotationRad,
			D3DXVECTOR2(playerWorldPos.x + offsetX, playerWorldPos.y + offsetY)
		);

		D3DXVECTOR3 rectFrom = CalculateScreenPosition(
			D3DXVECTOR3(
				rotatedRectFrom.x,
				rotatedRectFrom.y,
				0.0f
			)
		);

		D3DXVECTOR2 rotatedRectTo = RotatePoint(
			playerWorldPos,
			rotationRad,
			D3DXVECTOR2(playerWorldPos.x + offsetX + width, playerWorldPos.y + offsetY + height)
		);

		D3DXVECTOR3 rectTo = CalculateScreenPosition(
			D3DXVECTOR3(
				rotatedRectTo.x,
				rotatedRectTo.y,
				0.0f
			)
		);

		const unsigned int colorBlue = 0xFF0033CC;
		const unsigned int colorRed = 0xFFFF0000;
		const unsigned int rectBorderColor = entry.type == JonbChunkType_Hurtbox ? colorBlue : colorRed;

		RenderRect(ImVec2(rectFrom.x, rectFrom.y), ImVec2(rectTo.x, rectTo.y), rectBorderColor, 0, ImDrawCornerFlags_All, m_rectThickness);

		const unsigned char transparency = 0xFF * m_rectFillTransparency;
		unsigned int clearedTransparencyBits = (rectBorderColor & ~0xFF000000);
		unsigned int transparencyPercentage = ((int)transparency << 24) & 0xFF000000;
		const unsigned int rectFillColor = clearedTransparencyBits | transparencyPercentage;
		RenderRectFilled(ImVec2(rectFrom.x, rectFrom.y), ImVec2(rectTo.x, rectTo.y), rectFillColor);
	}
}

float& HitboxOverlay::GetScale()
{
	return m_scale;
}

void HitboxOverlay::DrawRectThicknessSlider()
{
	ImGui::SliderFloat("Border thickness", &m_rectThickness, 0.0f, 5.0f, "%.1f");
}

void HitboxOverlay::DrawRectFillTransparencySlider()
{
	ImGui::SliderFloat("Fill transparency", &m_rectFillTransparency, 0.0f, 1.0f, "%.2f");
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
