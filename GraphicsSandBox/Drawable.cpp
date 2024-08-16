#include "Drawable.h"
#include "IndexBuffer.h"

#include <cassert>
#include <typeinfo>

#include "GFX_MACROS.h"

void Drawable::Draw(Graphics& gfx) const noexcept
{
    for (auto& b : binds)
    {
        b->Bind(gfx);
    }

    for (auto& b : GetStaticBinds())
    {
        b->Bind(gfx);
    }

    gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept
{
    assert("Use AddIndexBuffer to bind index buffers" && typeid(*bind) != typeid(IndexBuffer));
    binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept
{
    assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
    pIndexBuffer = ibuf.get();
    binds.push_back(std::move(ibuf));
}