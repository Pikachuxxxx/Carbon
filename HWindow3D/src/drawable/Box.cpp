#include "Box.h"
#include "bindable/VertexBuffer.h"

#include <sstream>

Box::Box(Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist)
	: r(rdist(rng)), theta(adist(rng)), phi(adist(rng)), chi(adist(rng)), droll(ddist(rng)), dpithc(ddist(rng)), dyaw(ddist(rng)), dtheta(odist(rng)), dphi(odist(rng)), dchi(odist(rng))
{
	struct Vertex
	{
		float  x, y, z;
	}pos;
	const std::vector<Vertex> vertices =
	{
		{  -1.0f, -1.0f, -1.0f, },
		{ 1.0f, -1.0f, -1.0f, },
		{  -1.0f, 1.0f, -1.0f, },
		{  1.0f, 1.0f, -1.0f, },
		{ -1.0f, -1.0f, 1.0f, },
		{ 1.0f, -1.0f, 1.0f, },
		{ -1.0f, 1.0f, 1.0f, },
		{ 1.0f, 1.0f, 1.0f, }
	};

	AddBind(std::make_unique<VertexBuffer>(gfx, vertices));

}

DirectX::XMMATRIX Box::GetTransformXM() const
{
	//throw std::logic_error("The method or operation is not implemented.");
	return DirectX::XMMatrixIdentity();
}

void Box::Update(float dt)
{
	//throw std::logic_error("The method or operation is not implemented.");
	std::ostringstream oss;
	oss << "Hello";
}