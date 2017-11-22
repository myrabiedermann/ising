#pragma once

/*
 * class Spin
 */

#include <numeric>
#include <vector>
#include <iterator>


enum SPINTYPE{UP=+1,DOWN=-1};   // define possible Spin types


class Spin
{
    typedef Spin selftype;
    typedef std::vector<std::reference_wrapper<selftype> > neighbours_vec;

    const unsigned int  ID;
    SPINTYPE            type;
    neighbours_vec      neighbours {};

public:
    Spin(const unsigned int, SPINTYPE);
    inline auto getID()   const { return ID; }
    inline auto getType() const { return type; }

    inline void setType(SPINTYPE _t) { type = _t; }
    inline void setNeighbours(const neighbours_vec& _N) { neighbours = _N; }
    
    inline auto& getNeighbours() { return neighbours; }

    inline void flip() { type = ( type == UP ? DOWN : UP ); }

    template<SPINTYPE T> constexpr int num() const;
    template<SPINTYPE T> constexpr int num_signed() const;
    inline int num_opposite() const;

    inline auto begin()   { return std::begin(neighbours); }
    inline auto begin()   const { return std::cbegin(neighbours); }
    inline auto cbegin()  const { return std::cbegin(neighbours); }
    inline auto end()     { return std::end(neighbours); }
    inline auto end()     const { return std::cend(neighbours); }
    inline auto cend()    const { return std::cend(neighbours); }

};

inline Spin::Spin(const unsigned int _id, SPINTYPE _type)
 : ID (_id)
 , type (_type)
{}



template<SPINTYPE T>
constexpr inline int Spin::num() const
{
    // return sum over neighbours of type T
    return std::accumulate(std::begin(neighbours), std::end(neighbours), 0, [](int i, const selftype& N)
                {
                    return N.getType() == T ? i+1 : i;
                });
}



template<SPINTYPE T>
constexpr inline int Spin::num_signed() const
{
    // return signed sum over neighbours of type T
    // sign = + if T == type, sign = - if T != type
    const int num = this->num<T>();
    return T == type ? num : num*(-1);
}



inline int Spin::num_opposite() const
{
    // return number of neighbours of opposite type
    if( type == UP ) return num<DOWN>();
    else             return num<UP>();
}
