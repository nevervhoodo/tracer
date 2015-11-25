#include "tracer.h"
#include <cstdio>
#include <cmath>

using glm::dvec3;
using glm::dvec2;
using glm::uvec2;
using glm::length;

int main(int argc, char** argv)
{
        CTracer tracer;
        CScene scene;

        // int xRes = 1024;  // Default resolution
        // int yRes = 768;
        // tracer.m_camera.m_pos = dvec3(6.5e+10,0,0);          // Camera position and orientation
        // tracer.m_camera.m_forward = dvec3(1000,0,0);      // Orthonormal basis
        // tracer.m_camera.m_right = dvec3(0,1,0)*dvec3(1024,1024,1024);
        // //cout<<m_right.x<<" "<<m_right.y<<" "<<m_right.z<<endl;
        // tracer.m_camera.m_up = dvec3(0,0,1)*dvec3(768,768,768); 
        // //cout<<m_up.x<<" "<<m_up.y<<" "<<m_up.z<<endl;
        // tracer.m_camera.m_viewAngle = dvec2(30/180*M_PI,20/180*M_PI);    // View angles, rad
        // tracer.m_camera.m_resolution = uvec2(1024,768);  // Default resolutio
        // tracer.m_camera.m_pixels.resize(1024*768);
        
        int xRes = 512;  // Default resolution
        int yRes = 512;
        tracer.m_camera.m_pos = dvec3(0,0,0);          // Camera position and orientation
        tracer.m_camera.m_forward = dvec3(1 ,0, 0);
        tracer.m_camera.m_right = dvec3(0,1,0);
        //cout<<m_right.x<<" "<<m_right.y<<" "<<m_right.z<<endl;
        tracer.m_camera.m_up = dvec3(0 ,0, 1);
        // double norm = pow(length(tracer.m_camera.m_forward),2);
        // norm += pow(length(tracer.m_camera.m_up),2);
        // norm += pow(length(tracer.m_camera.m_right),2);
        // tracer.m_camera.m_right /= double(norm);
        // tracer.m_camera.m_up /= double(norm);
        // tracer.m_camera.m_forward /= double(norm);
        // tracer.len_forward = norm;
        //cout<<m_up.x<<" "<<m_up.y<<" "<<m_up.z<<endl;
        tracer.m_camera.m_viewAngle = dvec2(30.0/180*M_PI,30.0/180*M_PI);    // View angles, rad
        tracer.m_camera.m_resolution = uvec2(512,512); 
        tracer.m_camera.m_pixels.resize(512 * 512);

// 4.5e+11 0 0.2e+11
// -20 0 -1
// -1 0 20
// 0 -1 0
// 60 33.75
        if(argc == 2) // There is input file in parameters
        {
                FILE* file = fopen(argv[1], "r");
                if(file) {
                        int xResFromFile = 0;
                        int yResFromFile = 0;
                        if(fscanf(file, "%d %d", &xResFromFile, &yResFromFile) == 2) {
                               xRes = xResFromFile;
                               yRes = yResFromFile;
                        } else {
                                printf("Invalid config format! Using default parameters.\r\n");
                        }

                        fclose(file);
                } else {
                        printf("Invalid config path! Using default parameters.\r\n");
                }
        } else {
                printf("No config! Using default parameters.\r\n");
        }

        tracer.m_pScene = &scene;
        tracer.RenderImage(xRes, yRes);
        tracer.SaveImageToFile("Result.png");

        return 0;
}
