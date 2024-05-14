#include <bits/stdc++.h>

using namespace std::chrono;

struct _Avl_tree_node_base {
    typedef _Avl_tree_node_base*     _Base_ptr;
    typedef _Avl_tree_node_base      base_type;
    typedef std::size_t              size_type;


    size_type  __height;
    size_type  __tmp_h;
    _Base_ptr  _M_right, _M_left, _M_parent;

    _Avl_tree_node_base() : __height(0) , __tmp_h(0), _M_right(nullptr), _M_left(nullptr), _M_parent(nullptr) {}

    _Avl_tree_node_base(size_type __height) : __height(__height) , __tmp_h(0), _M_right(nullptr), _M_left(nullptr), _M_parent(nullptr) {}

    const base_type& operator=(const base_type& other) {
        _M_right = other._M_right;
        _M_left = other._M_left;
        _M_parent = other._M_left;
        __height = other.__height;
        __tmp_h = other.__tmp_h;
        return *this;
    }

    virtual ~_Avl_tree_node_base() {_M_right = _M_left = _M_parent = nullptr; }
};

template<class T>
struct _Avl_tree_node : public _Avl_tree_node_base {
    T _M_value;

    _Avl_tree_node() : _M_value(0) {}
    _Avl_tree_node(const T& d) : _Avl_tree_node_base(1), _M_value(d) {}

    T* ptr() {return std::addressof(_M_value);}

    const T* ptr() const {return std::addressof(_M_value);}

    const _Avl_tree_node& operator=(const _Avl_tree_node& other) {
        _M_value = other._M_value;
        _M_right = other._M_right;
        _M_left = other._M_left;
        _M_parent = other._M_left;
        __height = other.__height;
        __tmp_h = other.__tmp_h;
        return *this;
    }
};

struct _Avl_tree_helper {
    typedef std::size_t size_type;

    static void fix(_Avl_tree_node_base* p) {
        if(!p) {return;}

        size_type h1 = p->_M_right ? p->_M_right->__height : 0;
        size_type h2 = p->_M_left ? p->_M_left->__height : 0;
        p->__height = (h1 > h2 ? h1 : h2) + 1;
    }

    static int balance_factor(_Avl_tree_node_base* a) {
        if(!a) {return 0;}

        if (a->_M_right && a->_M_left) {
            return a->_M_right->__height - a->_M_left->__height;
        } else if (a->_M_right) {
            return a->_M_right->__height;
        } else if (a->_M_left) {
            return -1 * (a->_M_left->__height);
        }

        return 0;
    }

    static _Avl_tree_node_base* right_rotate(_Avl_tree_node_base* p) {
        if(!p) {return p;}
        _Avl_tree_node_base* q = p->_M_left;
        p->_M_left = q->_M_right;
        q->_M_right = p;
        if(q) {q->_M_parent = p->_M_parent;}
        if(p->_M_left) {p->_M_left->_M_parent = p;}

        p->_M_parent = q;

        fix(p);
        fix(q);

        return q;
    }

    static _Avl_tree_node_base* left_rotate(_Avl_tree_node_base* q) {
        if(!q) {return q;}
        _Avl_tree_node_base* p = q->_M_right;
        q->_M_right = p->_M_left;
        p->_M_left = q;
        if(p) {p->_M_parent = q->_M_parent;}
        if(q->_M_right) {q->_M_right->_M_parent = q;}
        q->_M_parent = p;

        fix(q);
        fix(p);

        return p;
    }

    static _Avl_tree_node_base* balance(_Avl_tree_node_base* p) {
        if(!p) {return nullptr;}

        fix(p);

        if(balance_factor(p) > 1) {
            if(balance_factor(p->_M_right) < 0) {
                _Avl_tree_node_base* child = right_rotate(p->_M_right);
                p->_M_right = child;
                if(child) {child->_M_parent = p;}
            }
            return left_rotate(p);
        }

        if(balance_factor(p) < -1)
        {
            if(balance_factor(p->_M_left) > 0) {
                _Avl_tree_node_base* child = left_rotate(p->_M_left);
                p->_M_left = child;
                if(child) {child->_M_parent = p;}
            }
            return right_rotate(p);
        }
        return p;
    }

