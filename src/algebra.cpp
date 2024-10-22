#include "algebra.hpp"

#include <cmath>

vec3 cross(
    const vec3& v0, 
    const vec3& v1
    ) 
{
  return {
    v0[1]*v1[2] - v1[1]*v0[2],
    v0[2]*v1[0] - v1[2]*v0[0],
    v0[0]*v1[1] - v1[0]*v0[1]
  } ;
}

double det(
    const mat2& m
    )
{
  return m[0]*m[3] - m[1]*m[2] ;
}

double det(
    const mat3& m
    )
{
  return dot(
      cross({m[0], m[1], m[2]}, {m[3], m[4], m[5]}),
      {m[6], m[7], m[8]}
      ) ;
}

double det(
    const mat4& m
    ) 
{
  return
      m[ 3] * dot(
        cross({m[ 4],m[ 5],m[ 6]}, {m[ 8],m[ 9],m[10]}), 
        {m[12],m[13],m[14]}
        )
    - m[ 7] * dot(
        cross({m[ 0],m[ 1],m[2]}, {m[ 8],m[ 9],m[10]}), 
        {m[12],m[13],m[14]}
        )
    + m[11] * dot(
        cross({m[ 0],m[ 1],m[ 2]}, {m[ 4],m[ 5],m[ 6]}), 
        {m[12],m[13],m[14]}
        )
    - m[15] * dot(
        cross({m[ 0],m[ 1],m[ 2]}, {m[ 4],m[ 5],m[ 6]}), 
        {m[ 8],m[ 9],m[10]}
        )
  ;
}
