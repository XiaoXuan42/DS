#pragma once

#include "./pair.h"
#include "../iter/iter.h"
#include "../functor/ds_function.h"
#include "../alloc/ds_memory.h"
#include <cassert>

namespace xxds
{
    namespace {
        typedef bool __rb_tree_color;
        const __rb_tree_color __rb_tree_black = false;
        const __rb_tree_color __rb_tree_red = true;

        struct __rb_tree_node_base
        {
            typedef __rb_tree_node_base* base_ptr;
            typedef __rb_tree_color color_type;
            base_ptr lch, rch;
            base_ptr parent;
            color_type color;
            __rb_tree_node_base(): lch(nullptr), rch(nullptr), parent(nullptr), color(__rb_tree_black) {}
            static base_ptr minimum(base_ptr x) {
                while(x->lch != nullptr) {
                    x = x->lch;
                }
                return x;
            }
            static base_ptr maximum(base_ptr y) {
                while(y->rch != nullptr) {
                    y = y->rch;
                }
                return y;
            }
        };
        template<typename Value>
        struct __rb_tree_node : public __rb_tree_node_base
        {
            typedef __rb_tree_node<Value> * link_type;
            Value val;
        };
        struct __rb_tree_iterator_base
        {
            typedef typename __rb_tree_node_base::base_ptr base_ptr;
            typedef bidirectional_iterator_tag iterator_category;
            base_ptr node;

            __rb_tree_iterator_base(): node(nullptr) {}
            __rb_tree_iterator_base(base_ptr node): node(node) {}
            
            void increment() {
                if(node->rch != nullptr) {
                    node = node->rch;
                    while(node->lch != nullptr) {
                        node = node->lch;
                    }
                }
                else {
                    base_ptr p = node->parent;
                    while(node == p->rch) {
                        node = p;
                        p = p->parent;
                    }
                    if(node->rch != p) {
                        node = p;
                    }  
                    // if node points to the maximum element before current call, then now node points to the header
                    // this also holds when the maximum element is the root
               }
            }
            void decrement() {
                //if the node point to the header, then set it point to the maximum element
                if(node->color == __rb_tree_red && node->parent->parent == node) {
                    node = node->rch;
                    return;
                }
            
                if(node->lch != nullptr) {
                    node = node->lch;
                    while(node->rch != nullptr) {
                        node = node->rch;
                    }
                }
                else {
                    base_ptr p = node->parent;
                    while(node == p->lch) {
                        node = p;
                        p = p->parent;
                    }
                    if(node->lch != p) {
                        node = p;
                    }
                    // if the node point to the minimum element before current call, then now node points to the header
                    // this also holds when the minimum element is the root
                }
            }
        };

        template<typename Value, typename Ref, typename Ptr>
        struct __rb_tree_iterator : public __rb_tree_iterator_base
        {
            using value_type = Value;
            using pointer = Ptr;
            using reference = Ref;
            using difference_type = ptrdiff_t;
            using link_type = __rb_tree_node<Value>*;

            using self = __rb_tree_iterator<Value, Ref, Ptr>;

            __rb_tree_iterator() {}
            __rb_tree_iterator(link_type lk): __rb_tree_iterator_base(lk) {
            }

            bool operator == (const __rb_tree_iterator &rhs) const {
                return node == rhs.node;
            }
            bool operator != (const __rb_tree_iterator &rhs) const {
                return !operator==(rhs);
            }
            reference operator * () const {
                return link_type(node)->val;
            }
            pointer operator ->() const {
                return &(operator *());
            }
            self & operator ++() {
                increment();
                return *this;
            }
            self & operator --() {
                decrement();
                return *this;
            }
            self operator ++(int) {
                self tmp = *this;
                operator++();
                return tmp;
            }
            self operator --(int) {
                self tmp = *this;
                operator--();
                return tmp;
            }
        };
    }

    // the keyofvalue maps a value to it's key
    template<typename Key, typename Value, typename KeyOfValue,
        typename Compare = less<Key>, typename Alloc = alloc>
    class rb_tree
    {
    public:
        using value_type = Value;
        using reference = value_type&;
        using const_pointer = const value_type *;
        using const_reference = const value_type &;
        using difference_type = ptrdiff_t;
        using iterator = __rb_tree_iterator<Value, Value&, Value*>;
        using key_type = Key;
        using size_type = size_t;

