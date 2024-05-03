#include "d3d11_init.h"

internal void
OnD3DResize(d3d11_basic* D3D11Basic, uint32 WindowWidth, uint32 WindowHeight)
{
	assert(D3D11Basic->D3d11DeviceContext);
	assert(D3D11Basic->D3dDevice);
	assert(D3D11Basic->SwapChain);

	D3D11Basic->RenderTargetView = 0;
	D3D11Basic->DepthStencilBuffer = 0;
	D3D11Basic->DepthStencilView = 0;

	ID3D11Texture2D* BackBuffer;
	D3D11Basic->SwapChain->ResizeBuffers(1, WindowWidth, WindowHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	D3D11Basic->SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBuffer));
	D3D11Basic->D3dDevice->CreateRenderTargetView(BackBuffer, 0, &D3D11Basic->RenderTargetView);


	D3D11_TEXTURE2D_DESC depthStencilDesc;
	
	depthStencilDesc.Width     = WindowWidth;
	depthStencilDesc.Height    = WindowHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count   = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0; 
	depthStencilDesc.MiscFlags      = 0;

	D3D11Basic->D3dDevice->CreateTexture2D(&depthStencilDesc, 0, &D3D11Basic->DepthStencilBuffer);
	D3D11Basic->D3dDevice->CreateDepthStencilView(D3D11Basic->DepthStencilBuffer, 0, &D3D11Basic->DepthStencilView);

	// Bind the render target view and depth/stencil view to the pipeline.

	D3D11Basic->D3d11DeviceContext->OMSetRenderTargets(1, &D3D11Basic->RenderTargetView, D3D11Basic->DepthStencilView);

	D3D11Basic->ScreenViewPort.TopLeftX = 0;
	D3D11Basic->ScreenViewPort.TopLeftY = 0;
	D3D11Basic->ScreenViewPort.Width    = static_cast<float>(WindowWidth);
	D3D11Basic->ScreenViewPort.Height   = static_cast<float>(WindowHeight);
	D3D11Basic->ScreenViewPort.MinDepth = 0.0f;
	D3D11Basic->ScreenViewPort.MaxDepth = 1.0f;

	D3D11Basic->D3d11DeviceContext->RSSetViewports(1, &D3D11Basic->ScreenViewPort);

}

internal bool
InitDirect3D(HWND hwnd, d3d11_basic *D3D11Basic, uint32 Width, uint32 Height)
{
    HRESULT hr = (HRESULT)0L;

    uint32 CreateDeviceFlag = 0;

#if defined(DEBUG) || defined(_DEBUG)  
	CreateDeviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D11Basic->D3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
    D3D_FEATURE_LEVEL FeatureLevel;
	hr = D3D11CreateDevice(
			0,                 // default adapter
			D3D11Basic->D3dDriverType,
			0,                 // no software device
			CreateDeviceFlag, 
			0, 0,              // default feature level array
			D3D11_SDK_VERSION,
			&D3D11Basic->D3dDevice,
			&FeatureLevel,
			&D3D11Basic->D3d11DeviceContext);
    
    if( FAILED(hr) )
	{
		MessageBox(0, "D3D11CreateDevice Failed.", 0, 0);
		return false;
	}

    if( FeatureLevel != D3D_FEATURE_LEVEL_11_0 )
	{
		MessageBox(0, "Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

    D3D11Basic->D3dDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &D3D11Basic->m4xMsaaQuality);

	assert(D3D11Basic->m4xMsaaQuality > 0);

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width  = Width;
	sd.BufferDesc.Height = Height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount  = 1;
	sd.OutputWindow = hwnd;
	sd.Windowed     = true;
	sd.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags        = 0;
    
	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."

	IDXGIDevice* dxgiDevice = 0;
	D3D11Basic->D3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	      
	IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	dxgiFactory->CreateSwapChain(D3D11Basic->D3dDevice, &sd, &D3D11Basic->SwapChain);

	OnD3DResize(D3D11Basic, Width, Height);

    return true;
}

internal void 
D3D11DrawScene(d3d11_basic* D3d11Basic)
{
	assert(D3d11Basic->D3dDevice);
	assert(D3d11Basic->D3d11DeviceContext);
	assert(D3d11Basic->SwapChain);

	D3d11Basic->D3d11DeviceContext->ClearRenderTargetView(D3d11Basic->RenderTargetView,
		reinterpret_cast<const float*>(&Colors::Silver));
	D3d11Basic->D3d11DeviceContext->ClearDepthStencilView(D3d11Basic->DepthStencilView,
		D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	D3d11Basic->SwapChain->Present(0,0);
}


