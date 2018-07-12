#include "WindowManager.h"
#include "fonts.h"
#include "../Settings.h"
#include "../utils.h"
#include "../update_check.h"
#include "../SteamApiWrapper/SteamApiHelper.h"
#include "../logger.h"
#include "../Game/gamestates.h"
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <shellapi.h>
#include <sstream>
#include <time.h>

#define MAX_LOG_MSG_LEN 1024
#define MAIN_WINDOW_DISAPPEAR_TIME_SECONDS 15.0f
#define DEFAULT_ALPHA 0.87f

bool WindowManager::IsUpdateAvailable = false;
bool WindowManager::DoLogging = true;
bool WindowManager::Initialized = false;
CustomHud* WindowManager::m_customHud = 0;
PaletteEditor* WindowManager::m_paletteEditor = 0;

bool show_main_window = true;
bool show_demo_window = false;
bool show_notification = false;
bool show_notification_window = false;
bool show_log_window = false;
bool show_debug_window = false;
bool show_custom_hud = false;
bool *NO_CLOSE_FLAG = NULL;

bool show_palette_editor = false;

float main_window_disappear_time = MAIN_WINDOW_DISAPPEAR_TIME_SECONDS;

std::string notificationText;
float notificationTimer = 0;

std::string main_title = "";

int toggle_key;
int toggleHUD_key;
int toggleCustomHUD_key;

ImVec2 middlescreen;

// Usage:
//  static ExampleAppLog my_log;
//  my_log.AddLog("Hello %d world\n", 123);
//  my_log.Draw("title");
struct ImGuiLog
{
	ImGuiTextBuffer     Buf;
	ImGuiTextFilter     Filter;
	ImVector<int>       LineOffsets;        // Index to lines offset
	float				PrevScrollMaxY = 0;

	void _Clear() { Buf.clear(); LineOffsets.clear(); }

	void _AddLog(const char* fmt, ...) IM_FMTARGS(2)
	{
		int old_size = Buf.size();
		va_list args;
		va_start(args, fmt);
		Buf.appendfv(fmt, args);
		va_end(args);
		for (int new_size = Buf.size(); old_size < new_size; old_size++)
		{
			if (Buf[old_size] == '\n')
			{
				LineOffsets.push_back(old_size);
			}
		}
	}

	void _Draw(const char* title, bool* p_open = NULL)
	{
		ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
		ImGui::Begin(title, p_open, ImGuiWindowFlags_NoCollapse);
		if (ImGui::Button("Clear")) _Clear();
		ImGui::SameLine();
		bool copy = ImGui::Button("Copy");
		ImGui::SameLine();
		Filter.Draw("Filter", -100.0f);
		ImGui::Separator();
		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		if (copy)
		{
			ImGui::LogToClipboard();
			WindowManager::AddLog("[system] Log has been copied to clipboard\n");
		}

		if (Filter.IsActive())
		{
			const char* buf_begin = Buf.begin();
			const char* line = buf_begin;
			for (int line_no = 0; line != NULL; line_no++)
			{
				const char* line_end = (line_no < LineOffsets.Size) ? buf_begin + LineOffsets[line_no] : NULL;
				if (Filter.PassFilter(line, line_end))
				{
					ImGui::TextUnformatted(line, line_end);
					//ImGui::TextWrapped(line, line_end); causes a line limit in the logging for some reason
				}
				line = line_end && line_end[1] ? line_end + 1 : NULL;
			}
		}
		else
		{
			ImGui::TextUnformatted(Buf.begin());
			//ImGui::TextWrapped(Buf.begin()); //causes a line limit in the logging for some reason
		}

		//handle automatic scrolling
		if (PrevScrollMaxY < ImGui::GetScrollMaxY())
		{
			//scroll down automatically only if we didnt scroll up or we closed the window
			if (ImGui::GetScrollY() >= PrevScrollMaxY - 5)
			{
				ImGui::SetScrollY(ImGui::GetScrollMaxY());
			}
			PrevScrollMaxY = ImGui::GetScrollMaxY();
		}

		ImGui::EndChild();
		ImGui::End();
	}

	void _ToFile(FILE *file)
	{
		fprintf(file, "%s", Buf.begin());
	}
};

ImGuiLog WindowManager::Log;