    private:
        using base_ptr = typename __rb_tree_node_base::base_ptr;
        using link_type = typename __rb_tree_node<value_type>::link_type;
        using data_alloc = simple_alloc<__rb_tree_node<value_type>, Alloc>;
        using color_type = __rb_tree_color;
        using self = rb_tree<Key, Value, KeyOfValue, Compare, Alloc>;

        // make them static because they are just functors.
        static const Compare cmp;
        static const KeyOfValue get_key;
        
        size_type node_count;
        link_type header;

        /* some funciton aliasment */
        static link_type & left(link_type x) {
            return (link_type &)(x->lch);
        }
        static link_type & right(link_type x) {
            return (link_type &)(x->rch);
        }
        static link_type & parent(link_type x) {
            return (link_type &)(x->parent);
        }
        static reference value(link_type x) {
            return x->val;
        }
        static const Key key(link_type x) {
            return get_key(value(x));
        }
        static color_type & color(link_type x) {
            return (color_type&)(x->color);
        }
        static link_type & left(base_ptr x) {
            return (link_type &)(x->lch);
        }
        static link_type & right(base_ptr x) {
            return (link_type &)(x->rch);
        }
        static link_type & parent(base_ptr x) {
            return (link_type &)(x->parent);
        }
        static reference value(base_ptr x) {
            return (link_type(x))->val;
        }
        static Key key(base_ptr x) {
            return get_key(value(x));
        }
        static color_type & color(base_ptr x) {
            return (color_type &)(x->color);
        }
        static link_type minimum(link_type x) {
            return (link_type)__rb_tree_node_base::minimum((base_ptr)x);
        }
        static link_type maximum(base_ptr x) {
            return (link_type)__rb_tree_node_base::maximum(x);
        }

        link_type & leftmost() const {
            return left(header);
        }
        link_type & rightmost() const {
            return right(header);
        }
        link_type & root() const {
            return parent(header);
        }

        static link_type get_node() {
            // notice: data_alloc::allocate() will not autonomously call the _rb_tree_node's constructor
            // to allocate memory and call the Value's constructor, use create_node instead
            return data_alloc::allocate();
        }
        static void del_node(link_type tp) {
            destroy(tp);
            data_alloc::deallocate(tp);
        }
        static link_type create_node(const value_type &x) {
            link_type tmp = get_node();
            left(tmp) = nullptr;
            right(tmp) = nullptr;
            construct(&tmp->val, x);
            return tmp;
        }
        void init() {
            header = get_node();
            leftmost() = header;
            rightmost() = header;
            color(header) = __rb_tree_red;
            node_count = 0;

            root() = nullptr;
        }
        link_type __insert(link_type x, link_type y, const value_type &v); // the parent is y, current is x
        void __insert_rebalance_tree(base_ptr x, base_ptr y);
        void __remove_rebalance_tree(base_ptr x, base_ptr y);
        link_type __copy(link_type x, link_type p);
        void __erase(link_type x);
        void __clear(link_type x) {
            if(x == nullptr || x == header)    return;
            __clear(left(x));
            __clear(right(x));
            del_node(x);
        }
        inline void __rb_tree_rotateright(base_ptr x);
        inline void __rb_tree_rotateleft(base_ptr x);

        link_type __find(link_type cur, const key_type &v) const;

