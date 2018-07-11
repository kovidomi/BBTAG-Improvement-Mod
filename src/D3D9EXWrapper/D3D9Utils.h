#pragma once
#include "d3d9.h"
#include <Windows.h>
#include <vector>

class D3D9Utils
{
public:
	static HRESULT WINAPI DumpTextureToFile(IDirect3DBaseTexture9* pTexture, D3DXIMAGE_FILEFORMAT format, std::wstring dumpFolder = L"C:\\");
private:
	static std::vector<long> texturePointers;
	static bool WINAPI Contains(long textureP);
	static HRESULT WINAPI DumpTexture(IDirect3DBaseTexture9 * texture, D3DXIMAGE_FILEFORMAT format, std::wstring dumpFolder);
	static std::wstring GetImageExtension(D3DXIMAGE_FILEFORMAT format);
};