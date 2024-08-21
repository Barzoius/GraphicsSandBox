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

        const float lattitudeAngle = FPI / stacks;
        const float longitudeAngle = 2.0f * FPI / sectors;

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
        return MakeTesselated<V>(12, 24);
    }
};