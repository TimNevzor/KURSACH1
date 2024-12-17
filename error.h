//error.h
#pragma once
#include <stdexcept>
#include <string>

class criticalerr : public std::runtime_error
{
public:
    criticalerr(const std::string& s) : std::runtime_error(s) {}
};

class noncriticalerr : public std::runtime_error
{
public:
    noncriticalerr(const std::string& s) : std::runtime_error(s) {}
};
