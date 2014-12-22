#ifndef _VECTOR_HPP_
# define _VECTOR_HPP_

template <typename T>
struct Vector
{
  Vector(): x(), y() {};
  Vector(T x1, T y1): x(x1), y(y1) {};
  Vector(const Vector<T> &vec): x(vec.x), y(vec.y) {};
  
  Vector<T> operator*(const Vector<T> &vec) const { return Vector<T>(x * vec.x, y * vec.y); };
  Vector<T> operator*(T nb) const { return Vector<T>(x * nb, y * nb); };

  Vector<T> operator+(const Vector<T> &vec) const { return Vector<T>(x + vec.x, y + vec.y); };
  Vector<T> operator+(T nb) const { return Vector<T>(x + nb, y + nb); };

  Vector<T> operator-(const Vector<T> &vec) const { return Vector<T>(x - vec.x, y - vec.y); };
  Vector<T> operator-(T nb) const { return Vector<T>(x - nb, y - nb); };

  Vector<T> &operator=(const Vector<T> &vec) { x = vec.x; y = vec.y; return *this;  };

  T x;
  T y;
};

#endif
