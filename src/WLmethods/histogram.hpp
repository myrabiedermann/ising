#pragma once

#include <string>
#include <algorithm>
#include <vector>
// #include <boost/filesystem.hpp>

/*
 * struct bin
 */

template <typename T>
struct bin
{
  typedef T type;

  bin(const int& _value, const T& _counter = 0);

  const int value;
  T counter;

  // check if _val fits and if it does, increment counter
  constexpr inline bool operator()(const int& _val, const T& _increment = 1)
  {
      const bool increase = (_val == value);
      if( increase ) counter += _increment;
      return increase;
  }

  // check if _val fits and if it does, return true (else: false);
  constexpr inline bool operator[](const int& _val)
  {
    return _val == value;

  }

};

template <typename T>
inline bin<T>::bin(const int& _value, const T& _counter)
  : value(_value)
  , counter(_counter)
{}


/***************************************************************************/

/*
 * struct histogram
 */

template <typename T>
struct histogram
{
  typedef T type;

  void add_bin(const int&, const T& = 0);
  void add_data(const int&);
  void add_data(const int&, const T&);
  auto get_data(const int&);

  std::string formatted_string() const;
  void        print_to_file (const std::string&) const;

  double meanHeight() const;
  inline auto populated_bins() const { return std::count_if(bins.cbegin(), bins.cend(), [](auto B) { return B.counter > 0;} ); }
  inline auto num_bins()       const { return bins.size(); }
  inline auto minimum()        const { return min; }
  inline auto maximum()        const { return max; }
  inline auto reset(const type& i = 0) { std::for_each(bins.begin(), bins.end(), [&i](auto& B) { B.counter = i;} ); }

  inline auto begin()          const   { return bins.begin(); }
  inline auto end()            const   { return bins.end(); }

protected:
    
  int min = 0;
  int max = 0;
  std::vector<bin<T>> bins;

};



template<typename T>
void histogram<T>::add_bin(const int& _value, const T& _counter)
{
  if( bins.size() == 0 ){
    min = _value;
    max = _value;
  }
  if( std::all_of(bins.cbegin(), bins.cend(), [&](auto B){ return _value < B.value; }) ){
        min = _value;
  }
  if( std::all_of(bins.cbegin(), bins.cend(), [&](auto B){ return _value > B.value; }) ){
        max = _value;
  }
  bins.emplace_back(_value, _counter);
}



template<typename T>
inline void histogram<T>::add_data(const int& _data)
{
    for( auto& B : bins )
        if (B(_data)) break;
}



template<typename T>
inline void histogram<T>::add_data(const int& _data,  const T& _increment)
{
    for( auto& B : bins )
        if (B(_data, _increment)) break;
}



template<typename T>
inline auto histogram<T>::get_data(const int& _data)
{
    T r = -1;
    for( auto& B: bins ){
      if( B[_data] ){
        r = B.counter;
        break;
      }
    }
    if( r == -1) std::cout << "WARNING: out of range in histogram<T>::get_data()  ! " << _data << "\n";
    return r;
}



template<typename T>
inline std::string histogram<T>::formatted_string() const
{
    std::ostringstream STREAM;
    for( auto& B : bins )
        STREAM << std::setw(10) << B.value << std::setw(12) << std::setprecision(6) << B.counter << '\n';
    return STREAM.str();
}



template<typename T>
inline void histogram<T>::print_to_file (const std::string& _filename) const
{
    boost::filesystem::ofstream FILE( _filename );
    FILE << formatted_string();
    FILE.close();
}



template<typename T>
inline double histogram<T>::meanHeight() const
{
  return std::accumulate(bins.cbegin(), bins.cend(), 0, [](auto i, auto B) { return i + B.counter; } ) / populated_bins();
}
