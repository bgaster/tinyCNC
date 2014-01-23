/*
 * File: gcode.cpp
 * Author: Benedict R. Gaster
 * Desc: Basic interface for loading Gcode.
 *
 * Copyright 2014 Benedict R. Gaster
 * License: See the file license.
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <regex>
#include <map>
#include <unordered_map>
#include <tuple>
#include <algorithm>

#include <gcode.hpp>
#include <error_exception.hpp>

const std::string gcode::float_regex_("([+-]?[0-9]*\\.[0-9]+|[0-9]+)");
const std::string gcode::int_regex_("([-+]?[0-9]*)");
const std::string gcode::unsigned_regex_("([0-9]*)");

bool gcode::parse_code(
    std::string line,
    std::string path,
    std::shared_ptr<gcode_entry> entry)
{
    
    // Currently for TinyCNC we only process:
    // 
    //   GCode G1 X Y ...
    //   MCode M300 S ..
    return 
	parse_type(
	    line, 
	    ID_TYPE::MCODE, 
	    entry,
	    "M300",
	    { match_arg_t{"S", float_regex_} })
	||
	parse_type(
	    line, 
	    ID_TYPE::GCODE, 
	    entry,
	    "G1",
	    { match_arg_t{"X", float_regex_} , match_arg_t{"Y", float_regex_} });
}

gcode::gcode(std::string filename) throw(error_exception)
{
    std::ifstream stream(filename);
    
    if (!stream) {
	throw error_exception("Failed to find Gcode file: " + filename);
    }

    std::string path("");
    {
	std::smatch matches;
	if (std::regex_match(filename, matches, std::regex("(.*/)(.*$)"))) {
	    path = matches[1];
	}
    }  

    std::string line;
    while (getline(stream, line)) {
	std::shared_ptr<gcode_entry> entry = std::make_shared<gcode_entry>();
	
	// is whole line a comment
	if (parse_comment(line)) {
	    continue;
	}
	

	if (parse_code(line, path, entry)) {
	    codes_.push_back(entry);
	}
    }
}

