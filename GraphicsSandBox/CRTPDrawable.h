#pragma once

#include "Drawable.h"
#include "IndexBuffer.h"

template <class C>
class CRTPDrawable : public Drawable
{
protected:

    bool IsStaticInitialized() const noexcept
    {
        return !staticBinds.empty();
    }

    void AddStaticBind(std::unique_ptr<Bindable> bind) noexcept
    {
        assert("Use AddIndexBuffer to bind an index buffer" && typeid(*bind) != typeid(IndexBuffer));
        staticBinds.push_back(std::move(bind));
    }

    void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept
    {
        assert("Attempt to add a index buffer a second time" && pIndexBuffer == nullptr);
        pIndexBuffer = ibuf.get();
        staticBinds.push_back(std::move(ibuf));
    }

    void SetIndexFromStatic() noexcept
    {
        assert("Attempt to add a index buffer a second time" && pIndexBuffer == nullptr);
        for (const auto& b : staticBinds)
        {
            if (const auto p = dynamic_cast<IndexBuffer*>(b.get()))
            {
                pIndexBuffer = p;
                return;
            }
        }
        assert("FAILED TO FIND INDEX BUFFER IN staticBinds" && pIndexBuffer != nullptr);
    }

private:
    const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept override
    {
        return staticBinds;
    }

private:
    static std::vector<std::unique_ptr<Bindable>> staticBinds;
};

template<class C>
std::vector<std::unique_ptr<Bindable>> CRTPDrawable<C>::staticBinds;