#pragma once

#include "../exception/exception.h"
#include <cstring>

namespace DS {
    /* TODO: string的底层实现是用一个char类型的动态数组，但是这样改变它的大小的时候就会很麻烦 */
    /* 所以将来要替换成一个预先多分配空间的结构，但是它又能保持动态特性，就像vector */
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
        String splice(int left, int right) const;
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
    String String::splice(int left, int right) const {
        String newstr;
        if(right > this->size() || left >= this->size()) {
            return newstr;
        }
        else if(right <= left) {
            return newstr;
        }
        newstr.str = new char[right-left+1];
        for(int i = left; i < right; ++i) {
            newstr.str[i-left] = str[i];
        }
        newstr.str[right-left] = '\0';
        return newstr;
    }
}