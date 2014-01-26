//
//  gldrawview.h
//  TinyCNC
//
//  Created by Gaster, Benedict on 1/23/14.
//  Copyright (c) 2014 Gaster, Benedict. All rights reserved.
//

#include <Cocoa/Cocoa.h>

#include "gl_renderer.hpp"

@interface GLDrawView : NSOpenGLView
{
    // system timer, needed to synchronize the frame rate
    NSTimer * render_timer;

    // the C++ renderer, as no need for addtional Objective-C
    gl_renderer glrenderer;
}

- (void) drawRect: (NSRect)rect;

@end

