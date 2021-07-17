#include "Graphics.h"

#include "dxerr.h"
#include <sstream>

#pragma comment(lib, "d3d11.lib")

#define CBN_GFX_THROW_FAILED(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HRException( __LINE__,__FILE__,hr )


#define CBN_EXCEPT_NOINFO(hr) Graphics::HRException(__LINE__, __FILE__, hr)
#define CBN_GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HRException( __LINE__,__FILE__,hr )

#define CBN_GFX_EXCEPT(hr) if( FAILED( hr = (hrcall) ) ) throw Graphics::HRException( __LINE__,__FILE__,hr )
#define CBN_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, (hr))

Graphics::Graphics(HWND hwnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hwnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	HRESULT hr;

	// Create the device and swapchain
	CBN_GFX_THROW_FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapchain,
		&pDevice,
		nullptr,
		&pContext
	));
	// Get the back buffer and render target
	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	CBN_GFX_THROW_FAILED(pSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), &pBackBuffer));
	CBN_GFX_THROW_FAILED(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget));
}

void Graphics::SetClearColor(float r, float g, float b)
{
	const float  color[] = {r, g, b, 1.0f};
	pContext->ClearRenderTargetView(pTarget.Get(), color);
}

void Graphics::OnFlip()
{
	HRESULT hr;
	if (FAILED(hr = pSwapchain->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw CBN_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
		}
		else
			CBN_GFX_THROW_FAILED(hr);
	}
}

Graphics::HRException::HRException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
	: Exception(line, file), m_HR(hr)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
}

const char* Graphics::HRException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorStringDescription() << std::endl
		<< GetOriginString();
	whatBufffer = oss.str();
	return whatBufffer.c_str();
}

const char* Graphics::HRException::GetType() const noexcept
{
	return "Carbon Graphics Exception";
}

HRESULT Graphics::HRException::GetErrorCode() const noexcept
{
	return m_HR;
}

std::string Graphics::HRException::GetErrorString() const noexcept
{
	return DXGetErrorString(m_HR);
}

std::string Graphics::HRException::GetErrorStringDescription() const noexcept
{
	char buf[512];
	DXGetErrorDescription(m_HR, buf, sizeof(buf));
	return buf;
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}

const char* Graphics::NoGfxException::GetType() const noexcept
{
	return "Graphics Exception! No Context";
}
