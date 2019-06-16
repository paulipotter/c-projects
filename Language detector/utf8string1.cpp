/*
* Paula Mendez
* CIS 308
* Assignment 6
* utf8string.cpp
*/
#include <iostream>
#include "utf8string.hpp"
#include "ksu_utf8.h"
#include <string>

using namespace std;
    //utf8string.cppabc

//implementation goes here


// Constructors
utf8string::utf8string()  // Default cosntructor
{

}

// Constructor from an existing string
utf8string::utf8string(string s):_str(s)
{

}

// Copy constructor
utf8string::utf8string(const utf8string& new_str)
{
    _str = new_str._str;
}

// Destructor
utf8string::~utf8string()
{

}

// Assignment operator
utf8string& utf8string::operator=(const utf8string& new_str)
{
    _str = new_str._str;
    return *this;
}

//finds the length of the string
size_t     utf8string::length()
{
    unsigned char *c = (unsigned char*) _str.c_str();
    return (size_t)ksu_charlen(c);
}

//finds the byte length of the string
size_t     utf8string::byte_length()
{
    return _str.length();
}

//implements the '<<' operator
std::ostream &operator<<(std::ostream& a, utf8string b)
{
    a << b._str;
    return a;
}

//Implements the contcatenation for the utf8 string
utf8string utf8string::operator+(const utf8string s) // Regular concatenation
{
    return _str + s._str;
}

//converts the current string to all lowercase
utf8string utf8string::lower()
{
    int i = 0;
    const unsigned char* low = ksu_utf8_lower((const unsigned char*)_str.c_str());
    while(i<_str.length())
    {
        _str[i] = low[i];

    i++;
    }
    return *this;
}