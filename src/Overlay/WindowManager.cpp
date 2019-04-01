#include "WindowManager.h"

#include "fonts.h"
#include "WindowDrawer.h"
#include "WindowContainer/WindowContainerImpl.h"
#include "Window/LogWindow.h"
#include "Window/PaletteEditorWindow.h"
#include "Window/UpdateNotifierWindow.h"

#include "Core/info.h"
#include "Core/interfaces.h"
#include "Core/logger.h"
#include "Core/Settings.h"
#include "Core/utils.h"
#include "Web/donators_fetch.h"
#include "Web/update_check.h"
#include <imgui.h>
#include <imgui_impl_dx9.h>

#include <sstream>
#include <time.h>

#define DEFAULT_ALPHA 0.87f

WindowManager* WindowManager::m_instance = nullptr;

int toggle_key;
int toggleHUD_key;
int toggleCustomHUD_key;

void WindowManager::OnMatchInit()
{
	if (!m_initialized)
		return;

	g_interfaces.pPaletteManager->OnMatchInit(
		g_interfaces.player1.GetChar1(),
		g_interfaces.player1.GetChar2(),
		g_interfaces.player2.GetChar1(),
		g_interfaces.player2.GetChar2());

	m_windowContainer->GetWindow<PaletteEditorWindow>(WindowType_PaletteEditor)->OnMatchInit();
}

WindowManager & WindowManager::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new WindowManager();
	}

	return *m_instance;
}

bool WindowManager::Initialize(void *hwnd, IDirect3DDevice9 *device)
{
	if (m_initialized)
		return true;

	LOG(2, "WindowManager::Initialize\n");

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

	m_initialized = ImGui_ImplDX9_Init(hwnd, device);
	if (!m_initialized)
	{
		LOG(2, "ImGui_ImplDX9_Init failed!\n");
		return false;
	}

	m_pLogger = g_imGuiLogger;

	m_pLogger->Log("[system] Initialization starting...\n");

	m_windowContainer = new WindowContainerImpl();

	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowBorderSize = 1;
	style.FrameBorderSize = 1;
	style.ScrollbarSize = 14;
	style.Alpha = DEFAULT_ALPHA;

	if (Settings::settingsIni.menusize == 1)
	{
		ImFont* font = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(TinyFont_compressed_data_base85, 10);
		font->DisplayOffset.y += 1;
	}
	else if (Settings::settingsIni.menusize == 3)
		ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(DroidSans_compressed_data, DroidSans_compressed_size, 20);

	toggle_key = Settings::getButtonValue(Settings::settingsIni.togglebutton);
	m_pLogger->Log("[system] Toggling key set to '%s'\n", Settings::settingsIni.togglebutton.c_str());

	toggleHUD_key = Settings::getButtonValue(Settings::settingsIni.toggleHUDbutton);
	m_pLogger->Log("[system] HUD toggling key set to '%s'\n", Settings::settingsIni.toggleHUDbutton.c_str());

	toggleCustomHUD_key = Settings::getButtonValue(Settings::settingsIni.togglecustomHUDbutton);
	m_pLogger->Log("[system] CustomHUD toggling key set to '%s'\n", Settings::settingsIni.togglecustomHUDbutton.c_str());

	g_interfaces.pPaletteManager->LoadAllPalettes();

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
	float hud_scale_y = ((float)Settings::settingsIni.renderheight * Settings::settingsIni.customhudscale) / 1042.0f;
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

	StartAsyncUpdateCheck();
	StartAsyncDonatorsFetch();

	m_pLogger->Log("[system] Finished initialization\n");
	m_pLogger->LogSeparator();
	LOG(2, "Initialize end\n");

	return true;
}

void WindowManager::Shutdown()
{
	if (!m_initialized)
		return;

	LOG(2, "WindowManager::Shutdown\n");

	SAFE_DELETE(m_windowContainer);
	delete m_instance;

	ImGui_ImplDX9_Shutdown();
}

void WindowManager::InvalidateDeviceObjects()
{
	if (!m_initialized)
		return;

	LOG(2, "WindowManager::InvalidateDeviceObjects\n");
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

void WindowManager::CreateDeviceObjects()
{
	if (!m_initialized)
		return;

	LOG(2, "WindowManager::CreateDeviceObjects\n");
	ImGui_ImplDX9_CreateDeviceObjects();
}

void WindowManager::Render()
{
	if (!m_initialized)
		return;

	LOG(7, "WindowManager::Render\n");

	WindowDrawer::DrawAllWindows(m_windowContainer);
	ImGui::Render();
}

void WindowManager::Update()
{
	if (!m_initialized)
		return;

	LOG(7, "WindowManager::Update\n");

	g_interfaces.pPaletteManager->OnUpdate(
		g_interfaces.player1.GetChar1().GetPalHandle(),
		g_interfaces.player1.GetChar2().GetPalHandle(),
		g_interfaces.player2.GetChar1().GetPalHandle(),
		g_interfaces.player2.GetChar2().GetPalHandle());

	HandleButtons();

	ImGui_ImplDX9_NewFrame();

	bool isUpdateNotifierWindowOpen =
		m_windowContainer->GetWindow<UpdateNotifierWindow>(WindowType_UpdateNotifier)->IsOpen();
	bool isPaletteEditorWindowOpen =
		m_windowContainer->GetWindow<PaletteEditorWindow>(WindowType_PaletteEditor)->IsOpen();
	bool isLogWindowOpen =
		m_windowContainer->GetWindow<LogWindow>(WindowType_Log)->IsOpen();

	ImGuiIO& io = ImGui::GetIO();
	io.MouseDrawCursor = m_windowContainer->GetWindow(WindowType_Main)->IsOpen() | isLogWindowOpen
		| isPaletteEditorWindowOpen | isUpdateNotifierWindowOpen; // show_notification_window | show_demo_window;

	LOG(7, "END OF WindowManager::Update\n");
}

void WindowManager::HandleButtons()
{
	if (!m_initialized)
	{
		return;
	}

	if (ImGui::IsKeyPressed(toggleHUD_key) && g_gameVals.pIsHUDHidden)
	{
		*g_gameVals.pIsHUDHidden ^= 1;
	}

	if (ImGui::IsKeyPressed(toggleCustomHUD_key))
	{
		m_windowContainer->GetWindow(WindowType_CustomHud)->ToggleOpen();
	}

	if (ImGui::IsKeyPressed(toggle_key))
	{
		m_windowContainer->GetWindow(WindowType_Main)->ToggleOpen();
	}
}
