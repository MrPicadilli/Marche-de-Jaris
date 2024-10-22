#include "algebra.hpp"
#include "sampling.hpp"
#include "svg.hpp"

#include <map>
#include <stack>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

static constexpr double res = 100;
static constexpr double pt_rad = 2;

//===== Orientation predicate =====

int sign(double d)
{
  if (d > 0)
    return 1;
  if (d < 0)
    return -1;
  return 0;
}

int orient(
    const vec2 &p0,
    const vec2 &p1,
    const vec2 &p2)
{
  mat3 m = {
      p0[0], p0[1], 1,
      p1[0], p1[1], 1,
      p2[0], p2[1], 1};

  double d = det(m);
  return sign(d);
}

int orient(
    const vec3 &p0,
    const vec3 &p1,
    const vec3 &p2,
    const vec3 &p3)
{
  mat4 m = {
      p0[0], p0[1], p0[2], 1,
      p1[0], p1[1], p1[2], 1,
      p2[0], p2[1], p2[2], 1,
      p3[0], p3[1], p3[2], 1};

  double d = det(m);
  return sign(d);
}

//===== Jarvis walk =====

//-- getting a point on the convex hull --

std::size_t hull_point(
    const std::vector<vec2> &points)
{
  // std::cout << "hull_point " << std::endl;
  double xmin = points[0][0];
  size_t indice = 0;
  size_t x = 0;
  for (vec2 v : points)
  {

    if (v[0] < xmin)
    {
      xmin = v[0];
      indice = x;
    }
    x++;
    // std::cout << "vx " << v[0] << "vy " << v[1] << std::endl;
  }
  // std::cout << "indice " << indice << std::endl;

  return indice;
}

std::size_t hull_point(
    const std::vector<vec3> &points)
{
  std::cout << "hull_point " << std::endl;

  double xmin = points[0][0];
  size_t indice = 0;
  size_t x = 0;
  for (vec3 v : points)
  {

    if (v[0] < xmin)
    {
      xmin = v[0];
      indice = x;
    }
    x++;
  }
  //  std::cout << "vx " << v[0] << "vy " << v[1] << std::endl;
  std::cout << "vx " << points[indice][0] << "vy " << points[indice][1] << std::endl;
  std::cout << "indice " << indice << std::endl;
  return indice;
}

//-- 2D wrapping

std::size_t wrap(
    const std::vector<vec2> &points,
    const vec2 &p0)
{
  size_t indice = 0;
  vec2 p1 = points[0];
  for (size_t i = 0; i < points.size(); i++)
  {
    if (points[i] != p0)
    {
      if (orient(p0, p1, points[i]) == -1)
      {
        p1 = points[i];
        indice = i;
      }
    }
  }
  // std::cout << "indice " << indice << std::endl;
  return indice;
}

std::size_t wrap(
    const std::vector<vec3> &points,
    const vec3 &p0,
    const vec3 &p1)
{

  size_t indice = 0;
  vec3 p2 = points[0];
  for (size_t i = 0; i < points.size(); i++)
  {
    if (points[i] != p0 && points[i] != p1)
    {
      if (orient(p0, p1, p2, points[i]) == -1)
      {
        p2 = points[i];
        indice = i;
      }
    }
  }

  return indice;
}

//-- convex hull

void jarvis2d(
    const std::vector<vec2> &points,
    std::vector<std::size_t> &hull)
{
  size_t indice_0 = hull_point(points);
  hull.push_back(indice_0);
  size_t temp = 0;
  size_t temp1 = indice_0;
  // std::cout << "huljarvis2dl " << std::endl;

  while (indice_0 != temp)
  {
    temp = wrap(points, points[temp1]);
    temp1 = temp;
    // std::cout << "temp "  << temp<< std::endl;

    hull.push_back(temp);
  }

  return;
}
using edge = std::pair<std::size_t, std::size_t>;
// using edgeset = std::set<edge>;
void jarvis3d(
    const std::vector<vec3> &points,
    std::vector<std::size_t> &hull)
{
  edge d;
  std::vector<edge> tab_edge_to_work;

  size_t indice_0 = hull_point(points);
  hull.push_back(indice_0);
  vec3 p1;
  vec3 p0 = points[indice_0];
  p1[0] = p0[0];
  p1[1] = 0;
  p1[2] = 0;

  size_t indice_p2 = wrap(points, p0, p1);
  //vec3 p2 = points[indice_p2];

  hull.push_back(indice_0);
  hull.push_back(indice_p2);

  edge temp ;
  //size_t temp1 = indice_0;

  edge finalle;
  finalle.first = indice_0;
  finalle.second = indice_p2;
  // std::cout << "huljarvis2dl " << std::endl;
  size_t ind_p0_temp = indice_0;
  size_t ind_p1_temp = indice_p2;

  size_t ind_p2_temp ;
  while (finalle != temp)
  {
    ind_p2_temp = wrap(points, points[ind_p0_temp],points[ind_p1_temp]);

    // std::cout << "temp "  << temp<< std::endl;

    //hull.push_back(temp);
  }
  return;
}