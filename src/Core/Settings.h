#pragma once
#include "D3D9EXWrapper/d3d9.h"
#include <string>

#define DISPLAYMODE_WINDOWED 1
#define DISPLAYMODE_BORDERLESS 2
#define DISPLAYMODE_FULLSCREEN 3
#define VSYNC_OFF 0
#define VSYNC_ON 1
#define VIEWPORT_DEFAULT 0
#define VIEWPORT_OVERRIDE 1
#define FORCE_PROMPT_LAYOUT_STEAM_CONTROLLER 0
#define FORCE_PROMPT_LAYOUT_KEYBOARD 1
#define FORCE_PROMPT_LAYOUT_OFF 2
#define NOTIFICATIONPOPUPS_OFF 0
#define NOTIFICATIONSPOPUPS_ON 1
#define CHECKUPDATES_OFF 0
#define CHECKUPDATES_ON 1

struct settingsIni_t
{
	//X-Macro
#define SETTING(_type, _var, _inistring, _defaultval) \
	_type _var
#include "settings.def"
#undef SETTING
};

struct savedSettings_t
{
	RECT newSourceRect;
	D3DVIEWPORT9 newViewport;
	//used to scale the sprite sizes in ID3D9Wrapper_Sprite
	D3DXVECTOR2 origViewport;
	D3DPRESENT_PARAMETERS origPresentationParameters;
	D3DPRESENT_PARAMETERS newPresentationParameters;
};

class Settings
{
public:
	static settingsIni_t settingsIni;
	static savedSettings_t savedSettings;

	static void applySettingsIni(D3DPRESENT_PARAMETERS* pPresentationParameters);
	static bool loadSettingsFile();
	static void initSavedSettings();
	//static void setViewportSize(D3DVIEWPORT9 Viewport);
	static short getButtonValue(std::string button);
private:
	static int readSettingsFilePropertyInt(LPCWSTR key, LPCWSTR defaultVal, LPCWSTR filename);
	static float readSettingsFilePropertyFloat(LPCWSTR key, LPCWSTR defaultVal, LPCWSTR filename);
	static const char* readSettingsFilePropertyString(LPCWSTR key, LPCWSTR defaultVal, LPCWSTR filename);
};