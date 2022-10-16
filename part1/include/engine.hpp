#pragma once

#include "mesh.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>
#include <algorithm>

// data structers and methods required for our engine
class engine 
{
    public:
        mat4x4 projMat, rotateMatX, rotateMatZ, rotateMatY;
        mesh meshObj;
        point3d center;
        std::vector<point3d> allPoints;

        engine(std::string filePath)
        {
            meshObj.readVertices(filePath);
            allPoints.resize(meshObj.numVertices);
        }

        void drawOnWindow(sf::RenderWindow *window)
        {
            // Draw triangles
            int currentTriangle = 0;
            center = { 0.5f * (float)window->getSize().x, 0.5f * (float)window->getSize().y, 0.0f};
            for(auto tri : meshObj.tris)
            {
                triangle triProjected, triTranslated, triRotatedX, triRotatedXY;

                // std::cout<<"Triangle "<<currentTriangle<<std::endl;
                for(int i = 0; i < 3; ++i)
                {   
                    matVecMult(tri.pt[i], rotateMatX, triRotatedX.pt[i]);
                    matVecMult(triRotatedX.pt[i], rotateMatY, triRotatedXY.pt[i]);

                    triTranslated = triRotatedXY;
                    triTranslated.pt[i].z = triRotatedXY.pt[i].z + 3.0f;

                    matVecMult(triTranslated.pt[i], projMat, triProjected.pt[i]);

                    // Scale into view
                    triProjected.pt[i].x += 1.0f;
                    triProjected.pt[i].y += 1.0f;
                    triProjected.pt[i].x *= 0.5f * (float)(*window).getSize().x;
                    triProjected.pt[i].y *= 0.5f * (float)(*window).getSize().y;
                    

                    // std::cout<<"Raw Point "<<i<<": ";
                    // std::cout<<tri.pt[i].x<<" "<<tri.pt[i].y<<" "<<tri.pt[i].z<<std::endl;
                    
                    // std::cout<<"Point Proj "<<i<<": ";
                    // std::cout<<triProjected.pt[i].x<<" "<<triProjected.pt[i].y<<" "<<triProjected.pt[i].z<<std::endl;
                    
                    // std::cout<<"Mult"<<std::endl;
                    // std::cout<<triProjected.pt[i].x<<" "<<triProjected.pt[i].y<<" "<<triProjected.pt[i].z<<std::endl;
                }

                drawTri(window, triProjected);
                
            }

        }

        void getProjMat(float fovDegrees, float aspectRatio, float fNear, float fFar)
        {
            float fovRad;
            // Populate the matrix
            fovRad = 1.0f / tanf( fovDegrees * 0.5f / 180.0f * PI);
            projMat.m[0][0] = aspectRatio * fovRad;
            projMat.m[1][1] = fovRad;
            projMat.m[2][2] = fFar / (fFar - fNear);
            projMat.m[3][2] = (-fFar * fNear) / (fFar - fNear);
            projMat.m[2][3] = 1.0f;
            projMat.m[3][3] = 0.0f;
        }
        float getDistanceToPoint(point3d pt)
        {
            return eucledianDistance(center, pt);
        }

        void rotate(float phiX, float phiY, float phiZ = 0.0f)
        {
            getRotatationMat(phiX, phiY);
        }

    private:

        // TODO: Convert point to array and mult. using loop
        void matVecMult(point3d &in, mat4x4 &mat, point3d &out)
        {
            out.x = in.x * mat.m[0][0] + in.y * mat.m[1][0] + in.z * mat.m[2][0] + mat.m[3][0];
            out.y = in.x * mat.m[0][1] + in.y * mat.m[1][1] + in.z * mat.m[2][1] + mat.m[3][1];
            out.z = in.x * mat.m[0][2] + in.y * mat.m[1][2] + in.z * mat.m[2][2] + mat.m[3][2];
            
            // Normalize the projections by "z" value
            float norm = in.x * mat.m[0][3] + in.y * mat.m[1][3] + in.z * mat.m[2][3] + mat.m[3][3];

            if (norm != 0)
            {
                out /= norm;
            }
        }

        void getRotatationMat(float phiX, float phiY, float phiZ = 0.0f)
        {
            getRotateMatX(phiX);
            getRotateMatZ(phiZ);
            getRotateMatY(phiY);
        }

        void getRotateMatX(float theta)
        {
            rotateMatX.m[0][0] = 1.0f;
            rotateMatX.m[1][1] = cosf(theta);
            rotateMatX.m[1][2] = sinf(theta);
            rotateMatX.m[2][1] = -sinf(theta);
            rotateMatX.m[2][2] = cosf(theta);
            rotateMatX.m[3][3] = 1.0f;
        }

        void getRotateMatZ(float theta)
        {
            rotateMatZ.m[0][0] = cosf(theta);
            rotateMatZ.m[0][1] = sinf(theta);
            rotateMatZ.m[1][0] = -sinf(theta);
            rotateMatZ.m[1][1] = cosf(theta);
            rotateMatZ.m[2][2] = 1.0f;
            rotateMatZ.m[3][3] = 1.0f;
        }

        void getRotateMatY(float theta)
        {
            rotateMatY.m[0][0] = cosf(theta);
            rotateMatY.m[0][2] = sinf(theta);
            rotateMatY.m[2][0] = -sinf(theta);
            rotateMatY.m[1][1] = 1.0f;
            rotateMatY.m[2][2] = cosf(theta);
            rotateMatY.m[3][3] = 1.0f;
        }

        void drawVertex(sf::RenderWindow *window, point3d &pt)
        {
            sf::CircleShape circle(5.0f);
            circle.setFillColor(sf::Color::Blue);
            circle.setPosition(pt.x, pt.y);
            circle.setOrigin(5.0f, 5.0f);
            window->draw(circle);
        }

        void drawTri(sf::RenderWindow *window, triangle &tris)
        {
            sf::Vertex line[2];
            int count = 0;
            // Draw lines between the three points
            for (int i = 0; i < 2; ++i)
            {
                for (int j = i+1; j < 3; ++j)
                {
                    line[0].position = sf::Vector2f(tris.pt[i].x, tris.pt[i].y);
                    line[0].color  = sf::Color::Blue;
                    line[1].position = sf::Vector2f(tris.pt[j].x, tris.pt[j].y);
                    line[1].color  = sf::Color::Blue;

                    drawVertex(window, tris.pt[j]);
                    drawVertex(window, tris.pt[i]);
                    window->draw(line, 2, sf::Lines);
                    
                }
            }

        }

        float eucledianDistance(point3d &p1, point3d &p2)
        {
            float dist_x = pow(p1.x - p2.x,2);
            float dist_y = pow(p1.y - p2.y,2);
            float dist_z = pow(p1.z - p2.z,2);

            return sqrt(dist_x + dist_y + dist_z);
        }

};