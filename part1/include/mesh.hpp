#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>
#include <algorithm>

# define PI 3.14159265358979323846  /* pi */

/*  Classes for handling data */
// {x,y,z,1} - convert to array later
class point3d 
{
    public:
        float x, y, z;
        point3d() {}
        point3d(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

        void operator *=(float val)
        {
            this->x = this->x * val;
            this->y = this->y * val;
            this->z = this->z * val;
        }

        void operator +=(float val)
        {
            this->x = this->x + val;
            this->y = this->y + val;
            this->z = this->z + val;
        }

        void operator /=(float val)
        {
            this->x = this->x / val;
            this->y = this->y / val;
            this->z = this->z / val;
        }

        void pointPrint()
        {
            std::cout<<this->x<<" "<<" "<<this->y<<" "<<this->z<<std::endl;
        }
        
};

// Collection of 3 points
class triangle 
{
    public:
        point3d pt[3] = { point3d() };
};


struct mat4x4
{
    float m[4][4] = { 0 };
};



// Collection of triangles
class mesh 
{
    public:
        std::vector<triangle> tris;
        unsigned int numVertices;
        unsigned int numTriangles;

        void readVertices(std::string filename)
        {
            // open file
            std::ifstream file(filename);
            if (!file.is_open())
            {
                std::cout << "Error opening file" << std::endl;
                return;
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
                vertices.push_back(point3d{ x, -y, z });
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

        }
};