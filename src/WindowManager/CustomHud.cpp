#include "CustomHud.h"
#include "../globals.h"
#include "../logger.h"
#include <imgui.h>

ImGuiWindowFlags customHUDWindowFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar;
const ImVec4 default_cross_bar_color(0.15f, 1.0f, 1.0f, 1.0f);
const ImVec4 default_skill_bar_color(1.0f, 0.34f, 0.0f, 1.0f);
const ImVec4 skill_bar_color_blaze_active(0.45f, 1.0f, 1.0f, 1.0f);
const ImVec4 cross_bar_color_blaze_active(0.0f, 0.4f, 1.0f, 1.0f);
ImVec4 skill_bar_color(1.0f, 0.34f, 0.0f, 1.0f);
ImVec4 cross_bar_color(0.15f, 1.0f, 1.0f, 1.0f);
const ImVec4 blaze_val_color(1.0f, 0.4f, 1.0f, 1.0f);
const ImVec4 invis_color(0.0f, 0.0f, 0.0f, 0.0f);

//bool *NO_CLOSE_FLAG = NULL;

CustomHud::CustomHud(float hud_scale_x, float hud_scale_y)
{
	LOG(2, "CustomHud constructor\n");
	this->hud_scale_x = hud_scale_x;
	this->hud_scale_y = hud_scale_y;
}

void CustomHud::Update(bool show_custom_hud, bool show_main_window)
{
	if (!show_custom_hud)
		return;
	
	//sanity check 1
	if (!g_gameVals.CharObj_P1Char1 || !g_gameVals.CharObj_P1Char2 ||
		!g_gameVals.CharObj_P2Char1 || !g_gameVals.CharObj_P2Char2)
	{
		LOG(2, "One of g_gameVals.CharObj is NULL !!!!!\n");
		return;
	}

	//sanity check 2
	if (!*g_gameVals.CharObj_P1Char1 || !*g_gameVals.CharObj_P1Char2 ||
		!*g_gameVals.CharObj_P2Char1 || !*g_gameVals.CharObj_P2Char2)
	{
		LOG(2, "One of *g_gameVals.CharObj is NULL !!!!\n");
		return;
	}

	//sanity check 3
	if (!g_gameVals.PlayerMetersObj)
	{
		LOG(2, "g_gameVals.PlayerMetersObj is NULL !!!!\n");
		return;
	}

	if (show_main_window)
		customHUDWindowFlags &= ~ImGuiWindowFlags_NoMove; //clear the flag
	else
		customHUDWindowFlags |= ImGuiWindowFlags_NoMove; // set the flag

														 //push styles
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 4.0f));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f)); //black
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.286f, 0.286f, 0.286f, 0.54f)); //grey

	if (g_gameVals.PlayerMetersObj->P1_is_blaze_active || g_gameVals.PlayerMetersObj->P2_is_blaze_active)
	{
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 1.0f, 1.0f, 1.0f)); //cyan
	}

	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

	//render top part of the hud
	{
		UpdateTimer(g_gameVals.pGameTimer);

		if ((*g_gameVals.CharObj_P1Char1)->is_char_active)
		{
			UpdateHP(*g_gameVals.CharObj_P1Char1);
			UpdateHP(*g_gameVals.CharObj_P1Char2);
		}
		else
		{
			UpdateHP(*g_gameVals.CharObj_P1Char2);
			UpdateHP(*g_gameVals.CharObj_P1Char1);
		}

		if ((*g_gameVals.CharObj_P2Char1)->is_char_active)
		{
			UpdateHP(*g_gameVals.CharObj_P2Char1, true);
			UpdateHP(*g_gameVals.CharObj_P2Char2, true);
		}
		else
		{
			UpdateHP(*g_gameVals.CharObj_P2Char2, true);
			UpdateHP(*g_gameVals.CharObj_P2Char1, true);
		}
	}

	//render bottom part of the hud
	{
		UpdateMeters(g_gameVals.PlayerMetersObj->P1_cur_skill, g_gameVals.PlayerMetersObj->P1_cur_cross,
			g_gameVals.PlayerMetersObj->P1_cur_blaze, g_gameVals.PlayerMetersObj->P1_is_blaze_available,
			g_gameVals.PlayerMetersObj->P1_is_blaze_active, false);

		UpdateMeters(g_gameVals.PlayerMetersObj->P2_cur_skill, g_gameVals.PlayerMetersObj->P2_cur_cross,
			g_gameVals.PlayerMetersObj->P2_cur_blaze, g_gameVals.PlayerMetersObj->P2_is_blaze_available,
			g_gameVals.PlayerMetersObj->P2_is_blaze_active, true);

		//render unique meters
		{
			if ((*g_gameVals.CharObj_P1Char1)->is_char_active)
			{
				UpdateCharSpecificMeters(*g_gameVals.CharObj_P1Char1);
				UpdateCharSpecificMeters(*g_gameVals.CharObj_P1Char2);
			}
			else
			{
				UpdateCharSpecificMeters(*g_gameVals.CharObj_P1Char2);
				UpdateCharSpecificMeters(*g_gameVals.CharObj_P1Char1);
			}

			if ((*g_gameVals.CharObj_P2Char1)->is_char_active)
			{
				UpdateCharSpecificMeters(*g_gameVals.CharObj_P2Char1, true);
				UpdateCharSpecificMeters(*g_gameVals.CharObj_P2Char2, true);
			}
			else
			{
				UpdateCharSpecificMeters(*g_gameVals.CharObj_P2Char2, true);
				UpdateCharSpecificMeters(*g_gameVals.CharObj_P2Char1, true);
			}
		}
	}

	ImGui::PopFont();

	if (g_gameVals.PlayerMetersObj->P1_is_blaze_active || g_gameVals.PlayerMetersObj->P2_is_blaze_active)
		ImGui::PopStyleColor();

	ImGui::PopStyleColor(2);
	ImGui::PopStyleVar(2);
}