void WindowManager::OnMatchInit()
{
	if (!Initialized || !m_paletteEditor)
		return;

	m_paletteEditor->OnMatchInit();

	g_interfaces.pPaletteManager->OnMatchInit(
		g_interfaces.player1.GetChar1().GetPalHandle(),
		g_interfaces.player1.GetChar2().GetPalHandle(),
		g_interfaces.player2.GetChar1().GetPalHandle(),
		g_interfaces.player2.GetChar2().GetPalHandle());
}

void WindowManager::SetMainWindowTitle(const char *text)
{
	if (text)
		main_title = text;
	else
	{
		main_title = MOD_WINDOW_TITLE;
		main_title += " ";
		main_title += MOD_VERSION_NUM;
#ifndef RELEASE_VER
		main_title += " (DEBUG)";
#endif
	}
	main_title += "###MainTitle"; //set unique identifier
}

bool WindowManager::Init(void *hwnd, IDirect3DDevice9 *device)
{
	if (Initialized)
		return true;

	LOG(2, "WindowManager::Init\n");

	if (!hwnd)
	{
		LOG(2, "HWND not found!\n");
		return false;
	}
	if (!device)
	{
		LOG(2, "Direct3DDevice9 not found!\n");
		return false;
	}

	bool ret = ImGui_ImplDX9_Init(hwnd, device);
	if (!ret)
	{
		LOG(2, "ImGui_ImplDX9_Init failed!\n");
		return false;
	}

	SetMainWindowTitle();

	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowBorderSize = 1;
	style.FrameBorderSize = 1;
	style.ScrollbarSize = 14;
	style.Alpha = DEFAULT_ALPHA;

	//disable menu.ini saving?
	//ImGui::GetIO().IniFilename = NULL;

	if (Settings::settingsIni.menusize == 1)
	{
		ImFont* font = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(TinyFont_compressed_data_base85, 10);
		font->DisplayOffset.y += 1;
	}
	else if (Settings::settingsIni.menusize == 3)
		ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(DroidSans_compressed_data, DroidSans_compressed_size, 20);

	Initialized = true;
	WindowManager::AddLog("[system] Initialization starting...\n");


	toggle_key = Settings::getButtonValue(Settings::settingsIni.togglebutton);
	WindowManager::AddLog("[system] Toggling key set to '%s'\n", Settings::settingsIni.togglebutton.c_str());

	toggleHUD_key = Settings::getButtonValue(Settings::settingsIni.toggleHUDbutton);
	WindowManager::AddLog("[system] HUD toggling key set to '%s'\n", Settings::settingsIni.toggleHUDbutton.c_str());

	toggleCustomHUD_key = Settings::getButtonValue(Settings::settingsIni.togglecustomHUDbutton);
	WindowManager::AddLog("[system] CustomHUD toggling key set to '%s'\n", Settings::settingsIni.togglecustomHUDbutton.c_str());

	show_custom_hud = Settings::settingsIni.forcecustomhud;

	middlescreen = ImVec2((float)Settings::settingsIni.renderwidth / 2, (float)Settings::settingsIni.renderheight / 2);
	//dividing by 1904x1042 because the custom HUD was designed on that resolution
	float hud_scale_x = ((float)Settings::settingsIni.renderwidth * Settings::settingsIni.customhudscale) / 1904.0f;
	float hud_scale_y = ((float)Settings::settingsIni.renderheight * Settings::settingsIni.customhudscale) / 1042.0f;
	LOG(2, "hud_scale_x: %f\n", hud_scale_x);
	LOG(2, "hud_scale_y: %f\n", hud_scale_y);

	m_customHud = new CustomHud(hud_scale_x, hud_scale_y);
	m_paletteEditor = new PaletteEditor();
	g_interfaces.pPaletteManager->LoadPalettesFromFolder();

	if (Settings::settingsIni.checkupdates)
	{
		HANDLE thread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CheckUpdate, NULL, NULL, NULL);
		if(thread)
			CloseHandle(thread);
	}

	if (g_interfaces.pSteamUserStatsWrapper && g_interfaces.pSteamFriendsWrapper)
		g_interfaces.pSteamApiHelper = new SteamApiHelper(g_interfaces.pSteamUserStatsWrapper, g_interfaces.pSteamFriendsWrapper);

	//Add the default font
	ImFontConfig cfg;
	cfg.SizePixels = 13;
	ImGui::GetIO().Fonts->AddFontDefault(&cfg);// ->DisplayOffset.y = SCALE;

	//Add our font
	ImFontConfig config;
	config.OversampleH = 3;
	config.OversampleV = 3;
	config.GlyphExtraSpacing.x = 1.0f;
	strcpy(config.Name, "CustomHUD");
	float fontsize = 30.0f;
	fontsize *= hud_scale_y;
	LOG(2, "CustomHUD fontsize: %f\n", fontsize);
	ImFont* font = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(TinyFont_compressed_data_base85, fontsize, &config);
	font->DisplayOffset.y += 1;

	/*Calling a frame to initialize beforehand to prevent a crash upon first call of
	/Update if the game window is not focused. Simply calling ImGui_ImplDX9_CreateDeviceObjects() might be enough too.*/
	ImGui_ImplDX9_NewFrame();
	ImGui::EndFrame();
	///////

	WindowManager::AddLog("[system] Finished initialization\n");
	WindowManager::AddLogSeparator();

	LOG(2, "Init end\n");
	return ret;
}

