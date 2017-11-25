#pragma once

#include <vector>
#include <functional>


class Spin
{

    const unsigned int  ID;
    int                 type;
    std::vector<std::reference_wrapper<Spin>> neighbours {};

public:
    Spin(const unsigned int, const int);

    void setType(int _t);
    void setNeighbours(const std::vector<std::reference_wrapper<Spin>>&);

    auto  getID()   const { return ID; }
    auto  getType() const { return type; }
    auto& getNeighbours() { return neighbours; }

    void flip();

    int sumNeighbours() const;
    int sumOppositeNeighbours() const;

};



