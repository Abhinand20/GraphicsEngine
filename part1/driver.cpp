/**
 * @file driver.cpp
 * 
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-15
 * 
 * @copyright Copyright (c) 2022
 * 
 * 
 * TODO: 1) Get filename from CLI
 *       2) Fix resize issue (Should work on all sizes)
 *       3) Try to make the rotation continous (currently resets on every click)
 *      
 */

#include "mesh.hpp"
#include "engine.hpp"
#include "window.hpp"
#include <iostream>
#include <string>

const unsigned int WINDOW_W = 800;
const unsigned int WINDOW_H = 800;
const char* title = "Neocis Challenge";



int main(int argc, char* argv[]){
    std::string objPath = "./obj/object.txt";
    window currWindow;
    engine *engineObj = new engine(objPath);

    currWindow.start(WINDOW_W, WINDOW_H, title, engineObj);
    
    delete engineObj;
    
    return 0;
}