/*
 * File: util.hpp
 * Author: Benedict R. Gaster
 * Desc: Some handy stuff
 *
 * Copyright 2014 Benedict R. Gaster
 * License: See the file license.
 */

#include <iostream>
#include <sstream>
#include <string>
#include "error_exception.hpp"

template <typename T>
std::string to_str(const T& x)
{
    std::ostringstream o;
    if (!(o<<x)) {
        throw error_exception("failed to convert to string in to_str");
    }
    return o.str();
}
