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
#include <functional>

#include "error_exception.hpp"
#include "debug.hpp"
#include "util.hpp"

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

class gcode_entry
{
private:
    void apply(
        std::function<void (unsigned int, float, float)> g,
        std::function<void (unsigned int, float)> m)
    {
        switch(type_) {
            case ID_TYPE::GCODE:
                g(value_, arguments_[0], arguments_[1]);
                break;
            case ID_TYPE::MCODE:
                m(value_, arguments_[0]);
                break;
            default:
                // don't current support and so don't print
                return;
        }
    }
public:
    
    ID_TYPE type_;
    unsigned int value_;
    std::vector<float> arguments_;
    std::string comment_;

    void print()
    {
        apply(
              [](unsigned int v, float x, float y) {
                  printf("G%d X%f Y%f\n", v, x, y);
              },
              [] (unsigned int v, float x) {
                  printf("M%d S%f\n", v, x);
        });
        
    }
    
    std::string to_string()
    {
        std::string str("");
        apply(
              [&str](unsigned int v, float x, float y) {
                  str = "G" + to_str(v) + " X" + to_str(x) + " Y" + to_str(y) + "\n";
              },
              [&str] (unsigned int v, float x) {
                  str = "M" + to_str(v) + " X" + to_str(x) + "\n";
              });
        
        return str;
    }
    
    bool is_type(ID_TYPE t)
    {
        return type_ == t;
    }
    
    bool is_value(unsigned int v)
    {
        return value_ == v;
    }
    
    float get_argument(unsigned int i)
    {
        return arguments_[i];
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
    static const unsigned int pen_      = 300;
    static const unsigned int pen_up_   = 50;
    static const unsigned int pen_down_ = 30;

    static const unsigned int move_to_  = 92;
    static const unsigned int draw_     = 1;
    
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
    
    std::string to_string()
    {
        std::string str("");
        
        std::for_each (begin(), end(), [&str] (std::shared_ptr<gcode_entry> entry) {
            str.append(entry->to_string());
        });
             
        return str;
    }
};
