#pragma once

#include "Bindable.h"
#include "GFX_MACROS.h"
#include "VertexSystem.h"

namespace Bind
{
    class VertexBuffer : public Bindable
    {
    public:
      
        VertexBuffer(Graphics& gfx, const std::string& identityTag, const DVS::VertexBuffer& vbuf);
        
        VertexBuffer(Graphics& gfx, const DVS::VertexBuffer& vbuf);

        void Bind(Graphics& gfx) noexcept override;

        static std::shared_ptr<VertexBuffer> Resolve(Graphics& gfx, const std::string& tag,const DVS::VertexBuffer& vbuf);
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
        UINT stride;
        Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
    };

}