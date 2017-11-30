#include "spin.hpp"



Spin::Spin(unsigned int _id, int _type)
{
    ID = _id;
    type = _type;
}



void Spin::setType(int _type)
{   
    // set type of this spin
    type = _type; 
}



void Spin::setNeighbours(std::vector<std::reference_wrapper<Spin>>& _neighbours) 
{
    // set neighbours for this spin
    neighbours = _neighbours; 
}



void Spin::flip()
{ 
    // flip this spin
    type = -1 * type;
}



int Spin::sumOppositeNeighbours() const
{
    // return number of neighbours of opposite type
    
    int sum = 0;
    for(auto& N: neighbours)
    {
        if( N.get().getType() != type )
        {
            sum ++;
        }
    }
    return sum;
}



int Spin::sumNeighbours() const
{
    // return sum s_i*s_j, where s_i is this spin and s_j are all neighbours of this spin

    int sum = 0;
    for(auto& N: neighbours)
    {
        sum += type * N.get().getType();
    }
    return sum;
}