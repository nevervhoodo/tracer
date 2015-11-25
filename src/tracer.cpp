#include "tracer.h"
#include "glmextra.hpp"

#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>

using namespace glm;
using namespace std;

#define DEBUG false
#define DEBUG2 false

dvec3 CTracer::MakeSky (dvec3 ray_pos)
{
        ray_pos = normalize(ray_pos);
        if (DEBUG2)
                cout<<ray_pos.x<<" "<<
                ray_pos.y<<" "<<ray_pos.z<<endl;
        double dphi,dteta;
        uint phi, teta;
        if (length(ray_pos)>PRECISION)
                ray_pos = normalize(ray_pos);
        else
                cout<<"tracer.cpp 15 normalize"<<endl;

        dphi = (atan2(ray_pos.x, ray_pos.y)+M_PI)/M_PI/2*(stars.width()-1);
        //dphi = (atan2(ray_pos.x, ray_pos.y)+M_PI)*stars.width()/M_PI/2;
        dteta = (asin (ray_pos.z)+M_PI/2)/M_PI*(stars.height()-1);
        //dteta = (asin (ray_pos.z)+M_PI/2)*stars.height()/M_PI;
        phi = dphi;//*stars.width();
        teta = dteta;//*stars.height();
        if (DEBUG)
                cout<<dteta<<" "<<
                dphi<<" "<<teta<<" "<<phi<<
                " "<<endl;
        uint8_t r = stars(phi, teta, 0);
        uint8_t g = stars(phi, teta, 1);
        uint8_t b = stars(phi, teta, 2);
        //uint8_t a = stars(j, i, 3);
        return dvec3(r,g,b) / 255.0;
}

dvec3 CTracer::FoundDisk(SRay ray)
{

}

SRay CTracer::MakeRay(uvec2 pixelPos)
{
        double ppx = double(pixelPos.x) - m_camera.m_resolution.x / 2.0;
        double ppy = double(pixelPos.y) - m_camera.m_resolution.y / 2.0;

        dvec3 n_forw, n_up, n_right;
        SRay ray;
        n_forw = normalize(m_camera.m_forward);
        n_up = normalize(m_camera.m_up);
        n_right = normalize(m_camera.m_right);
        double x = 2.0 * length(m_camera.m_forward) * tan(m_camera.m_viewAngle.x) / m_camera.m_resolution.x;
        double y = 2.0 * length(m_camera.m_forward) * tan(m_camera.m_viewAngle.y) / m_camera.m_resolution.y;

        x *= 0.5 + ppx;
        y *= 0.5 + ppy;

        ray.m_start = m_camera.m_forward;
        ray.m_start += x * n_right;
        ray.m_start += y * n_up;


        ray.m_dir = normalize(ray.m_start);
        ray.m_start += m_camera.m_pos;
        return ray;
}

glm::dvec3 CTracer::TraceRay(SRay ray)
{
        // return ray.m_dir;

        dvec3 a,v,tmpv;
        dvec3 color;
        double tmp;
        double dt;
        ray.m_dir=normalize(ray.m_dir)*dvec3(VC,VC,VC);
        /*for (int iter;iter<1000;iter++)
        //for (;;)
        {
                tmp = coeff/pow(length(ray.m_start),3);
                a = perp(ray.m_start*dvec3(tmp,tmp,tmp),ray.m_dir);
                tmpv = ray.m_dir*dvec3(dtime,dtime,dtime)+
                        a*dvec3(dtime*dtime/2,dtime*dtime/2,dtime*dtime/2);
                if (length(tmpv)<10)
                {
                        return MakeSky(ray.m_dir+ray.m_start);
                }
                ray.m_start+=tmpv;
                ray.m_dir+=a*dvec3(dtime,dtime,dtime);
                if (length(ray.m_dir)>PRECISION)
                ray.m_dir=normalize(ray.m_dir)*dvec3(VC,VC,VC);
                dt = -ray.m_start.z/ray.m_dir.z;
                // if ((dt>PRECISION)&&(dt<=1))
                //         return FoundDisk(ray);
        }*/
        //return dvec3(1,0,1);
        return MakeSky(ray.m_dir);
}

void CTracer::RenderImage(int xRes, int yRes)
{
        // Reading input texture sample
        //CImage* pImage = LoadImageFromFile("data/disk_32.png");

        mass = 8.57e+36f; //default !!
        radhole = GCONST*2*mass/VC/VC;
        coeff = GCONST*mass;

        disk = CImage("data/disk_32.png");
        stars = CImage("data/stars.jpg");
        /*for (int i = 0; i < img.height(); i++) { // Image lines
                for(int j = 0; j < img.width(); j++) { // Pixels in line
                        uint8_t r = img(j, i, 0);
                        uint8_t g = img(j, i, 1);
                        uint8_t b = img(j, i, 2);
                        uint8_t a = img(j, i, 3); // TODO: check!
                }
        }*/

        // Rendering
        //m_camera.m_resolution = uvec2(xRes, yRes);
        //m_camera.m_pixels.resize(xRes * yRes);

        for (int i = 0; i < yRes; i++) {
                for (int j = 0; j < xRes; j++) {
                        SRay ray = MakeRay(uvec2(j, i));
                        m_camera.m_pixels[i * xRes + j] = TraceRay(ray);
                }
        }
}

void CTracer::SaveImageToFile(std::string fileName)
{
        int width = m_camera.m_resolution.x;
        int height = m_camera.m_resolution.y;

        CImage image(width, height, 1, 3);

        //unsigned char* imageBuffer = (unsigned char*)image.data();

        int i, j;
        int imageDisplacement = 0;
        int textureDisplacement = 0;

        for (i = 0; i < height; i++) {
                for (j = 0; j < width; j++) {
                        dvec3 color = m_camera.m_pixels[textureDisplacement + j];
                        
                        image(j, i, 0) = clamp(color[0], 0.0, 1.0) * 255.0;
                        image(j, i, 1) = clamp(color[1], 0.0, 1.0) * 255.0;
                        image(j, i, 2) = clamp(color[2], 0.0, 1.0) * 255.0;
                        
                        /*
                        imageBuffer[imageDisplacement + j * 3] = 255.0f; //clamp(color.b, 0.0f, 1.0f) * 255.0f;
                        imageBuffer[imageDisplacement + j * 3 + 1] = clamp(color.g, 0.0f, 1.0f) * 255.0f;
                        imageBuffer[imageDisplacement + j * 3 + 2] = clamp(color.r, 0.0f, 1.0f) * 255.0f;
                        */
                }

                textureDisplacement += width;
        }

        image.save(fileName.c_str());
}

CImage* CTracer::LoadImageFromFile(std::string fileName)
{
        try {
                CImage* pImage = new CImage(fileName.c_str());
                return pImage;
        } catch (...) {
                std::cerr << "Error opening file " << fileName << std::endl;
                return NULL;
        }
}
