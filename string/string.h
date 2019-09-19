#pragma once

#include "../exception/exception.h"
#include "../linear/vector.h"
#include <cstring>
#include <fstream>
#include <iostream>

namespace {
    inline int safe_strlen(const char *str) {
        if(str == nullptr) {
            return 0;
        }
        else {
            return strlen(str);
        }
    }
    inline void safe_strcpy(char *dest, const char *src) {
        if(src == nullptr) {
            return;
        }
        else {
            strcpy(dest, src);
        }
    }
    inline void safe_strcat(char *dest, int startpos, const char *src) {
        if(src == nullptr) {
            return;
        }
        else {
            int src_len = safe_strlen(src);
            for(int i = startpos; i < startpos + src_len; i++) {
                dest[i] = src[i-startpos];
            }
            dest[startpos+src_len] = '\0';
        }
    }
}
namespace DS {
    class String
    {
        static const int npos = -1;
        char *string;
        int len;

    public:
        String();
        explicit String(const char *str);
        String(const String &other);
        String(String &&other);
        ~String();

        int length() const {
            return len;
        }
        bool overflow(int at) const {
            return at >= len;
        }
        char * getRaw() const {
            return string;
        }

        String operator + (const String &rhs) const;
        String & operator += (const String &rhs);
        String & operator = (const String &rhs);
        String & operator = (String &&rhs);
        String & operator = (const char *str);
        friend String operator + (const String &rhs, const char *str);
        friend String operator + (const char *str, const String &rhs);

        char & operator[] (const int at);
        String subString(const int start, const int len) const;//如果长度不足的话，就取到字符串末尾
        int find(const char c) const;
        int find(const String &other) const;
        int find(const char *str) const;

        friend std::ostream & operator << (std::ostream &out, const String &str);
    };
    String::String(): string(nullptr) {}
    String::String(const char *str) {
        len = strlen(str);
        string = new char[len+1];
        for(int i = 0; i < len; ++i) {
            string[i] = str[i];
        }
        string[len] = '\0';
    }
    String::String(const String &other) {
        len = other.len;
        string = new char[len+1];
        for(int i = 0; i < len; ++i) {
            string[i] = other.string[i];
        }
        string[len] = '\0';
    }
    String::String(String &&other) {
        string = other.string;
        len = other.len;
        other.string = nullptr;
    }
    String::~String() {
        if(string != nullptr) {
            delete string;
            string = nullptr;
        }
        len = 0;
    }
    String & String::operator = (const char *str) {
        if(string == str) {
            return *this;
        }
        if(string != nullptr) {
            delete string;
        }
        int other_len = safe_strlen(str);
        string = new char[other_len+1];
        for(int i = 0; i < other_len; ++i) {
            string[i] = str[i];
        }
        string[other_len] = '\0';
        len = other_len;
        return *this;
    }
    String & String::operator = (const String &rhs) {
        return this->operator = (rhs.getRaw());
    }
    String & String::operator = (String &&rhs) {
        string = rhs.string;
        len = rhs.len;
        rhs.string = nullptr;
        return *this;
    }
    String String::operator + (const String &rhs) const {
        char *newstr = new char[len+rhs.len+1];
        safe_strcpy(newstr, string);
        safe_strcat(newstr, len, rhs.string);
        return String(newstr);
    }
    String & String::operator += (const String &rhs) {
        char *newstr = new char[len+rhs.len+1];
        safe_strcpy(newstr, string);
        safe_strcat(newstr, len, rhs.string);
        if(string != nullptr) {
            delete string;
        }
        string = newstr;
        len = len+rhs.len;
        return *this;
    }
    char & String::operator[] (const int at) {
        if(this->overflow(at)) {
            throw DS::DSMemoryExceed();
        }
        return string[at];
    }
    String String::subString(const int start, const int getlen) const {
        char *newstr = new char[getlen+1];
        int i = start;
        for(; i < start + getlen && i < len; ++i) {
            newstr[i-start] = string[i];
        }
        newstr[i-start] = '\0';
        return String(newstr);
    }
    int String::find(const char c) const {
        for(int i = 0; i < len; ++i) {
            if(string[i] == c) {
                return i;
            }
        }
        return String::npos;
    }
    int String::find(const char *str) const {
        int other_len = strlen(str);
        bool match;
        for(int i = 0; i < len - other_len + 1; ++i) {
            match = true;
            for(int j = i; j < i + other_len; ++j) {
                if(string[j] != str[j-i]) {
                    match = false;
                    break;
                }
            }
            if(match) {
                return i;
            }
        }
        return String::npos;
    }
    int String::find(const String &other) const {
        return this->find(other.getRaw());
    }
    String operator + (const String &rhs, const char *str) {
        int str_len = safe_strlen(str);
        char *newstr = new char[str_len + rhs.len + 1];
        safe_strcpy(newstr, rhs.getRaw());
        safe_strcat(newstr, rhs.len, str);
        return String(newstr);
    }
    String operator + (const char *str, const String &rhs) {
        int str_len = safe_strlen(str);
        char *newstr = new char[str_len + rhs.len + 1];
        safe_strcpy(newstr, str);
        safe_strcat(newstr, str_len, str);
        return String(newstr);
    }

    std::ostream & operator << (std::ostream &out, const String &str) {
        if(str.len != 0) {
            out << str.string;
        }
        return out;
    }
}