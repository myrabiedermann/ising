#pragma once

#include "lib/enhance.hpp"
#include <vector>
#include <functional>


class Spin
{

    unsigned int  ID {0};
    int           type {1};
    std::vector<std::reference_wrapper<Spin>> neighbours {};

public:
    Spin(unsigned int, int);

    void setType(int);
    void setNeighbours(std::vector<std::reference_wrapper<Spin>>&);

    auto  getID()   const { return ID; }
    auto  getType() const { return type; }
    auto& getNeighbours() { return neighbours; }

    Spin& getRandomNeighbour();

    void flip();

    int sumNeighbours() const;
    int sumOppositeNeighbours() const;

};



