#include "sampling.hpp"

#include<cmath>

//===== sampling =====

static std::default_random_engine alea ;

namespace SamplingOptions {

  //seed the default engine with a random device seed
  void use_random_device() {
    std::random_device dev ;
    alea.seed(dev()) ;
  }
}

//-- 2D sampling --

//random point on the unit circle
vec2 rand_in_circle() 
{
  std::normal_distribution<> rand_coord ;
  return normalized(vec2({rand_coord(alea), rand_coord(alea)})) ;
}

//random point in the unit disk
vec2 rand_in_disk() 
{
  std::uniform_real_distribution<double> rand_radius(0,1) ;
  return sqrt(rand_radius(alea)) * rand_in_circle() ;
}

//-- 3D sampling --

//random point on the unit sphere
vec3 rand_in_sphere() 
{
  std::normal_distribution<> rand_coord ;
  return normalized(vec3({rand_coord(alea), rand_coord(alea), rand_coord(alea)})) ;
}

//random point in the unit ball
vec3 rand_in_ball() 
{
  std::uniform_real_distribution<double> rand_radius(0,1) ;
  return pow(rand_radius(alea), 1.d/3.d) * rand_in_sphere() ;
}
