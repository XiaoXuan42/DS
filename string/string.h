#pragma once

#include "../exception/exception.h"
#include <cstring>

namespace DS {
    class String
    {
        char *str;

    public:
        String() {
            str = nullptr;
        }
        String(const char *src) {
            if(src == str)
                return;
            size_t len = strlen(src);
            str = new char[len+1];
            strcpy(str, src);
        }
        String(const String &src) {
            String(src.c_str());
        }
        String(const String &&src) {
            str = str.c_str();
            str.c_str = nullptr;
        }

        ~String() {
            if(str != nullptr) {
                delete str;
            }
            str = nullptr;
        }
        const char * c_str() const {
            return str;
        }
        char * c_str() {
            return str;
        }
        size_t size() const {
            if(str == nullptr) {
                return 0;
            }
            return strlen(str);
        }
        const String operator + (const String &rhs) const;
        const String operator = (const String &other);
        const String operator = (const String &&other);
        const char operator[] (const int at) const;
    };

    const String String::operator + (const String &rhs) const {
        String newstr;
        if(str == nullptr && rhs.str == nullptr) {
            return newstr;
        }
        else if(str == nullptr) {
            newstr = new char[rhs.size()+1];
            strcpy(newstr.str, rhs.str);
            return newstr;
        }
        else if(rhs.str == nullptr) {
            newstr = new char[strlen(str)];
            strcpy(newstr.str, str);
            return newstr;
        }
        else {
            newstr = new char[this->size()+rhs.size()+1];
            strcpy(newstr.str, str);
            strcat(newstr.str, newstr.str);
            return newstr;
        }
    }
    const String& String::operator = (const String &other) {
        if(str != nullptr) {
            delete str;
            str = nullptr;
        }
        if(other.size() == 0) {
            return *this;
        }
        str = new char[other.size()+1];
        strcpy(str, other.str);
        return *this;
    }
    const String& String::operator = (const String &&other) {
        str = other.str;
        other.str = nullptr;
        return *this;
    }
    const char String::operator [] (const int at) const {
        if(at >= this->size()) {
            throw DSMemoryExceed();
        }
        return str[at];
    }
}