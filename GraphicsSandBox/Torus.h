//#pragma once
//
//#include "IndexedTrigList.h"
//#include "Math.h"
//
//#include <vector>
//#include <array>
//
//class Torus
//{
//public:
//    template<typename V>
//    static IndexedTrigList<V> MakeTesselated(int sides, int sectors)
//    {
//        const float PI = acos(-1.0f);
//        float x, y, z, xy;
//
//        const float majorRadius = 1.0f;
//        const float minorRadius = 0.5f;
//
//        const float sectorStep = 2.0f * PI / sectors;
//        const float sideStep = 2.0f * PI / sides;
//
//        float sectorAngle, sideAngle;
//
//        std::vector<V> vertices;
//
//        for (int i = 0; i <= sides; i++)
//        {
//            sideAngle = PI - i * sideStep;
//            xy = minorRadius * cosf(sideAngle);
//            z = minorRadius * sinf(sideAngle);
//
//            // add (sectorCount+1) vertices per side
//            // the first and last vertices have same position 
//            for (int j = 0; j <= sectors; ++j)
//            {
//                vertices.emplace_back();
//
//                sectorAngle = j * sectorStep;
//
//                x = xy * cosf(sectorAngle);
//                y = xy * sinf(sectorAngle);
//
//                // shift x & y, and vertex position
//                x += majorRadius * cosf(sectorAngle);   // (R + r * cos(u)) * cos(v)
//                y += majorRadius * sinf(sectorAngle);   // (R + r * cos(u)) * sin(v)
//
//                const auto vertex = DirectX::XMVectorSet(x, y, z, 0.0f);
//
//                DirectX::XMStoreFloat3(&vertices.back().pos, vertex);
//                
//            }
//        }
//
//        // indices
//        //  k1--k1+1
//        //  |  / |
//        //  | /  |
//        //  k2--k2+1
//
//        std::vector<unsigned short> indices;
//        unsigned short k1, k2;
//        for (int i = 0; i < sides; ++i)
//        {
//            k1 = i * (sectors + 1);     // beginning of current side
//            k2 = k1 + sectors + 1;      // beginning of next side
//
//            for (int j = 0; j < sectors; ++j, ++k1, ++k2)
//            {
//                // 2 triangles per sector
//                indices.push_back(k1);
//                indices.push_back(k2);
//                indices.push_back(k1+1);
//
//                indices.push_back(k1+1);
//                indices.push_back(k2);
//                indices.push_back(k2+1);
//            }
//        }
//
//        return { std::move(vertices), std::move(indices) };
//    }
//
//
//    template<typename V>
//    static IndexedTrigList<V> Make()
//    {
//        return MakeTesselated<V>(40, 40);
//
//    }
//};