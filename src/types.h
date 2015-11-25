#pragma once

#include "glm/glm.hpp"
#include "vector"

typedef unsigned int uint;
typedef unsigned char uchar;

struct SRay
{
  glm::dvec3 m_start;
  glm::dvec3 m_dir;
};

struct SCamera
{
  glm::dvec3 m_pos;          // Camera position and orientation
  glm::dvec3 m_forward;      // Orthonormal basis
  glm::dvec3 m_right;
  glm::dvec3 m_up;

  glm::dvec2 m_viewAngle;    // View angles, rad
  glm::uvec2 m_resolution;  // Image resolution: w, h

  std::vector<glm::dvec3> m_pixels;  // Pixel array
};

struct SMesh
{
  std::vector<glm::dvec3> m_vertices;  // vertex positions
  std::vector<glm::uvec3> m_triangles;  // vetrex indices
};
