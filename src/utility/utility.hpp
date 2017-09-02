#pragma once

#include <ostream>
#include <cstdint>


// to be able to pass my own class objects to a stream via << :
template<typename T>
auto operator<<(std::ostream& stream, const T& t) -> decltype(t.print(stream),stream)
{
	t.print(stream);
	return stream;
}


