#include "D3D9Utils.h"

#include <sstream>

std::vector<long> D3D9Utils::texturePointers;

HRESULT D3D9Utils::DumpTextureToFile(IDirect3DBaseTexture9 * pTexture, D3DXIMAGE_FILEFORMAT format, std::wstring dumpFolder)
{
	if (!Contains((long)pTexture))
	{
		texturePointers.push_back((long)pTexture);
		return DumpTexture(pTexture, format, dumpFolder);
	}
}

bool WINAPI D3D9Utils::Contains(long textureP)
{
	for (std::vector<long>::iterator iter = texturePointers.begin(); iter != texturePointers.end(); iter++)
	{
		if (*iter == textureP)
			return true;
	}
	return false;
}

HRESULT WINAPI D3D9Utils::DumpTexture(IDirect3DBaseTexture9 * texture, D3DXIMAGE_FILEFORMAT format, std::wstring dumpFolder)
{
	std::wstringstream sstm;
	std::wstring filename = dumpFolder;

	sstm << filename << (long)texture << GetImageExtension(format);
	return D3DXSaveTextureToFile(sstm.str().c_str(), D3DXIFF_BMP, texture, NULL);
}

std::wstring D3D9Utils::GetImageExtension(D3DXIMAGE_FILEFORMAT format)
{
	std::wstring ext = L"";
	switch (format)
	{
	case D3DXIMAGE_FILEFORMAT::D3DXIFF_BMP:
		ext = L".bmp";
		break;
	case D3DXIMAGE_FILEFORMAT::D3DXIFF_JPG:
		ext = L".jpg";
		break;
	case D3DXIMAGE_FILEFORMAT::D3DXIFF_PNG:
		ext = L".png";
		break;
	case D3DXIMAGE_FILEFORMAT::D3DXIFF_TGA:
		ext = L".tga";
		break;
	}

	return ext;
}
