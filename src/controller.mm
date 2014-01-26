
#include <memory>

#import "controller.h"
#include "gcode.hpp"
#include "globals.hpp"

@implementation Controller

- (IBAction)do_open:(id)sender {
    NSOpenPanel *openPanel  = [NSOpenPanel openPanel];
    NSArray *fileTypes = [NSArray arrayWithObjects:@"gcode",nil];
    NSInteger result  = [openPanel runModalForDirectory:NSHomeDirectory() file:nil types:fileTypes ];
    if(result == NSOKButton){
        
        std::string filename([[openPanel filename] UTF8String]);
        renderer_g->add_gcode(std::make_shared<gcode>(filename));
        
         NSString* result = [NSString stringWithUTF8String:renderer_g->get()->to_string().c_str()];
        [gcode_text_view_ setString:result];
    }
} // end do_open

@end
