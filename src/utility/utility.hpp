#pragma once

#include <ostream>
#include <random>
#include <cstdint>
#include "randomnumbers.hpp"


// to be able to pass my own class objects to a stream via << :
template<typename T>
auto operator<<(std::ostream& stream, const T& t) -> decltype(t.print(stream),stream)
{
	t.print(stream);
	return stream;
}


/*
inline auto random_iterator() -> aligned_vector<std::shared_ptr<T>>::iterator
{
    std::uniform_int_distribution<LONGSIZE> dist(0,cells.size());
    return cells.begin() + dist(enhance::engine.pseudo_ENGINE);
}

inline auto random_cell_iterator() const -> aligned_vector<std::shared_ptr<T>>::const_iterator
{
    std::uniform_int_distribution<LONGSIZE> dist(0,cells.size());
    return cells.cbegin() + dist(enhance::engine.pseudo_ENGINE);
}

inline auto random_cell_const_iterator() const -> aligned_vector<std::shared_ptr<T>>::const_iterator
{
    std::uniform_int_distribution<LONGSIZE> dist(0,cells.size());
    return cells.cbegin() + dist(enhance::engine.pseudo_ENGINE);
}*/
