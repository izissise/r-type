#ifndef ENUMCHECKER_HPP_INCLUDED
# define ENUMCHECKER_HPP_INCLUDED

# include <algorithm>

template <typename T>
struct enum_traits {};

template<typename T, size_t N>
T *endof(T (&ra)[N]) {
  return ra + N;
}

template<typename T, typename ValType>
T checkEnum(ValType v) {
  typedef enum_traits<T> traits;
  const T *first = traits::enumerators;
  const T *last = endof(traits::enumerators);
  if (std::find(first, last, static_cast<T>(v)) != last)
      return static_cast<T>(v);
  throw std::runtime_error("No such enum value.");
}

///Enum must be defined this way to be able to be check:
/*

// "enhanced" definition of enum
enum e {
  x = 1,
  y = 4,
  z = 10,
};

template<>
struct enum_traits<e> {
  static const e enumerators[];
};
// must appear in only one TU,
// so if the above is in a header then it will need the array size
const e enum_traits<e>::enumerators[] = {x, y, z};
*/

#endif // ENUMCHECKER_HPP_INCLUDED
