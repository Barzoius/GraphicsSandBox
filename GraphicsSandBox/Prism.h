#pragma once

#include "IndexedTrigList.h"
#include "Math.h"

#include <vector>
#include <array>

class Prism
{
public:

    static IndexedTrigList MakeTesselated(DVS::VertexLayout layout, int sectors)
    {
        const float radius = 1.0f;
        const float height = 3.0f;

        const float longitudeAngle = 2.0f * FPI / sectors;

        float sectorAngle;

        DVS::VertexBuffer vb{ std::move(layout) };

        std::vector<float> circleVertices;

        for (int i = 0; i <= sectors; i++)
        {
            sectorAngle = longitudeAngle * i;
            circleVertices.push_back(cosf(sectorAngle));
            circleVertices.push_back(sinf(sectorAngle));
            circleVertices.push_back(0.0f); // z
        }

        int Vsize = 0;

    
        for (int i = 0; i < 2; i++)
        {
            float z = -height / 2.0f + i * height;

            for (int j = 0, k = 0; j <= sectors; j++, k += 3)
            {


                float ux = circleVertices[k];
                float uy = circleVertices[k + 1];
                float uz = circleVertices[k + 2];

                Vsize++;

                vb.EmplaceBack(
                    DirectX::XMFLOAT3{ ux * radius ,uy * radius, z }
                );

            }
        }

        unsigned short baseCenterIndex = (unsigned short)Vsize;
        unsigned short topCenterIndex = (unsigned short)(baseCenterIndex + sectors + 1);

        for (int i = 0; i < 2; i++)
        {
            float z = -height / 2.0f + i * height;
            float nz = -1 + i * 2;

            const auto circleVertex = DirectX::XMVectorSet(0.0f, 0.0f, z, 0.0f);

            vb.EmplaceBack(
                DirectX::XMFLOAT3{ 0.0f, 0.0f, z }
            );


            for (int j = 0, k = 0; j < sectors; j++, k += 3)
            {

                float ux = circleVertices[k];
                float uy = circleVertices[k + 1];

          
                const auto vertex = DirectX::XMVectorSet(ux * radius, uy * radius, z, 0.0f);

                vb.EmplaceBack(
                    DirectX::XMFLOAT3{ ux * radius, uy * radius, z }
                );

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

        return { std::move(vb),std::move(indices) };
    }


    static IndexedTrigList Make()
    {

        using DVS::VertexLayout;
        VertexLayout vl;
        vl.Append(VertexLayout::Position3D);

        return MakeTesselated(std::move(vl), 3);
    }

};
