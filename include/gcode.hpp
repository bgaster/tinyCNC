/*
 * File: gcode.hpp
 * Author: Benedict R. Gaster
 * Desc: Basic interface for loading a Gcode file
 *
 * Copyright 2014 Benedict R. Gaster
 * License: See the file license.
 */
#pragma once

#include <vector>
#include <string>
#include <utility>
#include <regex>
#include <iostream>
#include <string>
#include <memory>
#include <utility>
#include <tuple>
#include <initializer_list>
#include <algorithm>

#include <error_exception.hpp>
#include <debug.hpp>

enum class ID_TYPE
{
    GCODE, MCODE, UNDEFINED 
};    

enum class ARG_TYPE
{
    X, // absoloute position
    Y, // absoloute position
    S // pen up or down 
    // currently we only support these arguments but designed to support more
};

struct gcode_entry
{
    ID_TYPE type_;
    unsigned int value_;
    std::vector<float> arguments_;
    std::string comment_;

    void print()
    {
	switch(type_) {
	case ID_TYPE::GCODE:
	    printf("G%d X%f Y%f\n", value_, arguments_[0], arguments_[1]);
	    break;
	case ID_TYPE::MCODE:
	    printf("M%d S%f\n", value_, arguments_[0]);
	    break;
	default:
	    // don't current support and so don't print
	    return;
	}	
    }
};
    
class gcode
{
private:
    typedef std::tuple<std::string, std::string> match_arg_t;
    
    std::vector<std::shared_ptr<gcode_entry>> codes_;

    static const std::string float_regex_;
    static const std::string int_regex_;
    static const std::string unsigned_regex_;    

    bool parse_code(
	std::string line,
	std::string path,
	std::shared_ptr<gcode_entry> entry);

    bool parse_comment(std::string line)
    {
	return std::regex_match(line, std::regex("\\(.*\\)") );
    }
    
    bool parse_type(
	std::string line,
	ID_TYPE t,
	std::shared_ptr<gcode_entry> entry,
	std::string tstr,
	std::initializer_list<match_arg_t> params
	)
    {
	// check code matches
	if (line.compare(0, tstr.length(), tstr) != 0) {
	    return false;
	}
	entry->type_  = t;
	entry->value_ = std::stoi(&tstr.c_str()[1]);
	
	std::vector<match_arg_t> args(params);

	std::regex ws_re("\\s+"); // whitespace

	auto iargs     = args.begin();
	auto iargs_end = args.end();
	bool result = true;

	std::for_each( 
	    std::sregex_token_iterator(
		line.begin() + tstr.length(), 
		line.end(), 
		ws_re, 
		-1),
	    std::sregex_token_iterator(),
	    [entry, &iargs, iargs_end, &result] (std::string str) {
		// we were a little lazy with our regexp...
		if (str.length() != 0) {
		    if (iargs == iargs_end) {
			// more aguments than there are expects params
			return;
		    }		 

		    std::smatch matches;   

		    if (!std::regex_match(
			    str,
			    matches,
			    std::regex( 
				"(" + std::get<0>(*iargs) + ")" + std::get<1>(*iargs)))) {
			result = false;
			return;
		    }

		    entry->arguments_.push_back(std::stof(matches[2]));
		    iargs++;		    
		}
	    });
	
	
	return result;
    }
        
public:
    gcode(std::string filename) throw(error_exception);
    
    ~gcode()
    {
    }

    gcode(const gcode& rhs)
    {
	*this = rhs;
    }

    gcode& operator== (const gcode & rhs)
    {
	if (this == &rhs) {
	    return *this;
	}
    
	// shallow copy of codes_
	codes_ = rhs.codes_;

	return *this;
    }

    std::vector<std::shared_ptr<gcode_entry>>::iterator begin()
    {
	return codes_.begin();
    }

    std::vector<std::shared_ptr<gcode_entry>>::iterator end()
    {
	return codes_.end();
    }
};
