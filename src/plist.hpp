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