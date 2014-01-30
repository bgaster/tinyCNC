
#include <memory>

#import "controller.h"
#include "gcode.hpp"
#include "globals.hpp"

@implementation Controller

-(id)init
{
    self = [super init];
    if (self) {
        
    }
    return self;
}

- (IBAction)do_open:(id)sender {
    NSOpenPanel *openPanel  = [NSOpenPanel openPanel];
    NSArray *fileTypes = [NSArray arrayWithObjects:@"gcode",nil];
    NSInteger result  = [openPanel runModalForDirectory:NSHomeDirectory() file:nil types:fileTypes ];
    if(result == NSOKButton){
        
        std::string filename([[openPanel filename] UTF8String]);
        
        renderer_g->set_renderer(false);
        renderer_g->add_gcode(std::make_shared<gcode>(filename));
        
        [renderer_tinyWIN_button_ setTransparent:false];
        
         NSString* result = [NSString stringWithUTF8String:renderer_g->get()->to_string().c_str()];
        [gcode_text_view_ setString:result];
    }
} // end do_open

// send gcode to robot for paper rendering
- (IBAction)renderer_tinyCNC:(id)sender {
}

// send gcode to OpenGL framebuffer for on screen rendering
- (IBAction)renderer_tinyWIN:(id)sender {
    renderer_g->set_renderer(true);
}

// clear any gcode and stop rendering
- (IBAction)do_clear:(id)sender {
    renderer_g->set_renderer(false);
    renderer_g->add_gcode(nullptr);
    [gcode_text_view_ setString:@""];
}

@end
