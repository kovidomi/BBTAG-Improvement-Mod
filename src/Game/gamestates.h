#pragma once

enum GameState_
{
	GameState_TitleScreen = 4,
	GameState_CharacterSelectScreen = 6,
	GameState_VersusScreen = 10,
	GameState_Match = 11,
	GameState_VictoryScreen = 12, //rematch
	GameState_EpisodeSelection = 20,
	GameState_GalleryMode = 21,
	GameState_ReplayTheater = 22,
	GameState_Entrance = 23,
	GameState_Tactics = 24,
	GameState_StoryMode = 30,
	GameState_CustomizeScreen = 35,
	GameState_ItemShop = 35, //not a typo, uses 35 as well as customize_screen
};

enum GameMode_
{
	GameMode_Versus = 5,
	GameMode_Training = 6,
};