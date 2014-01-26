//
//  gldrawview.h
//  TinyCNC
//
//  Created by Gaster, Benedict on 1/23/14.
//  Copyright (c) 2014 Gaster, Benedict. All rights reserved.
//

#include <Cocoa/Cocoa.h>

#include "gcode.hpp"

@interface Controller : NSObject
{
    IBOutlet NSTextView * gcode_text_view_;
}

- (IBAction)do_open:(id)sender;

@end
