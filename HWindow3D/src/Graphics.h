#pragma once

#include "Win.h"
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include "utils/CarbonException.h"
#include "DxgiInfoManager.h"

#define CBN_NO_GFX_EXCEPT() Graphics::NoGfxException(__LINE__, __FILE__)

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
private:
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif // !NDEBUG
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapchain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget = nullptr;
};

