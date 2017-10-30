#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>


template <typename T>
struct Bin
{
    typedef T type;

    Bin(const T& _value, const float& _counter = 0);

    T value;
    float counter;

    // check if _val fits and if it does, increment counter
    constexpr inline bool operator()(const T& _val, const float& _increment = 1)
    {
        const bool increase = (_val == value);
        if( increase ) counter += _increment;
        return increase;
    }

    // check if _val fits and if it does, return true (else: false);
    constexpr inline bool operator[](const T& _val)
    {
        return _val == value;
    }

};

template <typename T>
inline Bin<T>::Bin(const T& _value, const float& _counter)
  : value(_value)
  , counter(_counter)
{}


/***************************************************************************/

template <typename T>
struct histogram
{
    typedef T type;

    void add_bin(const T&, const float& = 0);
    void add_data(const T&);
    void add_data(const T&, const float&);
    auto get_data(const T&);

    std::string formatted_string() const;
    
    void        print_to_file (const std::string&) const;

    float meanHeight() const;
    inline auto populated_bins() const { return std::count_if(bins.cbegin(), bins.cend(), [](auto B) { return B.counter > 0;} ); }
    inline auto num_bins()       const { return bins.size(); }
    inline auto minimum()        const { return min; }
    inline auto maximum()        const { return max; }
    inline auto reset(const float& i = 0) { std::for_each(bins.begin(), bins.end(), [&i](auto& B) { B.counter = i;} ); }
    inline auto clear()           { bins.clear(); min = 0; max = 0; }
    inline auto normalise(const float& v) { std::for_each(bins.begin(), bins.end(), [&v](auto& B) { B.counter /= v; }); }

    inline auto begin()          const   { return bins.begin(); }
    inline auto begin()                  { return bins.begin(); }
    inline auto cbegin()         const   { return bins.cbegin(); }
    inline auto end()            const   { return bins.end(); }
    inline auto end()                    { return bins.end(); }
    inline auto cend()           const   { return bins.cend(); }

protected:
    T min = 0;
    T max = 0;
    std::vector<Bin<T>> bins;

};



template<typename T>
void histogram<T>::add_bin(const T& _value, const float& _counter)
{
    if( bins.size() == 0 )
    {
        min = _value;
        max = _value;
    }
    if( std::all_of(bins.cbegin(), bins.cend(), [&](auto B){ return _value < B.value; }) )
    {
        min = _value;
    }
    if( std::all_of(bins.cbegin(), bins.cend(), [&](auto B){ return _value > B.value; }) )
    {
        max = _value;
    }
    bins.emplace_back(_value, _counter);
}



template<typename T>
inline void histogram<T>::add_data(const T& _data)
{
    for( auto& B : bins )
        if (B(_data)) break;
}



template<typename T>
inline void histogram<T>::add_data(const T& _data,  const float& _increment)
{
    for( auto& B : bins )
        if (B(_data, _increment)) break;
}



template<typename T>
inline auto histogram<T>::get_data(const T& _data)
{
    bool found = false;
    for( auto& B: bins )
    {
        if( B[_data] )
        {
            return B.counter;
            found = true;
        }
    }
    if( found == false ) throw std::range_error("out of range in histogram<T>::get_data() ! ");
    return static_cast<int>(0);
}



template<typename T>
inline std::string histogram<T>::formatted_string() const
{
    // first sort the bins:
    //std::sort(std::begin(bins), std::end(bins), [](auto& B1, auto& B2){ return B1.value < B2.value; });
    std::ostringstream STREAM;
    for( auto& B : bins )
        STREAM << std::setw(10) << std::setprecision(3) << B.value
               << std::setw(20) << std::setprecision(4) << B.counter << '\n';
    return STREAM.str();
}



template<typename T>
inline void histogram<T>::print_to_file (const std::string& _filename) const
{
    std::ofstream FILE( _filename );
    FILE << formatted_string();
    FILE.close();
}



template<typename T>
inline float histogram<T>::meanHeight() const
{
    return std::accumulate(bins.cbegin(), bins.cend(), 0.0, [](float i, auto B) { return i + B.counter; } ) / populated_bins();
}

