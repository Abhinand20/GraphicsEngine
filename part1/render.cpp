/**
 * 
 * PART 1.1 : Get 3d points - transform them for the screen and depth
 * Project 3d points onto 2d plane
 * Draw lines for the planes
 * 
 * PART 1.2 : Add mouse drag and rotate functionality
 *  - Hold down left mouse button and drag to rotate
 *  - Horizontal movement rotates the object around y-axis
 *  - Vertical movement rotates the object around x-axis
 *  - Diagonal movement is broken down into horizontal and vertical and applied
 *  - Point nearest to observer follows the mouse cursor
 * 
 * @file render.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>
#include <algorithm>

# define PI 3.14159265358979323846  /* pi */

// For rotation, calculate phi as the relative angle between the mouse and the center of the screen
// Use phi to calculate the rotation matrix
// Find the nearest point to the mouse, make it follow the mouse



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

// Collection of triangles
class mesh 
{
    public:
        std::vector<triangle> tris;
        
        void readVertices(std::string filename)
        {
            // open file
            std::ifstream file(filename);
            if (!file.is_open())
            {
                std::cerr << "Error opening file" << std::endl;
                return;
            }

            // read first line
            std::string line;
            getline(file, line);

            int numVertices, numTriangles;

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


struct mat4x4
{
    float m[4][4] = { 0 };

};

// data structers and methods required for our engine
class engine 
{
    public:

        mat4x4 projMat, rotateMatX, rotateMatZ, rotateMatY;
        mesh meshObj;

        engine(std::string filePath)
        {
            meshObj.readVertices(filePath);        
        }

        void drawOnWindow(sf::RenderWindow &window, sf::Clock &clk)
        {
            // Draw triangles
            int currentTriangle = 0;
            for(auto tri : meshObj.tris)
            {
                triangle triProjected, triTranslated, triRotatedZ, triRotatedZX, triRotatedY;
                rotateTheta += 1.0f * (float)clk.getElapsedTime().asSeconds() / 100.0f;
                getRotatationMat(rotateTheta);

                // std::cout<<"Triangle "<<currentTriangle<<std::endl;

                for(int i = 0; i < 3; ++i)
                {   
                    matVecMult(tri.pt[i], rotateMatZ, triRotatedZ.pt[i]);
                    // matVecMult(triRotatedZ.pt[i], rotateMatX, triRotatedZX.pt[i]);
                    matVecMult(tri.pt[i], rotateMatX, triRotatedZX.pt[i]);
                    // matVecMult(tri.pt[i], rotateMatY, triRotatedZX.pt[i]);

                    triTranslated = triRotatedZX;
                    triTranslated.pt[i].z = triRotatedZX.pt[i].z + 3.0f;

                    matVecMult(triTranslated.pt[i], projMat, triProjected.pt[i]);

                    // Scale into view
                    triProjected.pt[i].x += 1.0f;
                    triProjected.pt[i].y += 1.0f;
                    triProjected.pt[i].x *= 0.5f * (float)window.getSize().x;
                    triProjected.pt[i].y *= 0.5f * (float)window.getSize().y;
                    
                    // std::cout<<"Raw Point "<<i<<": ";
                    // std::cout<<tri.pt[i].x<<" "<<tri.pt[i].y<<" "<<tri.pt[i].z<<std::endl;
                    
                    // std::cout<<"Point Proj "<<i<<": ";
                    // std::cout<<triProjected.pt[i].x<<" "<<triProjected.pt[i].y<<" "<<triProjected.pt[i].z<<std::endl;
                    
                    // std::cout<<"Mult"<<std::endl;
                    // std::cout<<triProjected.pt[i].x<<" "<<triProjected.pt[i].y<<" "<<triProjected.pt[i].z<<std::endl;
                }

                drawTri(window, triProjected);
                ++currentTriangle;

            }

            clk.restart();
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

        void getRotatationMat(float phiX, float phiY, float phiZ = 0.0f)
        {
            getRotateMatX(theta);
            getRotateMatZ(theta);
            getRotateMatY(theta);
        }

    private:
        float rotateTheta;

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

        void getRotateMatX(float theta)
        {
            rotateMatX.m[0][0] = 1.0f;
            rotateMatX.m[1][1] = cosf(theta * 0.5f);
            rotateMatX.m[1][2] = sinf(theta * 0.5f);
            rotateMatX.m[2][1] = -sinf(theta * 05.f);
            rotateMatX.m[2][2] = cosf(theta * 0.5f);
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

        void drawVertex(sf::RenderWindow &window, point3d &pt)
        {
            sf::CircleShape circle(5.0f);
            circle.setFillColor(sf::Color::Blue);
            circle.setPosition(pt.x, pt.y);
            circle.setOrigin(5.0f, 5.0f);
            window.draw(circle);
        }

        void drawTri(sf::RenderWindow &window, triangle &tris)
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
                    window.draw(line, 2, sf::Lines);
                    
                }
            }

        }

        
};


int main(){
    sf::RenderWindow window(sf::VideoMode(800, 800), "SFML test!");
    sf::Vector2i initMouse;
    sf::Vector2i endMouse;
    sf::Mouse mouse;


    engine engineObj("./object.txt");
    // TODO: Make params modular
    sf::Vector2u size = window.getSize();
    unsigned int width = size.x;
    unsigned int height = size.y;
    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fov = 90.0f;
    // std::cout<<height<<" "<<width<<std::endl;
    engineObj.getProjMat(fov, (float)height / (float)width, fNear, fFar);

    for(int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
            std::cout<<engineObj.projMat.m[i][j]<<" ";
        std::cout<<std::endl;
    }

    std::cout<<engineObj.meshObj.tris.size();
    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        engineObj.drawOnWindow(window, clock);

        window.display();
    }
    return 0;
    }