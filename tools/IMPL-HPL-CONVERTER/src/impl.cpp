#include "impl.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <atlstr.h>

//delete allocated memory after use
IMPL_t* LoadImplFromFile(std::wstring& wFolderPath)
{
	std::string folderPath(wFolderPath.begin(), wFolderPath.end());

	IMPL_t* fileContents = new IMPL_t();

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

			if (fileName.find(".impl") == std::string::npos)
			{
				printf("Skipping loading of '%s' : not an .impl file\n", fileName.c_str());
				continue;
			}

			std::ifstream file(fullPath, std::ios::binary);

			if (!file.is_open())
			{
				printf("ERROR, Unable to open '%s' : %s\n", fileName.c_str(), strerror(errno));
				continue;
			}

			file.read((char*)fileContents, sizeof(IMPL_t)); //what if the file is smaller then sizeof(IMPL_t) ?
			file.close();

			//check filesig
			if (strcmp(fileContents->header.filesig, "IMPL") != 0)
			{
				printf("ERROR, unrecognized file format of '%s' !\n", fileName.c_str());
				SAFE_DELETE(fileContents);
				break;
			}
			if (fileContents->header.datalen != sizeof(IMPL_data_t))
			{
				printf("ERROR, data size mismatch in '%s' !\n", fileName.c_str());
				SAFE_DELETE(fileContents);
				break;
			}

		} while (FindNextFile(hFind, &data));

		FindClose(hFind);
		return fileContents;
	}

	SAFE_DELETE(fileContents);
	return nullptr;
}

bool WriteImplToFile(IMPL_t *filledImpl)
{
	std::string path = std::string(filledImpl->paldata.palname) + ".impl";

	std::ofstream file;
	file.open(path, std::fstream::binary);

	if (!file.is_open())
	{
		printf("ERROR, Unable to open '%s' : %s\n", path.c_str(), strerror(errno));
		return false;
	}

	file.write((char*)filledImpl, sizeof(IMPL_t));
	file.close();

	printf("Written '%s'\n", path.c_str());

	return true;
}