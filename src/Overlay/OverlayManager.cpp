#include "OverlayManager.h"

#include "fonts.h"

#include "Core/info.h"
#include "Core/interfaces.h"
#include "Core/logger.h"
#include "Core/Settings.h"
#include "Core/utils.h"
#include "Game/gamestates.h"
#include "SteamApiWrapper/SteamApiHelper.h"
#include "Web/update_check.h"
#include "Web/donators_fetch.h"

#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <shellapi.h>
#include <sstream>
#include <time.h>

#define MAX_LOG_MSG_LEN 1024
#define MAIN_WINDOW_DISAPPEAR_TIME_SECONDS 15.0f
#define DEFAULT_ALPHA 0.87f

OverlayManager* OverlayManager::m_instance = nullptr;
bool OverlayManager::Initialized = false;

bool show_main_window = true;
bool show_demo_window = false;
bool show_notification = false;
bool show_notification_window = false;
bool show_log_window = false;
bool show_debug_window = false;
bool show_donators_window = false;
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
			OverlayManager::getInstance().AddLog("[system] Log has been copied to clipboard\n");
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

ImGuiLog OverlayManager::m_log;

void OverlayManager::OnMatchInit()
{
	if (!Initialized || !m_paletteEditor)
		return;

	g_interfaces.pPaletteManager->OnMatchInit(
		g_interfaces.player1.GetChar1(),
		g_interfaces.player1.GetChar2(),
		g_interfaces.player2.GetChar1(),
		g_interfaces.player2.GetChar2());

	m_paletteEditor->OnMatchInit();
}

void OverlayManager::SetMainWindowTitle(const char *text)
{
	if (text)
		main_title = text;
	else
	{
		main_title = MOD_WINDOW_TITLE;
		main_title += " ";
		main_title += MOD_VERSION_NUM;
#ifdef _DEBUG
		main_title += " (DEBUG)";
#endif
	}
	main_title += "###MainTitle"; //set unique identifier
}

OverlayManager & OverlayManager::getInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new OverlayManager();
	}

	return *m_instance;
}

bool OverlayManager::Init(void *hwnd, IDirect3DDevice9 *device)
{
	if (Initialized)
		return true;

	LOG(2, "OverlayManager::Init\n");

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
	OverlayManager::getInstance().AddLog("[system] Initialization starting...\n");


	toggle_key = Settings::getButtonValue(Settings::settingsIni.togglebutton);
	OverlayManager::getInstance().AddLog("[system] Toggling key set to '%s'\n", Settings::settingsIni.togglebutton.c_str());

	toggleHUD_key = Settings::getButtonValue(Settings::settingsIni.toggleHUDbutton);
	OverlayManager::getInstance().AddLog("[system] HUD toggling key set to '%s'\n", Settings::settingsIni.toggleHUDbutton.c_str());

	toggleCustomHUD_key = Settings::getButtonValue(Settings::settingsIni.togglecustomHUDbutton);
	OverlayManager::getInstance().AddLog("[system] CustomHUD toggling key set to '%s'\n", Settings::settingsIni.togglecustomHUDbutton.c_str());

	show_custom_hud = Settings::settingsIni.forcecustomhud;

	middlescreen = ImVec2((float)Settings::settingsIni.renderwidth / 2, (float)Settings::settingsIni.renderheight / 2);
	//dividing by 1904x1042 because the custom HUD was designed on that resolution
	float hud_scale_x = ((float)Settings::settingsIni.renderwidth * Settings::settingsIni.customhudscale) / 1904.0f;
	float hud_scale_y = ((float)Settings::settingsIni.renderheight * Settings::settingsIni.customhudscale) / 1042.0f;
	LOG(2, "hud_scale_x: %f\n", hud_scale_x);
	LOG(2, "hud_scale_y: %f\n", hud_scale_y);

	m_customHud = new CustomHud(hud_scale_x, hud_scale_y);
	m_paletteEditor = new PaletteEditor();
	g_interfaces.pPaletteManager->LoadAllPalettes();

	if (Settings::settingsIni.checkupdates)
	{
		HANDLE updateThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CheckUpdate, NULL, NULL, NULL);
		if(updateThread)
			CloseHandle(updateThread);
	}

	HANDLE donatorsThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)FetchDonators, NULL, NULL, NULL);
	if (donatorsThread)
		CloseHandle(donatorsThread);

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

	srand(time(NULL));

	OverlayManager::getInstance().AddLog("[system] Finished initialization\n");
	OverlayManager::getInstance().AddLogSeparator();

	LOG(2, "Init end\n");
	return ret;
}

