#include "CustomHud.h"

#include "Core/interfaces.h"
#include "Core/logger.h"
#include "Game/MeterInfo.h"

ImGuiWindowFlags customHUDWindowFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar;
//const ImVec4 default_cross_bar_color(0.15f, 1.0f, 1.0f, 1.0f);
//const ImVec4 default_skill_bar_color(1.0f, 0.34f, 0.0f, 1.0f);
//const ImVec4 skill_bar_color_blaze_active(0.45f, 1.0f, 1.0f, 1.0f);
//const ImVec4 cross_bar_color_blaze_active(0.0f, 0.4f, 1.0f, 1.0f);
//ImVec4 skill_bar_color(1.0f, 0.34f, 0.0f, 1.0f);
//ImVec4 cross_bar_color(0.15f, 1.0f, 1.0f, 1.0f);
//const ImVec4 blaze_val_color(1.0f, 0.4f, 1.0f, 1.0f);
//const ImVec4 astral_available_color(1.000f, 0.949f, 0.000f, 1.000f);
const ImVec4 invis_color(0.0f, 0.0f, 0.0f, 0.0f);

//void CustomHud::SetScale(float hudScaleX, float hudScaleY)
//{
//	this->hud_scale_x = hudScaleX;
//	this->hud_scale_y = hudScaleY;
//}

void CustomHud::OnUpdate(bool show_custom_hud, bool show_main_window)
{
	// custom hud visibility syncing with the original, not finished yet
	//bool origIsHUDHidden = UpdateVisibility();
	//if (origIsHUDHidden || !show_custom_hud)
	//	return;

	// forcecustomHUD 
	if (Settings::settingsIni.forcecustomhud && *g_gameVals.pIsHUDHidden == 0)
	{
		*g_gameVals.pIsHUDHidden = 1;
	}

	if (!show_custom_hud)
		return;
	
	//sanity check 1
	if (g_interfaces.player1.GetChar1().IsCharDataNullPtr() ||
		g_interfaces.player1.GetChar2().IsCharDataNullPtr() ||
		g_interfaces.player2.GetChar1().IsCharDataNullPtr() ||
		g_interfaces.player2.GetChar2().IsCharDataNullPtr())
	{
		LOG(2, "One of Player.Char() is NULL !!!!!\n");
		return;
	}

	//sanity check 2
	if (g_interfaces.player1.IsMetersNullPtr() ||
		g_interfaces.player2.IsMetersNullPtr())
	{
		LOG(2, "One of Player.Meter() is NULL !!!!\n");
		return;
	}

	if (show_main_window)
		customHUDWindowFlags &= ~ImGuiWindowFlags_NoMove; //clear the flag
	else
		customHUDWindowFlags |= ImGuiWindowFlags_NoMove; // set the flag

														 //push styles
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 4.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f)); //black
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.286f, 0.286f, 0.286f, 0.54f)); //grey

	const MeterInfo& p1Meters = *g_interfaces.player1.GetMeters();
	const MeterInfo& p2Meters = *g_interfaces.player2.GetMeters();

	const CharInfo& p1Ch1Info = *g_interfaces.player1.GetChar1().GetData();
	const CharInfo& p1Ch2Info = *g_interfaces.player1.GetChar2().GetData();
	const CharInfo& p2Ch1Info = *g_interfaces.player2.GetChar1().GetData();
	const CharInfo& p2Ch2Info = *g_interfaces.player2.GetChar2().GetData();

	if (p1Meters.is_blaze_active || p2Meters.is_blaze_active)
	{
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 1.0f, 1.0f, 1.0f)); //cyan
		ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0.0f, 1.0f, 1.0f, 1.0f)); //cyan
	}

	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

	//render top part of the hud
	{
		//UpdateTimer(g_gameVals.pGameTimer);

		//if (p1Ch1Info.is_char_active)
		//{
		//	UpdateHP(p1Ch1Info);
		//	UpdateHP(p1Ch2Info);
		//}
		//else
		//{
		//	UpdateHP(p1Ch2Info);
		//	UpdateHP(p1Ch1Info);
		//}

		//if (p2Ch1Info.is_char_active)
		//{
		//	UpdateHP(p2Ch1Info, true);
		//	UpdateHP(p2Ch2Info, true);
		//}
		//else
		//{
		//	UpdateHP(p2Ch2Info, true);
		//	UpdateHP(p2Ch1Info, true);
		//}
	}

	//render bottom part of the hud
	{
		//bool p1_is_astral_avail = IsAstralAvailable(p1Meters.cur_skill, p2Ch1Info.cur_hp, p2Ch2Info.cur_hp);
		//UpdateMeters(p1Meters.cur_skill, p1Meters.cur_cross,
		//	p1Meters.cur_blaze, p1Meters.is_blaze_available,
		//	p1_is_astral_avail, p1Meters.is_blaze_active, false);

		//bool p2_is_astral_avail = IsAstralAvailable(p2Meters.cur_skill, p1Ch1Info.cur_hp, p1Ch2Info.cur_hp);
		//UpdateMeters(p2Meters.cur_skill, p2Meters.cur_cross,
		//	p2Meters.cur_blaze, p2Meters.is_blaze_available, 
		//	p2_is_astral_avail, p2Meters.is_blaze_active, true);

		//render unique meters
		{
			if (p1Ch1Info.is_char_active)
			{
				UpdateCharSpecificMeters(p1Ch1Info);
				UpdateCharSpecificMeters(p1Ch2Info);
			}
			else
			{
				UpdateCharSpecificMeters(p1Ch2Info);
				UpdateCharSpecificMeters(p1Ch1Info);
			}

			if (p2Ch1Info.is_char_active)
			{
				UpdateCharSpecificMeters(p2Ch1Info, true);
				UpdateCharSpecificMeters(p2Ch2Info, true);
			}
			else
			{
				UpdateCharSpecificMeters(p2Ch2Info, true);
				UpdateCharSpecificMeters(p2Ch1Info, true);
			}
		}
	}

	ImGui::PopFont();

	if (p1Meters.is_blaze_active || p2Meters.is_blaze_active)
		ImGui::PopStyleColor(2);

	ImGui::PopStyleColor(2);
	ImGui::PopStyleVar(3);
}

