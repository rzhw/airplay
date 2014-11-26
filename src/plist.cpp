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

std::string PlistBuilder::str() {
    std::string str = this->ss.str();
    str.append(
" </dict>\n"\
"</plist>");
    return str;
}