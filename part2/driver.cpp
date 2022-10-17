/**
 * @file driver.cpp
 * 
 * @author Abhinand Jha (abhinandj@andrew.cmu.edu)
 * @brief  Driver file for the 3D engine, takes in the object file from
 *         the command line and passes it to the engine for rendering
 *        
 *         Object file is assumed to have "clock-wise winding order" for
 *         the triangle vertices
 * 
 * @version 0.1
 * @date 2022-10-15
 * 
 */

#include "include/mesh.hpp"
#include "include/engine.hpp"
#include "include/window.hpp"
#include <iostream>
#include <string>
#include <getopt.h>

const unsigned int WINDOW_W = 800;
const unsigned int WINDOW_H = 800;
const char* title = "Neocis Challenge! - Abhinand";

int main(int argc, char* argv[]){
    std::string objPath;
    int opt;
    bool fFlag;

    while ((opt = getopt(argc, argv, "f:")) != -1)
    {
        switch (opt)
        {
        case ('f'):
            objPath = optarg;
            fFlag = true;
            break;

        default:
            fprintf(
                stderr,
                "Usage: %s [-f file] \n",
                argv[0]);
            return 0;
        }
    }

    if (fFlag == false)
    {
        fprintf(
                stderr,
                "Usage: %s [-f file] \n",
                argv[0]);
        return 0;
    }

    engine *engineObj = new engine(objPath);
    if(engineObj->success == false)
    {   
        std::cout<<"Please validate input file!"<<std::endl;
        std::cout<<"Exiting...";
        return 0;
    }

    window currWindow;
    currWindow.start(WINDOW_W, WINDOW_H, title, engineObj);

    delete engineObj;
    
    return 0;
}