void CustomHud::ShowResetPositionsButton(ImVec2 middlescreen)
{
	if (ImGui::Button("Reset custom HUD positions"))
	{
		ImGui::SetWindowPos("P1_meters", middlescreen);
		ImGui::SetWindowPos("P2_meters", middlescreen);
		ImGui::SetWindowPos("P1_hp_gauge", middlescreen);
		ImGui::SetWindowPos("P2_hp_gauge", middlescreen);
		ImGui::SetWindowPos("P1_unique_meters", middlescreen);
		ImGui::SetWindowPos("P2_unique_meters", middlescreen);
		ImGui::SetWindowPos("TIMER", middlescreen);
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("Used to recover elements of the custom HUD that have\nbecome unrecoverable due to going beyond the screen");
		ImGui::EndTooltip();
	}
}

bool CustomHud::UpdateVisibility()
{
	// Keeping track of *g_gameVals.pIsHUDHidden 's original value as well

	// *g_gameVals.pIsHUDHidden is a bitfield:
	// 0x00 - (nothing is activated) hud is visible
	// 0x01 - hud is invisible (intro)
	// 0x02 - hud is invisible (astral)
	// 0x04 - loading icon is shown

	static bool isOrigHUDHidden = false;
	static int prev = 0;

	if (g_gameVals.pIsHUDHidden)
	{
		if (prev != *g_gameVals.pIsHUDHidden)
		{
			// Syncing the visibility of the custom hud with the original's (so it disappears during intros and astral)
			// If either the first or second bits are on, the hud is invisible 
			// clear the flag that we set ourselves when we are forcing the hud off
			if (prev & 2 && !(*g_gameVals.pIsHUDHidden & 2))
				*g_gameVals.pIsHUDHidden &= ~(1);

			if (*g_gameVals.pIsHUDHidden & 1 || *g_gameVals.pIsHUDHidden & 2)
				isOrigHUDHidden = true;
			else
				isOrigHUDHidden = false;
		}

		//override the visibility of the game's original HUD
		if (Settings::settingsIni.forcecustomhud && *g_gameVals.pIsHUDHidden == 0)
		{
			isOrigHUDHidden = false;
			*g_gameVals.pIsHUDHidden = 1;
		}

		prev = *g_gameVals.pIsHUDHidden;
	}

	return isOrigHUDHidden;
}

void CustomHud::UpdateCharSpecificMeters(const CharInfo & charInfo, bool right_side)
{

	if (!char_has_unique_meter((CharIndex)charInfo.char_index))
		return;

	bool show_unique_bar = show_char_unique_meter_bar(charInfo);
	bool show_unique_num = show_char_unique_meter_num(charInfo);

	if ((!show_unique_bar && !show_unique_num) || charInfo.cur_hp == 0)
		return;

	char wndtitle[36];

	if (right_side)
	{
		sprintf(wndtitle, "P2_unique_meters");
	}
	else
		sprintf(wndtitle, "P1_unique_meters");

	ImGui::Begin(wndtitle, NULL, customHUDWindowFlags);

	ImVec2 bar_size(140.0f * hud_scale_x, 20.0f * hud_scale_y);
	ImVec4 num_bar_color = get_char_unique_meter_color(charInfo);

	char meter_num[32] = "?";

	switch (charInfo.char_index)
	{
	case CharIndex_Yukiko:
	case CharIndex_Akihiko:
		sprintf(meter_num, "%d", charInfo.unique_meter_cur_val);
		break;
	case CharIndex_Naoto:
		sprintf(meter_num, "%s", charInfo.naoto_is_enemy_marked == 1 ? "X" : " ");
		break;
	case CharIndex_Chie:
		sprintf(meter_num, "%d", charInfo.chie_charge_lvl);
		break;
	}

	float bar_percent = 0.0f;

	if (show_unique_bar)
		bar_percent = (float)charInfo.unique_meter_cur_val / (float)charInfo.unique_meter_max_val;

	if (right_side)
	{
		if (show_unique_num)
			ImGui::TextColored(num_bar_color, "%s", meter_num);
		else  //drawing invis num for alignment
		{
			ImGui::TextColored(invis_color, "%s", meter_num);
		}

		ImGui::SameLine();
		if (show_unique_bar)
		{
			ImGui::ColoredProgressBar(bar_percent, bar_size, num_bar_color, NULL, false, right_side);
		}
		else
		{
			ImGui::ColoredProgressBar(bar_percent, bar_size, invis_color, NULL, false, right_side);
		}

		ImGui::SameLine();
		ImGui::Text("%s", meterCharNames[charInfo.char_index].c_str());
	}
	else
	{
		ImGui::Text("%s", meterCharNames[charInfo.char_index].c_str());
		ImGui::SameLine();

		if (show_unique_bar)
			ImGui::ColoredProgressBar(bar_percent, bar_size, num_bar_color, NULL, false);
		else
		{
			ImGui::ColoredProgressBar(0.0f, bar_size, invis_color, NULL, false);
		}

		ImGui::SameLine();
		if (show_unique_num)
		{
			ImGui::TextColored(num_bar_color, "%s", meter_num);
		}
		else
		{
			ImGui::TextColored(invis_color, "%s", meter_num);
		}
	}

	ImGui::End();
}
