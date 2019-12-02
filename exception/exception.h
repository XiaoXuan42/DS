#pragma once

#include <exception>

namespace DS {
    class DSException : public std::exception
    {
    public:
        virtual const char * what() = 0;
    };
    class DSMemoryExceed : public DSException
    {
    public:
        virtual const char * what() {
            return "You are trying to operate on an invalid memory address\n";
        }
    };
    class DSReadError : public DSException
    {
    public:
        virtual const char * what() {
            return "The element you are trying to read is not found";
        }
    };
}