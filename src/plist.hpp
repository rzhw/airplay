// Copyright (C) 2014 Richard Wang
//
// This file is part of rzhw/airplay.
//
// rzhw/airplay is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// rzhw/airplay is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with rzhw/airplay. If not, see <http://www.gnu.org/licenses/>.

#include <string>
#include <iostream>
#include <sstream>

#ifndef __PLIST_HPP__
#define __PLIST_HPP__

class PlistBuilder
{
public:
    PlistBuilder();
    ~PlistBuilder();
    void add_string(const char *, const char *);
    void add_integer(const char *, int);
    void add_real(const char *, const char *);
    void add_array(const char *, const PlistBuilder &);
    std::string str() const;
private:
    std::stringstream ss;
};

#endif