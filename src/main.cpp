/*
 * File: main.cpp
 * Author: Benedict R. Gaster
 * Desc: TinyCnC Controller
 *       Currently the most basic 
 *         - loads gcode text file
 *         - parses file wih respect to TinyCnC
 *         - outputs to stdout...
 *
 * Copyright 2014 Benedict R. Gaster
 * License: See the file license.
 */
#include <iostream>
#include <memory>
#include <GLUI/glui.h>

#include <gcode.hpp>
#include <renderer.hpp>

int main_window; //store window id of main gfx window

GLUI* glui; //pointer to main glui window

void usage(std::string progname)
{
    std::cout << "usage: " 
	      << progname 
	      << " : "
	      << std::endl
	      << "\t<gcode-filename>"
	      << std::endl;
}


int main(int argc, char ** argv)
{
    try
    {
	if (argc != 2) {
	    usage(argv[0]);
	    exit(1);
	}

	std::shared_ptr<gcode> gc = std::make_shared<gcode>(argv[1]);
	std::shared_ptr<text_renderer>  r = std::make_shared<text_renderer>(gc);
	r->draw();	

	exit(0);
	
    }
    catch (error_exception e)
    {
	std::cout << "Error -- "
		  << e.what()
		  << std::endl;
	exit(1);
    }
    

    return 0;
}
