#include "PaletteManager.h"
#include "../utils.h"
#include "../WindowManager/WindowManager.h"
#include "../logger.h"
#include <sstream>
#include <fstream>
#include <atlstr.h>

PaletteManager::PaletteManager()
{
	CreatePaletteFolders();
	InitCustomPaletteVector();
}

PaletteManager::~PaletteManager()
{
}

void PaletteManager::CreatePaletteFolders()
{
	CreateDirectory(L"BBTAG_IM\\Palettes", NULL);

	std::wstring path;

	//(TOTAL_CHAR_INDEXES - 1) to exclude the boss
	for (int i = 0; i < (TOTAL_CHAR_INDEXES - 1); i++)
	{
		path = std::wstring(L"BBTAG_IM\\Palettes\\") + wCharNames[i];
		CreateDirectory(path.c_str(), NULL);
	}
}

void PaletteManager::InitCustomPaletteVector()
{
	LOG(2, "InitCustomPaletteVector\n");

	m_customPalettes.resize(TOTAL_CHAR_INDEXES);

	//(TOTAL_CHAR_INDEXES - 1) to exclude the boss
	for (int i = 0; i < (TOTAL_CHAR_INDEXES - 1); i++)
	{
		//make the character palette array's 0th element an empty one, that will be used to set back to the default palette
		IMPL_data_t customPal { "Default" };
		m_customPalettes[i].push_back(customPal);
	}
}

void PaletteManager::LoadPalettesFromFolder()
{
	LOG(2, "LoadPaletteFiles\n");
	WindowManager::AddLogSeparator();
	WindowManager::AddLog("[system] Loading custom palettes...\n");

	std::wstring path;

	//(TOTAL_CHAR_INDEXES - 1) to exclude the boss
	for (int i = 0; i < (TOTAL_CHAR_INDEXES - 1); i++)
	{
		path = std::wstring(L"BBTAG_IM\\Palettes\\") + wCharNames[i] + L"\\*";
		LoadPalettesIntoVector((CharIndex)i, path.c_str());
	}
	WindowManager::AddLog("[system] Finished loading custom palettes\n");
	WindowManager::AddLogSeparator();
}

void PaletteManager::LoadPalettesIntoVector(CharIndex charIndex, const wchar_t* path)
{
	std::wstring wws(path);
	std::string ppath(wws.begin(), wws.end());
	LOG(2, "LoadPalettesIntoContainer %s\n", ppath);

	HANDLE hFind;
	WIN32_FIND_DATA data;

	hFind = FindFirstFile(path, &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {

			//ignore current and parent directories
			if (_tcscmp(data.cFileName, TEXT(".")) == 0 || _tcscmp(data.cFileName, TEXT("..")) == 0)
				continue;
			//recursively search subfolders
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				std::wstring subfolderpath(path);
				subfolderpath.pop_back(); //delete "*" at the end
				subfolderpath += data.cFileName;
				subfolderpath += L"\\*";
				LoadPalettesIntoVector(charIndex, subfolderpath.c_str());
				continue;
			}
			//continue;

			std::wstring ws(data.cFileName);
			std::string fileName(ws.begin(), ws.end());
			std::string fullPath(ppath);
			fullPath.erase(fullPath.end() - 1);
			fullPath += fileName;

			LOG(2, "\tFILE: %s", fileName.c_str());
			LOG(2, "\t\tFull path: %s\n", fullPath.c_str());

			////read binary file into string
			std::ifstream file(fullPath, std::ios::binary);

			if (!file.is_open())
			{
				LOG(2, "\tCouldn't open %s!\n", strerror(errno));
				WindowManager::AddLog("[error] Unable to open '%s' : %s\n", fileName.c_str(), strerror(errno));
				continue;
			}
			if (fileName.find(".impl") == std::string::npos)
			{
				WindowManager::AddLog("[error] Unable to open '%s' : not an .impl file\n", fileName.c_str());
				continue;
			}

			IMPL_t fileContents;
		
			file.read((char*)&fileContents, sizeof(IMPL_t)); //what if the file is smaller then sizeof(IMPL_t) ?
			file.close();

			//check filesig
			if (strcmp(fileContents.header.filesig, "IMPL") != 0)
			{
				LOG(2, "ERROR, unrecognized file format!\n");
				WindowManager::AddLog("[error] '%s' unrecognized file format!\n", fileName.c_str());
				continue;
			}
			if (fileContents.header.datalen != sizeof(IMPL_data_t))
			{
				LOG(2, "ERROR, data size mismatch!\n");
				WindowManager::AddLog("[error] '%s' data size mismatch!\n", fileName.c_str());
				continue;
			}

			m_customPalettes[charIndex].push_back(fileContents.palData);

			WindowManager::AddLog("[system] Loaded '%s'\n", fileName.c_str());
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}
}

