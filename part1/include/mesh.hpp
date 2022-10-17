/**
 * @file mesh.hpp
 * @author Abhinand Jha (abhinanj@andrew.cmu.edu)
 * @brief  Mesh class, stores the vertices and faces of the object
 *         loads the object from a file and efines functions to 
 *         operate on the vertices 
 * @version 0.1
 * @date 2022-10-15
 * 
 * 
 */
#pragma once

#include <vector>

struct point3d 
{

    float x, y, z;
    
    point3d();
    point3d(float _x, float _y, float _z);

    point3d operator +(point3d pt);
    point3d operator -(point3d pt);
    void operator *=(float val);
    void operator +=(float val);
    void operator /=(float val);
    void pointPrint();
        
};

// Collection of 3 points
struct triangle 
{
    point3d pt[3] = { point3d() };
    float illumination = 0.0f;
};


struct mat4x4
{
    float m[4][4] = { 0 };
};

// Collection of triangles
struct mesh 
{
    std::vector<triangle> tris;
    std::vector<point3d> allVertices;
    unsigned int numVertices;
    unsigned int numTriangles;

    bool readVertices(std::string filename);
};