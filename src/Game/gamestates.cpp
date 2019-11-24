#include "gamestates.h"

#include "Core/interfaces.h"

bool isPaletteEditingEnabledInCurrentGameMode()
{
	return *g_gameVals.pGameMode == GameMode_Training
	|| *g_gameVals.pGameMode == GameMode_Versus;
}
