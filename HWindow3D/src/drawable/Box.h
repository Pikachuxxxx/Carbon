#pragma once
#include "Drawable.h"

#include <random>

class Box : public Drawable
{
public:
	Box(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist);
	DirectX::XMMATRIX GetTransformXM() const override;
	void Update(float dt) override;
private:
	float r;
	float yaw = 0.0f, pitch = 0.0f, roll = 0.0f;
	float theta, phi, chi;
	float droll, dpithc, dyaw, dtheta, dphi, dchi;
};

