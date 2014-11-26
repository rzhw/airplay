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
#include "plist.hpp"

// here be dragons

PlistBuilder::PlistBuilder() {
    this->ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"\
"<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\"\n"\
" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"\
"<plist version=\"1.0\">\n"\
" <dict>\n";
}

PlistBuilder::~PlistBuilder() {
}

void PlistBuilder::add_string(const char *key, const char *string) {
    this->ss << "  <key>" << key << "</key>\n";
    this->ss << "  <string>" << string << "</string>\n";
}

void PlistBuilder::add_integer(const char *key, int integer) {
    this->ss << "  <key>" << key << "</key>\n";
    this->ss << "  <integer>" << integer << "</integer>\n";
}

void PlistBuilder::add_real(const char *key, const char *string) {
    this->ss << "  <key>" << key << "</key>\n";
    this->ss << "  <real>" << string << "</real>\n";
}

void PlistBuilder::add_array(const char *key, const PlistBuilder &array) {
    this->ss << "  <key>" << key << "</key>\n";
    this->ss << "  <array>\n";
    auto str = array.str();
    size_t dictStart = str.find("<dict>");
    size_t dictEnd = str.find("</dict>");
    auto str2 = str.substr(dictStart, dictEnd - dictStart + 8);
    this->ss << "    " << str2;
    this->ss << "  </array>\n";
}

std::string PlistBuilder::str() const {
    std::string str = this->ss.str();
    str.append(
" </dict>\n"\
"</plist>");
    return str;
}