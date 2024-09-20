#pragma once

#include "Bindable.h"



namespace Bind
{
    class IndexBuffer : public Bindable
    {
    public:
        IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices);
        IndexBuffer(Graphics& gfx, std::string indentityTag, const std::vector<unsigned short>& indices);
        
        void Bind(Graphics& gfx) noexcept override;
        UINT GetCount() const noexcept;

        static std::shared_ptr<IndexBuffer> Resolve(Graphics& gfx, const std::string& identityTag, const std::vector<unsigned short>& indices);

        template<typename...Ignore>
        static std::string GenerateUID(const std::string& tag, Ignore&&...ignore)
        {
            return GenerateUID_(tag);
        }
        std::string GetUID() const noexcept override;

    private:
        static std::string GenerateUID_(const std::string& tag);

    protected:
        std::string mIdentityTag;

        UINT count;
        Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
    };
}