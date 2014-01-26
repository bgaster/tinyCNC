//
//  globals.hpp
//  tinyCNC
//
//  Created by Gaster, Benedict on 1/23/14.
//  Copyright (c) 2014 Gaster, Benedict. All rights reserved.
//

#include <memory>

#include "arduino-serial.hpp"

#include "gcode.hpp"
#include "renderer.hpp"


extern std::shared_ptr<gcode> gcode_g;
extern std::shared_ptr<renderer> renderer_g;
extern std::shared_ptr<arduino::serial> aserial_port_g;
