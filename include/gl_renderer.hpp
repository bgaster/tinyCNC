//
//  gl_renderer.hpp
//  TinyCNC
//
//  Created by Gaster, Benedict on 1/23/14.
//  Copyright (c) 2014 Gaster, Benedict. All rights reserved.
//

#pragma once

#include <OpenGL/gl.h>

#include <iostream>

class base_renderer
{
protected:
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
    
    void flush()
    {
        // assuming that ObjeciveC++ class handles swaping GL buffers
        //glFlush();
    }

public:
    virtual void init() = 0;
    virtual void render() = 0;
    virtual void update() = 0;
};

class gl_renderer : base_renderer
{
private:
    float shift;
    float shift_direction;
    
    void draw_triangles()
    {
        glColor3f(1.0f, 0.85f, 0.35f);
        glBegin(GL_TRIANGLES);
        glVertex3f( -1.0+shift,  1.0, 0.0);
        glVertex3f( -1.0, -1.0, 0.0);
        glVertex3f(  1.0, -1.0 ,0.0);
        glColor3f(1.0f, 0.0f, 0.35f);
        glVertex3f(  1.0-shift,  1.0, 0.0);
        glVertex3f( -1.0, -1.0, 0.0);
        glVertex3f(  1.0, -1.0 ,0.0);
        glEnd();
    }

public:
    virtual void init()
    {
        shift_direction = 1;
        shift = 0.0f;
    }
    virtual void update()
    {
#if 0
        if (shift_direction==1)
        {
            shift +=SHIFT_MOVE;
            if (shift>=1.0)
                shift_direction = 0;
        } else
        {
            shift -=SHIFT_MOVE;
            if (shift<=0.0)
                shift_direction = 1;
        }
#endif
    }
    
    virtual void render()
    {
        clear();
        draw_triangles();
        flush();
    }
};

