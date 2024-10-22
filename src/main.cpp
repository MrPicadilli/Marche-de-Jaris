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

int main()
{

  {
    // 2D points for 2D tests

    std::vector<vec2> points;

    // 100 random points in the unit disk
    for (int i = 0; i < 100; ++i)
    {
      points.push_back(rand_in_disk());
    }

#define ORIENT_TEST
#ifdef ORIENT_TEST
    {
      // test orientation

      // segment
      vec2 s0 = {-1, -1};
      vec2 s1 = {1, 1};

      // plot the input points
      SVG::SVG svg;
      SVG::Element *grp = svg.add_child<SVG::Group>();
      for (vec2 &pt : points)
      {
        grp->add_child<SVG::Circle>(res * pt[0], -res * pt[1], pt_rad);
      }

      // plot the separating line
      SVG::Line *segment = grp->add_child<SVG::Line>(
          res * s0[0],
          res * s1[0],
          -res * s0[1],
          -res * s1[1]);

      segment->set_attr("stroke", "black");

      // color them according to their side wrt. the line between the extremes
      for (vec2 &pt : points)
      {
        SVG::Circle *c = grp->add_child<SVG::Circle>(res * pt[0], -res * pt[1], 2 * pt_rad);
        c->set_attr("fill", "none");
        int o = orient(s0, s1, pt);
        if (o > 0)
        {
          c->set_attr("stroke", "red");
        }
        else
        {
          c->set_attr("stroke", "green");
        }
      }

      // export the final svg
      svg.autoscale();
      std::ofstream file("results/orient.svg");
      file << std::string(svg);

      std::cout << "orientation test generated in orient.svg" << std::endl;
    }
#endif

#define HULL_POINT_TEST
#ifdef HULL_POINT_TEST
    {
      // test hull point

      // plot the input points
      SVG::SVG svg;
      SVG::Element *grp = svg.add_child<SVG::Group>();
      for (vec2 &pt : points)
      {
        grp->add_child<SVG::Circle>(res * pt[0], -res * pt[1], pt_rad);
      }

      // plot the hull point
      std::size_t l = hull_point(points);
      SVG::Circle *c = grp->add_child<SVG::Circle>(res * points[l][0], -res * points[l][1], 2 * pt_rad);
      c->set_attr("fill", "none");
      c->set_attr("stroke", "black");

      // export the final svg
      svg.autoscale();
      std::ofstream file("results/hull_point.svg");
      file << std::string(svg);

      std::cout << "hull point test generated hull_point.svg" << std::endl;
    }
#endif

#define WRAP_2D_TEST
#ifdef WRAP_2D_TEST
    {
      // test wrap

      // plot the input points
      SVG::SVG svg;
      SVG::Element *grp = svg.add_child<SVG::Group>();
      for (vec2 &pt : points)
      {
        grp->add_child<SVG::Circle>(res * pt[0], -res * pt[1], pt_rad);
      }

      // plot the hull point
      std::size_t l = hull_point(points);
      SVG::Circle *cl = grp->add_child<SVG::Circle>(res * points[l][0], -res * points[l][1], 2 * pt_rad);
      cl->set_attr("fill", "none");
      cl->set_attr("stroke", "black");

      // plot the next point obtained through wrapping
      std::size_t n = wrap(points, points[l]);
      SVG::Circle *cn = grp->add_child<SVG::Circle>(res * points[n][0], -res * points[n][1], 2 * pt_rad);
      cn->set_attr("fill", "none");
      cn->set_attr("stroke", "blue");

      // plot the hull segment
      SVG::Line *segment = grp->add_child<SVG::Line>(
          res * points[l][0],
          res * points[n][0],
          -res * points[l][1],
          -res * points[n][1]);

      segment->set_attr("stroke", "black");

      // export the final svg
      svg.autoscale();
      std::ofstream file("results/wrap.svg");
      file << std::string(svg);

      std::cout << "2D wrap test generated wrap_2d.svg" << std::endl;
    }
#endif

#define HULL_2D_TEST
#ifdef HULL_2D_TEST
    {
      // test hull 2d

      // compute the convex hull
      std::vector<std::size_t> hull;
      // gw_plot.toggle() ;
      jarvis2d(points, hull);
      // gw_plot.toggle() ;

      // plot the input points
      SVG::SVG svg;
      SVG::Element *grp = svg.add_child<SVG::Group>();
      for (vec2 &pt : points)
      {
        grp->add_child<SVG::Circle>(res * pt[0], -res * pt[1], pt_rad);
      }

      // plot the hull polygon
      std::vector<SVG::Point> svgpts;
      for (std::size_t &i : hull)
      {
        SVG::Circle *c = grp->add_child<SVG::Circle>(res * points[i][0], -res * points[i][1], 2 * pt_rad);
        c->set_attr("fill", "none");
        c->set_attr("stroke", "black");
        svgpts.push_back({res * points[i][0], -res * points[i][1]});
      }

      SVG::Polygon *poly = grp->add_child<SVG::Polygon>(svgpts);
      poly->set_attr("fill", "none");
      poly->set_attr("stroke", "black");

      // export the final svg
      svg.autoscale();
      std::ofstream file("results/hull_2d.svg");
      file << std::string(svg);

      std::cout << "2D hull generated in hull_2d.svg" << std::endl;
    }
#endif
  }

  {
    // 3D points for 3D tests

    std::vector<vec3> points;

    // 100 random points in the unit disk
    for (int i = 0; i < 100; ++i)
    {
      points.push_back(rand_in_ball());
    }

#define WRAP_3D_TEST
#ifdef WRAP_3D_TEST
    {
      // test hull 3d

      // export to wavefront obj file
      std::ofstream file("results/wrap_3d.obj");

      // vertex export
      for (const vec3 &pt : points)
      {
        file << "v " << pt[0] << " " << pt[1] << " " << pt[2] << "\n";
      }

      // hull point p0
      vec3 p0 = {-1, -1, -1};
      vec3 p1 = {-1, 1, -1};

      file << "v " << p0[0] << " " << p0[1] << " " << p0[2] << "\n";
      file << "v " << p1[0] << " " << p1[1] << " " << p1[2] << "\n";

      // wrapped point p2
      std::size_t p_index = wrap(points, p0, p1);

      // initial triangle export
      file << "f " << points.size() + 1 << " " << p_index + 1 << " " << points.size() + 2 << "\n";
      file.flush();

      std::cout << "3D wrap test generated in wrap_3d.obj" << std::endl;
    }
#endif

#define HULL_EDGE_TEST
#ifdef HULL_EDGE_TEST
    {
      // test hull 3d

      // export to wavefront obj file
      std::ofstream file("results/hull_edge.obj");

      // vertex export
      for (const vec3 &pt : points)
      {
        file << "v " << pt[0] << " " << pt[1] << " " << pt[2] << "\n";
      }

      // hull point p0
      std::size_t p0_index = hull_point(points);

      // additional point q
      vec3 q = points[p0_index] + vec3({0, 1, 0});

      // wrapped point p1
      std::size_t p1_index = wrap(points, points[p0_index], q);

      // medium point to draw edge as a degenerate triangle
      vec3 mid = 0.5 * (points[p0_index] + points[p1_index]);
      file << "v " << mid[0] << " " << mid[1] << " " << mid[2] << "\n";

      // initial triangle export
      file << "f " << p0_index + 1 << " " << points.size() + 1 << " " << p1_index + 1 << "\n";
      file.flush();

      std::cout << "3D hull_edge test generated in hull_edge.obj" << std::endl;
    }
#endif

#define HULL_3D_TEST
#ifdef HULL_3D_TEST
    {
      // test hull 3d

      // compute the convex hull
      std::vector<std::size_t> hull;
      jarvis3d(points, hull);

      // export to wavefront obj file
      std::ofstream file("results/hull_3d.obj");

      // vertex export
      for (const vec3 &pt : points)
      {
        file << "v " << pt[0] << " " << pt[1] << " " << pt[2] << "\n";
      }

      // triangle export
      for (std::size_t i = 0; i < hull.size(); i += 3)
      {
        file << "f " << hull[i] + 1 << " " << hull[i + 1] + 1 << " " << hull[i + 2] + 1 << "\n";
      }
      file.flush();

      std::cout << "3D hull generated in hull_3d.svg" << std::endl;
    }
#endif
  }

  return 0;
}
