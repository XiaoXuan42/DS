#pragma once
#include<iterator>
#include "../iter/iter.h"

std::back_insert_iterator<int> bk;
namespace DS
{
    template<class Container>
    class back_insert_iterator : public iterator<output_iterator_tag, void, void, void, void>
    {
    protected:
        Container *container;
        typedef back_insert_iterator<Container> self;
    public:
        typedef Container container_type;
        explicit back_insert_iterator(Container &x) : container(&x) {}
        self& operator=(const typename Container::value_type &value) {
            container->push_back(value);
            return *this;
        }
        self operator*() = delete;
        self operator++() = delete;
        self operator++(int) = delete;
    };
    template<class Container>
    inline back_insert_iterator<Container> back_inserter(Container &x) {
        return back_insert_iterator<Container>(x);
    }
    template<class Container>
    class front_insert_iterator : public iterator<output_iterator_tag, void, void, void, void>
    {
    private:
        Container *container;
        typedef front_insert_iterator<Container> self;
    public:
        typedef Container container_type;
        explicit front_insert_iterator(Container &x) : container(&x) {}
        self& operator=(const typename Container::value_type &value) {
            container->push_front(value);
            return *this;
        }
        self operator*() = delete;
        self operator++() = delete;
        self operator++(int) = delete;
    };
    template<class Container>
    inline front_insert_iterator<Container> front_inserter(Container &x) {
        return front_insert_iterator<Container>(x);
    }
    template<class Container>
    class insert_iterator : public iterator<output_iterator_tag, void, void, void, void>
    {
    private:
        typedef typename Container::iterator iter_type;
        typedef insert_iterator<Container> self;
        Container * container;
        iter_type iter;
    public:
        insert_iterator(Container &x, iter_type it): container(&x), iter(it) {}
        self & operator=(const typename Container::value_type &value) {
            iter = container->insert(iter, value);
            ++iter;
            return *this;
        }
        self& operator*() = delete;
        self& operator++() = delete;
        self& operator++(int) = delete;
    };
    template<class Container, class Iterator>
    inline insert_iterator<Container> inserter(Container &x, Iterator i) {
        typedef typename Container::iterator iter;
        return insert_iterator<Container>(x, iter(i));
    }
    template<typename Iterator>
    class reverse_iterator : public iterator<Iterator::iterator_category,
                                    Iterator::value_type,
                                    Iterator::difference_type,
                                    Iterator::reference,
                                    Iterator::pointer>
    {
    protected:
        Iterator current;
        typedef reverse_iterator<Iterator> self;
    public:
        typedef Iterator iterator_type;
        reverse_iterator() {}
        explicit reverse_iterator(iterator_type x) : current(x) {}
        reverse_iterator(const self &x) : current(x.current) {}

        iterator_type base() const {
            return current;
        }
        reference operator*() const {
            Iterator tmp = current;
            return *--tmp;
        }
        pointer operator->() const {
            return &(operator*());
        }
        self& operator++() {
            --current;
            return *this;
        }
        self operator++(int) {
            self tmp = *this;
            --current;
            return tmp;
        }
        self operator--() {
            ++current;
            return *this;
        }
        self operator--(int) {
            self tmp = *this;
            ++current;
            return tmp;
        }
        self operator+ (difference_type n) const {
            return self(current-n);
        }
        self& operator+=(difference_type n) {
            current -= n;
            return *this;
        }
        self operator-(difference_type n) const {
            return self(current + n);
        }
        self& operator-=(difference_type n) {
            current -= n;
            return self(current+n);
        }
        reference operator[] (difference_type n) const {
            return *(*this + n);
        }
    };
}

