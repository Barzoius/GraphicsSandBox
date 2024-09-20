#pragma once

#include "Bindable.h"

namespace Bind
{
    class VertexShader : public Bindable
    {
    public:
        VertexShader(Graphics& gfx, const std::string& path);
        void Bind(Graphics& gfx) noexcept override;
        ID3DBlob* GetBytecode() const noexcept;


        static std::shared_ptr<VertexShader> Resolve(Graphics& gfx, const std::string& path);
        static std::string GenerateUID(const std::string& path);
        std::string GetUID() const noexcept override;

    protected:
        std::string vertexPath;
        Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
        Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob;
    };
}