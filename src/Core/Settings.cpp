#include "Settings.h"

#include "interfaces.h"
#include "logger.h"
#include <ctime>
#include <atlstr.h>

settingsIni_t Settings::settingsIni = {};
savedSettings_t Settings::savedSettings = {};

void Settings::applySettingsIni(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	//Settings::savedSettings.origPresentationParameters = *pPresentationParameters;

	if (settingsIni.viewportoverride == VIEWPORT_OVERRIDE)
	{
		pPresentationParameters->BackBufferHeight = settingsIni.renderheight;
		pPresentationParameters->BackBufferWidth = settingsIni.renderwidth;

		switch (Settings::settingsIni.displaymode)
		{
		case DISPLAYMODE_BORDERLESS:
			//borderlesss
			pPresentationParameters->Windowed = true;
			break;
		case DISPLAYMODE_FULLSCREEN:
			//fullscreen
			pPresentationParameters->Windowed = false;
			break;
			//windowed
		case DISPLAYMODE_WINDOWED:
		default:
			pPresentationParameters->Windowed = true;
		}
	}

	switch (Settings::settingsIni.antialiasing)
	{
	case 0:
		pPresentationParameters->MultiSampleType = D3DMULTISAMPLE_NONE;
		pPresentationParameters->MultiSampleQuality = 0;
		break;
		//case 2:
		//	pPresentationParameters->MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
		//	break;
	case 4:
		pPresentationParameters->MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
		break;
	case 5:
	default:
		break;
	}

	switch (settingsIni.vsync)
	{
	case VSYNC_OFF:
		//vsync off
		pPresentationParameters->PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		break;
	case VSYNC_ON:
		// vsync on 
		pPresentationParameters->PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		break;
	}
	
	if (!pPresentationParameters->Windowed)
	{
		pPresentationParameters->FullScreen_RefreshRateInHz = settingsIni.fullscreenhz; // savedSettings.adapterRefreshRate;
	}

	//Settings::savedSettings.newPresentationParameters = *pPresentationParameters;
}


int Settings::readSettingsFilePropertyInt(LPCWSTR key, LPCWSTR defaultVal, LPCWSTR filename)
{
	CString strNotificationPopups;
	GetPrivateProfileString(_T("Settings"), key, defaultVal, strNotificationPopups.GetBuffer(MAX_PATH), MAX_PATH, filename);
	strNotificationPopups.ReleaseBuffer();
	return _ttoi(strNotificationPopups);
}

float Settings::readSettingsFilePropertyFloat(LPCWSTR key, LPCWSTR defaultVal, LPCWSTR filename)
{
	CString strCustomHUDScale;
	GetPrivateProfileString(_T("Settings"), key, defaultVal, strCustomHUDScale.GetBuffer(MAX_PATH), MAX_PATH, filename);
	strCustomHUDScale.ReleaseBuffer();
	return _ttof(strCustomHUDScale);
}

const char* Settings::readSettingsFilePropertyString(LPCWSTR key, LPCWSTR defaultVal, LPCWSTR filename)
{
	CString strBuffer;
	GetPrivateProfileString(_T("Settings"), key, defaultVal, strBuffer.GetBuffer(MAX_PATH), MAX_PATH, filename);
	strBuffer.ReleaseBuffer();
	CT2CA pszConvertedAnsiString(strBuffer);
	return pszConvertedAnsiString;
}

bool Settings::loadSettingsFile()
{
	CString strINIPath;

	_wfullpath((wchar_t*)strINIPath.GetBuffer(MAX_PATH), L"settings.ini", MAX_PATH);
	strINIPath.ReleaseBuffer();

	if (GetFileAttributes(strINIPath) == 0xFFFFFFFF)
	{
		MessageBoxA(NULL, "Settings INI File Was Not Found!", "Error", MB_OK);
		return false;
	}

void *iniPtr = 0;

//X-Macro
#define SETTING(_type, _var, _inistring, _defaultval) \
	iniPtr = &settingsIni.##_var; \
	if(strcmp(#_type, "bool") == 0 || strcmp(#_type, "int") == 0) { \
		*(int*)iniPtr = readSettingsFilePropertyInt(L##_inistring, L##_defaultval, strINIPath); } \
	else if(strcmp(#_type, "float") == 0) { \
		*(float*)iniPtr = readSettingsFilePropertyFloat(L##_inistring, L##_defaultval, strINIPath); } \
	else if (strcmp(#_type, "std::string") == 0) { \
		*(std::string*)iniPtr = readSettingsFilePropertyString(L##_inistring, L##_defaultval, strINIPath); }
#include "settings.def"
#undef SETTING

//set buttons back to default if their values are incorrect
if (settingsIni.togglebutton.length() > 2 || settingsIni.togglebutton[0] != 'F')
	settingsIni.togglebutton = "F1";

if (settingsIni.toggleHUDbutton.length() > 2 || settingsIni.toggleHUDbutton[0] != 'F')
	settingsIni.toggleHUDbutton = "F2";

if (settingsIni.togglecustomHUDbutton.length() > 2 || settingsIni.togglecustomHUDbutton[0] != 'F')
	settingsIni.togglecustomHUDbutton = "F3";

	return true;
}

void Settings::initSavedSettings()
{
	LOG(7, "initSavedSettings\n");
	switch (settingsIni.viewportoverride)
	{
	case VIEWPORT_OVERRIDE:
		LOG(7, " - case 2\n");
		savedSettings.newSourceRect.right = settingsIni.renderwidth;
		savedSettings.newSourceRect.bottom = settingsIni.renderheight;
		savedSettings.newViewport.Width = settingsIni.renderwidth;
		savedSettings.newViewport.Height = settingsIni.renderheight;
		break;
	case VIEWPORT_DEFAULT:
	default:
		LOG(7, " - case 1, default\n");
		//in this case the value is set in Direct3DDevice9ExWrapper::CreateRenderTargetEx!
		break;
	}

	savedSettings.origViewport.x = 0.0;
	savedSettings.origViewport.y = 0.0;
}

short Settings::getButtonValue(std::string button)
{
	if (button == "F1")
		return 112;
	else if (button == "F2")
		return 113;
	else if (button == "F3")
		return 114;
	else if (button == "F4")
		return 115;
	else if (button == "F5")
		return 116;
	else if (button == "F6")
		return 117;
	else if (button == "F7")
		return 118;
	else if (button == "F8")
		return 119;
	else if (button == "F9")
		return 120;
	else//default to F1
	{
		button = "F1";
		return 112;
	}
}