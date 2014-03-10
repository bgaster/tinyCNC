//
//  controller.h
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
    IBOutlet NSButton * renderer_tinyCNC_button_;
    IBOutlet NSButton * renderer_tinyWIN_button_;
    IBOutlet NSButton * renderer_clear_button_;
}

- (void)clear;
- (IBAction)progress_local_command:(id)sender;

- (IBAction)do_open:(id)sender;
- (IBAction)renderer_tinyCNC:(id)sender;
- (IBAction)renderer_tinyWIN:(id)sender;
- (IBAction)do_clear:(id)sender;
- (IBAction)do_change_view_gcode:(id)sender;
- (IBAction)do_change_view_logo:(id)sender;

@end
