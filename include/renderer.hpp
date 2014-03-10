/*
 * File: renderer.hpp
 * Author: Benedict R. Gaster
 * Desc: Base renderering interface
 *
 *  1in = 25.4mm
 *  3 x 25.4mm = 76.2 mm
 *  2 x 25.4mm = 50.8mm
 *
 *  We scale by four to make the window a little bigger
 *
 *    304.8mm x 203.2mm
 *
 *
 * Copyright 2014 Benedict R. Gaster
 * License: See the file license.
 */

#pragma once

#include <memory>
#include <iostream>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <OpenGL/gl.h>

#include "arduino-serial.hpp"
#include "gcode.hpp"

struct point
{
    float x_;
    float y_;
    
    point()
    {
    }
    
    point(float x, float y) :
        x_(x), y_(y)
    {
    }
    
    point(const point& rhs)
    {
        *this = rhs;
    }
    
    point& operator= (const point& rhs)
    {
        if (this == &rhs) {
            return *this;
        }
        
        x_ = rhs.x_;
        y_ = rhs.y_;
        
        return *this;
    }
};

inline point make_point(float x, float y)
{
    return point(x,y);
}

class renderer
{
private:
    std::shared_ptr<gcode> gcode_;
    bool renderer_;
public:
    renderer(std::shared_ptr<gcode> gcode, bool r) :
        gcode_(gcode),
        renderer_(r)
    {
    }
    
    renderer(bool r = true) :
        gcode_(nullptr),
        renderer_(r)
    {}

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
        renderer_ = rhs.renderer_;
        
        return *this;
    }   

    std::shared_ptr<gcode> get() const
    {
        return gcode_;
    }
    
    void set_renderer(bool r)
    {
        renderer_ = r;
    }
    
    bool get_renderer() const
    {
        return renderer_;
    }
    
    void add_gcode(std::shared_ptr<gcode> gc)
    {
        gcode_ = gc;
    }
    
    virtual void init()
    {
    }
    
    virtual void update()
    {
    }

    virtual void draw() = 0;
};

// Basic text renderer which simply output gcode to stdout...

class text_renderer : public renderer
{
private:
public:
    text_renderer(std::shared_ptr<gcode> gc) :
	renderer(gc, true)
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

class bot_renderer : public renderer
{
private:
    bool pen_down_; // track if pen is up or down
     arduino::serial serial_;
    
public:
    bot_renderer(std::string serialport, int baud) :
        renderer(nullptr, true),
        pen_down_(false),
        serial_{serialport.c_str(), baud}
    {
    }
    
    void draw()
    {
        std::shared_ptr<gcode> gc = get();
        
        point position{0.f, 0.f}; //
        
        for_each(gc->begin(), gc->end(), [this, &position] (std::shared_ptr<gcode_entry> code) {
            if (code->is_type(ID_TYPE::MCODE))
            {
                // process pen up and pen down commands
                if(code->is_value(gcode::pen_) &&
                   (code->get_argument(0) == gcode::pen_down_)) {
                    serial_.write("pen_down\n");
                }
                else if(code->is_value(gcode::pen_) &&
                   (code->get_argument(0) == gcode::pen_up_)) {
                    serial_.write("pen_up\n");
                }
            }
            else if (code->is_type(ID_TYPE::GCODE)) {
                // process GCODES G92 and G1
                if (code->is_value(gcode::move_to_)) {
                    position = make_point(code->get_argument(0), code->get_argument(1));
                }
                else if (code->is_value(gcode::draw_)) {
                    point p(make_point(code->get_argument(0), code->get_argument(1)));
                    
                    char str[1024];
                    sprintf(str, "draw_line %f %f %f %f\n", position.x_, position.y_, p.x_, p.y_);
                    serial_.write(str);
                    std::string tmp;
                    serial_.read_until(tmp, '\n');
                    printf("msg: %s\n", tmp.c_str());
                    position = p;
                }
            }

	    });
        
    }
};


class ogl_renderer : public renderer
{
private:
    bool pen_down_; // track if pen is up or down
    
    constexpr static const float frame_width_  = 304.f;
    constexpr static const float frame_height_ = 202.f;
    
    void clear(float r=0,
               float g=0,
               float b=0,
               float a=1,
               bool depth=true)
    {
        glClearColor(r, g, b, a);
        if (depth) {
            glClear(GL_COLOR_BUFFER_BIT);
        }
    }
    
    point normalize(point p)
    {
        static float inv_frame_height = 1.f / frame_height_;
        static float inv_frame_width = 1.f / frame_width_;
#if 0
        return point(
            p.x_ * 4 * inv_frame_width,
            p.y_ * 4 * inv_frame_height);
#else
        return point(
                     p.x_,
                     p.y_);
#endif
    }

public:
    ogl_renderer() :
        renderer(nullptr, false),
        pen_down_(false)
    {
    }
    
    void draw()
    {
        clear();
        
        if (get_renderer()) {
            glColor3f(1.0f, 0.85f, 0.35f);
        
            point position{0.f, 0.f}; // center in the draw window
        
            std::shared_ptr<gcode> gc = get();
            if (gc != nullptr) {
                for_each(gc->begin(), gc->end(), [this, &position] (std::shared_ptr<gcode_entry> code) {
                    if (code->is_type(ID_TYPE::MCODE))
                    {
                        // process pen up and pen down commands
                        if(code->is_value(gcode::pen_) &&
                           ((code->get_argument(0) == gcode::pen_down_ && !pen_down_) ||
                           (code->get_argument(0) == gcode::pen_up_ && pen_down_))) {
                            pen_down_ = !pen_down_;
                        }
                    }
                    else if (code->is_type(ID_TYPE::GCODE)) {
                        // process GCODES G92 and G1
                        if (code->is_value(gcode::move_to_)) {
                            position = make_point(code->get_argument(0), code->get_argument(1));
                        }
                        else if (code->is_value(gcode::draw_)) {
                            point p(normalize(make_point(code->get_argument(0), code->get_argument(1))));
                     
                            if (pen_down_) {
                                glLineWidth(2.5);
                                glBegin(GL_LINES);
                                glVertex3f(position.x_, position.y_, 0.f);
                                glVertex3f(p.x_, p.y_, 0.f);
                                glEnd();
                            }
                        
                            position = p;
                        }
                    }
                });
            }
        }
    }
};
    

    
