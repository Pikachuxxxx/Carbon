#pragma once

#include "Win.h"
#include "dxerr.h"
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include "utils/CarbonException.h"
#include "DxgiInfoManager.h"

#include "ExceptionMacros.h"

/*
#define CBN_NO_GFX_EXCEPT() Graphics::NoGfxException(__LINE__, __FILE__)
#define CBN_GFX_THROW_FAILED(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HRException( __LINE__,__FILE__,hr )
#define CBN_EXCEPT_NOINFO(hr) Graphics::HRException(__LINE__, __FILE__, hr)
#define CBN_GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HRException( __LINE__,__FILE__,hr )
#define CBN_GFX_EXCEPT(hr) if( FAILED( hr = (hrcall) ) ) throw Graphics::HRException( __LINE__,__FILE__,hr )
#define CBN_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, (hr))
*/

class Graphics
{
public:
	class Exception : public CarbonException
	{
		using CarbonException::CarbonException;
	};
	class HRException : public Exception
	{
	public:
		HRException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorStringDescription() const noexcept;
		std::string GetErrorInfo() const noexcept { return info; }
	private:
		HRESULT m_HR;
		std::string info;
	};
	class InfoException : public Exception
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info;
	};
	class DeviceRemovedException : public HRException
	{
		using HRException::HRException;
	public:
		virtual const char* GetType() const noexcept override;
	private:
		std::string reason;
	};
	class NoGfxException : public Exception
	{
	public:
		using Exception::Exception;
		const char* GetType() const noexcept override;
	};
public:
	Graphics(HWND hwnd);
	~Graphics() = default;
	NONCOPYABLE(Graphics)
	void SetClearColor(float r, float g, float b);
	void OnFlip();
	void DrawHelloD3D11Triangle()
	{
		//pContext->IASetVertexBuffers();
		//pContext->Draw(3u, 0u);
	}
private:
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif // !NDEBUG
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapchain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget = nullptr;
};