    static _Avl_tree_node_base* minimum(_Avl_tree_node_base* p) {
        if(p->_M_left) { return minimum(p->_M_left); }
        return balance(p);
    }

    static _Avl_tree_node_base* delete_minimum(_Avl_tree_node_base* p) {
        if(!p->_M_left) { return p->_M_right; }
        p->_M_left = delete_minimum(p->_M_left);
        return balance(p);
    }

    static _Avl_tree_node_base* maximum(_Avl_tree_node_base* p) {
        if(p->_M_right) { return maximum(p->_M_right); }
        else { return balance(p); }
    }

    static _Avl_tree_node_base* delete_maximum(_Avl_tree_node_base* p) {
        if (!p->_M_right) { return p->_M_left; }
        p->_M_right = delete_maximum(p->_M_right);
        return balance(p);
    }
};

template<typename T>
struct _Avl_iterator {
    typedef T val_type;
    typedef T& reference;
    typedef T* pointer;

    typedef _Avl_tree_node_base::_Base_ptr _Base_ptr;
    typedef _Avl_tree_node<T>* _Tree_ptr;

    using tree_functions = _Avl_tree_helper;

    typedef _Avl_iterator<T> iterator;

    _Base_ptr _node;



    _Avl_iterator() : _node() {}

    _Avl_iterator(_Base_ptr __x) : _node(__x) {}

    _Base_ptr node_ptr() {return _node;}

    _Base_ptr parent_node() {return _node->_M_parent;}

    reference operator*() {return *static_cast<_Tree_ptr>(_node)->ptr();}

    pointer operator->() {return static_cast<_Tree_ptr>(_node)->ptr();}

    const iterator& operator++() {
        if (!_node) { return *this; }

        if (_node->_M_right) { _node = tree_functions::minimum(_node->_M_right); 
        } else {
            _Base_ptr tmp = _node;
            bool check = true;
            while(_node->_M_parent && _node->_M_parent->_M_left != _node) {
                if (_node->_M_parent) { _node = _node->_M_parent; }
                else { check = false; break; }
            }
            if(check) { _node = _node->_M_parent; }
            else { _node = tmp; }
        }
        return *this;
    }

    const iterator& operator--() {
        if (_node->_M_left) {
            _node = tree_functions::maximum(_node->_M_left);
        }
        else {
            _Base_ptr tmp = _node;
            bool check = true;
            while (_node->_M_parent && _node->_M_parent->_M_right != _node) {
                if (_node->_M_parent) { _node = _node->_M_parent; }
                else { check = false; break; }
            }
            if (check) { _node = _node->_M_parent; }
            else { _node = tmp; }
        }
        return *this;
    }

    const iterator& right() {
        if(_node) {_node = _node->_M_right;}
        return *this;
    }

    const iterator& left() {
        if(_node) {_node = _node->_M_left;}
        return *this;
    }

    const iterator& parent() {
        if(_node) {_node = _node->_M_parent;}
        return *this;
    }

    bool operator==(const iterator& other) {return _node == other._node;}

    bool operator!=(const iterator& other) {return _node != other._node;}
};

template<typename T1, typename T2>
struct comparator {
    typedef std::pair<T1, T2> __pair_type;

    bool operator()(const __pair_type& __x, const __pair_type& __y) {
        return __x.first < __y.first;
    }
};

template<typename T, typename _Comp, typename alloc>
class Tree {
public:
    typedef _Avl_iterator<T> iterator;

private:
    typename alloc::template rebind<_Avl_tree_node<T>>::other _allocator;
    typedef typename alloc::template rebind<_Avl_tree_node<T>>::other _alloc_type;
    using _Alloc_traits = std::allocator_traits<_alloc_type>;
    using tree_functions = _Avl_tree_helper;

