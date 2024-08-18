#pragma once

#include <DirectXMath.h>

#include <vector>

template <class T>
class IndexedTrigList
{
public:
    IndexedTrigList() = default;
    IndexedTrigLsit(std::vector<T> vertices, std::vector<unsigned short> indices)
        :
        vertices(vertices),
        indices(indices)
    {
        assert(vertices.size() > 2);
        assert(indices.size() % 3 == 0);
    }

    void Transform(DirectX::FXMATRIX mat)
    {
        for (auto& vert : vertices)
        {
            const DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&vert.pos);
            DirectX::XMStoreFloat3(&vert.pos, DirectX::XMVector3Transform(pos, mat);
        }
    }

public:
    std::vector<T> vertices;
    std::vector<unsigned short> indices
};