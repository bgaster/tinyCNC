/*
 * File: renderer.hpp
 * Author: Benedict R. Gaster
 * Desc: Base renderering interface
 *
 * Copyright 2014 Benedict R. Gaster
 * License: See the file license.
 */

#pragma once

#include <memory>
#include <iostream>
#include <algorithm>

#include <gcode.hpp>

class renderer
{
private:
    std::shared_ptr<gcode> gcode_;
public:
    renderer(std::shared_ptr<gcode> gcode) :
	gcode_(gcode)
    {
    }

   ~renderer()
    {
    }

    renderer(const renderer& rhs)
    {
	*this = rhs;
    }

    renderer& operator== (const renderer & rhs)
    {
	if (this == &rhs) {
	    return *this;
	}
    
	// shallow copy of codes_
	gcode_ = rhs.gcode_;

	return *this;
    }   

    std::shared_ptr<gcode> get()
    {
	return gcode_;
    }

    virtual void draw() = 0;
};

// Basic text renderer which simply output gcode to stdout...

class text_renderer : public renderer
{
private:
public:
    text_renderer(std::shared_ptr<gcode> gc) :
	renderer(gc)
    {
    }
    

    void draw()
    {
	std::shared_ptr<gcode> gc = get();
	
	for_each(gc->begin(), gc->end(), [this] (std::shared_ptr<gcode_entry> code) {
		code->print();
	    });
	
    }
};

    

    
