#include "PaletteManager.h"
#include "../utils.h"
#include "../WindowManager/WindowManager.h"
#include "../logger.h"
#include <sstream>
#include <fstream>
#include <atlstr.h>

#define MAX_NUM_OF_PAL_INDEXES 16

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

	//(TOTAL_CHAR_INDEXES - 1) to exclude the boss
	for (int i = 0; i < (TOTAL_CHAR_INDEXES - 1); i++)
	{
		std::wstring path = std::wstring(L"BBTAG_IM\\Palettes\\") + wCharNames[i];
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

	//(TOTAL_CHAR_INDEXES - 1) to exclude the boss
	for (int i = 0; i < (TOTAL_CHAR_INDEXES - 1); i++)
	{
		std::wstring wPath = std::wstring(L"BBTAG_IM\\Palettes\\") + wCharNames[i] + L"\\*";
		LoadPalettesIntoVector((CharIndex)i, wPath);
	}
	WindowManager::AddLog("[system] Finished loading custom palettes\n");
	WindowManager::AddLogSeparator();
}

void PaletteManager::ReloadPaletteSlotsFile()
{
	LOG(2, "ReloadPaletteSlotsFile\n");
	WindowManager::AddLog("[system] Reloading 'palette_slots.ini' ...\n");

	m_paletteSlots.clear();

	InitPaletteSlotsVector();
	LoadPaletteSlotsFile();
}

void PaletteManager::ApplyPaletteSlot(CharIndex charIndex, CharPaletteHandle & charPalHandle)
{
	if (charIndex > TOTAL_CHAR_INDEXES)
		return;

	const int curPalIndex = charPalHandle.GetOrigPalIndex();
	const char* palName = m_paletteSlots[charIndex][curPalIndex].c_str();

	if (strcmp(palName, "") == 0 || strcmp(palName, "Default") == 0)
		return;

	int foundCustomPalIndex = g_interfaces.pPaletteManager->FindCustomPalIndex(charIndex, palName);

	if (foundCustomPalIndex < 0)
	{
		WindowManager::AddLog("[error] Palette slot file '%s' not found.\n", palName);
		return;
	}

	g_interfaces.pPaletteManager->SwitchPalette(charIndex, charPalHandle, foundCustomPalIndex);
}

void PaletteManager::LoadPalettesIntoVector(CharIndex charIndex, std::wstring& wFolderPath)
{
	std::string folderPath(wFolderPath.begin(), wFolderPath.end());
	LOG(2, "LoadPalettesIntoContainer %s\n", folderPath);

	HANDLE hFind;
	WIN32_FIND_DATA data;

	hFind = FindFirstFile(wFolderPath.c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {

			//ignore current and parent directories
			if (_tcscmp(data.cFileName, TEXT(".")) == 0 || _tcscmp(data.cFileName, TEXT("..")) == 0)
				continue;
			//recursively search subfolders
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				std::wstring wSubfolderPath(wFolderPath.c_str());
				wSubfolderPath.pop_back(); //delete "*" at the end
				wSubfolderPath += data.cFileName;
				wSubfolderPath += L"\\*";
				LoadPalettesIntoVector(charIndex, wSubfolderPath);
				continue;
			}

			std::wstring wFileName(data.cFileName);
			std::string fileName(wFileName.begin(), wFileName.end());
			std::string fullPath(folderPath);
			fullPath.pop_back(); //delete "*" at the end
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

			m_customPalettes[charIndex].push_back(fileContents.paldata);

			WindowManager::AddLog("[system] Loaded '%s'\n", fileName.c_str());
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}
}

