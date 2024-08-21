#pragma once

#include "IndexedTrigList.h"
#include "Math.h"

#include <vector>
#include <array>

class Plane
{
public:
    template <typename V>
    static IndexedTrigList<V> MakeTesselated(int xDivisions, int yDivisions)
    {
        assert(xDivisions >= 1);
        assert(yDivisions >= 1);

        constexpr float width = 2.0f;
        constexpr float height = 2.0f;
        
        ///    +------ w ------+
        ///    +---+---+---+---+  +
        ///    |   |   |   |   |  |
        ///    +---+---+---+---+  |
        ///    |   |   |   |   |  |
        ///    +---+---+---+---+  h
        ///    |   |   |   |   |  |
        ///    +---+---+---+---+  |
        ///    |   |   |   |   |  |
        ///    +---+---+---+---+  +
        /// 4 dvisions => 5 vertices on respective row/column
        /// n divisions => n + 1 vertices
         
        const int nXVertices = xDivisions + 1;
        const int nYVertices = yDivisions + 1;

        std::vector<V> vertices(nXVertices * nYVertices);

        const float xSide = width / 2.0f;
        const float ySide = height / 2.0f;

        /// Grid cell/quad width and height sizes
        const float xDivisionSize = width / float(xDivisions);
        const float yDivisionSize = height / float(yDivisions);

        const auto bottomLeft = DirectX::XMVectorSet(-xSide, -ySide, 0.0f, 0.0f);

        
        for (int y = 0, i = 0; y < nYVertices; y++)
        {
            const float yPos = float(y) * yDivisionSize;

            for (int x = 0; x < nXVertices; x++, i++)
            {
                const float xPos = float(x) * xDivisionSize;

                /// The postion values of the vertex relative to the origin
                const auto relativeVertex = DirectX::XMVectorSet(xPos, yPos, 0.0f, 0.0f);

                /// The absoulte postion values of the vertex in 3D Space
                const auto vertex = DirectX::XMVectorAdd(bottomLeft, relativeVertex);

                DirectX::XMStoreFloat3(&vertices[i].pos, vertex);
            }
        }
        
        std::vector<unsigned short> indices;

        indices.reserve(sq(xDivisions * yDivisions) * 6);

        /// returns the index of a vetex in the vertices vector
        const auto v2i = [nXVertices](size_t x, size_t y)
            {
                return (unsigned short)(y * nXVertices + x);
            };

        for (size_t y = 0; y < yDivisions; y++)
        {
            for (size_t x = 0; x < xDivisions; x++)
            {
                const std::array<unsigned short, 4> indexArray =
                {
                    v2i(x,y),      // bottom left
                    v2i(x + 1, y),   // bottom right
                    v2i(x,y + 1),    // top left
                    v2i(x + 1, y + 1)  // top right
                };

                indices.push_back(indexArray[0]); /// V0   V2 +-----+ V1
                indices.push_back(indexArray[2]); /// V2      |\    |
                indices.push_back(indexArray[1]); /// V1      | \   |
                indices.push_back(indexArray[1]); /// V1      |  \  |
                indices.push_back(indexArray[2]); /// V2      |   \ |
                indices.push_back(indexArray[3]); /// V3   V0 +-----+ V3

            }
        }

        return{ std::move(vertices),std::move(indices) };
    }


    template<typename V>
    static IndexedTrigList<V> Make()
    {
        return MakeTesselated<V>(1, 1);
    }
};