        int check_valid(base_ptr target) {
            // check whether the tree satisfy the constraint condition
            // used for debug
            if(target == nullptr) return 1;
            if(color(target) == __rb_tree_red) {
                assert(target->lch == nullptr || target->lch->color == __rb_tree_black);
                assert(target->rch == nullptr || target->rch->color == __rb_tree_black);
            }
            int lside = check_valid(target->lch);
            int rside = check_valid(target->rch);
            assert(lside == rside);
            return lside + (target->color == __rb_tree_black ? 1 : 0);
        }
        link_type __lower_bound(link_type &rt, const key_type &v) const;
        link_type __upper_bound(link_type &rt, const key_type &v) const;
    public:
        void check_valid() {
            check_valid(root());
        }
        rb_tree(): node_count(0) {
            init();
        }
        rb_tree(const self &rhs) {
            init();
            if(rhs.root() == nullptr) return;
            link_type new_root = create_node(value(rhs.root()));
            color(new_root) = color(rhs.root());
            root() = new_root;
            parent(new_root) = header;
            __copy(new_root, rhs.root());
            leftmost() = minimum(root());
            rightmost() = maximum(root());
            node_count = rhs.node_count;
        }
        rb_tree(const self &&rhs) {
            header = rhs.header;
            node_count = rhs.node_count;
        }
        self & operator = (const self &rhs) {
            clear();
            if(rhs.root() == nullptr) return *this;
            link_type new_root = get_node();
            left(new_root) = nullptr;
            right(new_root) = nullptr;
            value(new_root) = value(rhs.root());
            color(new_root) = color(rhs.root());
            root() = new_root;
            parent(new_root) = header;
            __copy(new_root, rhs.root());
            leftmost() = minimum(root());
            rightmost() = maximum(root());
            node_count = rhs.node_count;
            return *this;
        }
        self & operator = (const self &&rhs) {
            clear();
            del_node(header);
            header = rhs.header;
            node_count = rhs.node_count;
            return *this;
        }
        ~rb_tree() {
            clear();
            del_node(header);
            node_count = 0;
            header = nullptr;
            node_count = 0;
        }
        void clear() {
            __clear(root());
            leftmost() = header;
            rightmost() = header;
            color(header) = __rb_tree_red;
            node_count = 0;
            root() = nullptr;
        }
        size_type size() const {
            return node_count;
        }
        bool empty() const {
            return header->parent == nullptr;
        }
        iterator begin() const {
            return iterator(leftmost());
        }
        iterator end() const {
            return iterator(header);
        }
        pair<iterator, bool> insert_unique(const value_type &v);
        iterator insert_equal(const value_type &v);
        void remove(const key_type &v);
        iterator find(const key_type &v) const;
        iterator lower_bound(const key_type &v) const;
        iterator upper_bound(const key_type &v) const;
    };

    // for simplicity I declare some alias
    #define Type_def_header template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    #define Rb_tree_t rb_tree<Key, Value, KeyOfValue, Compare, Alloc>
    #define Rb_attr(attribute_name) Rb_tree_t:: attribute_name
    #define Rb_type(type_name) typename Rb_tree_t:: type_name

    Type_def_header
    const Compare Rb_attr(cmp) = Compare();

    Type_def_header
    const KeyOfValue Rb_attr(get_key) = KeyOfValue();