void PaletteManager::LoadPaletteSlotsFile()
{
	TCHAR pathBuf[MAX_PATH];
	GetModuleFileName(NULL, pathBuf, MAX_PATH);
	std::wstring::size_type pos = std::wstring(pathBuf).find_last_of(L"\\");
	std::wstring wFullPath = std::wstring(pathBuf).substr(0, pos);

	wFullPath += L"\\BBTAG_IM\\Palettes\\palette_slots.ini";

	if (!PathFileExists(wFullPath.c_str()))
	{
		LOG(2, "\t'palette_slots.ini' file was not found!\n");
		WindowManager::AddLog("[error] 'palette_slots.ini' file was not found!\n");
		return;
	}

	for (int i = 0; i < (TOTAL_CHAR_INDEXES - 1); i++)
	{
		for (int iSlot = 1; iSlot <= MAX_NUM_OF_PAL_INDEXES; iSlot++)
		{
			CString strBuffer;
			GetPrivateProfileString(wCharNames[i], std::to_wstring(iSlot).c_str(), L"", strBuffer.GetBuffer(MAX_PATH), MAX_PATH, wFullPath.c_str());
			strBuffer.ReleaseBuffer();

			strBuffer.Remove('\"');
			//delete file extension if found
			int pos = strBuffer.Find(L".impl", 0);
			if(pos >= 0)
			{ 
				strBuffer.Delete(pos, strBuffer.StringLength(strBuffer));
			}

			CT2CA pszConvertedAnsiString(strBuffer);
			m_paletteSlots[i][iSlot-1] = pszConvertedAnsiString;
		}
	}
}

bool PaletteManager::CreatePaletteSlotsFile()
{
	TCHAR pathBuf[MAX_PATH];
	GetModuleFileName(NULL, pathBuf, MAX_PATH);
	std::wstring::size_type pos = std::wstring(pathBuf).find_last_of(L"\\");
	std::wstring wFullPath = std::wstring(pathBuf).substr(0, pos);

	wFullPath += L"\\BBTAG_IM\\Palettes\\palette_slots.ini";

	if (PathFileExists(wFullPath.c_str()))
	{
		LOG(2, "\t'palette_slots.ini' already exists\n");
		WindowManager::AddLog("[system] 'palette_slots.ini' already exists\n");
		return true;
	}

	std::string path = std::string("BBTAG_IM\\Palettes\\palette_slots.ini");

	std::ofstream file;
	file.open(path);

	if (!file.is_open())
	{
		LOG(2, "\tCouldn't open %s!\n", strerror(errno));
		WindowManager::AddLog("[error] Unable to open '%s' : %s\n", path.c_str(), strerror(errno));
		return false;
	}

	static const char* info 
	{
		"######################################################################################\r\n" \
		"# Lines starting with \"#\" are ignored.\t\t\t\t\t\t     #\r\n" \
		"# Valid palette indexes are between 1 - 16\t\t\t\t\t     #\r\n" \
		"# Filenames must be put between quotation marks (\"). (Extension .impl is not needed) #\r\n" \
		"#\t\t\t\t\t\t\t\t\t\t     #\r\n" \
		"# Example:\t\t\t\t\t\t\t\t\t     #\r\n" \
		"# If Aegis has palette files \"Fox.impl\" and \"Panda.impl\", then a section like:\t     #\r\n" \
		"# [Aegis]\t\t\t\t\t\t\t\t\t     #\r\n" \
		"# 1=\"Fox\"\t\t\t\t\t\t\t\t\t     #\r\n" \
		"# 3=\"Panda\"\t\t\t\t\t\t\t\t\t     #\r\n" \
		"# 4=\"Panda\"\t\t\t\t\t\t\t\t\t     #\r\n" \
		"# 6=\"Fox\"\t\t\t\t\t\t\t\t\t     #\r\n" \
		"# 14=\"Fox\"\t\t\t\t\t\t\t\t\t     #\r\n" \
		"# Will make the mod load the palette file \"Fox.impl\" on ingame palettes 1, 6, 14,    #\r\n" \
		"# and load the palette file \"Panda.impl\" on ingame palettes 3, 4\t\t     #\r\n" \
		"######################################################################################\r\n\r\n"
	};
	size_t infoSize = strlen(info);
	file.write(info, infoSize);

	file.close();

	//(TOTAL_CHAR_INDEXES - 1) to exclude the boss
	for (int i = 0; i < (TOTAL_CHAR_INDEXES - 1); i++)
	{
		WritePrivateProfileString(wOrderedCharNames[i], L"1", L"\"Default\"\r\n", wFullPath.c_str());
	}

	LOG(2, "\tCreated '%s'\n", path.c_str());
	WindowManager::AddLog("[system] Created '%s'\n", path.c_str());

	return true;
}

void PaletteManager::InitPaletteSlotsVector()
{
	LOG(2, "InitPaletteSlotsVector\n");

	m_paletteSlots.resize(TOTAL_CHAR_INDEXES);

	//(TOTAL_CHAR_INDEXES - 1) to exclude the boss
	for (int i = 0; i < (TOTAL_CHAR_INDEXES - 1); i++)
	{
		for (int iSlot = 0; iSlot < MAX_NUM_OF_PAL_INDEXES; iSlot++)
		{
			m_paletteSlots[i].push_back("");
		}
	}
}