    typedef _Avl_tree_node_base*    _Base_node_ptr;
    typedef _Avl_tree_node<T>*      _node_ptr;
    typedef _Comp                   _Key_compare;
    typedef std::size_t             size_type;


    _Base_node_ptr root;
    _Base_node_ptr first;
    std::size_t _size;

    bool _M_compare(const T& __x, const T& __y) const {
        return _Key_compare{}(__x, __y);
    }

    _Base_node_ptr insert(_Base_node_ptr p, const T& __x) {
        if(!p) {
            _node_ptr _new_node = _Alloc_traits::allocate(_allocator, 1);
            _Alloc_traits::construct(_allocator, _new_node, (__x));
            ++_size;

            return _new_node;
        }
        else if(_M_compare(__x,
                    static_cast<_node_ptr>(p)->_M_value)) {
            _Base_node_ptr child = insert(p->_M_left, __x);
            p->_M_left = child;
            child->_M_parent = p;
        }
        else if(_M_compare(static_cast<_node_ptr>(p)->_M_value,
                    __x)) {
            _Base_node_ptr child = insert(p->_M_right, __x);
            p->_M_right = child;
            child->_M_parent = p;
        }
        return tree_functions::balance(p);
    }

    _Base_node_ptr iterating_insert(const T& __x) {
        _Base_node_ptr _finder = root->_M_left;
        _Base_node_ptr _previous = nullptr;

        while(_finder) {
            if(_M_compare(static_cast<_node_ptr>(_finder)->_M_value,
                    __x)) {
                _previous = _finder;
                _finder = _finder->_M_right;
            } else if(_M_compare(__x,
                    static_cast<_node_ptr>(_finder)->_M_value)) {
                _previous = _finder;
                _finder = _finder->_M_left;
            } else {
                return _finder;
            }
        }

        _node_ptr __node = _Alloc_traits::allocate(_allocator, 1);
        ::new(__node) T(__x);
        _Alloc_traits::construct(_allocator, __node, __x);
        ++_size;

        if(_previous) {
            if(_M_compare(__x,
                    static_cast<_node_ptr>(_previous)->_M_value)) {

                _previous->_M_left = static_cast<_Base_node_ptr>(__node);
            } else if(_M_compare(static_cast<_node_ptr>(_previous)->_M_value,
                    __x)) {

                _previous->_M_right = static_cast<_Base_node_ptr>(__node);
            }
        }

        __node->_M_parent = _previous;
        _finder = __node;
        if(first == nullptr ||
                static_cast<_node_ptr>(first)->_M_value > static_cast<_node_ptr>(_finder)->_M_value) {
            first = _finder;
        }

        if(!root->_M_left) {
            _finder = __node;
            root->_M_left = __node;
            return _finder;
        }

        _Base_node_ptr road_to_root = __node->_M_parent;

        while(root->_M_left != road_to_root) {
            road_to_root = tree_functions::balance(road_to_root);
            _Base_node_ptr temp = road_to_root;

            road_to_root = temp->_M_parent;
            if(temp) {
                if(_M_compare(static_cast<_node_ptr>(temp)->_M_value
                        , static_cast<_node_ptr>(road_to_root)->_M_value)) {

                    road_to_root->_M_left = temp;
                } else if(_M_compare(static_cast<_node_ptr>(road_to_root)->_M_value
                        , static_cast<_node_ptr>(temp)->_M_value)) {

                    road_to_root->_M_right = temp;
                }
            }

        }

        root->_M_left = tree_functions::balance(road_to_root);
        root->_M_left->_M_parent = root;
        return _finder;
    }

