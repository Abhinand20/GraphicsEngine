/**
 * @file engine.cpp 
 * @author Abhinand Jha (abhinanj@andrew.cmu.edu)
 * @brief An interactive 3D engine that takes an object represented as 
 *        points and triangles and projects them onto a 2D window
 *        Object on the screen will rotate as per the user's mouse movement
 * 
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
    cameraPos = point3d{0,0,0};
}
/** @brief Translate and scale triangles, project them into 2D and render them 
 *         - Triangles not currently visible are not rendered
 *         - All visible faces are shaded according to their orientation w.r.t the observer
 *         - Orientation is calculated by casting a light ray into the direction of the observer
 *           and taking the dot product between the ray and surface normal
 * 
**/
void engine::drawOnWindow(sf::RenderWindow *window)
{
    std::vector<triangle> trianglesToRender;
    mat4x4 transMat, worldMat;

    center = { 0.5f * (float)window->getSize().x, 0.5f * (float)window->getSize().y, 0.0f};
    transMat = getTranslationMat(0.0f, 0.0f, 3.0f);
    worldMat = getIdentityMat();
    worldMat = matMul(rotateMatY, rotateMatX);
    worldMat = matMul(worldMat, transMat);

    for(auto tri : meshObj.tris)
    {
        triangle triProjected, triTransformed;
        point3d normal;
        
        // Transform each triangle
        for(int i = 0; i < 3; ++i)
        {   
            matVecMult(tri.pt[i], worldMat, triTransformed.pt[i]);
        }

        // Only render triangles that are facing the camera
        normal = getNormal(triTransformed);

        // Ray from camera to triangle
        point3d cameraRay = triTransformed.pt[0] - cameraPos;

        // If the dot product of the normal and camera ray is negative
        // the triangle is facing the camera
        if(vecDotProduct(normal, cameraRay) > 0.0f)
        {
            continue;
        }

        // Project triangles from 3D to 2D
        for(int i = 0; i < 3; ++i)
        {

            // Illumination
            point3d lightDirection = {0.0f, 0.0f, 1.0f};
            float normlize = eucledianDistance(point3d{0.0f,0.0f,0.0f}, lightDirection);
            lightDirection /= normlize;

            float dp = std::max(0.1f, vecDotProduct(normal, lightDirection));

            matVecMult(triTransformed.pt[i], projMat, triProjected.pt[i]);

            // Scale into view
            triProjected.pt[i].x += 1.0f;
            triProjected.pt[i].y += 1.0f;
            triProjected.pt[i].x *= 0.5f * (float)(*window).getSize().x;
            triProjected.pt[i].y *= 0.5f * (float)(*window).getSize().y;
            triProjected.illumination = dp; // value between 0.1 and 1.0
            
        }
                        
        trianglesToRender.push_back(triProjected);

        // Painter's algorithm
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
}

void engine::getProjMat(float fovDegrees, float aspectRatio, float fNear, float fFar)
{
    float fovRad;
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
    unsigned int minDist = INT_MAX, dist;
    size_t minIdx;
    for(size_t i = 0; i < meshObj.numVertices; ++i)
    {
        dist = eucledianDistance(meshObj.allVertices[i], pt);
        if(dist < minDist)
        {
            minDist = dist;
            minIdx = i;
        }
    }

    return eucledianDistance(meshObj.allVertices[minIdx], pt);
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
    getRotateMatY(phiY);
    getRotateMatZ(phiZ);
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

mat4x4 engine::getIdentityMat()
{
    mat4x4 mat;
    mat.m[0][0] = 1.0f;
    mat.m[1][1] = 1.0f;
    mat.m[2][2] = 1.0f;
    mat.m[3][3] = 1.0f;
    return mat;
}

mat4x4 engine::getTranslationMat(float x, float y, float z)
{
    mat4x4 mat;
    mat.m[0][0] = 1.0f;
    mat.m[1][1] = 1.0f;
    mat.m[2][2] = 1.0f;
    mat.m[3][3] = 1.0f;
    mat.m[3][0] = x;
    mat.m[3][1] = y;
    mat.m[3][2] = z;
    return mat;
}

mat4x4 engine::matMul(mat4x4 &mat1, mat4x4 &mat2)
{  
    mat4x4 out;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            float sum = 0;
            for (int k = 0; k < 4; k++)
            {
                sum += mat1.m[i][k] * mat2.m[k][j];
            }
            out.m[i][j] = sum;
        }
    }
    return out;
}

float engine::eucledianDistance(point3d p1, point3d p2)
{
    float dist_x = (p1.x - p2.x)*(p1.x - p2.x);
    float dist_y = (p1.y - p2.y)*(p1.y - p2.y);
    float dist_z = (p1.z - p2.z)*(p1.z - p2.z);

    return sqrtf(dist_x + dist_y + dist_z);
}