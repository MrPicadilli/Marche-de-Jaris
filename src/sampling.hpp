#include "algebra.hpp"

#include <random>

//-- seed random numbers --

//Note that these functions use a common internal random engine and are
//therefore not thread safe.

namespace SamplingOptions {
  void use_random_device() ;
}

//-- 2D sampling --

//random point on the unit circle
vec2 rand_in_circle() ;

//random point in the unit disk
vec2 rand_in_disk() ;

//-- 3D sampling --

//random point on the unit sphere
vec3 rand_in_sphere() ;

//random point in the unit ball
vec3 rand_in_ball() ;