    _Base_node_ptr remove(_Base_node_ptr pos, const T& __k) {
        if(!pos) {return pos;}

        if(_M_compare(static_cast<_node_ptr>(pos)->_M_value, __k)) {

            _Base_node_ptr child = remove(pos->_M_right, __k);
            pos->_M_right = child;
            if(child) { child->_M_parent = pos; }
        }
        else if(_M_compare(__k, static_cast<_node_ptr>(pos)->_M_value)) {

            _Base_node_ptr child = remove(pos->_M_left, __k);
            pos->_M_left = child;
            if(child) { child->_M_parent = pos; }
        }
        else {
            _Base_node_ptr left = pos->_M_left;
            _Base_node_ptr right = pos->_M_right;

            if(pos == first) {
                if(left) {first = left;}
                else {first = pos->_M_parent;}
            }
            pos->_M_right = nullptr;
            pos->_M_left = nullptr;
            pos->_M_parent = nullptr;

            _Alloc_traits::destroy(_allocator, static_cast<_node_ptr>(pos));
            _Alloc_traits::deallocate(_allocator, static_cast<_node_ptr>(pos), 1);

            if(!right) { --_size; return left; }
            if(!left) { --_size; return right; }
            _Base_node_ptr m = tree_functions::minimum(right);
            m->_M_right = tree_functions::delete_minimum(right);
            if(m->_M_right) { m->_M_right->_M_parent = m; }
            m->_M_left = left;
            if(left) { left->_M_parent = m; }
            --_size;
            return tree_functions::balance(m);
        }
        return tree_functions::balance(pos);
    }

public:

    iterator find(const T& __k) {
        iterator _finder = root->_M_left;

        while(_finder.node_ptr()) {
            if(_M_compare(*_finder, __k)) {
                _finder.right();
            } else if(_M_compare(__k, *_finder)) {
                _finder.left();
            }
            else {return _finder;}
        }
        return iterator(root);
    }

public:
    Tree() : _size(0) {
        root = static_cast<_Base_node_ptr>(_Alloc_traits::allocate(_allocator, 1));
        _Alloc_traits::construct(_allocator, static_cast<_Base_node_ptr>(root), 0);
        first = root;
    }

    size_type size() const { return _size; }

    iterator insert(const T& __x) {
        return iterator(iterating_insert(__x));
    }

    bool remove(const T& __x) {
        size_type previous = _size;
        root->_M_left = remove(root->_M_left, __x);
        if(root->_M_left) {
            root->_M_left->_M_parent = root;
        }
        return previous > _size;
    }

    _node_ptr get_root() { return static_cast<_node_ptr>(root->_M_left); }

    void set_root(_Base_node_ptr r) { root = r; }

public:

    iterator end() {return root;}

    // iterator begin() {return first;}
    iterator begin() {return tree_functions::minimum(root->_M_left);}

};

template<typename T1,
    typename T2,
    typename _Compare = comparator<const T1, T2>,
    typename _Alloc = std::allocator<std::pair<const T1, T2>>>
class map
{
    typedef std::pair<const T1, T2>         type;
    typedef _Compare                        key_compare;
    typedef Tree<type, key_compare, _Alloc> _Tree_type;
    typedef std::size_t                     size_type;

    _Tree_type _M_t;

public:
    typedef typename Tree<type, key_compare, _Alloc>::iterator iterator;

public:
    map() : _M_t() {}

    _Tree_type get_M_t() { return _M_t; }

    size_type size() const { return _M_t.size(); }

    iterator insert(const T1& _index, const T2& _x) {return _M_t.insert(std::make_pair(_index, _x));}

    bool erase(const T1& key) {return _M_t.remove(type(key, T2()));}

    T2& operator[](const T1& key) {
        type p(key, T2());
        iterator __i = _M_t.insert(p);
        return (*__i).second; 
    }

    iterator find(const T1& key) {return _M_t.find(type(key, T2()));} 

    void operator=(const map<T1, T2>& m)
    {
        _M_t = m._M_t;
    }

    iterator begin() {return _M_t.begin();}

    iterator end() {return _M_t.end();}
};