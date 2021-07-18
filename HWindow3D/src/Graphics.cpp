#include "Graphics.h"

#include <sstream>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

Graphics::Graphics(HWND hwnd)
{
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = hwnd;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    HRESULT hr;

    // Create the device and swapchain
    CBN_GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_DEBUG,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &pSwapchain,
        &pDevice,
        nullptr,
        &pContext
    ));
    // Get the back buffer and render target
    Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
    CBN_GFX_THROW_INFO(pSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), &pBackBuffer));
    CBN_GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget));
}

void Graphics::SetClearColor(float r, float g, float b)
{
    const float  color[] = {r, g, b, 1.0f};
    CBN_GFX_THROW_INFO_ONLY(pContext->ClearRenderTargetView(pTarget.Get(), color));
}

void Graphics::OnFlip()
{
    HRESULT hr;
    if (FAILED(hr = pSwapchain->Present(1u, 0u)))
    {
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
        {
            throw CBN_GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
        }
        else
            CBN_GFX_THROW_INFO(hr);
    }
}

void Graphics::DrawHelloD3D11Triangle()
{
    HRESULT hr;
    struct Vertex { float x, y, r, g, b, t; };
    float t = sin(gfxTimer.Peek());
    const Vertex vertices[] = {
        {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, t},
        { -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, t},
        { 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, t},
        { 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, t},

    };
    unsigned short indices[] = {
        0, 1, 2, 2, 3, 0
    };

    // Create the vertex buffer
    Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.ByteWidth = sizeof(vertices);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0u;
    bufferDesc.MiscFlags = 0u;
    bufferDesc.StructureByteStride = sizeof(Vertex);
    D3D11_SUBRESOURCE_DATA subres{};
    subres.pSysMem = vertices;
    CBN_GFX_THROW_INFO(pDevice->CreateBuffer(&bufferDesc, &subres, &pVertexBuffer));

    // Create the index buffer
    Microsoft::WRL::ComPtr<ID3D11Buffer> pindexBuffer;
    D3D11_BUFFER_DESC idxbufferDesc{};
    idxbufferDesc.ByteWidth = sizeof(indices);
    idxbufferDesc.Usage = D3D11_USAGE_DEFAULT;
    idxbufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    idxbufferDesc.CPUAccessFlags = 0u;
    idxbufferDesc.MiscFlags = 0u;
    idxbufferDesc.StructureByteStride = sizeof(unsigned short);
    D3D11_SUBRESOURCE_DATA idxsubres{};
    idxsubres.pSysMem = indices;
    CBN_GFX_THROW_INFO(pDevice->CreateBuffer(&idxbufferDesc, &idxsubres, &pindexBuffer));

    // Bind the vertex buffer to the input assembler
    // It makes no sense to describe how the buffer data is offset and stride is again, can it not take from the buffer description?
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;
    CBN_GFX_THROW_INFO_ONLY(pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset));
    CBN_GFX_THROW_INFO_ONLY(pContext->IASetIndexBuffer(pindexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u));

    // Primitive topology
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Pixel shader
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
    Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
    CBN_GFX_THROW_INFO(D3DReadFileToBlob(L"./src/shaders/cso/PixelShader.cso", &pBlob));
    CBN_GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
    // Bind the shader aka use as if in OpenGL
    pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

    // Vertex shader
    Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
    CBN_GFX_THROW_INFO(D3DReadFileToBlob(L"./src/shaders/cso/VertexShader.cso", &pBlob));
    CBN_GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
    // Bind the shader aka use as if in OpenGL
    pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

    // Set and Bind the IA object
    Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
    const D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {"Color", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 2 floats for position = 8bytes
        {"Time", 0, DXGI_FORMAT_R32_FLOAT, 0, 20u, D3D11_INPUT_PER_VERTEX_DATA, 0 }

    };
    CBN_GFX_THROW_INFO(pDevice->CreateInputLayout(ied, (UINT)std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout));
    // Bind the Input Layout
    CBN_GFX_THROW_INFO_ONLY(pContext->IASetInputLayout(pInputLayout.Get()));

    // Configure the viewport
    D3D11_VIEWPORT viewport;
    viewport.Width = 800;
    viewport.Height = 600;
    viewport.MaxDepth = 1;
    viewport.MinDepth = 0;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    pContext->RSSetViewports(1, &viewport);

    // Bind the render target
    pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

    //CBN_GFX_THROW_INFO_ONLY(pContext->Draw((UINT)std::size(vertices), 0u));
    CBN_GFX_THROW_INFO_ONLY(pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u));
}

Graphics::HRException::HRException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
    : Exception(line, file), m_HR(hr)
{
    // join all info messages with newlines into single string
    for (const auto& m : infoMsgs)
    {
        info += m;
        info.push_back('\n');
    }
    // remove final newline if exists
    if (!info.empty())
    {
        info.pop_back();
    }
}

const char* Graphics::HRException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
        << std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
        << "[Error String] " << GetErrorString() << std::endl
        << "[Description] " << GetErrorStringDescription() << std::endl;
    if (!info.empty())
    {
        oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
    }
    oss	<< GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Graphics::HRException::GetType() const noexcept
{
    return "Carbon Graphics Exception";
}

HRESULT Graphics::HRException::GetErrorCode() const noexcept
{
    return m_HR;
}

std::string Graphics::HRException::GetErrorString() const noexcept
{
    return DXGetErrorString(m_HR);
}

std::string Graphics::HRException::GetErrorStringDescription() const noexcept
{
    char buf[512];
    DXGetErrorDescription(m_HR, buf, sizeof(buf));
    return buf;
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
    return "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}

const char* Graphics::NoGfxException::GetType() const noexcept
{
    return "Graphics Exception! No Context";
}

Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
    : Exception(line, file)
{
    // join all info messages with newlines into single string
    for (const auto& m : infoMsgs)
    {
        info += m;
        info.push_back('\n');
    }
    // remove final newline if exists
    if (!info.empty())
    {
        info.pop_back();
    }
}

const char* Graphics::InfoException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const noexcept
{
    return "Carbon Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
    return info;
}
