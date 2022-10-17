/**
 * @file window.cpp
 * @author Abhinand Jha (abhinanj@andrew.cmu.edu)
 * @brief  Interface between the object and the user, handles user input and
 *         passes it to the engine for rendering
 *         Tracks the cursor position and translates its movement to the object's
 *         rotation
 * @version 0.1
 * @date 2022-10-16
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

/** @brief Start up the window and set up the projection matrix **/
void window::start(unsigned int Width, unsigned int Height, const char* Title, engine* engineObj)
	{
        view = sf::View(sf::FloatRect(0, 0, Width, Height));
		currWindow = new sf::RenderWindow(sf::VideoMode(Width, Height), Title);
		currEngine = engineObj;
		float fNear = 0.1f;
		float fFar = 1000.0f;
		float fov = 90.0f;
		currEngine->getProjMat(fov, (float)Width / (float)Height, fNear, fFar);
		currEngine->center = { 0.5f * (float)currWindow->getSize().x, 0.5f * (float)currWindow->getSize().y, 0.0f};
		currEngine->rotate(0.0f, 0.0f, 0.0f);
        currWindow->setView(view);
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

/** @brief Main loop of the program, handles user input and passes it to the engine **/
void window::runLoop()
{
    sf::Event event;
    while (currWindow->isOpen())
    {
        if (event.type == sf::Event::Resized)
        {
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            currWindow->setView(sf::View(visibleArea));
            currEngine->center = { 0.5f * event.size.width, 0.5f * event.size.height, 0.0f};
            currEngine->getProjMat(90.0f, event.size.width / event.size.height, 0.1f, 1000.0f);
        }

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
            if (buttonPressed == true)
            {
                translateToRotation(mouse, currEngine);
            }

        }
        update();
    }
    close();
}

/** @brief Translate the mouse movement to rotation of the object **/
void window::translateToRotation(sf::Mouse &mouse, engine* currEngine)
{
    mouseEnd = mouse.getPosition(*currWindow);
    float dx = mouseEnd.x - mouseStart.x;
    float dy = mouseEnd.y - mouseStart.y;
    float distPt = currEngine->getDistanceToPoint(point3d{(float)mouseStart.x, (float)mouseStart.y, posz});
    float phiX = 2.0f*atan2f(dx, distPt);
    float phiY = 2.0f*atan2f(dy, distPt);

    currEngine->rotate(phiY, phiX, 0.0f);
    currEngine->drawOnWindow(currWindow);
}