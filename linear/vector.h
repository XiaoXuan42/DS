#pragma once

#include "../exception/exception.h"
#include <cstdio>

namespace DS
{
    template<typename T>
    class Vector
    {
        int MaxSingleTwoPow = 20;
        int curTwoPow;

        size_t capacity;
        size_t used;
        T *array;

        void resetPointer(const T * const target); //将自己的内容删除，并且设置指针为target
        void copyContent(T * const target) const; //将自己的内容复制到target中
        void copyBasic(Vector &other) const;
        void allocNew(size_t newsz); //设置自己的空间为newsz
        T & get(const int at) const;        

    public:
        Vector(): curTwoPow(4), capacity(0), used(0), array(nullptr) {}
        ~Vector() {
            if(array != nullptr) {
                delete array;
                array = nullptr;
            }
            curTwoPow = 4;
            used = 0;
            capacity = 0;
        }
        Vector(const Vector &other);
        Vector(const Vector &&other);
        Vector<T> & operator = (const Vector<T> &other);
        Vector<T> & operator = (const Vector<T> &&other);

        size_t cap() const {
            return capacity;
        }
        const T operator [] (const int at) const; //usesd only for read
        T & operator [] (int at);
        void push(const T &data);
        bool resize(int cnt);
    };
    template<typename T>
    void Vector<T>::copyBasic(Vector<T> &other) const {
        other.curTwoPow = curTwoPow;
        other.capacity = capacity;
        other.used = used;
    }
    template<typename T>
    Vector<T>::Vector(const Vector<T> &other) {
        array = new T[capacity / sizeof(T)];
        other.copyContent(array);
        other.copyBasic(*this);
    }
    template<typename T>
    Vector<T>::Vector(const Vector<T> &&other) {
        array = other.array;
        other.array = nullptr;
        other.copyBasic(*this);
    }
    template<typename T>
    Vector<T> & Vector<T>::operator = (const Vector<T> &other) {
        this->~Vector();
        array = new T[other.capacity / sizeof(T)];
        other.copyContent(array);
        other.copyBasic(*this);
    }
    template<typename T>
    Vector<T> & Vector<T>::operator = (const Vector<T> &&other) {
        this->~Vector();
        array = other.array;
        other.copyBasic(*this);
        other.array = nullptr;
    }

    template<typename T>
    void Vector<T>::resetPointer(const T * const target) {
        if(array != nullptr) {
            delete array;
        }
        array = const_cast<T *>(target);
    }
    template<typename T>
    void Vector<T>::copyContent(T * const target) const {
        for(int i = 0; i < used / sizeof(T); ++i) {
            target[i] = array[i];
        }
    }
    template<typename T>    
    void Vector<T>::allocNew(size_t newsz) {
        while(capacity < newsz) {
            capacity += (1ll << curTwoPow) * sizeof(T);
            curTwoPow = curTwoPow < Vector::MaxSingleTwoPow ? MaxSingleTwoPow : curTwoPow + 1;
        }
        T *newarray = new T[capacity/sizeof(T)];
        this->copyContent(newarray);
        this->resetPointer(newarray);
    }
    template<typename T>
    T & Vector<T>::get(int at) const {
        at = at < 0 ? used / sizeof(T) + at : at;
        if(at >= used / sizeof(T) || at < 0) {
            throw DS::DSMemoryExceed();
        }
        return array[at];
    }
    template<typename T>
    const T Vector<T>::operator [] (const int at) const {
        return get(at);
    }
    template<typename T>
    T & Vector<T>::operator [] (int at) {
        at = at < 0 ? used / sizeof(T) + at : at;
        if(at < 0) {
            throw DS::DSMemoryExceed();
        }
        if(at >= capacity / sizeof(T)) {
            allocNew((at+1) * sizeof(T));
            used = (at+1) * sizeof(T);
        }
        return get(at);
    }
    template<typename T>
    bool Vector<T>::resize(int cnt) {
        if(cnt < used / sizeof(T)) {
            return false;
        }
        T *newarray = new T[cnt];
        for(int i = 0; i < used / sizeof(T); ++i) {
            newarray[i] = array[i];
        }
        if(array != nullptr) {
            delete array;
        }
        array = newarray;
        capacity = cnt * sizeof(T);
        return true;
    }
    template<typename T>
    void Vector<T>::push(const T &data) {
        if(used == capacity) {
            allocNew(used + sizeof(T));
        }
        array[used/sizeof(T)] = data;
        used += sizeof(T);
    }
}