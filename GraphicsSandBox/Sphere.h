#pragma once
#include "IndexedTrigList.h"
#include "Math.h"

#include <DirectXMath.h>

class Sphere
{
public:

    ///UV SPHERE
    template<typename V>
    static IndexedTrigList<V> MakeUVSphere(int stacks, int sectors)
    {
        assert(stacks >= 3);
        assert(sectors >= 3);

        constexpr float radius = 1.0f;

        const float lattitudeAngle = FPI / stacks; // stackstep
        const float longitudeAngle = 2.0f * FPI / sectors; //sectorStep

        float stackAngle, sectorAngle;

        std::vector<V> vertices;

        for (int i = 0; i <= stacks; i++)
        {
            stackAngle = FPI / 2 - i * lattitudeAngle;

            float xy = radius * cosf(stackAngle);
            float z = radius * sinf(stackAngle);

            for (int j = 0; j <= sectors; j++)
            {
                vertices.emplace_back();

                sectorAngle = j * longitudeAngle;

                float x = xy * cosf(sectorAngle);
                float y = xy * sinf(sectorAngle);

                const auto vertex = DirectX::XMVectorSet(x, y, z, 0.0f);

                DirectX::XMStoreFloat3(&vertices.back().pos, vertex);
            }

        }

        std::vector<unsigned short> indices;
        int k1, k2;
        for (int i = 0; i < stacks; i++)
        {
            k1 = i * (sectors + 1);     // beginning of current stack
            k2 = k1 + sectors + 1;      // beginning of next stack

            for (int j = 0; j < sectors; j++, k1++, k2++)
            {

                if (i != 0)
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                if (i != (stacks - 1))
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }

            }
        }

        return { std::move(vertices),std::move(indices) };

    }


    ///ICOSPHERE
    template<typename V>
    static IndexedTrigList<V> MakeIcoSphere()
    {

    }


    ///CUBESPHERE
    template<typename V>
    static IndexedTrigList<V> MakeCubeSphere()
    {

    }


    template<typename V>
    static IndexedTrigList<V> Make()
    {
        return MakeUVSphere<V>(12, 24);
    }
};