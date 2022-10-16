/**
 * @file window.cpp
 * @author Abhinand Jha (abhinanj@andrew.cmu.edu)
 * @brief 
 * @version 0.1
 * @date 2022-10-16
 * 
 * 
 */

#include <SFML/Graphics.hpp>
#include <iostream>
#include "include/mesh.hpp"
#include "include/engine.hpp"
#include "include/window.hpp"
#include <math.h>

window::window()
{
    currWindow = nullptr;
    posz = 0;
    buttonPressed = false;
}

void window::start(unsigned int Width, unsigned int Height, const char* Title, engine* engineObj)
	{

		currWindow = new sf::RenderWindow(sf::VideoMode(Width, Height), Title);
		currEngine = engineObj;

		float fNear = 0.1f;
		float fFar = 1000.0f;
		float fov = 90.0f;
		// currEngine->getProjMat(fov, (float)Height / (float)Width, fNear, fFar);
		currEngine->getProjMat(fov, (float)Width / (float)Height, fNear, fFar);
		currEngine->center = { 0.5f * (float)currWindow->getSize().x, 0.5f * (float)currWindow->getSize().y, 0.0f};
		currEngine->rotate(0.0f, 0.0f, 0.0f);
		
		runLoop();

	}

void window::update()
{
    currWindow->clear(sf::Color::White);
    currEngine->drawOnWindow(currWindow);
    currWindow->display();
}

void window::close(){

    delete currWindow;
}

void window::runLoop()
{
    sf::Event event;
    while (currWindow->isOpen())
    {
        while (currWindow->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                currWindow->close();
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                mouseStart = mouse.getPosition(*currWindow);
                buttonPressed = true;
            }

            if (event.type == sf::Event::MouseButtonReleased)
            {
                translateToRotation(mouse, currEngine);
                buttonPressed = false;
                mouseStart = mouseEnd;
            }
            // Get the closest point to the cursor and its rotation angle
            if (buttonPressed == true)
            {
                translateToRotation(mouse, currEngine);
            }

        }
        update();
    }
    close();
}

void window::translateToRotation(sf::Mouse &mouse, engine* currEngine)
{
    mouseEnd = mouse.getPosition(*currWindow);
    float dx = mouseEnd.x - mouseStart.x;
    float dy = mouseEnd.y - mouseStart.y;
    float distPt = currEngine->getDistanceToPoint(point3d{(float)mouseStart.x, (float)mouseStart.y, posz});
    float phiX = atan2f(dx, distPt);
    float phiY = atan2f(dy, distPt);

    
    currEngine->rotate(-phiY, phiX, 0.0f);
    currEngine->drawOnWindow(currWindow);
}