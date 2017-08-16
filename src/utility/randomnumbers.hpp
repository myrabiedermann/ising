#pragma once

#include <algorithm>
#include <random>
#include <iterator>
#include <type_traits>
#include <string>


extern unsigned int seed;
extern std::mt19937_64 rand_engine;

double randomDouble(double, double);
int randomInt(int, int);


// Type aliasing
template<typename Container>
using IteratorCategoryOf = typename std::iterator_traits<typename Container::iterator>::iterator_category;


// Compile time iterator check
template<typename Container>
using HaveRandomAccessIterator = std::is_base_of< std::random_access_iterator_tag, IteratorCategoryOf<Container>>;


static struct __random_iterator
{
    template<typename T>
    constexpr inline decltype(auto) operator() ( const T& _container ) const -> typename T::const_iterator
    {
        static_assert( HaveRandomAccessIterator<T>::value, "T has no std::random_access_iterator_tag in __random_iterator::operator()" );
        std::uniform_int_distribution<std::size_t> dist(0,_container.size()-1);
        return std::cbegin(_container) + dist(rand_engine);
    }

    template<typename T>
    constexpr inline decltype(auto) operator() ( T& _container ) -> typename T::iterator
    {
        static_assert( HaveRandomAccessIterator<T>::value, "T has no std::random_access_iterator_tag in __random_iterator::operator()" );
        std::uniform_int_distribution<std::size_t> dist(0,_container.size()-1);
        return std::begin(_container) + dist(rand_engine);
    }
} random_iterator __attribute__((unused));
