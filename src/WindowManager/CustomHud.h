#pragma once
#include "../charobj.h"

class CustomHud
{
public:
	CustomHud(float hud_scale_x, float hud_scale_y);
	void Update(bool show_custom_hud, bool show_main_window);
private:
	void UpdateHP(CharObj *CharInstance, bool right_side = false);
	void UpdateTimer(int *timer);
	void UpdateMeters(int cur_skill_val, int cur_cross_val, int cur_blaze_val,
		bool is_blaze_available, bool is_blaze_active, bool right_side = false);
	void UpdateCharSpecificMeters(CharObj *CharInstance, bool right_side = false);
	float hud_scale_x;
	float hud_scale_y;
};