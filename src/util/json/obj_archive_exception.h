#pragma once

#include <exception>

struct ObjArchiveException : public std::exception {
    const char* msg;
    ObjArchiveException() {}
    ObjArchiveException(const char* msg) : msg(msg) {}
    const char* what() const noexcept;
};