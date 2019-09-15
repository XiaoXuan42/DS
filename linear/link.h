#pragma once
#include "../exception/exception.h"
#include <iostream>

namespace DS {
    template <typename T>
    class LinkNode
    {
        T data;
        LinkNode<T> *next;
        LinkNode<T> *previous;
    public:
        LinkNode(): next(nullptr), previous(nullptr) {}
        explicit LinkNode(const T &newdata): next(nullptr), previous(nullptr), data(newdata) {}
        ~LinkNode() {
            next = previous = nullptr;
        }
        
        LinkNode<T> *getNext() const {
            return next;
        }
        LinkNode<T> *getPrevious() const {
            return previous;
        }
        T getData() const {
            return data;
        }
        void setData(const T &newdata) {
            data = newdata;
        }
        void setNext(LinkNode<T> *newnext) {
            next = newnext;
        } 
        void setPrevious(LinkNode<T> *newprevious) {
            previous = newprevious;
        }
    };
    template <typename T>
    class Link
    {
        //下标从0开始
        LinkNode<T> fakehead;
        LinkNode<T> *tail;
        int cnt; //统计链表中含有的节点数

        LinkNode<T> * getPointer(int at) const; //如果失败就返回nullptr
    public:
        Link(): tail(&fakehead), cnt(0) {}
        ~Link() {
            LinkNode<T> *cur = fakehead.getNext();
            while(cur != nullptr) {
                LinkNode<T> *tmp = cur;
                cur = cur->getNext();
                delete tmp;
            }
            tail = &fakehead;
            cnt = 0;
        }
        bool empty() const {
            return cnt == 0;
        }
        int size() const {
            return cnt;
        }
        void push(const T &data);
        bool insert(const T &data, int at);
        bool pop(); //如果为空就返回一个false
        bool remove(int at); //如果发生越界那么这个函数什么也不做，并且返回一个false
        
        T get(int at) const;
        bool modify(const T &data, int at);
    };
    template<typename T>
    LinkNode<T> * Link<T>::getPointer(int at) const {
        if(at > cnt) {
            return nullptr;
        }
        const LinkNode<T> *cur = &fakehead;
        for(int i = 0; i < at; ++i) {
            cur = cur->getNext();
        }
        return const_cast<LinkNode<T> *>(cur);
    }
    template<typename T>
    void Link<T>::push(const T &data) {
        LinkNode<T> *newnode = new LinkNode<T>(data);
        tail->setNext(newnode);
        newnode->setPrevious(tail);
        tail = newnode;
        cnt++;
    }
    template<typename T>
    bool Link<T>::insert(const T &data, int at) {
        LinkNode<T> *fork = getPointer(at);
        if(fork == nullptr) {
            return false;
        }
        LinkNode<T> *newnode = new LinkNode<T>(data);
        newnode->setNext(fork->getNext());
        newnode->setPrevious(fork);
        fork->setNext(newnode);
        if(newnode->getNext() != nullptr) {
            newnode->getNext()->setPrevious(newnode);
        }
        if(fork == tail) {
            tail = newnode;
        }
        cnt++;
        return true;
    }
    template<typename T>
    bool Link<T>::pop() {
        if(cnt == 0) {
            return false;
        }
        else {
            LinkNode<T> *cur = tail;
            tail = tail->getPrevious();
            delete cur;
            tail->setNext(nullptr);
            cnt--;
            return true;
        }
    }
    template<typename T>
    bool Link<T>::remove(int at) {
        LinkNode<T> *fork = getPointer(at);
        if(fork == nullptr || fork->getNext() == nullptr) {
            return false;
        }
        LinkNode<T> *tmp = fork->getNext();
        fork->setNext(tmp->getNext());
        if(fork->getNext() != nullptr) {
            fork->getNext()->setPrevious(fork);
        }
        if(tmp == tail) {
            tail = fork;
        }
        delete tmp;
        cnt--;
        return true;
    }
    template<typename T>
    T Link<T>::get(int at) const {
        LinkNode<T> *fork = getPointer(at);
        if(fork == nullptr || fork->getNext() == nullptr) {
            throw DSMemoryExceed();
        }
        return fork->getNext()->getData();
    }
    template<typename T>
    bool Link<T>::modify(const T &data, int at) {
        LinkNode<T> *fork = getPointer(at);
        if(fork == nullptr || fork->getNext() == nullptr) {
            return false;
        }
        fork->getNext()->setData(data);
        return true;
    }
}