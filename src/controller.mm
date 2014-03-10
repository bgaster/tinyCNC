
#include <memory>

#import "controller.h"
#include "gcode.hpp"
#include "globals.hpp"
#include "arduino-serial.hpp"

@implementation Controller

-(id)init
{
    self = [super init];
    
    if (self) {
        // Allocate our Bot renender
        renderer_bot_g = std::make_shared<bot_renderer>("/dev/cu.usbmodem1411", B57600); //TODO: move to -DSERIAL_PORT -DBAUD
    }
    return self;
}

- (void)clear {
    renderer_g->set_renderer(false);
    renderer_g->add_gcode(nullptr);
    
    renderer_bot_g->set_renderer(false);
    renderer_bot_g->add_gcode(nullptr);
    
    [gcode_text_view_ setString:@""];
}

- (IBAction)do_open:(id)sender {
    NSOpenPanel *openPanel  = [NSOpenPanel openPanel];
    NSArray *fileTypes = [NSArray arrayWithObjects:@"gcode",nil];
    NSInteger result  = [openPanel runModalForDirectory:NSHomeDirectory() file:nil types:fileTypes ];
    if(result == NSOKButton){
        
        std::string filename([[openPanel filename] UTF8String]);
        
        [self clear];
        gcode_view_g = true;
        renderer_g->set_renderer(false);
        renderer_g->add_gcode(std::make_shared<gcode>(filename));

        renderer_bot_g->set_renderer(false);
        renderer_bot_g->add_gcode(std::make_shared<gcode>(filename));
        
        [renderer_tinyWIN_button_ setTransparent:false];
        
         NSString* result = [NSString stringWithUTF8String:renderer_g->get()->to_string().c_str()];
        [gcode_text_view_ setString:result];
    }
} // end do_open

// send gcode to robot for paper rendering
- (IBAction)renderer_tinyCNC:(id)sender {
    renderer_bot_g->set_renderer(true);
    renderer_bot_g->draw();
}

// send gcode to OpenGL framebuffer for on screen rendering
- (IBAction)renderer_tinyWIN:(id)sender {
    renderer_g->set_renderer(true);
}

// clear any gcode and stop rendering
- (IBAction)do_clear:(id)sender {
    [self clear];
}

// switch between Gcode and Logo view
- (IBAction)do_change_view_gcode:(id)sender {
    gcode_view_g = true;
    [self clear];
}

// switch between Gcode and Logo view
- (IBAction)do_change_view_logo:(id)sender {
    gcode_view_g = false;
    [self clear];
}

- (IBAction)progress_local_command:(id)sender {
}

- (void)control:(NSControl *)control textView:(NSTextView *)fieldEditor doCommandBySelector:(SEL)commandSelector
{
    NSLog(@"Selector method is (%@)", NSStringFromSelector( commandSelector ) );
    if (commandSelector == @selector(insertNewline:)) {
        //Do something against ENTER key
        printf("dhsdhjs\n");
    } else if (commandSelector == @selector(deleteForward:)) {
        //Do something against DELETE key
        
    } else if (commandSelector == @selector(deleteBackward:)) {
        //Do something against BACKSPACE key
        
    } else if (commandSelector == @selector(insertTab:)) {
        //Do something against TAB key
    }
}

-(void)controlTextDidEndEditing:(NSNotification *)notification
{
    printf("in here\n    ");
    // See if it was due to a return
    if ( [[[notification userInfo] objectForKey:@"NSTextMovement"] intValue] == NSReturnTextMovement )
    {
        NSLog(@"Return was pressed!");
    }
}

@end