void WindowManager::Shutdown()
{
	if (!Initialized)
		return;

	LOG(2, "WindowManager::Shutdown\n");
	WriteLogToFile();

	SAFE_DELETE(m_customHud);
	SAFE_DELETE(m_paletteEditor);

	ImGui_ImplDX9_Shutdown();
}

void WindowManager::InvalidateDeviceObjects()
{
	if (!Initialized)
		return;

	LOG(2, "WindowManager::InvalidateDeviceObjects\n");
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

void WindowManager::CreateDeviceObjects()
{
	if (!Initialized)
		return;

	LOG(2, "WindowManager::CreateDeviceObjects\n");
	ImGui_ImplDX9_CreateDeviceObjects();
}

void WindowManager::Render()
{
	if (!Initialized)
		return;

	LOG(7, "WindowManager::Render\n");
	ImGui::Render();
}

void WindowManager::Update()
{
	if (!Initialized)
		return;

	LOG(7, "WindowManager::HandleImGui\n");

	//allowing palette updates
	g_interfaces.pPaletteManager->UnlockUpdates(
		g_interfaces.player1.GetChar1().GetPalHandle(),
		g_interfaces.player1.GetChar2().GetPalHandle(),
		g_interfaces.player2.GetChar1().GetPalHandle(),
		g_interfaces.player2.GetChar2().GetPalHandle());

	//constantly overriding the visibility of the game's HUD if the custom hud is forced on
	if (Settings::settingsIni.forcecustomhud)
	{
		if (g_gameVals.pIsHUDHidden)
		{
			if (*g_gameVals.pIsHUDHidden == 0)
				*g_gameVals.pIsHUDHidden = 1;
		}
	}

	//start making the mod's menu disappear upon start, within MAIN_WINDOW_DISAPPEAR_TIME_SECONDS
	if (main_window_disappear_time > 0)
	{
		main_window_disappear_time -= ImGui::GetIO().DeltaTime;
		
		if(main_window_disappear_time > 0)
			ImGui::GetStyle().Alpha = main_window_disappear_time / MAIN_WINDOW_DISAPPEAR_TIME_SECONDS;

		//disappear upon reaching main_window_disappear_time = 0
		if (main_window_disappear_time <= 0)
		{
			show_main_window = false;
			ImGui::GetStyle().Alpha = DEFAULT_ALPHA;
		}
	}

	//return if game window is minimized, to avoid the custom hud elements being thrown in the upper left corner due to resolution shrinking
	if (IsIconic(g_gameProc.hWndGameWindow))
		return;

	HandleButtons();

	ImGui_ImplDX9_NewFrame();

	m_customHud->Update(show_custom_hud, show_main_window);

	//First run settings
	ImGui::SetWindowPos(main_title.c_str(), ImVec2(12, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!

	if (Settings::settingsIni.menusize == 1)
		ImGui::SetNextWindowSizeConstraints(ImVec2(250, 190), ImVec2(1000, 1000));
	else if (Settings::settingsIni.menusize == 3)
		ImGui::SetNextWindowSizeConstraints(ImVec2(400, 230), ImVec2(1000, 1000));
	else
		ImGui::SetNextWindowSizeConstraints(ImVec2(330, 230), ImVec2(1000, 1000));

	ImGuiIO& io = ImGui::GetIO();

	io.MouseDrawCursor = show_main_window |show_log_window | show_notification_window | show_palette_editor | IsUpdateAvailable | show_demo_window;

	if (Settings::settingsIni.viewportoverride == VIEWPORT_OVERRIDE)
	{
		io.DisplaySize = ImVec2((float)Settings::settingsIni.renderwidth, (float)Settings::settingsIni.renderheight);
	}

	if (show_main_window)
	{
		ImGui::Begin(main_title.c_str(), NO_CLOSE_FLAG, ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::Text("Toggle me with %s", Settings::settingsIni.togglebutton.c_str());
		ImGui::Text("Toggle HUD with %s", Settings::settingsIni.toggleHUDbutton.c_str());
		ImGui::Text("Toggle Custom HUD with %s", Settings::settingsIni.togglecustomHUDbutton.c_str());
		ImGui::Separator();
		ImGui::Text("");

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

		if (ImGui::CollapsingHeader("Custom Palettes"))
		{
			if (*g_gameVals.pGameState != GameState_Match)
			{
				ImGui::Text(" "); ImGui::SameLine(); ImGui::TextDisabled("Not in match!");
			}
			else
				m_paletteEditor->ShowAllPaletteSelections();

			ImGui::Text(""); ImGui::Text(" "); ImGui::SameLine();
			m_paletteEditor->ShowReloadAllPalettesButton();

			ImGui::Text(" "); ImGui::SameLine();
			bool pressed = ImGui::Button("Palette editor");

			if(*g_gameVals.pGameMode != GameMode_Tutorial)
			{
				ImGui::SameLine(); ImGui::TextDisabled("Not in tutorial mode!");
			}
			else
			{
				if (pressed)
				{
					show_palette_editor ^= 1;
				}
			}
		}

		if (ImGui::CollapsingHeader("Loaded settings.ini values"))
		{
			ShowLoadedIniSettings();
		}

#ifndef RELEASE_VER
		if (ImGui::Button("Demo"))
			show_demo_window ^= 1;
		if (ImGui::Button("DEBUG"))
			show_debug_window ^= 1;
#endif
		if (ImGui::Button("Log"))
			show_log_window ^= 1;

		ImGui::Text("Current online players:"); ImGui::SameLine();
		if (g_interfaces.pSteamApiHelper)
		{
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s",
				g_interfaces.pSteamApiHelper->current_players <= 0 ? "<No data>" : std::to_string(g_interfaces.pSteamApiHelper->current_players).c_str());
		}
		else
		{
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s", "<No data>");
		}

		if (ImGui::Button("Discord"))
			ShellExecute(NULL, L"open", MOD_LINK_DISCORD, NULL, NULL, SW_SHOWNORMAL);
		ImGui::SameLine();
		if (ImGui::Button("Forum"))
			ShellExecute(NULL, L"open", MOD_LINK_FORUM, NULL, NULL, SW_SHOWNORMAL);
		ImGui::SameLine();
		if (ImGui::Button("Nexusmods"))
			ShellExecute(NULL, L"open", MOD_LINK_NEXUSMODS, NULL, NULL, SW_SHOWNORMAL);
		ImGui::SameLine();
		if(ImGui::Button("GitHub"))
			ShellExecute(NULL, L"open", MOD_LINK_GITHUB, NULL, NULL, SW_SHOWNORMAL);

		ImGui::End();
	}

	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, DEFAULT_ALPHA);

	// 3. Show the other windows!
#ifndef RELEASE_VER
	if (show_demo_window)
	{
		ImGui::SetNextWindowPos(ImVec2(550, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
		ImGui::ShowDemoWindow(&show_demo_window);
	}

	if (show_debug_window)
		ShowDebugWindow(&show_debug_window);

	if (show_palette_editor)
		m_paletteEditor->ShowPaletteEditorWindow(&show_palette_editor);
#endif

	if (show_notification)
		HandleNotification();

	if (show_log_window && show_main_window)
		ShowLogWindow(&show_log_window);

	if (IsUpdateAvailable)
		ShowUpdateWindow();

	ImGui::PopStyleVar();

	//ImGui::EndFrame();

	LOG(7, "END OF WindowManager::HandleImGui\n");
}

void WindowManager::SetNotification(const char *text, float timeToShowInSec, bool showNotificationWindow)
{
	if (!Initialized)
		return;

	notificationText = text;
	notificationTimer = timeToShowInSec;
	show_notification = true;
	show_notification_window = showNotificationWindow & Settings::settingsIni.notificationpopups;
}

void WindowManager::HandleNotification()
{
	std::ostringstream stringBuf;
	stringBuf << notificationText << " (" << round(ceil(notificationTimer)) << ")";
	SetMainWindowTitle(stringBuf.str().c_str());

	if (notificationTimer < 0.0f)
	{
		show_notification_window = false;
		show_notification = false;
		SetMainWindowTitle(); // reset title to default
	}

	if (show_notification_window)
		ShowNotificationWindow();

	notificationTimer -= ImGui::GetIO().DeltaTime;
}

void WindowManager::ShowNotificationWindow()
{
	//middle of screen
	ImGui::SetNextWindowPosCenter(ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSizeConstraints(ImVec2(200, 50), ImVec2(500, 500));
	ImVec2 OK_btn_size = ImVec2(100, 30);

	ImGui::Begin("Notification", NO_CLOSE_FLAG, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::Text(notificationText.c_str());
	//ImGui::CalcTextSize(notificationText.c_str()); ImGui::SetCursorPosX(ImGui::CalcTextSize(notificationText.c_str()).x);

	std::ostringstream stringBuf;
	stringBuf << "OK (" << round(ceil(notificationTimer)) << ")";
	std::string timeLeft(stringBuf.str());

	ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - (OK_btn_size.x / 2));

	if (ImGui::Button(timeLeft.c_str(), OK_btn_size))
	{
		show_notification_window = false;
		notificationTimer = -0.2f; //setting it below 0
	}

	ImGui::End();
}

// start with type a of message: "[system]", "[info]", "[warning]", "[error]", "[fatal]", "[notice]", "[log]"
void WindowManager::AddLog(const char* message, ...)
{
	if (!Initialized || !message || !DoLogging) 
	{ 
		return; 
	}

	//Get current time into a string
	//////
	time_t current_time;
	struct tm * time_info;
	char timeString[9];  // space for "HH:MM:SS\0"

	time(&current_time);
	time_info = localtime(&current_time);

	strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);
	/////

	if (strlen(message) > MAX_LOG_MSG_LEN)
	{
		LOG(2, "AddLog error: message too long!\nmessage: %s", message);
		WindowManager::Log._AddLog("%s [error] Log message too long.", timeString);
		return;
	}

	std::string fullMessage(timeString);

	char buf[MAX_LOG_MSG_LEN];
	va_list args;
	va_start(args, message);
	vsprintf(buf, message, args);
	va_end(args);

	fullMessage += " ";
	fullMessage += buf;

	WindowManager::Log._AddLog(fullMessage.c_str());
}

void WindowManager::AddLogSeparator()
{
	if (DoLogging)
		WindowManager::Log._AddLog("------------------------------------------------------------------\n");
}

void WindowManager::WriteLogToFile()
{
	if (!Initialized)
		return;

	FILE *file = fopen("BBTAG_IM\\log.txt", "a");

	if (!file)
	{
		LOG(2, "WindowManager::WriteLogToFile file opening failed!!\n");
		return;
	}

	char* time = getFullDate();

	if (time)
	{
		fprintf(file, "BBTAGIM %s -- %s\n", MOD_VERSION_NUM, time);
		free(time);
	}
	else
	{
		fprintf(file, "{Couldn't get the current time}\n");
	}

	fprintf(file, "-------------------------------------\n\n");
	fprintf(file, "settings.ini config:\n");

	//TODO: Put the strings into the X-Macro as well. Somehow...
	//strings cant be put into the X-Macro (.c_str() cannot be put on non-std::string types)
	fprintf(file, "\t- ToggleButton: %s\n", Settings::settingsIni.togglebutton.c_str());
	fprintf(file, "\t- ToggleHUDButton: %s\n", Settings::settingsIni.toggleHUDbutton.c_str());
	fprintf(file, "\t- ToggleCustomHUDButton: %s\n", Settings::settingsIni.togglecustomHUDbutton.c_str());

	std::string printText = "";

	//X-Macro
#define SETTING(_type, _var, _inistring, _defaultval) \
	printText = "\t- "; \
	printText += _inistring; \
	if(strcmp(#_type, "bool") == 0 || strcmp(#_type, "int") == 0) \
		printText += ": %d\n"; \
	else if(strcmp(#_type, "float") == 0) \
		printText += ": %.2f\n"; \
	if(strcmp(#_type, "std::string") != 0) \
		fprintf(file, printText.c_str(), Settings::settingsIni.##_var);
#include "../settings.def"
#undef SETTING

	fprintf(file, "\n");

	//d3dparams here

	WindowManager::Log._ToFile(file);
	fprintf(file, "\n#####################################\n\n\n");

	fclose(file);
}

void WindowManager::ShowLogWindow(bool* p_open)
{
	WindowManager::Log._Draw("Log", p_open);
}

void WindowManager::ShowUpdateWindow()
{
	//middle of screen
	ImGui::SetNextWindowPosCenter(ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSizeConstraints(ImVec2(200, 50), ImVec2(500, 500));
	ImVec2 OK_btn_size = ImVec2(100, 30);

	ImGui::Begin("Update available", &IsUpdateAvailable, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

	ImGui::Text("BBTAG Improvement Mod %s has been released!", GetNewVersionNum().c_str());
	ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - (OK_btn_size.x / 2));

	if (ImGui::Button("OK", OK_btn_size))
	{
		IsUpdateAvailable = false;
	}
	ImGui::End();
}

void WindowManager::ShowLoadedIniSettings()
{
	//not using ImGui columns here because they are bugged if the window has always_autoresize flag. The window 
	//starts extending to infinity, if the left edge of the window touches any edges of the screen

	//TODO: Put the strings into the X-Macro as well. Somehow...
	//strings cant be put into the X-Macro (.c_str() cannot be put on non-std::string types)
	ImGui::Separator();
	ImGui::Text(" ToggleButton"); ImGui::SameLine(ImGui::GetWindowWidth() * 0.5f);
	ImGui::Text("= %s", Settings::settingsIni.togglebutton.c_str());
	ImGui::Separator();

	ImGui::Text(" ToggleHUDButton"); ImGui::SameLine(ImGui::GetWindowWidth() * 0.5f);
	ImGui::Text("= %s", Settings::settingsIni.toggleHUDbutton.c_str());
	ImGui::Separator();

	ImGui::Text(" ToggleCustomHUDButton"); ImGui::SameLine(ImGui::GetWindowWidth() * 0.5f);
	ImGui::Text("= %s", Settings::settingsIni.togglecustomHUDbutton.c_str());
	ImGui::Separator();

	std::string printText = "";

	//X-Macro
#define SETTING(_type, _var, _inistring, _defaultval) \
	if(strcmp(#_type, "std::string") != 0) { \
	printText = " "; \
	printText += _inistring; \
	ImGui::Text(printText.c_str()); ImGui::SameLine(ImGui::GetWindowWidth() * 0.5f); \
	if(strcmp(#_type, "bool") == 0 || strcmp(#_type, "int") == 0) \
		printText = "= %d"; \
	else if(strcmp(#_type, "float") == 0) \
		printText = "= %.2f"; \
	ImGui::Text(printText.c_str(), Settings::settingsIni.##_var); ImGui::Separator(); }
#include "../settings.def"
#undef SETTING

}

void WindowManager::ShowDebugWindow(bool * p_open)
{
	ImGui::Begin("DEBUG", p_open);

	if (ImGui::CollapsingHeader("Gameval addresses"))
	{
		if(!g_interfaces.player1.GetChar1().IsNullPtrCharData())
			ImGui::Text("P1Char1 Data 0x%p", g_interfaces.player1.GetChar1().GetData());

		if(!g_interfaces.player1.GetChar2().IsNullPtrCharData())
			ImGui::Text("P1Char2 Data 0x%p", g_interfaces.player1.GetChar2().GetData());

		if(!g_interfaces.player2.GetChar1().IsNullPtrCharData())
			ImGui::Text("P2Char1 Data 0x%p", g_interfaces.player2.GetChar1().GetData());

		if(!g_interfaces.player2.GetChar2().IsNullPtrCharData())
			ImGui::Text("P2Char2 Data 0x%p", g_interfaces.player2.GetChar2().GetData());

		ImGui::Separator();
		ImGui::Text("P1Meters 0x%p", g_interfaces.player1.GetMeters());
		ImGui::Text("P2Meters 0x%p", g_interfaces.player2.GetMeters());

		ImGui::Separator();
		//ImGui::Text("PalIndex_P1Char1 0x%p", &(*g_interfaces.pPaletteManager)[CharPalInfoIndex::P1Char1]->GetPalIndexRef());
		//if (g_gameVals.PalIndex_P1Char1)

			//ImGui::SliderInt("PalIndex_P1Char1", &(*g_interfaces.pPaletteManager)[CharPalInfoIndex::P1Char1]->GetPalIndexRef(), 0, 15);

			//if (ImGui::CollapsingHeader("P1Char1 palarray"))
			//{
			//	ImGui::Text("%p", (*g_interfaces.pPaletteManager)[CharPalInfoIndex::P1Char1]->getPaletteArray(NULL, 0, 0));
			//}

		//ImGui::Text("PalIndex_P1Char2 0x%p", g_gameVals.PalIndex_P1Char2);
		//if (g_gameVals.PalIndex_P1Char2)
		//	ImGui::SliderInt("PalIndex_P1Char2", g_gameVals.PalIndex_P1Char2, 0, 15);

		//ImGui::Text("PalIndex_P2Char1 0x%p", g_gameVals.PalIndex_P2Char1);
		//if (g_gameVals.PalIndex_P2Char1)
		//	ImGui::SliderInt("PalIndex_P2Char1", g_gameVals.PalIndex_P2Char1, 0, 15);

		//ImGui::Text("PalIndex_P2Char2 0x%p", g_gameVals.PalIndex_P2Char2);
		//if (g_gameVals.PalIndex_P2Char2)
		//	ImGui::SliderInt("PalIndex_P2Char2", g_gameVals.PalIndex_P2Char2, 0, 15);

		//ImGui::Separator();
		//ImGui::Text("PalBaseAddrP1Char1 0x%p", g_gameVals.PalBaseAddrP1Char1);
		//ImGui::Text("PalBaseAddrP1Char2 0x%p", g_gameVals.PalBaseAddrP1Char2);
		//ImGui::Text("PalBaseAddrP2Char1 0x%p", g_gameVals.PalBaseAddrP2Char1);
		//ImGui::Text("PalBaseAddrP2Char2 0x%p", g_gameVals.PalBaseAddrP2Char2);


		ImGui::Separator();
		ImGui::Text("pGameState: 0x%p : %d", g_gameVals.pGameState, *g_gameVals.pGameState);
		ImGui::Text("pGameMode: 0x%p : %d", g_gameVals.pGameMode, *g_gameVals.pGameMode);
		ImGui::Text("pIsHUDHidden: 0x%p", g_gameVals.pIsHUDHidden);
		if (g_gameVals.pIsHUDHidden)
			ImGui::Checkbox("pIsHUDHidden", (bool*)g_gameVals.pIsHUDHidden);
	}
	if (ImGui::CollapsingHeader("Color tester"))
	{
		static float col[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
		ImVec4 color(col[0], col[1], col[2], col[3]);
		ImGui::TextColored(color, "ABCD 0123");
		ImGui::ColoredProgressBar(1.0f, ImVec2(-1.0f, 0.0f), color, NULL, NULL, NULL);
		ImGui::ColorEdit4("ColEdit", col);
	}
	ImGui::End();
}

void WindowManager::HandleButtons()
{
	if (ImGui::IsKeyPressed(toggleHUD_key) && g_gameVals.pIsHUDHidden)
	{
		*g_gameVals.pIsHUDHidden ^= 1;
	}

	if (ImGui::IsKeyPressed(toggleCustomHUD_key))
	{
		show_custom_hud ^= 1;
	}

	if (ImGui::IsKeyPressed(toggle_key))
	{
		show_main_window ^= 1;
		ImGui::GetStyle().Alpha = DEFAULT_ALPHA;
		main_window_disappear_time = 0;
	}
}