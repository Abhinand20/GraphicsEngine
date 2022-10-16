/**
 * @file mesh.cpp
 * @author Abhinand Jha (abhinanj@andrew.cmu.edu)
 * @brief 
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
    // open file
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "Error opening file" << std::endl;
        return false;
    }

    // read first line
    std::string line;
    getline(file, line);

    // read comma separated values
    std::stringstream ss(line);
    std::string token;
    getline(ss, token, ',');
    numVertices = stoi(token);
    getline(ss, token, ',');
    numTriangles = stoi(token);
    
    std::vector<point3d> vertices;


    // read vertices from file
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

        // TODO: Add ID as well
        // sf::Vertex vertex(sf::Vector2f(x, y), sf::Color::Blue);
        vertices.push_back(point3d{ x, -y, -z });
    }

    // read triangles from file
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

        tris.push_back({ vertices[v1-1], vertices[v2-1], vertices[v3-1] });
    }

    return true;
}   