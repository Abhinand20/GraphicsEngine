/**
 * @file window.hpp
 * @author Abhinand Jha (abhinanj@andrew.cmu.edu)
 * @brief  Interface between the object and the user, handles user input and
 * 		   passes it to the engine for rendering
 * @version 0.1
 * @date 2022-10-15
 * 
 * 
 */
#pragma once

#include <SFML/Graphics.hpp>
#include "engine.hpp"

class window {

	public:
		sf::RenderWindow* currWindow;
		sf::View view;
		void start(unsigned int Width, unsigned int Height, const char* Title, engine* engineObj);
		window();

	private:
		engine* currEngine;
		float posz;
		sf::Vector2i mouseStart;
		sf::Vector2i mouseEnd;
		sf::Mouse mouse;
		bool buttonPressed;

		void update();
		void close();
		void runLoop();
		void translateToRotation(sf::Mouse &mouse, engine* currEngine);

};