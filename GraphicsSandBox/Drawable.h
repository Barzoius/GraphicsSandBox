#pragma once
#include "Graphics.h"
#include <DirectXMath.h>

class Bindable;

class Drawable
{
public:
    Drawable() = default;
    Drawable(const Drawable&) = delete;

    virtual ~Drawable() = default;

    virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
    virtual void Update(float dt) noexcept = 0;

    void Draw(Graphics& gfx) const noexcept;
    void AddBind(std::unique_ptr<Bindable> bind) noexcept;
    void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf) noexcept;

private:
    const IndexBuffer* pIndexBuffer = nullptr;
    std::vector<std::unique_ptr<Bindable>> binds;
}; 
