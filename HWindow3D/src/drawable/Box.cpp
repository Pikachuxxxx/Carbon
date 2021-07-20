#include "Box.h"
#include "bindable/VertexBuffer.h"
#include "bindable/VertexShader.h"
#include "bindable/PixelShader.h"
#include "bindable/IndexBuffer.h"
#include "bindable/InpuLayout.h"
#include "bindable/Topology.h"
#include "bindable/Viewport.h"
#include "bindable/TransformCBuffer.h"

#include <sstream>

Box::Box(Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist)
    : r(rdist(rng)), theta(adist(rng)), phi(adist(rng)), chi(adist(rng)), droll(ddist(rng)), dpitch(ddist(rng)), dyaw(ddist(rng)), dtheta(odist(rng)), dphi(odist(rng)), dchi(odist(rng))
{
    if (!IsStaticInitialized())
    {
        struct Vertex
        {
            float  x, y, z, r, g, b;
        };
        const std::vector<Vertex> vertices =
        {
           {  -1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f},
           {  1.0f, -1.0f, -1.0f,   0.0f, 1.0f, 0.0f},
           {  -1.0f, 1.0f, -1.0f,   0.0f, 0.0f, 1.0f},
           {  1.0f, 1.0f, -1.0f,    1.0f, 1.0f, 1.0f},

           {  -1.0f, -1.0f, 1.0f,   1.0f, 1.0f, 0.0f},
           {  1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 1.0f},
           {  -1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f},
           {  1.0f, 1.0f, 1.0f,     0.0f, 0.0f, 0.0f},

        };

        AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));

        auto pVertexShader = std::make_unique<VertexShader>(gfx, L"./src/shaders/cso/VertexShader.cso");
        auto pVertexShaderByteCode = pVertexShader->GetByteCode();
        AddStaticBind(std::move(pVertexShader));
        AddStaticBind(std::make_unique<PixelShader>(gfx, L"./src/shaders/cso/PixelShader.cso"));

        std::vector<unsigned short> indices = {
           0, 2, 1,    2, 3, 1,
           1, 3, 5,    3, 7, 5,
           2, 6, 3,    3, 6, 7,
           4, 5, 7,    4, 7, 6,
           0, 4, 2,    2, 4, 6,
           0, 1, 4,    1, 5, 4
        };
        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
        {
            {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            {"Color", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pVertexShaderByteCode));
        AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    }
    else
        SetIndexFromStatic();
    AddBind(std::make_unique<TransformCBuffer>(gfx, *this));
}

DirectX::XMMATRIX Box::GetTransformXM() const
{
    //throw std::logic_error("The method or operation is not implemented.");
    return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
        DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
        DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
        DirectX::XMMatrixTranslation(0.0f, 0.0f, 10.0f);
}

void Box::Update(float dt)
{
    roll += droll * dt;
    pitch += dpitch * dt;
    yaw += dyaw * dt;
    theta + dtheta * dt;
    phi += dphi * dt;
    chi += dchi * dt;
}
