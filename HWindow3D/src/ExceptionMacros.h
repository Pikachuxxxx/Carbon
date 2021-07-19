#pragma once

#define CBN_GFX_EXCEPT_NOINFO(hr) Graphics::HRException(__LINE__, __FILE__, (hr))
#define CBN_GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HRException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define CBN_GFX_EXCEPT(hr) Graphics::HRException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define CBN_GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw CBN_GFX_EXCEPT(hr)
#define CBN_GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define CBN_GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw Graphics::InfoException( __LINE__,__FILE__,v);}}
#else
#define CBN_GFX_EXCEPT(hr) Graphics::HRException( __LINE__,__FILE__,(hr) )
#define CBN_GFX_THROW_INFO(hrcall) CBN_GFX_THROW_NOINFO(hrcall)
#define CBN_GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#define CBN_GFX_THROW_INFO_ONLY(call) (call)
#endif

#define CBN_CHWND_EXCEPT( hr ) WindowsWindow::WindowsWindowException( __LINE__,__FILE__,(hr) )
#define CBN_CHWND_LAST_EXCEPT() WindowsWindow::WindowsWindowException( __LINE__,__FILE__,GetLastError() )
#define CBN_CHWND_NOGFX_EXCEPT() Graphics::NoGfxException( __LINE__,__FILE__ )

#ifdef NDEBUG
#define INFOMAN(gfx) HRESULT hr
#else
#define INFOMAN(gfx) HRESULT hr;  DxgiInfoManager& infoManager = GetInfoManager((gfx))
#endif