void CustomHud::UpdateHP(CharObj *CharInstance, bool right_side)
{
	if (!CharInstance)
		return;

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

	float cur_hp_percent = (float)CharInstance->cur_hp / (float)CharInstance->max_hp;

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
	if (CharInstance->recoverable_hp > 0)
		recoverable_hp_percent = (float)(CharInstance->recoverable_hp + CharInstance->cur_hp) / (float)CharInstance->max_hp;

	ImGui::ColoredProgressBar(recoverable_hp_percent, ImVec2(bar_width, bar_height), recoverable_hp_color, NULL, false, swap_bar_dir);

	//actual hp
	//////////////////

	//positioning so we render the next progress bar on top of the previous one
	ImVec2 size = ImGui::GetItemRectSize();
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - size.y - ImGui::GetStyle().ItemSpacing.y);

	//char bufhp[32];
	//sprintf(bufhp, "%d", CharInstance->cur_hp);
	ImGui::ColoredProgressBar(cur_hp_percent, ImVec2(bar_width, bar_height), hp_color, NULL, false, swap_bar_dir);

	ImVec2 origPos = ImGui::GetCursorPos();

	///////////////////////////
	char* charname;
	if (CharInstance->char_index < TOTAL_CHAR_INDEXES)
		charname = charnames[CharInstance->char_index];
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

void CustomHud::UpdateMeters(int cur_skill_val, int cur_cross_val, int cur_blaze_val, bool is_blaze_available, bool is_blaze_active, bool right_side)
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

	ImGui::Begin(wndtitle, NULL, customHUDWindowFlags);

	//set colors
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

	ImGui::End();
}

void CustomHud::UpdateCharSpecificMeters(CharObj * CharInstance, bool right_side)
{

	if (!char_has_unique_meter((CharIndex)CharInstance->char_index))
		return;

	bool show_unique_bar = show_char_unique_meter_bar(CharInstance);
	bool show_unique_num = show_char_unique_meter_num(CharInstance);

	if ((!show_unique_bar && !show_unique_num) || CharInstance->cur_hp == 0)
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
	ImVec4 num_bar_color = get_char_unique_meter_color(CharInstance);

	char meter_num[32] = "?";

	switch (CharInstance->char_index)
	{
	case CharIndex_Yukiko:
		sprintf(meter_num, "%d", CharInstance->unique_meter_cur_val);
		break;
	case CharIndex_Naoto:
		sprintf(meter_num, "%s", CharInstance->naoto_is_enemy_marked == 1 ? "X" : " ");
		break;
	case CharIndex_Chie:
		sprintf(meter_num, "%d", CharInstance->chie_charge_lvl);
		break;
	}

	float bar_percent = 0.0f;

	if (show_unique_bar)
		bar_percent = (float)CharInstance->unique_meter_cur_val / (float)CharInstance->unique_meter_max_val;

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
		ImGui::Text("%s", metercharnames[CharInstance->char_index]);
	}
	else
	{
		ImGui::Text("%s", metercharnames[CharInstance->char_index]);
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