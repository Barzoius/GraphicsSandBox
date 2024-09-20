#pragma once


#include "Graphics.h"
#include "ErrorInfoManager.h"

namespace Bind
{
    class Bindable
    {
    public:
        virtual void Bind(Graphics& gfx) = 0;
        virtual ~Bindable() = default;

        virtual std::string GetUID() const noexcept
        {
            assert(false);
            return "";
        }

    protected:

        static ID3D11DeviceContext* GetContext(Graphics& gfx) noexcept;
        static ID3D11Device* GetDevice(Graphics& gfx) noexcept;
        static ErrorInfoManager& GetInfoManager(Graphics& gfx) noexcept;

    };
}