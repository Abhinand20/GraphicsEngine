/**
 * @file engine.hpp
 * @author Abhinand Jha (abhinanj@andrew.cmu.edu)
 * @brief 
 * @version 0.1
 * @date 2022-10-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include "mesh.hpp"
#include <string>
#include <SFML/Graphics.hpp>
#include <vector>

# define PI 3.14159265358979323846  /* pi */

// data structers and methods required for our engine
class engine 
{
    public:
        mat4x4 projMat, rotateMatX, rotateMatZ, rotateMatY;
        mesh meshObj;
        point3d center;
        std::vector<point3d> allPoints;
        point3d cameraPos;
        bool success;

        engine(std::string filePath);

        void drawOnWindow(sf::RenderWindow *window);

        void getProjMat(float fovDegrees, float aspectRatio, float fNear, float fFar);

        float vecDotProduct(point3d &v1, point3d &v2);

        float getDistanceToPoint(point3d pt);

        void rotate(float phiX, float phiY, float phiZ = 0.0f);

    private:
        void matVecMult(point3d &in, mat4x4 &mat, point3d &out);

        point3d getNormal(triangle tri);

        point3d vecCrossProduct(point3d &v1, point3d &v2);

        void getRotatationMat(float phiX, float phiY, float phiZ = 0.0f);

        void getRotateMatX(float theta);

        void getRotateMatZ(float theta);

        void getRotateMatY(float theta);

        void drawVertex(sf::RenderWindow *window, point3d &pt, float &illumination);

        void drawTri(sf::RenderWindow *window, triangle &tris);

        float eucledianDistance(point3d p1, point3d p2);
};