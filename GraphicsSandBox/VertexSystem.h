#pragma once

#include <vector>
#include <type_traits>

#include <DirectXMath.h>
#include "Graphics.h"

namespace DVS
{
    struct RGBAColor
    {
        unsigned char a;
        unsigned char r;
        unsigned char g;
        unsigned char b;

    };


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
            static constexpr const char* code = "P2";
        };

        template<ElementType> struct Map;
        template<> struct Map<Position3D>
        {
            using SysType = DirectX::XMFLOAT3;
            static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
            static constexpr const char* semantic = "Position";
            static constexpr const char* code = "P3";
        };
        template<> struct Map<Texture2D>
        {
            using SysType = DirectX::XMFLOAT2;
            static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
            static constexpr const char* semantic = "Texcoord";
            static constexpr const char* code = "TEX";
        };
        template<> struct Map<Normal>
        {
            using SysType = DirectX::XMFLOAT3;
            static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
            static constexpr const char* semantic = "Normal";
            static constexpr const char* code = "NOR";
        };
        template<> struct Map<Float3Color>
        {
            using SysType = DirectX::XMFLOAT3;
            static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
            static constexpr const char* semantic = "Color";
            static constexpr const char* code = "C3";
        };
        template<> struct Map<Float4Color>
        {
            using SysType = DirectX::XMFLOAT4;
            static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
            static constexpr const char* semantic = "Color";
            static constexpr const char* code = "P4";
        };
        template<> struct Map<RGBAColor>
        {
            using SysType = DVS::RGBAColor;
            static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
            static constexpr const char* semantic = "Color";
            static constexpr const char* code = "P8";
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
                switch (type)
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

            ElementType GetType() const noexcept
            {
                return type;
            }

            D3D11_INPUT_ELEMENT_DESC GetDesc() const noexcept
            {
                switch (type)
                {
                case Position2D:
                    return GenerateDesc<Position2D>(GetOffset());

                case Position3D:
                    return GenerateDesc<Position3D>(GetOffset());

                case Texture2D:
                    return GenerateDesc<Texture2D>(GetOffset());

                case Normal:
                    return GenerateDesc<Normal>(GetOffset());

                case Float3Color:
                    return GenerateDesc<Float3Color>(GetOffset());

                case Float4Color:
                    return GenerateDesc<Float4Color>(GetOffset());

                case RGBAColor:
                    return GenerateDesc<RGBAColor>(GetOffset());
                }
                
                assert("Invalid element type" && false);
                return { "INVALID",0,DXGI_FORMAT_UNKNOWN,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 };

            }

            const char* GetCode() const noexcept
            {
                {
                    switch (type)
                    {
                    case Position2D:
                        return Map<Position2D>::code;
                    case Position3D:
                        return Map<Position3D>::code;
                    case Texture2D:
                        return Map<Texture2D>::code;
                    case Normal:
                        return Map<Normal>::code;
                    case Float3Color:
                        return Map<Float3Color>::code;
                    case Float4Color:
                        return Map<Float4Color>::code;
                    case RGBAColor:
                        return Map<RGBAColor>::code;
                    }
                    assert("Invalid element type" && false);
                    return "Invalid";
                }
            }

        private:
            template <ElementType type>
            static constexpr D3D11_INPUT_ELEMENT_DESC GenerateDesc(size_t offset) noexcept
            {
                return { Map<type>::semantic, 0,
                        Map<type>::dxgiFormat, 0,
                        (UINT)offset, D3D11_INPUT_PER_VERTEX_DATA,0 };
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


        VertexLayout& Append(ElementType type) noexcept
        {
            elements.emplace_back(type, Size());
            return *this;
        }

        size_t Size() const noexcept
        {
            return elements.empty() ? 0u : elements.back().GetOffsetAfter();
        }

        size_t GetElementCount() const noexcept
        {
            return elements.size();
        }

        std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DLayout() const noexcept
        {
            std::vector<D3D11_INPUT_ELEMENT_DESC> desc;

            desc.reserve(GetElementCount());

            for (const auto& elem : elements)
            {
                desc.push_back(elem.GetDesc());
            }

            return desc;
        }

        std::string GetCode() const noexcept
        {
            {
                std::string code;
                for (const auto& e : elements)
                {
                    code += e.GetCode();
                }
                return code;
            }
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

        template<typename T>
        void SetAttributeByIndex(size_t index, T&& value) noexcept
        {
            const auto& element = layout.ResolveByIndex(index);
            auto pAttribute = pData + element.GetOffset();

            switch (element.GetType())
            {
            case VertexLayout::Position2D:
                SetAttribute<VertexLayout::Position2D>(pAttribute, std::forward<T>(value));
                break;

            case VertexLayout::Position3D:
                SetAttribute<VertexLayout::Position3D>(pAttribute, std::forward<T>(value));
                break;

            case VertexLayout::Texture2D:
                SetAttribute<VertexLayout::Texture2D>(pAttribute, std::forward<T>(value));
                break;

            case VertexLayout::Normal:
                SetAttribute<VertexLayout::Normal>(pAttribute, std::forward<T>(value));
                break;

            case VertexLayout::Float3Color:
                SetAttribute<VertexLayout::Float3Color>(pAttribute, std::forward<T>(value));
                break;

            case VertexLayout::Float4Color:
                SetAttribute<VertexLayout::Float4Color>(pAttribute, std::forward<T>(value));
                break;

            case VertexLayout::RGBAColor:
                SetAttribute<VertexLayout::RGBAColor>(pAttribute, std::forward<T>(value));
                break;

            default:
                assert("Bad element type" && false);
            }
        }

    protected:
        Vertex(char* pData, const VertexLayout& layout) noexcept
            :
            pData(pData),
            layout(layout)
        {
            assert(pData != nullptr);
        }

    private:

        template<typename First, typename ...Rest>
        void SetAttributeByIndex(size_t index, First&& first, Rest&&... rest) noexcept
        {
            SetAttributeByIndex(index, std::forward<First>(first));
            SetAttributeByIndex(index + 1, std::forward<Rest>(rest)...);
        }


        template<VertexLayout::ElementType DestLayoutType, typename SrcType>
        void SetAttribute(char* pAttribute, SrcType&& val) noexcept
        {
            using Dest = typename VertexLayout::Map<DestLayoutType>::SysType;
            if constexpr (std::is_assignable<Dest, SrcType>::value)
            {
                *reinterpret_cast<Dest*>(pAttribute) = val;
            }
            else
            {
                assert("Parameter attribute type mismatch" && false);
            }
        }




    private:
        char* pData = nullptr;
        const VertexLayout& layout;
    };


    class ConstVertex
    {
    public:
        ConstVertex(const Vertex& vertex) noexcept : vertex(vertex) {}

        template < VertexLayout::ElementType Type>
        const auto& Attr() const noexcept
        {
            return const_cast<Vertex&>(vertex).Attr<Type>();
        }

    private:
        Vertex vertex;
    };


    class VertexBuffer
    {
    public:
        VertexBuffer(VertexLayout layout) noexcept : layout(std::move(layout)) {}


        const char* GetData() const noexcept
        {
            return buffer.data();
        }

        const VertexLayout& GetLayout() const noexcept
        {
            return layout;
        }

        size_t Size() const noexcept // in vertices
        {
            return buffer.size() / layout.Size();
        }

        size_t SizeBytes() const noexcept
        {
            return buffer.size();
        }

        template<typename ...Params>
        void EmplaceBack(Params&&... params) noexcept
        {
            assert(sizeof...(params) == layout.GetElementCount() && "Param count doesn't match number of vertex elements");
            buffer.resize(buffer.size() + layout.Size());
            Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
        }

        Vertex Back() noexcept
        {
            assert(buffer.size() != 0u);
            return Vertex{ buffer.data() + buffer.size() - layout.Size(), layout };
        }

        Vertex Front() noexcept
        {
            assert(buffer.size() != 0u);
            return Vertex{ buffer.data(),layout };
        }

        Vertex operator[](size_t index) noexcept
        {
            assert(index < Size());
            return Vertex{ buffer.data() + layout.Size() * index, layout };
        }

        ConstVertex Back() const noexcept
        {
            return const_cast<VertexBuffer*>(this)->Back();
        }
        ConstVertex Front() const noexcept
        {
            return const_cast<VertexBuffer*>(this)->Front();
        }
        ConstVertex operator[](size_t i) const noexcept
        {
            return const_cast<VertexBuffer&>(*this)[i];
        }

    private:
        std::vector<char> buffer;
        VertexLayout layout;
    };

}