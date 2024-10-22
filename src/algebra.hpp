#ifndef MIF27_ALGEBRA_HPP
#define MIF27_ALGEBRA_HPP

#include <array>
#include <iostream>
#include <algorithm>
#include <functional>
#include <numeric>
#include <cmath>

template<std::size_t n>
using vec = std::array<double, n> ;

using vec3 = vec<3> ;
using vec2 = vec<2> ;

template<std::size_t n>
using mat = std::array<double, n*n> ;

using mat2 = mat<2> ;
using mat3 = mat<3> ;
using mat4 = mat<4> ;

//adding vec<n> : v0 + v1 -> vec<n>
template<std::size_t n>
vec<n> operator+(
    const vec<n>& v0, 
    const vec<n>& v1
    ) {
  vec<n> res ;
  std::transform(v0.begin(), v0.end(), v1.begin(), res.begin(), std::plus<double>()) ;
  return res ;
}

//subtracting vec<n> : v0 - v1 -> vec<n>
template<std::size_t n>
vec<n> operator-(
    const vec<n>& v0, 
    const vec<n>& v1
    ) {
  vec<n> res ;
  std::transform(v0.begin(), v0.end(), v1.begin(), res.begin(), std::minus<double>()) ;
  return res ;
}

//multiplying a vec<n> by a number : d * v -> vec<n>
template<std::size_t n>
vec<n> operator*(
    double d, 
    const vec<n>& v
    ) {
  vec<n> res ;
  std::transform(v.begin(), v.end(), res.begin(), [d](double v) {return d*v ;}) ;
  return res ;
}

template<std::size_t n>
vec<n> operator*(
    const vec<n>& v,
    double d
    ) {
  return d * v ;
}

//dividing a vec<n> by a number : v / d -> vec<n>
template<std::size_t n>
vec<n> operator/(
    const vec<n>& v,
    double d 
    ) {
  vec<n> res ;
  std::transform(v.begin(), v.end(), res.begin(), [d](double v) {return v/d ;}) ;
  return res ;
}

//dot product : dot(v0, v1) -> real
template<std::size_t n>
double dot(
    const vec<n>& v0, 
    const vec<n>& v1
    ) {
  return std::inner_product(v0.begin(), v0.end(), v1.begin(), 0.0d) ;
}

//cross product : cross(v0, v1) -> vec<n>
template<std::size_t n>
vec3 cross(
    const vec<n>& v0, 
    const vec<n>& v1
    ) ; 

//squated norm : norm2(v) -> real
template<std::size_t n>
double norm2(
    const vec<n>& v
    ) {
  return dot(v,v) ;
}

//norm : norm(v) -> real
template<std::size_t n>
double norm(
    const vec<n>& v
    ) {
  return sqrt(norm2(v)) ;
}

//normalization : normalize(v) -> vec<n>
template<std::size_t n>
vec<n> normalized(
    const vec<n>& v
    ) {
  return v / norm(v) ;
}

template<std::size_t n>
void normalize(
    vec<n>& v
    ) {
  v = v / norm(v) ;
}

//display
template<std::size_t n>
std::ostream& operator<<(
    std::ostream& out, 
    const vec<n>& v
    ) {
  out << "[ " ;
  for(double d : v) {
    out << d << " " ;
  }
  out << "]" ;
  return out ;
}

//determinant of matrix det(m) -> real
double det(
    const mat2& m
    ) ;

double det(
    const mat3& m
    ) ;

double det(
    const mat4& m
    ) ;

#endif
