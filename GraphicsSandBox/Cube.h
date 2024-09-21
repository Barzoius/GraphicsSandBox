#pragma once

#include "IndexedTrigList.h"
#include "VertexSystem.h"

#include <DirectXMath.h>

#include <optional>
#include <array>

class Cube
{
public:
   
    static IndexedTrigList Make(DVS::VertexLayout layout)
    {

        using namespace DVS;
        using Type = DVS::VertexLayout::ElementType;


        constexpr float side = 1.0f / 2.0f;

        DVS::VertexBuffer vertices{std::move(layout), 8u };

        vertices[0].Attr<Type::Position3D>() = { -side,-side,-side };
        vertices[1].Attr<Type::Position3D>() = { side, -side,-side };
        vertices[2].Attr<Type::Position3D>() = { -side, side,-side };
        vertices[3].Attr<Type::Position3D>() = { side, side,-side };
        vertices[4].Attr<Type::Position3D>() = { -side, -side,side };
        vertices[5].Attr<Type::Position3D>() = { side, -side,side };
        vertices[6].Attr<Type::Position3D>() = { -side, side,side };
        vertices[7].Attr<Type::Position3D>() = { side, side,side };


        return{
            std::move(vertices),{
                0,2,1, 2,3,1,
                1,3,5, 3,7,5,
                2,6,3, 3,6,7,
                4,5,7, 4,7,6,
                0,4,2, 2,4,6,
                0,1,4, 1,5,4
            }
        };
    }


    static IndexedTrigList MakeWithIndependentVertices(DVS::VertexLayout layout)
    {
        using namespace DVS;
        using Type = DVS::VertexLayout::ElementType;

        constexpr float side = 1.0f / 2.0f;

        VertexBuffer vertices(std::move(layout), 24u);

        vertices[0].Attr<Type::Position3D>() = { -side,-side,-side };
        vertices[1].Attr<Type::Position3D>() = { side,-side,-side };
        vertices[2].Attr<Type::Position3D>() = { -side,side,-side };
        vertices[3].Attr<Type::Position3D>() = { side,side,-side };
        vertices[4].Attr<Type::Position3D>() = { -side,-side,side };
        vertices[5].Attr<Type::Position3D>() = { side,-side,side };
        vertices[6].Attr<Type::Position3D>() = { -side,side,side };
        vertices[7].Attr<Type::Position3D>() = { side,side,side };
        vertices[8].Attr<Type::Position3D>() = { -side,-side,-side };
        vertices[9].Attr<Type::Position3D>() = { -side,side,-side };
        vertices[10].Attr<Type::Position3D>() = { -side,-side,side };
        vertices[11].Attr<Type::Position3D>() = { -side,side,side };
        vertices[12].Attr<Type::Position3D>() = { side,-side,-side };
        vertices[13].Attr<Type::Position3D>() = { side,side,-side };
        vertices[14].Attr<Type::Position3D>() = { side,-side,side };
        vertices[15].Attr<Type::Position3D>() = { side,side,side };
        vertices[16].Attr<Type::Position3D>() = { -side,-side,-side };
        vertices[17].Attr<Type::Position3D>() = { side,-side,-side };
        vertices[18].Attr<Type::Position3D>() = { -side,-side,side };
        vertices[19].Attr<Type::Position3D>() = { side,-side,side };
        vertices[20].Attr<Type::Position3D>() = { -side,side,-side };
        vertices[21].Attr<Type::Position3D>() = { side,side,-side };
        vertices[22].Attr<Type::Position3D>() = { -side,side,side };
        vertices[23].Attr<Type::Position3D>() = { side,side,side };

    return{
            std::move(vertices),{
            0,2, 1,    2,3,1,
                4,5, 7,    4,7,6,
                8,10, 9,  10,11,9,
                12,13,15, 12,15,14,
                16,17,18, 18,17,19,
                20,23,21, 20,22,23
            }
         };

    }


    static IndexedTrigList MakeIndependentTextured()
    {
        using namespace DVS;
        using Type = DVS::VertexLayout::ElementType;

        auto itl = MakeWithIndependentVertices(std::move(VertexLayout{}
            .Append(Type::Position3D)
            .Append(Type::Normal)
            .Append(Type::Texture2D)
        ));

        itl.vertices[0].Attr<Type::Texture2D>() = { 0.0f,0.0f };
        itl.vertices[1].Attr<Type::Texture2D>() = { 1.0f,0.0f };
        itl.vertices[2].Attr<Type::Texture2D>() = { 0.0f,1.0f };
        itl.vertices[3].Attr<Type::Texture2D>() = { 1.0f,1.0f };
        itl.vertices[4].Attr<Type::Texture2D>() = { 0.0f,0.0f };
        itl.vertices[5].Attr<Type::Texture2D>() = { 1.0f,0.0f };
        itl.vertices[6].Attr<Type::Texture2D>() = { 0.0f,1.0f };
        itl.vertices[7].Attr<Type::Texture2D>() = { 1.0f,1.0f };
        itl.vertices[8].Attr<Type::Texture2D>() = { 0.0f,0.0f };
        itl.vertices[9].Attr<Type::Texture2D>() = { 1.0f,0.0f };
        itl.vertices[10].Attr<Type::Texture2D>() = { 0.0f,1.0f };
        itl.vertices[11].Attr<Type::Texture2D>() = { 1.0f,1.0f };
        itl.vertices[12].Attr<Type::Texture2D>() = { 0.0f,0.0f };
        itl.vertices[13].Attr<Type::Texture2D>() = { 1.0f,0.0f };
        itl.vertices[14].Attr<Type::Texture2D>() = { 0.0f,1.0f };
        itl.vertices[15].Attr<Type::Texture2D>() = { 1.0f,1.0f };
        itl.vertices[16].Attr<Type::Texture2D>() = { 0.0f,0.0f };
        itl.vertices[17].Attr<Type::Texture2D>() = { 1.0f,0.0f };
        itl.vertices[18].Attr<Type::Texture2D>() = { 0.0f,1.0f };
        itl.vertices[19].Attr<Type::Texture2D>() = { 1.0f,1.0f };
        itl.vertices[20].Attr<Type::Texture2D>() = { 0.0f,0.0f };
        itl.vertices[21].Attr<Type::Texture2D>() = { 1.0f,0.0f };
        itl.vertices[22].Attr<Type::Texture2D>() = { 0.0f,1.0f };
        itl.vertices[23].Attr<Type::Texture2D>() = { 1.0f,1.0f };

        return itl;
    }
};
