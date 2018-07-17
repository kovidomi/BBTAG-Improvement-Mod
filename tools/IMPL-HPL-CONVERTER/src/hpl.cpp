#include "hpl.h"
#include <sstream>
#include <fstream>
#include <vector>
#include <atlstr.h>

#define HPAL_HEADER_LEN 32

const char HPAL_HEADER[] = { '\x48', '\x50', '\x41', '\x4C', '\x25', '\x01', '\x00', '\x00', '\x20', '\x04', '\x00','\x00', '\x00',
'\x01', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x01', '\x00','\x00', '\x10', '\x00', '\x00', 
'\x00', '\x00' };

//delete allocated memory after use
bool LoadHplsFromFile(std::wstring& wFolderPath, IMPL_t& templateFile)
{
	std::string folderPath(wFolderPath.begin(), wFolderPath.end());

	HANDLE hFind;
	WIN32_FIND_DATA data;

	hFind = FindFirstFile(wFolderPath.c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {

			//ignore current and parent directories
			if (_tcscmp(data.cFileName, TEXT(".")) == 0 || _tcscmp(data.cFileName, TEXT("..")) == 0)
				continue;

			std::wstring wFileName(data.cFileName);
			std::string fileName(wFileName.begin(), wFileName.end());
			std::string fullPath(folderPath);
			fullPath.pop_back(); //delete "*" at the end
			fullPath += fileName;

			if (fileName.find(".hpl") == std::string::npos)
			{
				printf("Skipping loading of '%s' : not a .hpl file\n", fileName.c_str());
				continue;
			}

			char* pDst = 0;

			if (fileName.find("_effect0") == std::string::npos)
			{
				//replace palname section with the filename, so renaming the file has effect on the actual ingame palname
				std::string fileNameWithoutExt = fileName.substr(0, fileName.length() - strlen(".hpl"));
				memset(templateFile.paldata.palname, 0, IMPL_PALNAME_LENGTH);
				memcpy(templateFile.paldata.palname, fileNameWithoutExt.c_str(), strlen(fileNameWithoutExt.c_str()));
				pDst = templateFile.paldata.file0;
			}
			else
			{
				int pos = fileName.find("_effect0");
				std::string index = fileName.substr(pos + 7, 2);

				if (!(std::istringstream(index) >> pos))
					pos = -1;

				if (pos > 7 || pos == -1)
				{
					printf("ERROR, effect file '%s' has wrong index!\n", fileName.c_str());
					return false;
				}
				pDst = templateFile.paldata.file0 + (pos * IMPL_PALETTE_DATALEN);
			}

			////read binary file into string
			std::ifstream file(fullPath, std::ios::binary);

			if (!file.is_open())
			{
				printf("ERROR, Unable to open '%s' : %s\n", fileName.c_str(), strerror(errno));
				return false;
			}
			file.seekg(HPAL_HEADER_LEN);
			file.read(pDst, IMPL_PALETTE_DATALEN); //what if the file is smaller then sizeof(IMPL_t) ?
			file.close();

		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}

	return true;
}

bool WriteHplsToFile(IMPL_t& filledImplFile)
{
	const int NUMBER_OF_PAL_FILES = 8;

	const char* pSrc = filledImplFile.paldata.file0;

	for (int i = 0; i < NUMBER_OF_PAL_FILES; i++)
	{
		std::string path = std::string(filledImplFile.paldata.palname);
		if (i > 0)
		{
			path += "_effect0";
			path += std::to_string(i);
		}
		path += ".hpl";

		std::ofstream file;
		file.open(path, std::fstream::binary);

		if (!file.is_open())
		{
			printf("ERROR, Unable to open '%s' : %s\n", path.c_str(), strerror(errno));
			return false;
		}

		file.write((char*)HPAL_HEADER, sizeof(HPAL_HEADER));
		file.write(pSrc, IMPL_PALETTE_DATALEN);
		file.close();

		printf("Written '%s'\n", path.c_str());

		pSrc += IMPL_PALETTE_DATALEN;
	}

	return true;
}

const char* GetCreatorName()
{
	printf("\nEnter creator's name (optional):\n");
	printf(">");

	static char buf[IMPL_CREATOR_LENGTH];
	if (fgets(buf, sizeof(buf), stdin))
	{
		buf[strcspn(buf, "\r\n")] = 0;
		return buf;
	}

	return "";
}

const char* GetDescription()
{
	printf("Enter description (optional):\n");
	printf(">");

	static char buf[IMPL_DESC_LENGTH];
	if (fgets(buf, sizeof(buf), stdin))
	{
		buf[strcspn(buf, "\r\n")] = 0;
		return buf;
	}

	return "";
}