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
        if (length(ray_pos)>PRECISION)
                ray_pos = normalize(ray_pos);
        else
                cout<<"sky"<<endl;
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

double CTracer::FoundDisk(SRay ray, dvec3 &color)
{
        double dt = -ray.m_start.z/ray.m_dir.z;
        double rad;
        dvec2 coord;
        uint i,j;
        if ((dt>PRECISION)&&(dt<=dtime))
        {
                cout<<"here"<<endl;
                ray.m_start += ray.m_dir*double(dt);
                rad = sqrt(ray.m_start.x*ray.m_start.x + 
                        ray.m_start.y*ray.m_start.y); 
                if (rad <= raddisk )
                {
                        coord = dvec2(ray.m_start.x,ray.m_start.y);
                        coord /= (double)raddisk;
                        coord *= (double)diskrad;
                        i = coord.x;
                        j = coord.y;
                        uint8_t r = disk(i, j, 0);
                        uint8_t g = disk(i, j, 1);
                        uint8_t b = disk(i, j, 2);
                        uint8_t a = disk(i, j, 3);
                        if (a)
                        {
                                color = dvec3(r,g,b) / 255.0;
                                return dt;
                        }
                }
        }
        return -1;
}

double CTracer::BlackHole(SRay ray)
{
        double b,c,d,ht;
        double r1,r2;
        b = length (ray.m_dir);//mb -length!!
        c = dot(ray.m_dir,ray.m_dir) - radhole*radhole;
        d = b*b - 4*c;//strange official 
        if (d>=PRECISION)
        {   
                cout<<"black here"<<endl;
                ht = fmin((-b + sqrt(d))/2.0,(-b - sqrt(d))/2.0);
                if ((ht>-PRECISION)&&(ht<=dtime))
                {
                        cout<<"black surely here"<<endl;
                        return ht;
                }
        }
        return -1;
        // dvec3 m_fin = ray.m_start + ray.m_dir;
        // r1 = sqrt(ray.m_start.x*ray.m_start.x + ray.m_start.y*ray.m_start.y 
        //         + ray.m_start.z*ray.m_start.z);
        
        // r2 = sqrt(m_fin.x*m_fin.x + m_fin.y*m_fin.y + m_fin.z*m_fin.z); 

        // dvec3 k = ray_pos - spos;
        // if (((r1>radhole)^(r2>radhole))||(r2<radhole))
        // {
        //         cout<<"black here"<<endl;
        //         return true;
        // }
        // else
        //         return false;  
}

SRay CTracer::MakeRay(uvec2 pixelPos)
{
        //cout<<"evrika"<<endl;
        double ppx = double(pixelPos.x) - m_camera.m_resolution.x / 2.0;
        double ppy = double(pixelPos.y) - m_camera.m_resolution.y / 2.0;

        dvec3 n_forw, n_up, n_right;
        SRay ray;
        
        n_up = normalize(m_camera.m_up);
        n_right = normalize(m_camera.m_right);
        double x = 2.0 * length(m_camera.m_forward) * 
            tan(m_camera.m_viewAngle.x) / m_camera.m_resolution.x;
        double y = 2.0 * length(m_camera.m_forward) * 
            tan(m_camera.m_viewAngle.y) / m_camera.m_resolution.y;

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
        //cout<<"*";
        double r, ht, dt;
        dvec3 a,an,change;
        dvec3 color = dvec3(0.0,0.0,0.0);
        if (length(ray.m_dir)>PRECISION)
                ray.m_dir = normalize(ray.m_dir);
        else
                cout<<"tracer0"<<endl;
        ray.m_dir *= VC;
        //for (int iter;iter<10000;iter++)
        for (;;)
        {
                r = length(ray.m_start);
                a = -coeff/r/r/r * ray.m_start;
                an = perp (a,ray.m_dir);
                change = dtime*ray.m_dir+an*double(dtime*dtime/2.0);
                if (length(change)<1)
                {
                        cout << "happy"<<endl;
                        return MakeSky(ray.m_dir);
                }
                ray.m_start+=change;
                ray.m_dir+=dtime*an;
                if (length(ray.m_dir)>PRECISION)
                        ray.m_dir = normalize(ray.m_dir);
                else
                        cout<<"tracer"<<endl;
                ray.m_dir *= VC;

                ht = BlackHole(ray);
                dt = FoundDisk(ray,color);
                //ray.m_dir=normalize(ray.m_dir)*double(VC); 
                /*
                if (BlackHole(ray))
                        return color;
                if (FoundDisk(ray,color))
                        return color;  */
                if ((ht>PRECISION)&&(ht<dtime))
                        if ((dt>PRECISION)&&(dt<dtime))
                                if (dt<ht)
                                        return color;
                                else
                                        return dvec3(0.0,0.0,0.0);
                        else
                                return dvec3(0.0,0.0,0.0);
                else if ((dt>PRECISION)&&(dt<dtime))
                        return color;
                else if (r > length(m_camera.m_pos))
                        return MakeSky(ray.m_dir+ray.m_start);
        }
        return dvec3(1,0,1);
        //return MakeSky(ray.m_dir);
}

void CTracer::RenderImage(int xRes, int yRes)
{
        // Reading input texture sample
        //CImage* pImage = LoadImageFromFile("data/disk_32.png");

        mass = 8.57e+36f; //default !!
        radhole = GCONST*2.0*mass/VC/VC;
        coeff = GCONST*mass;
        raddisk = radhole * 10.0; //!CONFIG
        disk = CImage("data/disk_32.png");
        diskrad = fmin(disk.height(),disk.width());
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