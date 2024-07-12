#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <stdint.h>
#include <d3dcompiler.h>
#include <d3d11.h>
#include <d3d11_4.h>
#include <shlwapi.h>

typedef HRESULT(__stdcall* fn_D3D11CreateDevice)(
	IDXGIAdapter* pAdapter,
	D3D_DRIVER_TYPE         DriverType,
	HMODULE                 Software,
	UINT                    Flags,
	const D3D_FEATURE_LEVEL* pFeatureLevels,
	UINT                    FeatureLevels,
	UINT                    SDKVersion,
	ID3D11Device** ppDevice,
	D3D_FEATURE_LEVEL* pFeatureLevel,
	ID3D11DeviceContext** ppImmediateContext);

fn_D3D11CreateDevice LoadD3D11AndGetOriginalFuncPointer()
{
	char path[MAX_PATH];
	if (!GetSystemDirectoryA(path, MAX_PATH)) return 0;

	strcat_s(path, MAX_PATH * sizeof(char), "\\d3d11.dll");
	HMODULE d3d_dll = LoadLibraryA(path);

	if (!d3d_dll)
	{
		MessageBox(NULL, TEXT("Could Not Locate Original D3D11 DLL"), TEXT("Darn"), 0);
		return 0;
	}

	return (fn_D3D11CreateDevice)GetProcAddress(d3d_dll, TEXT("D3D11CreateDevice"));
}

HRESULT __stdcall D3D11CreateDevice(
	IDXGIAdapter* pAdapter,
	D3D_DRIVER_TYPE         DriverType,
	HMODULE                 Software,
	UINT                    Flags,
	const D3D_FEATURE_LEVEL* pFeatureLevels,
	UINT                    FeatureLevels,
	UINT                    SDKVersion,
	ID3D11Device** ppDevice,
	D3D_FEATURE_LEVEL* pFeatureLevel,
	ID3D11DeviceContext** ppImmediateContext
)
{
	fn_D3D11CreateDevice D3D11CreateDevice_Orig = LoadD3D11AndGetOriginalFuncPointer();

	HRESULT res = D3D11CreateDevice_Orig(
		pAdapter,
		DriverType,
		Software,
		Flags,
		pFeatureLevels,
		FeatureLevels,
		SDKVersion,
		ppDevice,
		pFeatureLevel,
		ppImmediateContext);

	return res;
}