    Type_def_header
    inline void Rb_attr(__rb_tree_rotateleft) (Rb_type(base_ptr) x) {
        // x should never be the header
        link_type newroot = right(x);
        link_type pa_x = parent(x);
        link_type _x = link_type(x);
        right(x) = left(newroot);
        if(left(newroot) != nullptr)
            parent(left(newroot)) = _x;
        left(newroot) = _x;

        if(x == root()) {
            parent(newroot) = header;
            parent(x) = newroot;
            root() = newroot;
        }
        else if(x == left(pa_x)) {
            parent(newroot) = pa_x;
            parent(x) = newroot;
            left(pa_x) = newroot;
        }
        else {
            parent(newroot) = pa_x;
            parent(x) = newroot;
            right(pa_x) = newroot;
        }
    }
    Type_def_header
    inline void Rb_attr(__rb_tree_rotateright) (Rb_type(base_ptr) x) {
        link_type newroot = left(x);
        link_type pa_x = parent(x);
        link_type _x = link_type(x);
        left(x) = right(newroot);
        if(right(newroot) != nullptr)
            parent(right(newroot)) = _x;
        right(newroot) = _x;
        
        if(x == root()) {
            parent(newroot) = header;
            parent(x) = newroot;
            root() = newroot;
        }
        else if(x == left(pa_x)) {
            parent(newroot) = pa_x;
            parent(x) = newroot;
            left(pa_x) = newroot;
        }
        else {
            parent(newroot) = pa_x;
            parent(x) = newroot;
            right(pa_x) = newroot;
        }
    }
    Type_def_header
    void Rb_attr(__insert_rebalance_tree) (Rb_type(base_ptr) x, Rb_type(base_ptr) y) {
        // x points to a red node whose parent is a red node
        base_ptr grandpa;
        base_ptr uncle;
        while(x != header && color(y) == __rb_tree_red) {
            // the x is the root
            if(y == header) {
                color(x) = __rb_tree_black;
                break;
            }
            // because y is a red node, it must have a parent
            // because y is a red node, it's not the root
            grandpa = parent(y);
            if(y == left(grandpa)) {
                uncle = right(grandpa);
            }
            else {
                uncle = left(grandpa);
            }
            if(uncle != nullptr && color(uncle) == __rb_tree_red) {
                color(uncle) = __rb_tree_black;
                color(y) = __rb_tree_black;
                color(grandpa) = __rb_tree_red;
                x = grandpa;
                y = parent(x);
            }
            else {
                if(x == left(y)) {
                    if(y == left(grandpa)) {
                        color(y) = __rb_tree_black;
                        color(grandpa) = __rb_tree_red;
                        __rb_tree_rotateright(grandpa);
                    }
                    else {
                        color(x) = __rb_tree_black;
                        color(grandpa) = __rb_tree_red;
                        __rb_tree_rotateright(y);
                        __rb_tree_rotateleft(grandpa);
                    }
                }
                else {
                    if(y == right(grandpa)) {
                        color(y) = __rb_tree_black;
                        color(grandpa) = __rb_tree_red;
                        __rb_tree_rotateleft(grandpa);
                    }
                    else {
                        color(x) = __rb_tree_black;
                        color(grandpa) = __rb_tree_red;
                        __rb_tree_rotateleft(y);
                        __rb_tree_rotateright(grandpa);
                    }
                }
                // after these rotation, all the propertities are kept
                break;
            }
        } 
    }

    Type_def_header
    Rb_type(link_type) Rb_attr(__insert) (Rb_type(link_type) x, Rb_type(link_type) y, const Rb_type(value_type) &v) {
        link_type z;
        z = create_node(v);
        color(z) = __rb_tree_red;
        if(y == header || cmp(get_key(v), key(y))) {
            // if y == header, this will set the leftmost autonomously
            left(y) = z;
            if(y == header) {
                rightmost() = z;
                root() = z;
                // color(z) = __rb_tree_black;
                // if z is the root, the color will be set to black latter in the __insert_rebalance_tree
            }
            else if(y == leftmost()) {
                leftmost() = z;
            }
        }
        else {
            right(y) = z;
            if(y == rightmost()) {
                rightmost() = z;
            }
        }
        parent(z) = y;
        left(z) = nullptr;
        right(z) = nullptr;
        __insert_rebalance_tree(z, y);
        return z;
    }

    Type_def_header
    Rb_type(iterator) Rb_attr(insert_equal) (const Rb_type(value_type) &v) {
        link_type y = header;
        link_type x = root();
        while(x != nullptr) {
            y = x;
            x = cmp(get_key(v), key(x)) ? left(x) : right(x);
        }
        node_count++;
        return iterator(__insert(x, y, v));
    }
    Type_def_header
    pair<Rb_type(iterator), bool> Rb_attr(insert_unique) (const Rb_type(value_type) &v) {
        link_type y = header;
        link_type x = root();
        bool last_cmp = false;
        if(x == nullptr) {
            // root() == nullptr
            return pair<iterator, bool>(iterator(__insert(x, y, v)), true);
        }
        while(x != nullptr) {
            y = x;
            last_cmp = cmp(get_key(v), key(x));
            x = last_cmp ? left(x) : right(x);
        }
        iterator j = iterator(y);
        //determine whether there is already v in the tree
        if(last_cmp) {
            if(j == begin()) {
                node_count++;
                return pair<iterator, bool>(iterator(__insert(x, y, v)), true);
            }
            else {
                // the grandparent of j may be equal to the v
                j--;
            }
        }
        if(cmp(key(j.node), get_key(v))) {
            node_count++;
            return pair<iterator, bool>(iterator(__insert(x, y, v)), true);
        }
        return pair<iterator, bool>(j, false);
    }

