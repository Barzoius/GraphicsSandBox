#pragma once

#include <vector>
#include <type_traits>

#include <DirectXMath.h>
#include "Graphics.h"

namespace Engine_RGBA
{
    struct RGBAColor
    {
        unsigned char a;
        unsigned char r;
        unsigned char g;
        unsigned char b;

    };
}

class VertexLayout
{
public:

    enum ElementType
    {
        Position2D,
        Position3D,
        Texture2D,
        Normal,
        Float3Color,
        Float4Color,
        RGBAColor,
    };

    template<ElementType> struct Map;
    template<> struct Map<Position2D>
    {
        using SysType = DirectX::XMFLOAT2;
        static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
        static constexpr const char* semantic = "Position";
    };

    template<ElementType> struct Map;
    template<> struct Map<Position3D>
    {
        using SysType = DirectX::XMFLOAT3;
        static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
        static constexpr const char* semantic = "Position";
    };
    template<> struct Map<Texture2D>
    {
        using SysType = DirectX::XMFLOAT2;
        static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
        static constexpr const char* semantic = "Texcoord";
    };
    template<> struct Map<Normal>
    {
        using SysType = DirectX::XMFLOAT3;
        static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
        static constexpr const char* semantic = "Normal";
    };
    template<> struct Map<Float3Color>
    {
        using SysType = DirectX::XMFLOAT3;
        static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
        static constexpr const char* semantic = "Color";
    };
    template<> struct Map<Float4Color>
    {
        using SysType = DirectX::XMFLOAT4;
        static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
        static constexpr const char* semantic = "Color";
    };
    template<> struct Map<RGBAColor>
    {
        using SysType = Engine_RGBA::RGBAColor;
        static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
        static constexpr const char* semantic = "Color";
    };

    class Element
    {
    public:
        Element(ElementType type, size_t offset) : type(type), offset(offset) {}

        size_t GetOffsetAfter() const noexcept
        {
            return offset + Size();
        }

        size_t GetOffset() const noexcept
        {
            return offset;
        }

        size_t Size() const noexcept
        {
            return SizeOf(type);
        }

        static constexpr size_t SizeOf(ElementType type) noexcept
        {
            switch(type)
            {
            case Position2D:
                return sizeof(DirectX::XMFLOAT2);
            case Position3D:
                return sizeof(DirectX::XMFLOAT3);
            case Texture2D:
                return sizeof(DirectX::XMFLOAT2);
            case Normal:
                return sizeof(DirectX::XMFLOAT3);
            case Float3Color:
                return sizeof(DirectX::XMFLOAT3);
            case Float4Color:
                return sizeof(DirectX::XMFLOAT3);
            case RGBAColor:
                return sizeof(unsigned int);
            }
            assert("Invalid type" && false);
            return 0u;
        }

        ElementType GetTpe() const noexcept
        {
            return type;
        }

    private:
        ElementType type;
        size_t offset;
    };

public:
    template<ElementType Type>
    const Element& Resolve() const noexcept
    {
        for (auto& e : elements)
        {
            if (e.GetType() == Type)
                return e;
        }
        assert("Couldn't match element type" && false);
        return elements.front();
    }

    const Element& ResolveByIndex(size_t index) const noexcept
    {
        return elements[index];
    }

    template<ElementType Type>
    VertexLayout& Append() noexcept
    {
        elements.emplace_back(Type, Size());
        return *this;
    }

    size_t Size() const noexcept
    {
        return elements.empty() ? 0u : elements.back().GetOffsetAfter();
    }

private:
    std::vector<Element> elements;
};




class Vertex
{
    friend class VertexBuffer;

public:
    template<VertexLayout::ElementType Type>
    auto& Attr() noexcept
    {
        auto pAttribute = pData + layout.Resolve<Type>().GetOffset();
        return *reinterpret_cast<typename VertexLayout::Map<Type>::SysType*>(pAttribute);
    }






private:
    char* pData = nullptr;
    const VertexLayout& layout;
};