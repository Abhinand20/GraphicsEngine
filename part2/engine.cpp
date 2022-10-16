/**
 * @file engine.cpp 
 * @author Abhinand Jha (abhinanj@andrew.cmu.edu)
 * @brief 
 * @version 0.1
 * @date 2022-10-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "include/engine.hpp"
#include "include/mesh.hpp"
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>
#include <algorithm>

engine::engine(std::string filePath)
{
    success = meshObj.readVertices(filePath);
    allPoints.resize(meshObj.numVertices);
    cameraPos = point3d{0,0,0};
}

void engine::drawOnWindow(sf::RenderWindow *window)
{
    int currentTriangle = 0;
    bool toRender;
    std::vector<triangle> trianglesToRender;
    center = { 0.5f * (float)window->getSize().x, 0.5f * (float)window->getSize().y, 0.0f};
    for(auto tri : meshObj.tris)
    {
        triangle triProjected, triTranslated, triRotatedX, triRotatedXY, ray, normalRotatedX, normalRotatedXY, normalProjected;
        point3d normal;
        
        // Only render triangles that are facing the camera
        for(int i = 0; i < 3; ++i)
        {   
            // Translate the triangle
            matVecMult(tri.pt[i], rotateMatX, triRotatedX.pt[i]);
            matVecMult(triRotatedX.pt[i], rotateMatY, triRotatedXY.pt[i]);

            triTranslated.pt[i] = triRotatedXY.pt[i];
            triTranslated.pt[i].z = triRotatedXY.pt[i].z + 3.0f;
        }

        // Calculate the normal of the triangle
        normal = getNormal(triTranslated);

        // Get ray from triangle to camera
        point3d cameraRay = triTranslated.pt[0] - cameraPos;

        if(vecDotProduct(normal, cameraRay) > 0.0f)
        {
            continue;
        }

        // Project triangles from 3D to 2D
        for(int i = 0; i < 3; ++i)
        {

            // Illumination
            point3d lightDirection = {0.0f, 0.0f, 1.0f};
            float normlize = sqrt(lightDirection.x * lightDirection.x + lightDirection.y * lightDirection.y + lightDirection.z * lightDirection.z);
            lightDirection /= normlize;

            float dp = std::max(0.1f, vecDotProduct(normal, lightDirection));

            matVecMult(triTranslated.pt[i], projMat, triProjected.pt[i]);

            // Scale into view
            triProjected.pt[i].x += 1.0f;
            triProjected.pt[i].y += 1.0f;
            triProjected.pt[i].x *= 0.5f * (float)(*window).getSize().x;
            triProjected.pt[i].y *= 0.5f * (float)(*window).getSize().y;
            triProjected.illumination = dp; // value between 0.1 and 1.0
            
        }
                        
        trianglesToRender.push_back(triProjected);

        // Painter's algorithm
        // Get the midpoint of the triangles and sort them
        // Draw the triangles from back to front
        sort(trianglesToRender.begin(), trianglesToRender.end(), [](triangle &a, triangle &b) {
            float z1 = (a.pt[0].z + a.pt[1].z + a.pt[2].z) / 3.0f;
            float z2 = (b.pt[0].z + b.pt[1].z + b.pt[2].z) / 3.0f;
            return z1 > z2;
        });

        for(auto tri : trianglesToRender)
        {
            drawTri(window, tri);
        }
        
    }

}

void engine::getProjMat(float fovDegrees, float aspectRatio, float fNear, float fFar)
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

float engine::vecDotProduct(point3d &v1, point3d &v2)
{
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

float engine::getDistanceToPoint(point3d pt)
{
    return eucledianDistance(center, pt);
}

void engine::rotate(float phiX, float phiY, float phiZ)
{
    getRotatationMat(phiX, phiY);
}



void engine::matVecMult(point3d &in, mat4x4 &mat, point3d &out)
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

point3d engine::getNormal(triangle tri)
{
    point3d normal, line1, line2;
    line1 = tri.pt[1] - tri.pt[0];
    line2 = tri.pt[2] - tri.pt[0];

    // Cross-product between the lines
    normal = vecCrossProduct(line1, line2);

    // Normalize the normal
    normal /= eucledianDistance(normal, point3d{0.0f, 0.0f, 0.0f});

    return normal;
}

point3d engine::vecCrossProduct(point3d &v1, point3d &v2)
{
    point3d cross;
    cross.x = v1.y * v2.z - v1.z * v2.y;
    cross.y = v1.z * v2.x - v1.x * v2.z;
    cross.z = v1.x * v2.y - v1.y * v2.x;
    return cross;
}

void engine::getRotatationMat(float phiX, float phiY, float phiZ)
{
    getRotateMatX(phiX);
    getRotateMatZ(phiZ);
    getRotateMatY(phiY);
}

void engine::getRotateMatX(float theta)
{
    rotateMatX.m[0][0] = 1.0f;
    rotateMatX.m[1][1] = cosf(theta);
    rotateMatX.m[1][2] = sinf(theta);
    rotateMatX.m[2][1] = -sinf(theta);
    rotateMatX.m[2][2] = cosf(theta);
    rotateMatX.m[3][3] = 1.0f;
}

void engine::getRotateMatZ(float theta)
{
    rotateMatZ.m[0][0] = cosf(theta);
    rotateMatZ.m[0][1] = sinf(theta);
    rotateMatZ.m[1][0] = -sinf(theta);
    rotateMatZ.m[1][1] = cosf(theta);
    rotateMatZ.m[2][2] = 1.0f;
    rotateMatZ.m[3][3] = 1.0f;
}

void engine::getRotateMatY(float theta)
{
    rotateMatY.m[0][0] = cosf(theta);
    rotateMatY.m[0][2] = sinf(theta);
    rotateMatY.m[2][0] = -sinf(theta);
    rotateMatY.m[1][1] = 1.0f;
    rotateMatY.m[2][2] = cosf(theta);
    rotateMatY.m[3][3] = 1.0f;
}

void engine::drawVertex(sf::RenderWindow *window, point3d &pt, float &illumination)
{
    sf::CircleShape circle(5.0f);
    circle.setFillColor(sf::Color(0,0,std::max(95.0f, illumination * 255.0f)));
    circle.setPosition(pt.x, pt.y);
    circle.setOrigin(5.0f, 5.0f);
    window->draw(circle);
}

void engine::drawTri(sf::RenderWindow *window, triangle &tris)
{
    sf::VertexArray triangleStrip(sf::TrianglesStrip, 3);

    for(int i = 0; i < 3; ++i)
    {
        triangleStrip[i].position = sf::Vector2f(tris.pt[i].x, tris.pt[i].y);
        
        triangleStrip[i].color = sf::Color(0, 0, std::max(95.0f,255.0f * tris.illumination));
    }
    window->draw(triangleStrip);

    sf::Vertex line[2];
    // Draw lines between the three points
    // for (int i = 0; i < 2; ++i)
    // {
    //     for (int j = i+1; j < 3; ++j)
    //     {
    //         line[0].position = sf::Vector2f(tris.pt[i].x, tris.pt[i].y);
    //         line[0].color  = sf::Color::Red;
    //         line[1].position = sf::Vector2f(tris.pt[j].x, tris.pt[j].y);
    //         line[1].color  = sf::Color::Red;

    //         drawVertex(window, tris.pt[j], tris.illumination);
    //         drawVertex(window, tris.pt[i], tris.illumination);
    //         // window->draw(line, 2, sf::Lines);
            
    //     }
    // }

}

float engine::eucledianDistance(point3d p1, point3d p2)
{
    float dist_x = (p1.x - p2.x)*(p1.x - p2.x);
    float dist_y = (p1.y - p2.y)*(p1.y - p2.y);
    float dist_z = (p1.z - p2.z)*(p1.z - p2.z);

    return sqrtf(dist_x + dist_y + dist_z);
}