void OverlayManager::Shutdown()
{
	if (!Initialized)
		return;

	LOG(2, "OverlayManager::Shutdown\n");
	WriteLogToFile();

	SAFE_DELETE(m_customHud);
	SAFE_DELETE(m_paletteEditor);
	delete m_instance;

	ImGui_ImplDX9_Shutdown();
}

void OverlayManager::InvalidateDeviceObjects()
{
	if (!Initialized)
		return;

	LOG(2, "OverlayManager::InvalidateDeviceObjects\n");
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

void OverlayManager::CreateDeviceObjects()
{
	if (!Initialized)
		return;

	LOG(2, "OverlayManager::CreateDeviceObjects\n");
	ImGui_ImplDX9_CreateDeviceObjects();
}

void OverlayManager::OnRender()
{
	if (!Initialized)
		return;

	LOG(7, "OverlayManager::OnRender\n");
	ImGui::Render();
}

void OverlayManager::OnUpdate()
{
	if (!Initialized)
		return;

	LOG(7, "OverlayManager::HandleImGui\n");

	g_interfaces.pPaletteManager->OnUpdate(
		g_interfaces.player1.GetChar1().GetPalHandle(),
		g_interfaces.player1.GetChar2().GetPalHandle(),
		g_interfaces.player2.GetChar1().GetPalHandle(),
		g_interfaces.player2.GetChar2().GetPalHandle());

	HandleMainWindowVisibility(main_window_disappear_time);

	// return if game window is minimized, to avoid the custom hud elements
	// being thrown in the upper left corner due to resolution shrinking
	if (IsIconic(g_gameProc.hWndGameWindow))
		return;

	HandleButtons();

	ImGui_ImplDX9_NewFrame();

	m_customHud->OnUpdate(show_custom_hud, show_main_window);

	ImGuiIO& io = ImGui::GetIO();

	io.MouseDrawCursor = show_main_window |show_log_window | show_notification_window
		| show_palette_editor | IsUpdateAvailable | show_demo_window;

	if (Settings::settingsIni.viewportoverride == VIEWPORT_OVERRIDE)
	{
		io.DisplaySize = ImVec2((float)Settings::settingsIni.renderwidth, (float)Settings::settingsIni.renderheight);
	}

	ShowAllWindows();

	LOG(7, "END OF OverlayManager::HandleImGui\n");
}

void OverlayManager::SetNotification(const char *text, float timeToShowInSec, bool showNotificationWindow)
{
	if (!Initialized)
		return;

	notificationText = text;
	notificationTimer = timeToShowInSec;
	show_notification = true;
	show_notification_window = showNotificationWindow & Settings::settingsIni.notificationpopups;
}

OverlayManager::OverlayManager()
{
}

void OverlayManager::HandleNotification()
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

void OverlayManager::ShowNotificationWindow()
{
	ImGui::SetNextWindowPosCenter(ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSizeConstraints(ImVec2(200, 50), ImVec2(500, 500));
	ImVec2 OK_btn_size = ImVec2(100, 30);

	ImGui::Begin("Notification", NO_CLOSE_FLAG, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::Text(notificationText.c_str());

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
void OverlayManager::AddLog(const char* message, ...)
{
	if (!Initialized || !message || !m_loggingEnabled)
	{ 
		return; 
	}

	//Get current time into a string
	time_t current_time;
	struct tm * time_info;
	char timeString[9];  // extra space for nullbyte: "HH:MM:SS\0"

	time(&current_time);
	time_info = localtime(&current_time);

	strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);

	if (strlen(message) > MAX_LOG_MSG_LEN)
	{
		LOG(2, "AddLog error: message too long!\nmessage: %s", message);
		m_log._AddLog("%s [error] Log message too long.", timeString);
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

	m_log._AddLog(fullMessage.c_str());
}

void OverlayManager::AddLogSeparator()
{
	if (!m_loggingEnabled)
		return;

	m_log._AddLog("------------------------------------------------------------------\n");
}

void OverlayManager::DisableLogging()
{
	m_loggingEnabled = false;
}

void OverlayManager::EnableLogging()
{
	m_loggingEnabled = true;
}

void OverlayManager::WriteLogToFile()
{
	if (!Initialized)
		return;

	FILE *file = fopen("BBTAG_IM\\log.txt", "a");

	if (!file)
	{
		LOG(2, "OverlayManager::WriteLogToFile file opening failed!!\n");
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
#include "Core/settings.def"
#undef SETTING

	fprintf(file, "\n");

	//d3dparams here

	m_log._ToFile(file);
	fprintf(file, "\n#####################################\n\n\n");

	fclose(file);
}

void OverlayManager::ShowLogWindow(bool* p_open)
{
	m_log._Draw("Log", p_open);
}

void OverlayManager::ShowUpdateWindow()
{
	ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
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

void OverlayManager::ShowLoadedSettingsValues()
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
#include "Core/settings.def"
#undef SETTING

}

void OverlayManager::ShowDebugWindow(bool * p_open)
{
	ImGui::Begin("DEBUG", p_open);

	if (ImGui::CollapsingHeader("Gameval addresses"))
	{
		if(!g_interfaces.player1.GetChar1().IsCharDataNullPtr())
			ImGui::Text("pP1Char1_Data 0x%p", g_interfaces.player1.GetChar1().GetData());

		if(!g_interfaces.player1.GetChar2().IsCharDataNullPtr())
			ImGui::Text("pP1Char2_Data 0x%p", g_interfaces.player1.GetChar2().GetData());

		if(!g_interfaces.player2.GetChar1().IsCharDataNullPtr())
			ImGui::Text("pP2Char1_Data 0x%p", g_interfaces.player2.GetChar1().GetData());

		if(!g_interfaces.player2.GetChar2().IsCharDataNullPtr())
			ImGui::Text("pP2Char2_Data 0x%p", g_interfaces.player2.GetChar2().GetData());

		ImGui::Separator();
		ImGui::Text("pP1Meters 0x%p", g_interfaces.player1.GetMeters());
		ImGui::Text("pP2Meters 0x%p", g_interfaces.player2.GetMeters());

		ImGui::Separator();
		if (!g_interfaces.player1.GetChar1().IsCharDataNullPtr())
			ImGui::Text("P1Char1 meter: %d / %d", 
				g_interfaces.player1.GetChar1().GetData()->unique_meter_cur_val,
				g_interfaces.player1.GetChar1().GetData()->unique_meter_max_val);

		if (!g_interfaces.player1.GetChar2().IsCharDataNullPtr())
			ImGui::Text("P1Char2 meter: %d / %d", 
				g_interfaces.player1.GetChar2().GetData()->unique_meter_cur_val,
				g_interfaces.player1.GetChar2().GetData()->unique_meter_max_val);

		if (!g_interfaces.player2.GetChar1().IsCharDataNullPtr())
			ImGui::Text("P2Char1 meter: %d / %d", 
				g_interfaces.player2.GetChar1().GetData()->unique_meter_cur_val,
				g_interfaces.player2.GetChar1().GetData()->unique_meter_max_val);

		if (!g_interfaces.player2.GetChar2().IsCharDataNullPtr())
			ImGui::Text("P2Char2 meter: %d / %d", 
				g_interfaces.player2.GetChar2().GetData()->unique_meter_cur_val,
				g_interfaces.player2.GetChar2().GetData()->unique_meter_max_val);

		ImGui::Separator();
		ImGui::Text("pPalIndex_P1Char1 0x%p", &(g_interfaces.player1.GetChar1().GetPalHandle().GetPalIndexRef()));
		ImGui::Text("pPalIndex_P1Char2 0x%p", &(g_interfaces.player1.GetChar2().GetPalHandle().GetPalIndexRef()));
		ImGui::Text("pPalIndex_P2Char1 0x%p", &(g_interfaces.player2.GetChar1().GetPalHandle().GetPalIndexRef()));
		ImGui::Text("pPalIndex_P2Char2 0x%p", &(g_interfaces.player2.GetChar2().GetPalHandle().GetPalIndexRef()));

		const int PAL_INDEX_MIN = 0;
		const int PAL_INDEX_MAX = 15;
		if(!g_interfaces.player1.GetChar1().GetPalHandle().IsNullPointerPalIndex())
			ImGui::SliderInt("PalIndex_P1Char1", &g_interfaces.player1.GetChar1().GetPalHandle().GetPalIndexRef(), PAL_INDEX_MIN, PAL_INDEX_MAX);

		if (!g_interfaces.player1.GetChar2().GetPalHandle().IsNullPointerPalIndex())
			ImGui::SliderInt("PalIndex_P1Char2", &g_interfaces.player1.GetChar2().GetPalHandle().GetPalIndexRef(), PAL_INDEX_MIN, PAL_INDEX_MAX);

		if (!g_interfaces.player2.GetChar1().GetPalHandle().IsNullPointerPalIndex())
			ImGui::SliderInt("PalIndex_P2Char1", &g_interfaces.player2.GetChar1().GetPalHandle().GetPalIndexRef(), PAL_INDEX_MIN, PAL_INDEX_MAX);

		if (!g_interfaces.player2.GetChar2().GetPalHandle().IsNullPointerPalIndex())
			ImGui::SliderInt("PalIndex_P2Char2", &g_interfaces.player2.GetChar2().GetPalHandle().GetPalIndexRef(), PAL_INDEX_MIN, PAL_INDEX_MAX);

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

void OverlayManager::ShowDonatorsWindow()
{
	const ImVec4 COLOR_PLATINUM(0.328f, 1.000f, 0.901f, 1.000f);
	const ImVec4 COLOR_GOLD(1.000f, 0.794f, 0.000f, 1.000f);
	const ImVec4 COLOR_SILVER(0.848f, 0.848f, 0.848f, 1.000f);
	const ImVec4 COLOR_BRONZE(0.824f, 0.497f, 0.170f, 1.000f);
	const ImVec4 COLOR_DEFAULT(1.000f, 1.000f, 1.000f, 1.000f);

	ImVec2 origWindowTitleAlign = ImGui::GetStyle().WindowTitleAlign;
	ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5f, 0.5f); //middle
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);

	ImGui::SetNextWindowSizeConstraints(ImVec2(200, 50), ImVec2(500, 500));
	ImVec2 OK_btn_size = ImVec2(100, 30);

	char buf[128];
	char flipper = "|/-\\"[(int)(ImGui::GetTime() / 0.25f) & 3];
	sprintf(buf, "%c SUPPORTERS %c###Donators", flipper, flipper);
	ImGui::Begin(buf, NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

	const auto donators = GetDonatorNames();
	const auto tiers = GetDonatorTiers();
	ImVec4 donatorColor = COLOR_BRONZE;
	for (int i = 0; i < donators.size(); i++)
	{
		if (i == 0)
		{
			sprintf(buf, "%s", "TOP DONATOR");
			ImVec4 invis = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
			ImGui::TextColored(invis, buf);
			float width = ImGui::GetItemRectSize().x;
			ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - (width / 2));
			ImGui::TextColored(COLOR_PLATINUM, buf);
			float height = ImGui::GetItemRectSize().y;
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - height - (ImGui::GetStyle().ItemSpacing.y * 2));
		}

		sprintf(buf, "%s", donators[i].c_str());
		ImVec4 invis = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		ImGui::TextColored(invis, buf);
		float width = ImGui::GetItemRectSize().x;
		ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - (width / 2));

		int tier = 99;
		if (i < tiers.size())
			tier = tiers[i];

		switch (tier)
		{
		case 0:
			donatorColor = COLOR_PLATINUM;
			break;
		case 1:
			donatorColor = COLOR_GOLD;
			break;
		case 2:
			donatorColor = COLOR_SILVER;
			break;
		case 3:
			donatorColor = COLOR_BRONZE;
			break;
		default:
			donatorColor = COLOR_DEFAULT;
			break;
		}

		ImGui::TextColored(donatorColor, buf);
	}
	ImGui::Text("");

	ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - (OK_btn_size.x / 2));

	if (ImGui::Button("OK", OK_btn_size))
	{
		show_donators_window = false;
	}

	//set window to the middle of the screen, on the first call the windowsize is always minimum
	ImVec2 wndSize = ImGui::GetWindowSize();
	if (wndSize.y > 51)
	{
		ImGuiIO io = ImGui::GetIO();
		ImGui::SetWindowPos(ImVec2((io.DisplaySize.x * 0.5f) - (wndSize.x / 2), (io.DisplaySize.y * 0.5f) - (wndSize.y / 2)), ImGuiCond_Once);
	}

	ImGui::End();

	ImGui::PopStyleVar();
	ImGui::GetStyle().WindowTitleAlign = origWindowTitleAlign;
}

void OverlayManager::ShowDonatorsButton()
{
	if (GetDonatorNames().size() == 0)
		return;

	const float SPEED = 2.0f;
	int passedTime = (int)(ImGui::GetTime() / SPEED);
	static int prevPassedTime = 0;
	int donatorSize = GetDonatorNames().size() - 1;
	static int index = 0;

	if (passedTime > prevPassedTime)
	{
		prevPassedTime = passedTime;
		index++;
		if (index > donatorSize)
			index = 0;
	}

	std::string donatorName = "";

	if (index == 0)
		donatorName += "Top Donator: ";

	donatorName += GetDonatorNames()[index];

	char buf[128];
	sprintf(buf, "%s", donatorName.c_str());
	if (ImGui::Button(buf, ImVec2(-1.0f, 0.0f)))
	{
		show_donators_window ^= 1;
	}
}

void OverlayManager::ShowMainWindow(bool * p_open)
{
	if (*p_open)
	{
		//First run settings
		ImGui::SetWindowPos(main_title.c_str(), ImVec2(12, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the initial state a bit more friendly!

		if (Settings::settingsIni.menusize == 1)
			ImGui::SetNextWindowSizeConstraints(ImVec2(250, 190), ImVec2(1000, 1000));
		else if (Settings::settingsIni.menusize == 3)
			ImGui::SetNextWindowSizeConstraints(ImVec2(400, 230), ImVec2(1000, 1000));
		else
			ImGui::SetNextWindowSizeConstraints(ImVec2(330, 230), ImVec2(1000, 1000));

		ImGui::Begin(main_title.c_str(), NO_CLOSE_FLAG, ImGuiWindowFlags_AlwaysAutoResize);

		// prevent disappearing if clicked on
		if (main_window_disappear_time > 0)
		{
			if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0))
			{
				HandleMainWindowVisibility(0);
			}
		}

		ImGui::Text("Toggle me with %s", Settings::settingsIni.togglebutton.c_str());
		ImGui::Text("Toggle HUD with %s", Settings::settingsIni.toggleHUDbutton.c_str());
		ImGui::Text("Toggle Custom HUD with %s", Settings::settingsIni.togglecustomHUDbutton.c_str());
		ImGui::Separator();

		ShowDonatorsButton();

		ImGui::Text("");

		if (ImGui::CollapsingHeader("Custom HUD"))
		{
			if (g_gameVals.pIsHUDHidden)
			{
				ImGui::Text(" "); ImGui::SameLine();
				ImGui::Checkbox("Show HUD", (bool*)g_gameVals.pIsHUDHidden);
				if (Settings::settingsIni.forcecustomhud)
				{
					ImGui::SameLine(); ImGui::TextDisabled("(ForceCustomHUD is ON)");
				}
			}

			ImGui::Text(" "); ImGui::SameLine();
			ImGui::Checkbox("Show Custom HUD", &show_custom_hud);

			ImGui::Text(" "); ImGui::SameLine();
			m_customHud->ShowResetPositionsButton(middlescreen);
		}

		if (ImGui::CollapsingHeader("Custom palettes"))
		{
			if (*g_gameVals.pGameState != GameState_Match)
			{
				ImGui::Text(" "); ImGui::SameLine(); 
				ImGui::TextDisabled("Not in match!");
			}
			else
			{
				m_paletteEditor->ShowAllPaletteSelections();
			}

			ImGui::Text(""); ImGui::Text(" "); ImGui::SameLine();
			m_paletteEditor->ShowReloadAllPalettesButton();

			ImGui::Text(" "); ImGui::SameLine();
			bool pressed = ImGui::Button("Palette editor");

			if (*g_gameVals.pGameMode != GameMode_Training)
			{
				ImGui::SameLine(); ImGui::TextDisabled("Not in training mode!");
			}
			else if (*g_gameVals.pGameMode == GameMode_Training && pressed)
			{
				show_palette_editor ^= 1;
			}
		}

		if (ImGui::CollapsingHeader("Loaded settings.ini values"))
		{
			ShowLoadedSettingsValues();
		}

#ifdef _DEBUG
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

		ShowLinks();

		ImGui::End();
	}
}

void OverlayManager::HandleMainWindowVisibility(float timeLeft)
{
	main_window_disappear_time = timeLeft;

	//start making the mod's menu disappear upon start, within MAIN_WINDOW_DISAPPEAR_TIME_SECONDS
	if (main_window_disappear_time > 0)
	{	
		main_window_disappear_time -= ImGui::GetIO().DeltaTime;
		
		if(main_window_disappear_time > 0)
			ImGui::GetStyle().Alpha = main_window_disappear_time / MAIN_WINDOW_DISAPPEAR_TIME_SECONDS;

		//disappear upon reaching main_window_disappear_time < 0
		if (main_window_disappear_time <= 0)
		{
			show_main_window = false;
			ImGui::GetStyle().Alpha = DEFAULT_ALPHA;
		}
	}
	else
	{
		ImGui::GetStyle().Alpha = DEFAULT_ALPHA;
	}
}

void OverlayManager::ShowLinks()
{
	if (ImGui::Button("Discord"))
		ShellExecute(NULL, L"open", MOD_LINK_DISCORD, NULL, NULL, SW_SHOWNORMAL);

	ImGui::SameLine();
	if (ImGui::Button("Forum"))
		ShellExecute(NULL, L"open", MOD_LINK_FORUM, NULL, NULL, SW_SHOWNORMAL);

	ImGui::SameLine();
	if (ImGui::Button("Nexusmods"))
		ShellExecute(NULL, L"open", MOD_LINK_NEXUSMODS, NULL, NULL, SW_SHOWNORMAL);

	ImGui::SameLine();
	if (ImGui::Button("GitHub"))
		ShellExecute(NULL, L"open", MOD_LINK_GITHUB, NULL, NULL, SW_SHOWNORMAL);

	ImGui::SameLine();
	if (ImGui::Button("Donate"))
		ShellExecute(NULL, L"open", MOD_LINK_DONATE, NULL, NULL, SW_SHOWNORMAL);
}

void OverlayManager::ShowAllWindows()
{
	ShowMainWindow(&show_main_window);

	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, DEFAULT_ALPHA);

	if (show_palette_editor)
		m_paletteEditor->ShowPaletteEditorWindow(&show_palette_editor);

	if (show_notification)
		HandleNotification();

	if (show_log_window && show_main_window)
		ShowLogWindow(&show_log_window);

	if (IsUpdateAvailable)
		ShowUpdateWindow();

	if (show_donators_window)
		ShowDonatorsWindow();

////////////// DEBUG Windows
#ifdef _DEBUG
	if (show_demo_window)
	{
		ImGui::SetNextWindowPos(ImVec2(550, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
		ImGui::ShowDemoWindow(&show_demo_window);
	}

	if (show_debug_window)
		ShowDebugWindow(&show_debug_window);
#endif
////////////// 

	ImGui::PopStyleVar();
}

void OverlayManager::HandleButtons()
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
		HandleMainWindowVisibility(0);
	}
}