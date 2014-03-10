//
//  globals.cpp
//  tinyCNC
//
//  Created by Gaster, Benedict on 1/23/14.
//  Copyright (c) 2014 Gaster, Benedict. All rights reserved.
//

#include <memory>

#include "globals.hpp"

std::shared_ptr<gcode> gcode_g(nullptr);
std::shared_ptr<renderer>  renderer_g(nullptr);
std::shared_ptr<renderer>  renderer_bot_g(nullptr);
bool gcode_view_g(true);