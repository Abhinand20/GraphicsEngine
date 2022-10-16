#pragma once

#include <SFML/Graphics.hpp>
#include "mesh.hpp"
#include "engine.hpp"

class window {

	sf::RenderWindow* window = nullptr;
	engine* currEngine;
	float posz = 0;
	sf::Vector2i mouseStart;
	sf::Vector2i mouseEnd;
	sf::Mouse mouse;

	void update()
	{
		window->clear(sf::Color::White);
		currEngine->drawOnWindow(window);
		window->display();
	}

	void close(){

		delete window;
	}

	bool buttonPressed = false;

	void runLoop()
	{
		sf::Event event;
		while (window->isOpen())
		{
			while (window->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					window->close();
				}

				if (event.type == sf::Event::MouseButtonPressed)
				{
					mouseStart = mouse.getPosition(*window);
				    buttonPressed = true;
				}

				if (event.type == sf::Event::MouseButtonReleased)
				{

					mouseEnd = mouse.getPosition(*window);
					float dx = mouseEnd.x - mouseStart.x;
					float dy = mouseEnd.y - mouseStart.y;
					float distPt = currEngine->getDistanceToPoint(point3d{(float)mouseStart.x, (float)mouseStart.y, posz});
					std::cout<<"Released : "<<distPt<<std::endl;

					float alfaX = atan2f(dx, distPt);
					float alfaY = atan2f(dy, distPt);

					
					currEngine->rotate(-alfaY, alfaX, 0.0f);
					std::cout<<"Pressed : "<<alfaX<<" "<<dx<<" "<<std::endl;

					// cube->draw(window->getSize().x / 2, window->getSize().y / 2, posz);
					currEngine->drawOnWindow(window);
					buttonPressed = false;
					mouseStart = mouseEnd;


				}
				// Get the closest point to the cursor and its rotation angle
				if (buttonPressed == true)
				{
					mouseEnd = mouse.getPosition(*window);
					float dx = mouseEnd.x - mouseStart.x;
					float dy = mouseEnd.y - mouseStart.y;
					float distPt = currEngine->getDistanceToPoint(point3d{(float)mouseStart.x, (float)mouseStart.y, posz});
					float alfaX = atan2f(dx, distPt);
					float alfaY = atan2f(dy, distPt);
					std::cout<<"Pressed : "<<alfaX<<" "<<dx<<" "<<alfaY<<" "<<dy<<std::endl;
					
					currEngine->rotate(-alfaY, alfaX, 0.0f);
					currEngine->drawOnWindow(window);

				}

			}
			update();
		}
		close();
	}


public :

	
	void start(unsigned int Width, unsigned int Height, const char* Title, engine* engineObj)
	{

		window = new sf::RenderWindow(sf::VideoMode(Width, Height), Title);
		currEngine = engineObj;

		float fNear = 0.1f;
		float fFar = 1000.0f;
		float fov = 90.0f;
		currEngine->getProjMat(fov, (float)Height / (float)Width, fNear, fFar);
		currEngine->center = { 0.5f * (float)window->getSize().x, 0.5f * (float)window->getSize().y, 0.0f};
		currEngine->rotate(0.0f, 0.0f, 15.0f);
		
		runLoop();

	}

};