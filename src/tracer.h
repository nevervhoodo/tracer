#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/perpendicular.hpp"
#include "glmextra.hpp"
#include "types.h"
#include "scene.h"

#include "string"
#include "CImg.h"

#define PRECISION 0.000001
#define NOCHANGE 0.01
#define GCONST 6.67384e-11f
#define VC 3e+8f

typedef cimg_library::CImg<unsigned char> CImage;

class CTracer
{
	CImage stars, disk;
	double mass, raddisk, radhole;
	double coeff;
	double dtime;
        uint diskrad;
public:
        SRay MakeRay(glm::uvec2 pixelPos);  // Create ray for specified pixel
        glm::dvec3 TraceRay(SRay ray); // Trace ray, compute its color
        void RenderImage(int xRes, int yRes);
        void SaveImageToFile(std::string fileName);
        CImage* LoadImageFromFile(std::string fileName);
        glm::dvec3 MakeSky (glm::dvec3 ray_pos);
        double FoundDisk(SRay ray, glm::dvec3 &color);
        double BlackHole(SRay ray);
public:
	// double len_forward;
        SCamera m_camera;
        CScene* m_pScene;
};