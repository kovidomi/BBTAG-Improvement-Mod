#include "MatchState.h"

#include "Core/interfaces.h"
#include "Overlay/Window/PaletteEditorWindow.h"
#include "Overlay/WindowContainer/WindowType.h"
#include "Overlay/WindowManager.h"

void MatchState::OnMatchInit()
{
	if (!WindowManager::GetInstance().IsInitialized())
	{
		return;
	}

	g_interfaces.pPaletteManager->OnMatchInit(
		g_interfaces.player1.GetChar1(),
		g_interfaces.player1.GetChar2(),
		g_interfaces.player2.GetChar1(),
		g_interfaces.player2.GetChar2());

	WindowManager::GetInstance().GetWindowContainer()->GetWindow<PaletteEditorWindow>(WindowType_PaletteEditor)->OnMatchInit();
}

void MatchState::OnMatchEnd()
{
	g_interfaces.pPaletteManager->OnMatchEnd(
		g_interfaces.player1.GetChar1(),
		g_interfaces.player1.GetChar2(),
		g_interfaces.player2.GetChar1(),
		g_interfaces.player2.GetChar2());
}

void MatchState::OnUpdate()
{
	g_interfaces.pPaletteManager->OnUpdate(
		g_interfaces.player1.GetChar1().GetPalHandle(),
		g_interfaces.player1.GetChar2().GetPalHandle(),
		g_interfaces.player2.GetChar1().GetPalHandle(),
		g_interfaces.player2.GetChar2().GetPalHandle());
}