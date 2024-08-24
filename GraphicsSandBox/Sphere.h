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
        assert(stacks >= 2);
        assert(sectors >= 2);

        constexpr float radius = 1.0f;

        const float lattitudeAngle = FPI / stacks; 
        const float longitudeAngle = 2.0f * FPI / sectors; 

        float stackAngle, sectorAngle;

        std::vector<V> vertices;

        for (int i = 0; i <= stacks; i++)
        {
///          [VERTICAL SECTION IN THE SPHERE]
///
///                         z
///                         |      xy
///                     ****|**** /
///                 ****    |    /***
///               **        |   /    ** 
///              *          |  /       *
///             **          | /        **
///             *           |/          *
///           -----------------------------
///             *           |           *
///             **          |          **
///              *          |          *
///               **        |        **
///                 ****    |    ****
///                     ****|****
///                         |

            stackAngle = FPI / 2 - i * lattitudeAngle;

            float z = radius * sinf(stackAngle);

            float xy = radius * cosf(stackAngle);

            
            for (int j = 0; j <= sectors; j++)
            {
                vertices.emplace_back();

///         [HORIZONTAL SECTION IN THE SPHERE]
///
///                         y
///                         |      x
///                     ****|**** /
///                 ****    |    /***
///               **        |   /    ** 
///              *          |  /       *
///             **          | /        **
///             *           |/          *
///           -----------------------------
///             *           |           *
///             **          |          **
///              *          |          *
///               **        |        **
///                 ****    |    ****
///                     ****|****
///      

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
                /// k1--k1+1
                /// |  / |
                /// | /  |
                /// k2--k2+1

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

    ///complet gresit sterge
    template<typename V>
    static IndexedTrigList<V> MakeIcoSphere()
    {
        ///-----icosahedron-----///
        constexpr float radius = 1.0f;
        const float H_ANGLE = FPI / 180 * 72;
        const float V_ANGLE = atanf(1.0f / 2);

        float hAngle1 = -FPI / 2 - H_ANGLE / 2;
        float hAngle2 = -FPI / 2;


        std::vector<float> fvertices(12* 3);

        fvertices[0] = 0;
        fvertices[1] = 0;
        fvertices[2] = radius;

        int i1, i2;
        for (int i = 1; i <= 5; ++i)
        {
            i1 = i * 3;         // index for 1st row
            i2 = (i + 5) * 3;   // index for 2nd row

            float z = radius * sinf(V_ANGLE);            // elevaton
            float xy = radius * cosf(V_ANGLE);            // length on XY plane

            fvertices[i1] = xy * cosf(hAngle1);      // x
            fvertices[i2] = xy * cosf(hAngle2);
            fvertices[i1 + 1] = xy * sinf(hAngle1);  // y
            fvertices[i2 + 1] = xy * sinf(hAngle2);
            fvertices[i1 + 2] = z;                   // z
            fvertices[i2 + 2] = -z;

            // next horizontal angles
            hAngle1 += H_ANGLE;
            hAngle2 += H_ANGLE;
        }

        i1 = 11 * 3;
        fvertices[i1] = 0;
        fvertices[i1 + 1] = 0;
        fvertices[i1 + 2] = -radius;

        ///---------------------///

        auto computeHalfVertex = [radius](const float v1[3], const float v2[3], float newV[3]) {
            newV[0] = v1[0] + v2[0];    // x
            newV[1] = v1[1] + v2[1];    // y
            newV[2] = v1[2] + v2[2];    // z
            float scale = radius / sqrtf(newV[0] * newV[0] + newV[1] * newV[1] + newV[2] * newV[2]);
            newV[0] *= scale;
            newV[1] *= scale;
            newV[2] *= scale;
            };

        std::vector<V> vertices;
        vertices.emplace_back();

        auto addVertices = [&vertices](float v1[3], float v2[3], float v3[3]) {

            const auto vertex1 = DirectX::XMVectorSet(v1[0], v1[1], v1[2], 0.0f);
            DirectX::XMStoreFloat3(&vertices.back().pos, vertex1);

          
            const auto vertex2 = DirectX::XMVectorSet(v2[0], v2[1], v2[2], 0.0f);
            DirectX::XMStoreFloat3(&vertices.back().pos, vertex2);

            const auto vertex3 = DirectX::XMVectorSet(v3[0], v3[1], v3[2], 0.0f);
            DirectX::XMStoreFloat3(&vertices.back().pos, vertex3);

            };

        ///



        std::vector<float> tmpVertices;
        std::vector<float> tmpIndices;
        float* v1, * v2, * v3;          // ptr to original vertices of a triangle
        float newV1[3], newV2[3], newV3[3]; // new vertex positions
        unsigned int index;

        std::vector<unsigned short> indices;

        int subdivision = 6;
        // iterate all subdivision levels
        for (int i = 1; i <= subdivision; ++i)
        {
            // copy prev vertex arrays and clear
            tmpVertices = fvertices;
            fvertices.clear();
            index = 0;

            // perform subdivision for each triangle
            for (int j = 0; j < tmpIndices.size(); j += 3)
            {
                // get 3 vertices of a triangle
                v1 = &tmpVertices[tmpIndices[j] * 3];
                v2 = &tmpVertices[tmpIndices[j + 1] * 3];
                v3 = &tmpVertices[tmpIndices[j + 2] * 3];

                // compute 3 new vertices by spliting half on each edge
                //         v1       
                //        / \       
                // newV1 *---* newV3
                //      / \ / \     
                //    v2---*---v3   
                //       newV2      
                computeHalfVertex(v1, v2, newV1);
                computeHalfVertex(v2, v3, newV2);
                computeHalfVertex(v1, v3, newV3);

                // add 4 new triangles to vertex array
                addVertices(v1, newV1, newV3);
                addVertices(newV1, v2, newV2);
                addVertices(newV1, newV2, newV3);
                addVertices(newV3, newV2, v3);

                // add indices of 4 new triangles
                indices.push_back(index);
                indices.push_back(index + 1);
                indices.push_back(index + 2);
                indices.push_back(index + 3);
                indices.push_back(index + 4);
                indices.push_back(index + 5);
                indices.push_back(index + 6);
                indices.push_back(index + 7);
                indices.push_back(index + 8);
                indices.push_back(index + 9);
                indices.push_back(index + 10);
                indices.push_back(index + 11);

                index += 12;    // next index
            }
        }
        
        return { std::move(vertices),std::move(indices) };

    }


    ///CUBESPHERE
    template<typename V>
    static IndexedTrigList<V> MakeCubeSphere()
    {

    }


    template<typename V>
    static IndexedTrigList<V> Make()
    {
        return MakeUVSphere<V>(32, 32);
       // return MakeIcoSphere<V>();
    }
};