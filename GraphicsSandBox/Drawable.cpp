#include "Drawable.h"
#include "IndexBuffer.h"

#include <cassert>
#include <typeinfo>

#include "GFX_MACROS.h"

using namespace Bind;

#include <iostream>

void Drawable::Draw(Graphics& gfx) const noexcept
{
    for (auto& b : binds)
    {
        //std::cout << "Bind\n";
        b->Bind(gfx);
    }

    gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::shared_ptr<Bindable> bind) noexcept
{
    if (typeid(*bind) == typeid(IndexBuffer))
    {
        assert("Binding multiple index buffers not allowed" && pIndexBuffer == nullptr);
        pIndexBuffer = &static_cast<IndexBuffer&>(*bind);
    }

    binds.push_back(std::move(bind));
}
