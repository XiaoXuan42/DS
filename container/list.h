#pragma once

#include "../alloc/ds_alloc.h"
#include "../alloc/ds_construct.h"
#include "../alloc/ds_uninitialized.h"
#include "../algorithm/ds_algorithm.h"
#include "../iter/iter.h"

namespace xxds
{
    namespace {
        template<typename T>
        struct __list_node
        {
            __list_node<T> *next;
            __list_node<T> *pre;
            T data;
        };
        template<typename T, typename Ref, typename Ptr>
        struct __list_iterator
        {
            using iterator_category = bidirectional_iterator_tag;
            using value_type = T;
            using pointer = Ptr;
            using reference = Ref;
            using difference_type = ptrdiff_t;
            using size_type = size_t;

            using link_type = __list_node<T> *;
            using const_link_type = const __list_node<T> *;
            using self = __list_iterator<T, Ref, Ptr>;
            link_type p_node;

            __list_iterator() {}
            __list_iterator(link_type x) { p_node = x; }
            __list_iterator(const_link_type x) { p_node = x; }
            bool operator == (const self &rhs) const {
                return p_node == rhs.p_node;
            }
            bool operator != (const self &rhs) const {
                return !(operator == (rhs));
            }
            self & operator ++ () {
                p_node = p_node->next;
                return *this;
            }
            self & operator -- () {
                p_node = p_node->pre;
                return *this;
            }
            self & operator ++ (int) {
                self tmp = *this;
                this->operator ++();
                return tmp;
            }
            self & operator -- (int) {
                self tmp = *this;
                this->operator --();
                return tmp;
            }
            difference_type operator - (const self &rhs) const {
                return distance(rhs, *this);
            }
            reference operator *() {
                return p_node->data;
            }
            pointer operator ->() {
                return &(p_node->data);
            }
        };
    }
    template<typename T, typename Alloc = alloc>
    class list
    {
    private:
        using link_node = __list_node<T>;
        using link_type = link_node *;
        using const_link_node = const link_node;
        using const_link_type = const link_node *;
        
        using list_node_alloc = simple_alloc<link_node, alloc>;
    public:
        using value_type = T;
        using pointer = value_type*;
        using iterator = __list_iterator<T, T&, T*>;
        using reference = value_type&;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

        using const_value_type = const T;
        using const_reference = const T&;
        using const_pointer = const T*;
        using const_iterator = const __list_iterator<T, T&, T*>;
        using const_size_type = const size_t;
        using const_difference_type = const ptrdiff_t;
    private:
        link_type node;    
        link_type get_node() {
            return list_node_alloc::allocate();
        }
        void put_node(link_type p) {
            list_node_alloc::deallocate(p);
        }
        link_type create_node(const T& x) {
            link_type p = get_node();
            construct(&(p->data), x);
            return p;
        }
        void destroy_node(link_type p) {
            destroy(&(p->data));
            put_node(p);
        }
        void empty_initialize() {
            node = get_node();
            node->next = node;
            node->pre = node;
        }
    
    public:
        list() {
            empty_initialize();
        }
        ~list() {
            link_type cur = node->next;
            while(cur != node) {
                link_type tmp = cur;
                cur = cur->next;
                destroy_node(tmp);
            }
            destroy_node(node);
        }
        list(const list &rhs) {
            empty_initialize();
            link_type local_cur = node;
            iterator cur = static_cast<iterator>(rhs.node->next);
            while(cur.p_node != rhs.node) {
                link_type tmp = create_node(*cur);
                local_cur->next = tmp;
                tmp->pre = local_cur;
                local_cur = tmp;
                ++cur;
            }
            local_cur->next = node;
            node->pre = local_cur;
        }
        list& operator = (const list &rhs) {
            clear();
            link_type local_cur = node;
            iterator cur = static_cast<iterator>(rhs.node->next);
            while(cur.p_node != rhs.node) {
                link_type tmp = create_node(*cur);
                local_cur->next = tmp;
                tmp->pre = local_cur;
                local_cur = tmp;
                ++cur;
            }
            local_cur->next = node;
            node->pre = local_cur;
            return *this;
        }

        iterator begin() const { return (iterator)(node->next); }
        iterator end() const { return (iterator)(node); }
        bool empty() const {
            return node->next = node;
        }
        size_type size() const {
            size_type result = static_cast<size_type>(distance(begin(), end())); // implicit conversion
            return result;
        }
        iterator insert(iterator position, const T &x) {
            link_type tmp = create_node(x);
            tmp->next = position.p_node;
            tmp->pre = (position.p_node)->pre;
            ((position.p_node)->pre)->next = tmp;
            (position.p_node)->pre = tmp;
            return static_cast<iterator>(tmp);
        }
        void push_back(const T &x) {
            insert(end(), x);
        }
        void push_front(const T &x) {
            insert(begin(), x);
        }
        iterator erase(iterator position) {
            link_type lnode = (position.p_node)->pre;
            link_type rnode = (position.p_node)->next;
            lnode->next = rnode;
            rnode->pre = lnode;
            destroy_node(position.p_node);
            return static_cast<iterator>(rnode);
        }
        void pop_front() {
            erase(begin());
        }
        void pop_back() {
            iterator tmp = end();
            erase(--tmp);
        }
        void clear() {
            link_type cur = node->next;
            while(cur != node) {
                link_type tmp = cur;
                cur = cur->next;
                destroy(tmp);
            }
            node->next = node;
            node->pre = node;
        }
    };
}
