#pragma once
#include "../D3D9EXWrapper/d3d9.h"

// Using D3D
bool WorldToScreen(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3* pos, D3DXVECTOR3* out);

// Using FOV
bool WorldToScreen(D3DXVECTOR3 src, D3DXVECTOR3 dst, D3DXVECTOR3 &screen, float fovx,
	float fovy, float windowWidth, float windowHeight, D3DXVECTOR3 left, D3DXVECTOR3 up, D3DXVECTOR3 forward);

// Using traditional matrix
bool WorldToScreen(D3DXVECTOR3 pos, D3DXVECTOR2 &screen, float matrix[16], int windowWidth, int windowHeight);