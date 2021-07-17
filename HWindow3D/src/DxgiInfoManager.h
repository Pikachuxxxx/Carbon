#pragma once

#include "Win.h"
#include <vector>
#include <string>

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
	struct IDXGIInfoQueue* pDxgiInfoQueue = nullptr;
};

