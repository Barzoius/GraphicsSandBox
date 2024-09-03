#pragma once
#include "Graphics.h"
#include <DirectXMath.h>

namespace Bind
{
    class Bindable;
    class IndexBuffer;
}

class Drawable
{
    template<class C>
    friend class CRTPDrawable;
public:
    Drawable() = default;
    Drawable(const Drawable&) = delete;

    virtual ~Drawable() = default;

    virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
    virtual void Update(float dt) noexcept {};

    void Draw(Graphics& gfx) const noexcept;

protected:
    void AddBind(std::unique_ptr<Bind::Bindable> bind) noexcept;
    void AddIndexBuffer(std::unique_ptr<Bind::IndexBuffer> ibuf) noexcept;

private:
    virtual const std::vector<std::unique_ptr<Bind::Bindable>>& GetStaticBinds() const noexcept = 0;

private:
    const Bind::IndexBuffer* pIndexBuffer = nullptr;
    std::vector < std::unique_ptr < Bind:: Bindable >> binds;
}; 
