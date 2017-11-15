#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>


template <typename T>
struct Bin
{
    typedef T type;

    T min = 0;
    T max = 0;
    double counter;

    // check if _val fits and if it does, increment counter
    constexpr inline bool operator()(const T& _val, const double& _increment = 1)
    {
        const bool increase = _val > min && _val <= max;
        if( increase ) counter += _increment;
        return increase;
    }

    // check if _val fits and if it does, return true (else: false);
    constexpr inline bool operator[](const T& _val)
    {
        return _val > min && _val <= max;
    }

    // integral bins return maximum
    // bin from 0 to 1 will return 1 as position
    template<typename Enabler = T>
    constexpr inline typename std::enable_if<std::is_integral<Enabler>::value, T>::type position() const
    {
        return max;
    }

    // floating point bins return the mean of min and max
    template<typename Enabler = T>
    constexpr inline typename std::enable_if<std::is_floating_point<Enabler>::value, T>::type position() const
    {
        return (max-min)/2.f + min;
    }
};

/***************************************************************************/

template <typename T>
struct Histogram
{
    typedef T type;

    Histogram(const T&, const T&, const unsigned int&);

    void add_data(const T&);
    void add_data(const T&, const double&);
    auto get_data(const T&);

    inline std::string formatted_string();
    inline void        print_to_file (const std::string&) const;

    inline double meanHeight() const;
    inline auto populated_bins() const { return std::count_if(bins.cbegin(), bins.cend(), [](auto B) { return B.counter > 0;} ); }
    inline auto num_bins()       const { return bins.size(); }
    inline auto minimum()        const { return min; }
    inline auto maximum()        const { return max; }
    inline auto reset(const double& i = 0) { std::for_each(bins.begin(), bins.end(), [&i](auto& B) { B.counter = i;} ); }
    inline auto normalise(const double& v) { std::for_each(bins.begin(), bins.end(), [&v](auto& B) { B.counter /= v; }); }
    
    inline auto begin()          const   { return bins.begin(); }
    inline auto begin()                  { return bins.begin(); }
    inline auto cbegin()         const   { return bins.cbegin(); }
    inline auto end()            const   { return bins.end(); }
    inline auto end()                    { return bins.end(); }
    inline auto cend()           const   { return bins.cend(); }
    
protected:
    const T min;
    const T max;
    const T bin_width;
    std::vector<Bin<T>> bins {};
    
};


template<typename T>
inline Histogram<T>::Histogram(const T& _min, const T& _max, const unsigned int& _nrbins)
 : min(_min)
 , max(_max)
 , bin_width((_max-_min)/_nrbins)
 , bins( _nrbins, {0,0,0} )
{
    for( unsigned int i=0; i<bins.size(); ++i)
    {
         bins[i].min = _min + bin_width*i;
         bins[i].max = _min + bin_width*i + bin_width;
    }
}



template<typename T>
inline void Histogram<T>::add_data(const T& _data)
{
    for( auto& B : bins )
        if (B(_data)) break;
}



template<typename T>
inline void Histogram<T>::add_data(const T& _data,  const double& _increment)
{
    for( auto& B : bins )
        if (B(_data, _increment)) break;
}



template<typename T>
inline auto Histogram<T>::get_data(const T& _data)
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
    return static_cast<T>(0);
}



template<typename T>
inline std::string Histogram<T>::formatted_string()
{
    std::ostringstream STREAM;
    for( auto& B : bins )
    {
        if( B.counter != 0 )
            STREAM << std::setw(10) << std::setprecision(4) << B.position()
                   << std::setw(20) << std::setprecision(4) << B.counter << '\n';
    }
    return STREAM.str();
}



template<typename T>
inline void Histogram<T>::print_to_file (const std::string& _filename) const
{
    std::ofstream FILE( _filename );
    FILE << formatted_string();
    FILE.close();
}



template<typename T>
inline double Histogram<T>::meanHeight() const
{
    return std::accumulate(bins.cbegin(), bins.cend(), 0.0, [](float i, auto B) { return i + B.counter; } ) / populated_bins();
}