bool PaletteManager::WritePaletteToFile(CharIndex charIndex, IMPL_data_t *filledPalData)
{
	std::string path = std::string("BBTAG_IM\\Palettes\\") + charNames[charIndex] + "\\" + filledPalData->palname + ".impl";

	IMPL_t IMPL_file;

	IMPL_file.header.headerlen = sizeof(IMPL_header_t);
	IMPL_file.header.datalen = sizeof(IMPL_data_t);
	IMPL_file.palData = *filledPalData;

	std::ofstream file;
	file.open(path, std::fstream::binary);
	if (!file.is_open())
	{
		LOG(2, "\tCouldn't open %s!\n", strerror(errno));
		WindowManager::AddLog("[error] Unable to open '%s' : %s\n", path.c_str(), strerror(errno));
		return false;
	}

	file.write((char*)&IMPL_file, sizeof(IMPL_t));
	file.close();

	return true;
}

void PaletteManager::ReloadPalettesFromFolder()
{
	LOG(2, "ReloadPalettesFromFolder\n");
	WindowManager::AddLog("[system] Reloading custom palettes...\n");

	m_customPalettes.clear();

	InitCustomPaletteVector();
	LoadPalettesFromFolder();
}

bool PaletteManager::SwitchPalette(CharIndex charIndex, int customPalIndex, CharPaletteHandle& palHandle)
{
	int totalCharPals = m_customPalettes[charIndex].size();
	if (customPalIndex >= totalCharPals)
		return false;

	palHandle.SetSelectedCustomPalIndex(customPalIndex);
	palHandle.ReplaceAllPalFiles(&m_customPalettes[charIndex][customPalIndex]);
	return true;
}

void PaletteManager::ReplacePaletteFile(char * newPalData, PaletteFile palFile, CharPaletteHandle& palHandle)
{
	palHandle.ReplaceSinglePalFile(newPalData, palFile);
}

char * PaletteManager::GetPalFileAddr(PaletteFile palFile, CharPaletteHandle& palHandle)
{
	return palHandle.GetOrigPalFileAddr(palFile);
}

int PaletteManager::GetCurrentCustomPalIndex(CharPaletteHandle& palHandle)
{
	return palHandle.GetSelectedCustomPalIndex();
}

const IMPL_data_t & PaletteManager::GetCurrentPalData(CharPaletteHandle& palHandle)
{
	return palHandle.GetCurrentPalData();
}

void PaletteManager::UnlockUpdates(CharPaletteHandle& P1Ch1, CharPaletteHandle& P1Ch2, CharPaletteHandle& P2Ch1, CharPaletteHandle& P2Ch2)
{
	P1Ch1.UnlockUpdate();
	P1Ch2.UnlockUpdate();
	P2Ch1.UnlockUpdate();
	P2Ch2.UnlockUpdate();
}

void PaletteManager::OnMatchInit(CharPaletteHandle& P1Ch1, CharPaletteHandle& P1Ch2, CharPaletteHandle& P2Ch1, CharPaletteHandle& P2Ch2)
{
	P1Ch1.OnMatchInit();
	P1Ch2.OnMatchInit();
	P2Ch1.OnMatchInit();
	P2Ch2.OnMatchInit();
}

std::vector<std::vector<IMPL_data_t>>& PaletteManager::GetCustomPalettesVector()
{
	return m_customPalettes;
}