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

#include "include/mesh.hpp"
#include "include/engine.hpp"
#include "include/window.hpp"
#include <iostream>
#include <string>
#include <getopt.h>

const unsigned int WINDOW_W = 800;
const unsigned int WINDOW_H = 800;
const char* title = "Neocis Challenge";



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