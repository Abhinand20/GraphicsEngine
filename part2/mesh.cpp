/**
 * @file mesh.cpp
 * @author Abhinand Jha (abhinanj@andrew.cmu.edu)
 * @brief Custom vector and matrix functions for graphics engine
 * @version 0.1
 * @date 2022-10-16
 * 
 * 
 */

#include <iostream>
#include "include/mesh.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>

point3d::point3d() {}

point3d::point3d(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

point3d point3d::operator +(point3d pt) 
{ 
    return point3d(pt.x+x,pt.y+y,pt.z+z); 
}

point3d point3d::operator -(point3d pt) 
{ 
    return point3d(pt.x-x,pt.y-y,pt.z-z); 
}

void point3d::operator *=(float val)
{
    this->x = this->x * val;
    this->y = this->y * val;
    this->z = this->z * val;
}

void point3d::operator +=(float val)
{
    this->x = this->x + val;
    this->y = this->y + val;
    this->z = this->z + val;
}

void point3d::operator /=(float val)
{
    this->x = this->x / val;
    this->y = this->y / val;
    this->z = this->z / val;
}

void point3d::pointPrint()
{
    std::cout<<this->x<<" "<<" "<<this->y<<" "<<this->z<<std::endl;
}


bool mesh::readVertices(std::string filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "Error opening file" << std::endl;
        return false;
    }

    std::string line;
    getline(file, line);

    std::stringstream ss(line);
    std::string token;
    getline(ss, token, ',');
    numVertices = stoi(token);
    getline(ss, token, ',');
    numTriangles = stoi(token);

    for (int i = 0; i < numVertices; i++)
    {
        
        getline(file, line);
        std::stringstream ss(line);
        std::string token;
        getline(ss, token, ',');
        int id = stoi(token);
        getline(ss, token, ',');
        float x = stof(token);
        getline(ss, token, ',');
        float y = stof(token);
        getline(ss, token, ',');
        float z = stof(token);

        allVertices.push_back(point3d{ x, -y, -z });
    }

    for (int i = 0; i < numTriangles; i++)
    {
        getline(file, line);
        std::stringstream ss(line);
        std::string token;
        getline(ss, token, ',');
        int v1 = stoi(token);
        getline(ss, token, ',');
        int v2 = stoi(token);
        getline(ss, token, ',');
        int v3 = stoi(token);

        tris.push_back({ allVertices[v1-1], allVertices[v2-1], allVertices[v3-1] });
    }

    return true;
}   