	Type_def_header
	void Rb_attr(__remove_rebalance_tree) (Rb_type(base_ptr) cur, Rb_type(base_ptr) pa) {
        // cur points to a sub-tree whose black depth is one less than it's sibling sub-tree
        // originally cur is nullptr
        // pa points to the parent of cur
		base_ptr sib, sr, sl; // sib: sibling, sr: sibling's right child, sl: sibling's left child
		bool is_left, tmp_col;
		while(cur != root()) {
			is_left = left(pa) == cur;
			sib = is_left ? right(pa) : left(pa);
			// because the cur side's bd is one less than the sib side, so the sib must exist
			sl = left(sib);
			sr = right(sib);
			if(color(sib) == __rb_tree_red) {
				color(pa) = __rb_tree_red;
				color(sib) = __rb_tree_black;
				if(is_left) {
					__rb_tree_rotateleft(pa);
				}
				else {
					__rb_tree_rotateright(pa);
				}
			}
			else {
				if(is_left) {
					if(sr != nullptr && color(sr) == __rb_tree_red) {
						tmp_col = color(pa);
						color(pa) = color(sib);
						color(sib) = tmp_col;
						color(sr) = __rb_tree_black;
						__rb_tree_rotateleft(pa);
						break;
					}
					else if(sl != nullptr && color(sl) == __rb_tree_red) {
						color(sib) = __rb_tree_red;
						color(sl) = __rb_tree_black;
						__rb_tree_rotateright(sib);
					}
					else if(color(pa) == __rb_tree_red) {
						color(pa) = __rb_tree_black;
						color(sib) = __rb_tree_red;
						break;
					}
					else {
						color(sib) = __rb_tree_red;
						cur = pa;
						pa = parent(pa);
					}
				}
				else {
					if(sl != nullptr && color(sl) == __rb_tree_red) {
						tmp_col = color(pa);
						color(pa) = color(sib);
						color(sib) = tmp_col;
						color(sl) = __rb_tree_black;
						__rb_tree_rotateright(pa);
						break;
					}
					else if(sr != nullptr && color(sr) == __rb_tree_red) {
						color(sib) = __rb_tree_red;
						color(sr) = __rb_tree_black;
						__rb_tree_rotateleft(sib);
					}
					else if(color(pa) == __rb_tree_red) {
						color(pa) = __rb_tree_black;
						color(sib) = __rb_tree_red;
						break;
					}
					else {
						color(sib) = __rb_tree_red;
						cur = pa;
						pa = parent(pa);
					}
				}
			}
		}
	}

	Type_def_header
	void Rb_attr(remove) (const Rb_type(key_type) &v) {
		link_type cur = root();
		link_type tmp, p;
		bool cmp1, cmp2, is_left;
		while(cur != nullptr) {
			cmp1 = cmp(key(cur), get_key(v));
			cmp2 = cmp(get_key(v), key(cur));
			if(!cmp1 && !cmp2) {
                node_count--;
				if(right(cur) != nullptr) {
					tmp = link_type(__rb_tree_node_base::minimum(right(cur)));
					value(cur) = value(tmp);
					cur = tmp;
				}
                // now cur points to the node to be deleted
                // there must be a cur's child that's nullptr
				if(color(cur) == __rb_tree_red) {
					// cur can't be root
                    // if cur is a red, then simply delete it
					p = parent(cur);
					if(cur == left(p)) {
						left(p) = nullptr;
					}
					else {
						right(p) = nullptr;
					}
					del_node(cur);
					break;	
				}
				else if(left(cur) != nullptr || right(cur) != nullptr) {
                    // if cur has a child that's not nullptr, then that's child must be a red node
                    // so simply make that child a black node then delete cur
					// cur can be root, so we need to judge it
					is_left = left(cur) != nullptr;
					if(is_left) {
						tmp = left(cur);
					}
					else {
						tmp = right(cur);
					}
					if(cur == root()) {
						root() = tmp;
						parent(tmp) = header;
						leftmost() = link_type(__rb_tree_node_base::minimum(tmp));
						rightmost() = link_type(__rb_tree_node_base::maximum(tmp));
					}
					else {
						p = parent(cur);
						if(cur == left(p)) {
							left(p) = tmp;
						}
						else {
							right(p) = tmp;
						}
						parent(tmp) = p;
					}
					del_node(cur);
					color(tmp) = __rb_tree_black;
					break;
				}
				else {
					if(cur == root()) {
						clear();
						break;
					}
					else {
						p = parent(cur);
                        if(cur == left(p)) {
                            left(p) = nullptr;
                        }
                        else {
                            right(p) = nullptr;
                        }
						del_node(cur);
						__remove_rebalance_tree(nullptr, p);
                        break;
					}
				}
			}
			else if(cmp2) {
				cur = left(cur);
			}
			else {
				cur = right(cur);
			}
		}
	}
    Type_def_header
    Rb_type(link_type) Rb_attr(__copy) (Rb_type(link_type) dst, Rb_type(link_type) src) {
        // the new created node's child is not set to nullptr because the constructor of link_type finish this
        if(left(src) != nullptr) {
            left(dst) = create_node(value(left(src)));
            color(left(dst)) = color(left(src));
            parent(left(dst)) = dst;
            __copy(left(dst), left(src));
        }
        if(right(src) != nullptr) {
            right(dst) = create_node(value(right(src)));
            color(right(dst)) = color(right(src));
            parent(right(dst)) = dst;
            __copy(right(dst), right(src));
        }
        return dst;
    }

