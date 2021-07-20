#pragma once

#include "Bindable.h"

class Topology : public Bindable
{
public:
	Topology(Graphics& gfx, D3D_PRIMITIVE_TOPOLOGY topology)
		: topology(topology)
	{}
	void Bind(Graphics& gfx) override
	{
		INFOMAN(gfx);
		CBN_GFX_THROW_INFO_ONLY(GetContext(gfx)->IASetPrimitiveTopology(topology));
	}
private:
	D3D_PRIMITIVE_TOPOLOGY topology;
};