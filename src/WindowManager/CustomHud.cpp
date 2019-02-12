#include "CustomHud.h"
#include "interfaces.h"
#include "Game/MeterInfo.h"
#include "logger.h"

ImGuiWindowFlags customHUDWindowFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar;
const ImVec4 default_cross_bar_color(0.15f, 1.0f, 1.0f, 1.0f);
const ImVec4 default_skill_bar_color(1.0f, 0.34f, 0.0f, 1.0f);
const ImVec4 skill_bar_color_blaze_active(0.45f, 1.0f, 1.0f, 1.0f);
const ImVec4 cross_bar_color_blaze_active(0.0f, 0.4f, 1.0f, 1.0f);
ImVec4 skill_bar_color(1.0f, 0.34f, 0.0f, 1.0f);
ImVec4 cross_bar_color(0.15f, 1.0f, 1.0f, 1.0f);
const ImVec4 blaze_val_color(1.0f, 0.4f, 1.0f, 1.0f);
const ImVec4 astral_available_color(1.000f, 0.949f, 0.000f, 1.000f);
const ImVec4 invis_color(0.0f, 0.0f, 0.0f, 0.0f);

//bool *NO_CLOSE_FLAG = NULL;

CustomHud::CustomHud(float hud_scale_x, float hud_scale_y)
{
	LOG(2, "CustomHud constructor\n");
	this->hud_scale_x = hud_scale_x;
	this->hud_scale_y = hud_scale_y;
}

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
		UpdateTimer(g_gameVals.pGameTimer);

		if (p1Ch1Info.is_char_active)
		{
			UpdateHP(p1Ch1Info);
			UpdateHP(p1Ch2Info);
		}
		else
		{
			UpdateHP(p1Ch2Info);
			UpdateHP(p1Ch1Info);
		}

		if (p2Ch1Info.is_char_active)
		{
			UpdateHP(p2Ch1Info, true);
			UpdateHP(p2Ch2Info, true);
		}
		else
		{
			UpdateHP(p2Ch2Info, true);
			UpdateHP(p2Ch1Info, true);
		}
	}

	//render bottom part of the hud
	{
		bool p1_is_astral_avail = IsAstralAvailable(p1Meters.cur_skill, p2Ch1Info.cur_hp, p2Ch2Info.cur_hp);
		UpdateMeters(p1Meters.cur_skill, p1Meters.cur_cross,
			p1Meters.cur_blaze, p1Meters.is_blaze_available,
			p1_is_astral_avail, p1Meters.is_blaze_active, false);

		bool p2_is_astral_avail = IsAstralAvailable(p2Meters.cur_skill, p1Ch1Info.cur_hp, p1Ch2Info.cur_hp);
		UpdateMeters(p2Meters.cur_skill, p2Meters.cur_cross,
			p2Meters.cur_blaze, p2Meters.is_blaze_available, 
			p2_is_astral_avail, p2Meters.is_blaze_active, true);

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

void CustomHud::UpdateHP(const CharInfo &charInfo, bool right_side)
{
	char wndtitle[36];
	bool swap_bar_dir = false;

	if (right_side)
		sprintf(wndtitle, "P2_hp_gauge");
	else
	{
		sprintf(wndtitle, "P1_hp_gauge");
		swap_bar_dir = true;
	}

	ImGui::Begin(wndtitle, NULL, customHUDWindowFlags);

	float cur_hp_percent = (float)charInfo.cur_hp / (float)charInfo.max_hp;

	ImVec4 hp_color;
	ImVec4 recoverable_hp_color(0.7f, 0.0f, 0.0f, 0.75f);

	if (cur_hp_percent < 0.35f)
		hp_color = ImVec4(1.0f, 0.25f, 0.0f, 1.0f);
	else if (cur_hp_percent > 0.35f && cur_hp_percent < 1.0f)
		hp_color = ImVec4(0.1f, 1.0f, 0.0f, 1.0f);
	else
		hp_color = ImVec4(0.0f, 0.85f, 1.0f, 1.0f);

	//////////////////
	float bar_width = 635.0f * hud_scale_x;
	float bar_height = 45.0f * hud_scale_y;

	//recoverable hp
	float recoverable_hp_percent = 0.0f;
	if (charInfo.recoverable_hp > 0)
		recoverable_hp_percent = (float)(charInfo.recoverable_hp + charInfo.cur_hp) / (float)charInfo.max_hp;

	ImGui::ColoredProgressBar(recoverable_hp_percent, ImVec2(bar_width, bar_height), recoverable_hp_color, NULL, false, swap_bar_dir);

	//actual hp
	//////////////////

	//positioning so we render the next progress bar on top of the previous one
	ImVec2 size = ImGui::GetItemRectSize();
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - size.y - ImGui::GetStyle().ItemSpacing.y);

	//char bufhp[32];
	//sprintf(bufhp, "%d", charInfo.cur_hp);
	ImGui::ColoredProgressBar(cur_hp_percent, ImVec2(bar_width, bar_height), hp_color, NULL, false, swap_bar_dir);

	ImVec2 origPos = ImGui::GetCursorPos();

	///////////////////////////
	const char* charname;
	if (charInfo.char_index < TOTAL_CHAR_INDEXES)
		charname = charNames[charInfo.char_index];
	else
		charname = "<UNKNOWN>";

	ImVec2 nameSize = ImGui::CalcTextSize(charname);
	ImVec2 newPos;
	if (right_side)
		newPos = ImVec2((ImGui::GetCursorPosX() + size.x) - nameSize.x, (ImGui::GetCursorPosY() - size.y - ImGui::GetStyle().ItemSpacing.y));
	else
		newPos = ImVec2(ImGui::GetCursorPosX(), (ImGui::GetCursorPosY() - size.y - ImGui::GetStyle().ItemSpacing.y));

	ImGui::SetCursorPos(newPos);
	ImGui::Text("%s", charname);

	//go back to original pos
	ImGui::SetCursorPos(origPos);

	ImGui::End();
}