void PaletteManager::LoadImplFile(IMPL_t & filledPal)
{
	CharIndex charIndex = (CharIndex)filledPal.header.charindex;
	m_customPalettes[charIndex].push_back(filledPal.paldata);
	WindowManager::AddLog("[system] Loaded '%s'\n", filledPal.paldata.palname);
}

bool PaletteManager::WritePaletteToFile(CharIndex charIndex, IMPL_data_t *filledPalData)
{
	std::string path = std::string("BBTAG_IM\\Palettes\\") + charNames[charIndex] + "\\" + filledPalData->palname + ".impl";

	IMPL_t IMPL_file;

	IMPL_file.header.headerlen = sizeof(IMPL_header_t);
	IMPL_file.header.datalen = sizeof(IMPL_data_t);
	IMPL_file.header.charindex = charIndex;
	IMPL_file.paldata = *filledPalData;

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

void PaletteManager::LoadAllPalettes()
{
	LoadPalettesFromFolder();
	CreatePaletteSlotsFile();

	InitPaletteSlotsVector();
	LoadPaletteSlotsFile();
}

void PaletteManager::ReloadAllPalettes()
{
	ReloadPalettesFromFolder();
	ReloadPaletteSlotsFile();
}

void PaletteManager::ReloadPalettesFromFolder()
{
	LOG(2, "ReloadPalettesFromFolder\n");
	WindowManager::AddLog("[system] Reloading custom palettes...\n");

	m_customPalettes.clear();

	InitCustomPaletteVector();
	LoadPalettesFromFolder();
}

int PaletteManager::FindCustomPalIndex(CharIndex charIndex, const char * palNameToFind)
{
	if (charIndex > TOTAL_CHAR_INDEXES)
		return -1;

	if (strcmp(palNameToFind, "") == 0 || strcmp(palNameToFind, "Default") == 0)
		return -1;

	for (int i = 0; i < m_customPalettes[charIndex].size(); i++)
	{
		if (strcmp(palNameToFind, m_customPalettes[charIndex][i].palname) == 0)
		{
			return i;
		}
	}

	return -1;
}

bool PaletteManager::SwitchPalette(CharIndex charIndex, CharPaletteHandle& palHandle, int newCustomPalIndex)
{
	int totalCharPals = m_customPalettes[charIndex].size();
	if (newCustomPalIndex >= totalCharPals)
		return false;

	palHandle.SetSelectedCustomPalIndex(newCustomPalIndex);
	palHandle.ReplaceAllPalFiles(&m_customPalettes[charIndex][newCustomPalIndex]);
	return true;
}

void PaletteManager::ReplacePaletteFile(const char * newPalData, PaletteFile palFile, CharPaletteHandle& palHandle)
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

void PaletteManager::OnUpdate(CharPaletteHandle & P1Ch1, CharPaletteHandle & P1Ch2, CharPaletteHandle & P2Ch1, CharPaletteHandle & P2Ch2)
{
	P1Ch1.UnlockUpdate();
	P1Ch2.UnlockUpdate();
	P2Ch1.UnlockUpdate();
	P2Ch2.UnlockUpdate();
}

void PaletteManager::OnMatchInit(CharHandle& P1Ch1, CharHandle& P1Ch2, CharHandle& P2Ch1, CharHandle& P2Ch2)
{
	P1Ch1.GetPalHandle().OnMatchInit();
	P1Ch2.GetPalHandle().OnMatchInit();
	P2Ch1.GetPalHandle().OnMatchInit();
	P2Ch2.GetPalHandle().OnMatchInit();

	ApplyPaletteSlot((CharIndex)P1Ch1.GetData()->char_index, P1Ch1.GetPalHandle());
	ApplyPaletteSlot((CharIndex)P1Ch2.GetData()->char_index, P1Ch2.GetPalHandle());
	ApplyPaletteSlot((CharIndex)P2Ch1.GetData()->char_index, P2Ch1.GetPalHandle());
	ApplyPaletteSlot((CharIndex)P2Ch2.GetData()->char_index, P2Ch2.GetPalHandle());
}

std::vector<std::vector<IMPL_data_t>>& PaletteManager::GetCustomPalettesVector()
{
	return m_customPalettes;
}