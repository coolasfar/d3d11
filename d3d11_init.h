
#ifndef D3D11_INIT_H

#define D3D11_INIT_H

#include <DirectXMath.h>
#include <d3d11.h>
#include "platform.h"


//NOTE: COM interfaces are prefixed with a capital I, for example, ID3D11Texture2D, some
// calls increases the COM reference count, so we need to release it
#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

struct d3d11_basic
{
	ID3D11Device* D3dDevice;
	ID3D11DeviceContext* D3d11DeviceContext;
	IDXGISwapChain* SwapChain;
	ID3D11RenderTargetView* RenderTargetView;
	ID3D11DepthStencilView* DepthStencilView;
	ID3D11Texture2D*  DepthStencilBuffer;
	D3D11_VIEWPORT ScreenViewPort;
	D3D_DRIVER_TYPE D3dDriverType;

	uint32 	 m4xMsaaQuality;
};




namespace Colors
{
	XMGLOBALCONST DirectX::XMVECTORF32 White     = {1.0f, 1.0f, 1.0f, 1.0f};
	XMGLOBALCONST DirectX::XMVECTORF32 Black     = {0.0f, 0.0f, 0.0f, 1.0f};
	XMGLOBALCONST DirectX::XMVECTORF32 Red       = {1.0f, 0.0f, 0.0f, 1.0f};
	XMGLOBALCONST DirectX::XMVECTORF32 Green     = {0.0f, 1.0f, 0.0f, 1.0f};
	XMGLOBALCONST DirectX::XMVECTORF32 Blue      = {0.0f, 0.0f, 1.0f, 1.0f};
	XMGLOBALCONST DirectX::XMVECTORF32 Yellow    = {1.0f, 1.0f, 0.0f, 1.0f};
	XMGLOBALCONST DirectX::XMVECTORF32 Cyan      = {0.0f, 1.0f, 1.0f, 1.0f};
	XMGLOBALCONST DirectX::XMVECTORF32 Magenta   = {1.0f, 0.0f, 1.0f, 1.0f};

	XMGLOBALCONST DirectX::XMVECTORF32 Silver    = {0.75f, 0.75f, 0.75f, 1.0f};
	XMGLOBALCONST DirectX::XMVECTORF32 LightSteelBlue = {0.69f, 0.77f, 0.87f, 1.0f};
}



#endif