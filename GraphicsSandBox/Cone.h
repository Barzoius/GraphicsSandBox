#pragma once
#include "IndexedTrigList.h"
#include "Math.h"

#include <DirectXMath.h>


class Cone
{
public:
    template <typename V>
    static IndexedTrigList<V> MakeTesselated(int longDiv)
    {
        assert(longDiv >= 3)

        const auto base = DirectX::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
        const float longitudeAngle = 2.0f * FPI / longDiv;

        std::vector<V> vertices;
        for (int iLong = 0; iLong < longDiv; iLong++)
        {
            vertices.emplace_back();
            auto vert = DirectX::XMVector3Transform(base, 
                DirecrX::XMMatrixRotationZ(longitudeAngle * iLong));

            DirectX::XMStoreFloat3(&vertices.back().pos, vert);
        }
    }


    template<class V>
    static IndexedTrigList<V> Make()
    {
        return MakeTesselated<V>(24);
    }
};