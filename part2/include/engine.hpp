/**
 * @file engine.hpp
 * @author Abhinand Jha (abhinanj@andrew.cmu.edu)
 * @brief A 3D engine that takes an object represented as
 *        vertices and triangles, and projects them onto 
 *        an interactive 2D window
 * 
 * @version 0.1
 * @date 2022-10-15
 * 
 * 
 */
#pragma once

#include "mesh.hpp"
#include <string>
#include <SFML/Graphics.hpp>
#include <vector>

# define PI 3.14159265358979323846  /* pi */

/** @brief data structers and methods required for the engine **/
class engine 
{
    public:
        mesh meshObj;
        point3d center;
        bool success;
        
        engine(std::string filePath);

        void drawOnWindow(sf::RenderWindow *window);
        void getProjMat(float fovDegrees, float aspectRatio, float fNear, float fFar);
        float getDistanceToPoint(point3d pt);
        void rotate(float phiX, float phiY, float phiZ = 0.0f);

    private:
        mat4x4 projMat, rotateMatX, rotateMatZ, rotateMatY;
        point3d cameraPos;

        void matVecMult(point3d &in, mat4x4 &mat, point3d &out);
        point3d getNormal(triangle tri);
        point3d vecCrossProduct(point3d &v1, point3d &v2);
        void getRotatationMat(float phiX, float phiY, float phiZ = 0.0f);
        void getRotateMatX(float theta);
        void getRotateMatZ(float theta);
        void getRotateMatY(float theta);
        mat4x4 getIdentityMat();
        mat4x4 matMul(mat4x4 &m1, mat4x4 &m2);
        mat4x4 getTranslationMat(float x, float y, float z);
        void drawVertex(sf::RenderWindow *window, point3d &pt, float &illumination);
        void drawTri(sf::RenderWindow *window, triangle &tris);
        float eucledianDistance(point3d p1, point3d p2);
        float vecDotProduct(point3d &v1, point3d &v2);

};