    Type_def_header
    Rb_type(link_type) Rb_attr(__find) (Rb_type(link_type) cur, const Rb_type(key_type) &v) const {
        if(cmp(v, key(cur))) {
            if(left(cur) == nullptr) return nullptr;
            else {
                return __find(left(cur), v);
            }
        }
        else {
            if(!cmp(key(cur), v)) return cur;
            else {
                if(right(cur) == nullptr)   return nullptr;
                else {
                    return __find(right(cur), v);
                }
            }
        }
        return nullptr;
    }

    Type_def_header
    Rb_type(iterator) Rb_attr(find) (const Rb_type(key_type) &v) const {
        if(root() == nullptr) {
            return end();
        }
        else {
            link_type target = __find(root(), v);
            if(target == nullptr) {
                return end();
            }
            return iterator(target);
        }
    }
    Type_def_header
    Rb_type(link_type) Rb_attr(__lower_bound) (link_type &rt, const Rb_type(key_type) &v) const {
        link_type cur = rt;
        link_type pos_ans = nullptr;
        while(cur != nullptr) {
            if(cmp(v, key(cur))) {
                pos_ans = cur;
                cur = left(cur);
            }
            else if(cmp(key(cur), v)) {
                cur = right(cur);                
            }
            else {
                if(left(cur) != nullptr) {
                    link_type l_tree = __lower_bound(left(cur), v);
                    if(l_tree != nullptr && key(l_tree) == v) {
                        return l_tree;
                    }
                    return cur;
                }
                else {
                    return cur;
                }
            }
        }
        return pos_ans;      // if none is greater than v, because pos_ans's initial value is header, so this will return end()
    }
    Type_def_header
    Rb_type(link_type) Rb_attr(__upper_bound) (link_type &rt, const Rb_type(key_type) &v) const {
        link_type cur = rt;
        while(cur != nullptr) {
            if(cmp(v, key(cur))) {
                if(left(cur) != nullptr) {
                    link_type l_tree = __upper_bound(left(cur), v);
                    if(l_tree != nullptr) {
                        return l_tree;
                    }
                    else {
                        return cur;
                    }
                }
                else {
                    return cur;
                }
            }
            else {
                cur = right(cur);
            }
        }
        return nullptr;       // if none is greater than v, because pos_ans's initial value is header, so this will return end() 
    } 
    Type_def_header
    Rb_type(iterator) Rb_attr(lower_bound) (const key_type &v) const {
        link_type res = __lower_bound(root(), v);
        if(res == nullptr) {
            return end();
        }
        else {
            return iterator(res);
        }
    }
    Type_def_header
    Rb_type(iterator) Rb_attr(upper_bound) (const key_type &v) const {
        link_type res = __upper_bound(root(), v);
        if(res == nullptr) {
            return end();
        }
        else {
            return iterator(res);
        }
    }

    #undef Rb_alias
    #undef Rb_tree_t
    #undef Type_def_header
};
