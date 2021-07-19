#pragma once

#include "Bindable.h"

class Topology : public Bindable
{
	Topology(Graphics& gfx, D3D_PRIMITIVE_TOPOLOGY topology)
		: topology(topology)
	{}
public:
	void Bind(Graphics& gfx) override
	{
		GetContext(gfx)->IASetPrimitiveTopology(topology);
	}
private:
	D3D_PRIMITIVE_TOPOLOGY topology;
};