#pragma once
#include "Game/CharInfo.h"

#include <imgui.h>

class CustomHud
{
public:
	void SetScale(float hud_scale_x, float hud_scale_y);
	void OnUpdate(bool show_custom_hud, bool show_main_window);
	void ShowResetPositionsButton(ImVec2 middlescreen);
private:
	bool UpdateVisibility();
	void UpdateHP(const CharInfo &charInfo, bool right_side = false);
	void UpdateTimer(int *timer);
	void UpdateMeters(int cur_skill_val, int cur_cross_val, int cur_blaze_val,
		bool is_blaze_available, bool is_astral_available, bool is_blaze_active, bool right_side = false);
	void UpdateCharSpecificMeters(const CharInfo &charInfo, bool right_side = false);
	bool IsAstralAvailable(const int curSkill, const int otherCharHP1, const int otherCharHP2);

	float hud_scale_x = 0;
	float hud_scale_y = 0;
};