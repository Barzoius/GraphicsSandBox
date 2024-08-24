#pragma once
#include "IndexedTrigList.h"
#include "Math.h"

#include <DirectXMath.h>


class Cone
{
public:
    template <typename V>
    static IndexedTrigList<V> MakeTesselated()
    {


        

        
    }


    template<class V>
    static IndexedTrigList<V> Make()
    {
        return MakeTesselated<V>(24);
    }
};