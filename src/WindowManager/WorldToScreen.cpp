#include "WorldToScreen.h"

#include "../interfaces.h"

#define PI 3.1415927f

D3DXVECTOR3 Subtract(D3DXVECTOR3 src, D3DXVECTOR3 dst)
{
	D3DXVECTOR3 diff;
	diff.x = src.x - dst.x;
	diff.y = src.y - dst.y;
	diff.z = src.z - dst.z;
	return diff;
}

D3DXVECTOR3 Divide(D3DXVECTOR3 src, float num)
{
	D3DXVECTOR3 vec;
	vec.x = src.x / num;
	vec.y = src.y / num;
	vec.z = src.z / num;

	return vec;
}

float Magnitude(D3DXVECTOR3 vec)
{
	return sqrtf(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

float DotProduct(D3DXVECTOR3 src, D3DXVECTOR3 dst)
{
	return src.x * dst.x + src.y * dst.y + src.z * dst.z;
}

// Using FOV
bool WorldToScreen(D3DXVECTOR3 src, D3DXVECTOR3 dst, D3DXVECTOR3 &screen, float fovx, 
	float fovy, float windowWidth, float windowHeight, D3DXVECTOR3 left, D3DXVECTOR3 up, D3DXVECTOR3 forward)
{
	D3DXVECTOR3 transform;
	float xc, yc;
	float px, py;
	float z;

	px = tan(fovx * PI / 360.0);
	py = tan(fovy * PI / 360.0);

	transform = Subtract(dst, src);

	xc = windowWidth / 2.0;
	yc = windowHeight / 2.0;

	z = DotProduct(transform, left);

	if (z <= 0.1)
	{
		return false;
	}

	screen.x = xc - DotProduct(transform, up) *xc / (z*px);
	screen.y = yc - DotProduct(transform, forward) *yc / (z*py);

	return true;
}


// Using traditional matrix
bool WorldToScreen(D3DXVECTOR3 pos, D3DXVECTOR2 &screen, float matrix[16], int windowWidth, int windowHeight)
{
	//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
	D3DXVECTOR4 clipCoords;
	clipCoords.x = pos.x*matrix[0] + pos.y*matrix[1] + pos.z*matrix[2] + matrix[3];
	clipCoords.y = pos.x*matrix[4] + pos.y*matrix[5] + pos.z*matrix[6] + matrix[7];
	clipCoords.z = pos.x*matrix[8] + pos.y*matrix[9] + pos.z*matrix[10] + matrix[11];
	clipCoords.w = pos.x*matrix[12] + pos.y*matrix[13] + pos.z*matrix[14] + matrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	//perspective division, dividing by clip.W = Normalized Device Coordinates
	D3DXVECTOR3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}

bool WorldToScreen(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3* pos, D3DXVECTOR3* out)
{
	D3DVIEWPORT9 viewPort;
	D3DXMATRIX view, projection, world;

	pDevice->GetViewport(&viewPort);
	pDevice->GetTransform(D3DTS_VIEW, &view);
	pDevice->GetTransform(D3DTS_PROJECTION, &projection);
	D3DXMatrixIdentity(&world);

	// D3DXVec3Project(out, pos, &viewPort, &projection, &view, &world);
	D3DXVec3Project(out, pos, &viewPort, g_gameVals.projMatrix, g_gameVals.viewMatrix, &world);
	if (out->z < 1) {
		return true;
	}
	return false;
}