void CustomHud::UpdateTimer(int *timer)
{
	if (!timer)
		return;

	ImGui::Begin("TIMER", NULL, customHUDWindowFlags);
	ImGui::Text("TIME");

	char time[32];
	sprintf(time, "%d", *g_gameVals.pGameTimer);
	ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - (ImGui::CalcTextSize(time).x / 2));
	ImGui::Text(time);
	ImGui::End();
}

void CustomHud::UpdateMeters(int cur_skill_val, int cur_cross_val, int cur_blaze_val, bool is_blaze_available, 
	bool is_astral_available, bool is_blaze_active, bool right_side)
{
	ImVec2 skill_bar_size(200.0f * hud_scale_x, 35.0f * hud_scale_y);
	ImVec2 cross_bar_size(400.0f * hud_scale_x, 40.0f * hud_scale_y);
	char wndtitle[36];

	bool swap_cross_bar_dir = false;
	bool swap_skill_bar_dir = false;

	if (right_side)
	{
		sprintf(wndtitle, "P2_meters");
		swap_skill_bar_dir = true;
		swap_cross_bar_dir = true;
	}
	else
		sprintf(wndtitle, "P1_meters");

	//set colors
	if (is_astral_available)
	{
		ImGui::PushStyleColor(ImGuiCol_Border, astral_available_color);
		ImGui::PushStyleColor(ImGuiCol_BorderShadow, astral_available_color);
	}

	if (is_blaze_active)
	{
		cross_bar_color = cross_bar_color_blaze_active;
		skill_bar_color = skill_bar_color_blaze_active;
	}
	else if (is_blaze_available)
	{
		static float cross_blaze_prog = 0.0f, progress_dir = 1.0f;
		const float BLINK_SPEED = 2.0f;
		cross_blaze_prog += progress_dir * BLINK_SPEED * ImGui::GetIO().DeltaTime;
		if (cross_blaze_prog >= +1.1f) { cross_blaze_prog = +1.1f; progress_dir *= -1.0f; }
		if (cross_blaze_prog <= -0.1f) { cross_blaze_prog = -0.1f; progress_dir *= -1.0f; }

		cross_bar_color.y = cross_blaze_prog;
	}

	ImGui::Begin(wndtitle, NULL, customHUDWindowFlags);

	if (right_side)
	{
		char blaze_lvl[32];
		char skill_lvl[32];
		sprintf(blaze_lvl, "Lv %d", blaze_to_single_digit(cur_blaze_val));
		sprintf(skill_lvl, "%d", total_skill_to_single_digit(cur_skill_val));

		///////////////// POSITIONING
		ImGui::TextColored(invis_color, blaze_lvl); //invis
		ImGui::SameLine();

		//push styles
		ImGui::PushStyleColor(ImGuiCol_Border, invis_color); //invis
		ImGui::PushStyleColor(ImGuiCol_FrameBg, invis_color); //invis

															  //invis box
		ImGui::ColoredProgressBar(total_skill_to_single_bar_percent(cur_skill_val, is_blaze_active),
			skill_bar_size, invis_color, NULL, false);

		ImGui::PopStyleColor(2);

		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::CalcTextSize(skill_lvl).x - (ImGui::GetStyle().ItemSpacing.x * 2));
		//////////////////////////

		/////////// SKILL BAR
		ImGui::TextColored(skill_bar_color, skill_lvl);
		ImGui::SameLine();
		ImGui::ColoredProgressBar(total_skill_to_single_bar_percent(cur_skill_val, is_blaze_active),
			skill_bar_size, skill_bar_color, NULL, false, swap_skill_bar_dir);

		////////// BLAZE VAL
		ImGui::TextColored(blaze_val_color, blaze_lvl);
		ImGui::SameLine();

		////////// CROSS BAR
		ImGui::ColoredProgressBar(total_cross_to_bar_percent(cur_cross_val),
			cross_bar_size, cross_bar_color, NULL, false, swap_cross_bar_dir);
	}
	else
	{
		/////////// SKILL BAR
		ImGui::ColoredProgressBar(total_skill_to_single_bar_percent(cur_skill_val, is_blaze_active),
			skill_bar_size, skill_bar_color, NULL, false, swap_skill_bar_dir);

		ImGui::SameLine();
		ImGui::TextColored(skill_bar_color, "%d", total_skill_to_single_digit(cur_skill_val));

		////////// CROSS BAR
		ImGui::ColoredProgressBar(total_cross_to_bar_percent(cur_cross_val),
			cross_bar_size, cross_bar_color, NULL, false, swap_cross_bar_dir);

		////////// BLAZE VAL
		ImGui::SameLine();
		ImGui::TextColored(blaze_val_color, "Lv %d", blaze_to_single_digit(cur_blaze_val));
	}

	//return the bar colors back to their default colors after done
	cross_bar_color = default_cross_bar_color;
	skill_bar_color = default_skill_bar_color;
	if (is_astral_available)
		ImGui::PopStyleColor(2);

	ImGui::End();
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
		ImGui::Text("%s", meterCharNames[charInfo.char_index]);
	}
	else
	{
		ImGui::Text("%s", meterCharNames[charInfo.char_index]);
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

bool CustomHud::IsAstralAvailable(const int curSkill, const int otherCharHP1, const int otherCharHP2)
{
	if (curSkill >= SKILL_GAUGE_MAX_VALUE_BLAZE_ACTIVE && (otherCharHP1 <= 0 || otherCharHP2 <= 0))
		return true;

	return false;
}
