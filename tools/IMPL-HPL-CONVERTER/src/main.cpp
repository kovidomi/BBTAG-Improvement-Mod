#include "hpl.h"
#include "impl.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <atlstr.h>

void ConvertHplsToImpl()
{
	printf("HPL to IMPL mode selected based on input files\n");
	printf("Converting HPLs to IMPL\n\n");

	IMPL_t* loadedTemplate = LoadImplFromFile(std::wstring(L"*"));
	if (!loadedTemplate)
	{
		printf("ERROR reading .impl file\n");
		return;
	}

	LoadHplsFromFile(std::wstring(L"*"), *loadedTemplate);

	const char* creatorName = GetCreatorName();
	strncpy(loadedTemplate->paldata.creator, creatorName, IMPL_CREATOR_LENGTH);

	const char* description = GetDescription();
	strncpy(loadedTemplate->paldata.desc, description, IMPL_DESC_LENGTH);

	if (!WriteImplToFile(loadedTemplate))
	{
		printf("ERROR writing .impl file\n");
	}

	delete loadedTemplate;
}

void ConvertImplToHpls()
{
	printf("IMPL to HPL mode selected based on input files\n");
	printf("Converting IMPL to HPLs\n\n");

	IMPL_t* implFile = LoadImplFromFile(std::wstring(L"*"));
	if (!implFile)
	{
		printf("ERROR reading .impl file\n");
		return;
	}

	if (!WriteHplsToFile(*implFile))
	{
		printf("ERROR writing .hpl file\n");
	}

	delete implFile;
}

void ExitPrompt()
{
	printf("Done. Press any key to exit.\n");
	getchar();
}

int CountNumberOfValidFilesInFolder(int& implFiles, int& hplFiles)
{
	int count = 0;

	HANDLE hFind;
	WIN32_FIND_DATA data;

	hFind = FindFirstFile(L"*", &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do 
		{
			//ignore current and parent directories
			if (_tcscmp(data.cFileName, TEXT(".")) == 0 || _tcscmp(data.cFileName, TEXT("..")) == 0)
				continue;

			std::wstring wFileName(data.cFileName);
			std::string fileName(wFileName.begin(), wFileName.end());

			if (fileName.find(".impl") != std::string::npos)
			{
				count++;
				implFiles++;
				continue;
			}
			else if (fileName.find(".hpl") != std::string::npos)
			{
				count++;
				hplFiles++;
				continue;
			}
			
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}

	return count;
}

int main(int argc, char* argv[])
{
	printf("IMPL-HPL-CONVERTER  -  by KoviDomi\n\n");

	int totalFiles = 0;
	int implFiles = 0;
	int hplFiles = 0;

	totalFiles = CountNumberOfValidFilesInFolder(implFiles, hplFiles);
	printf("Number of .impl files found: %d\n", implFiles);
	printf("Number of .hpl files found: %d\n\n", hplFiles);

	if (implFiles != 1)
	{
		printf("ERROR, there must be only ONE .impl file present in the folder\n");
		ExitPrompt();
		return 1;
	}

	if (hplFiles > 8)
	{
		printf("ERROR, too many .hpl files (8 max! 1 char and 7 effects)\n");
		ExitPrompt();
		return 2;
	}

	if (totalFiles == 1) //one file, can only be impl
	{
		ConvertImplToHpls();
	}
	else if (totalFiles >= 2) //two or more files (one impl and multiple hpls)
	{
		ConvertHplsToImpl();
	}

	ExitPrompt();
	return 0;
}