#pragma once

#include "IndexedTrigList.h"
#include "Math.h"

#include <vector>
#include <array>

class Prism
{
public:
    template <typename V>
    static IndexedTrigList<V> MakeTesselated(int sectors)
    {
        const float radius = 1.0f;
        const float height = 3.0f;

        const float longitudeAngle = 2.0f * FPI / sectors;

        float sectorAngle;

        std::vector<float> circleVertices;

        for (int i = 0; i <= sectors; i++)
        {
            sectorAngle = longitudeAngle * i;
            circleVertices.push_back(cosf(sectorAngle));
            circleVertices.push_back(sinf(sectorAngle));
            circleVertices.push_back(0.0f); // z
        }

        std::vector<V> vertices;

     

        for (int i = 0; i < 2; i++)
        {
            float z = -height / 2.0f + i * height;

            for (int j = 0, k = 0; j <= sectors; j++, k += 3)
            {
                vertices.emplace_back();

                float ux = circleVertices[k];
                float uy = circleVertices[k + 1];


                const auto vertex = DirectX::XMVectorSet(ux * radius, uy * radius, z, 0.0f);
                DirectX::XMStoreFloat3(&vertices.back().pos, vertex);

            }
        }

        unsigned short baseCenterIndex = (unsigned short)vertices.size();
        unsigned short topCenterIndex = (unsigned short)(baseCenterIndex + sectors + 1);

        for (int i = 0; i < 2; i++)
        {
            float z = -height / 2.0f + i * height;


            vertices.emplace_back();
            const auto circleVertex = DirectX::XMVectorSet(0.0f, 0.0f, z, 0.0f);
            DirectX::XMStoreFloat3(&vertices.back().pos, circleVertex);

            for (int j = 0, k = 0; j < sectors; j++, k += 3)
            {
                vertices.emplace_back();

                float ux = circleVertices[k];
                float uy = circleVertices[k + 1];

          
                const auto vertex = DirectX::XMVectorSet(ux * radius, uy * radius, z, 0.0f);
                DirectX::XMStoreFloat3(&vertices.back().pos, vertex);

            }
        }


        std::vector<unsigned short> indices;
        unsigned short k1 = 0;                         // 1st vertex index at base
        unsigned short k2 = sectors + 1;           // 1st vertex index at top

        for (int i = 0; i < sectors; i++, k1++, k2++)
        {
            // 2 triangles per sector
            // k1 => k1+1 => k2
            indices.push_back(k1);
            indices.push_back(k1 + 1);
            indices.push_back(k2);

            // k2 => k1+1 => k2+1
            indices.push_back(k2);
            indices.push_back(k1 + 1);
            indices.push_back(k2 + 1);
        }

        for (int i = 0, k = baseCenterIndex + 1; i < sectors; i++, k++)
        {
            if (i < sectors - 1)
            {
                indices.push_back((unsigned short)baseCenterIndex);
                indices.push_back((unsigned short)(k + 1));
                indices.push_back((unsigned short)k);
            }
            else // last triangle
            {
                indices.push_back((unsigned short)baseCenterIndex);
                indices.push_back((unsigned short)(baseCenterIndex + 1));
                indices.push_back((unsigned short)k);
            }
        }

        for (int i = 0, k = topCenterIndex + 1; i < sectors; i++, k++)
        {

            if (i < sectors - 1)
            {
                indices.push_back((unsigned short)topCenterIndex);
                indices.push_back((unsigned short)k);
                indices.push_back((unsigned short)(k + 1));
            }
            else // last triangle
            {
                indices.push_back(topCenterIndex);
                indices.push_back(k);
                indices.push_back(topCenterIndex + 1);
                
            }
        }

        return { std::move(vertices), std::move(indices) };
    }

    template <typename V>
    static IndexedTrigList <V> Make()
    {
        return MakeTesselated<V>(3);
    }
    
};