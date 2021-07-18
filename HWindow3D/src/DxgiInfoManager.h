#pragma once

#include "Win.h"
#include <vector>
#include <string>
#include <dxgidebug.h>
#include <wrl.h>

class DxgiInfoManager
{
public:
	DxgiInfoManager();
	~DxgiInfoManager();
	void Set() noexcept;
	std::vector<std::string> GetMessages() const;
	NONCOPYABLE(DxgiInfoManager)
private:
	unsigned long long next = 0;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDxgiInfoQueue;
};

