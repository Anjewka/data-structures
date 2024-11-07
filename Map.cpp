#include <bits/stdc++.h>
 
struct _Avl_tree_node_base {
    typedef _Avl_tree_node_base*     _Base_ptr;
    typedef _Avl_tree_node_base      base_type;
    typedef std::size_t              size_type;
 
    size_type  __height;
    _Base_ptr  _M_right, _M_left, _M_parent;
 
    _Avl_tree_node_base() noexcept
            : __height(0) 
            , _M_right(nullptr)
            , _M_left(nullptr)
            , _M_parent(nullptr) {}
};
 
struct _Avl_tree_header {
    _Avl_tree_node_base _header;
    std::size_t _size;
 
    _Avl_tree_header() noexcept {reset();}
 
    _Avl_tree_header(_Avl_tree_header&& __h) noexcept {
        if(__h._header._M_left) {
            move_data(__h);
        } else {
            reset();
        }
    }
 
    void move_data(_Avl_tree_header& __h) noexcept {
        _header._M_right = __h._header._M_right;
        _header._M_left = __h._header._M_left;
        if(_header._M_left) {
            _header._M_left->_M_parent = &_header;
        }
        if(_header._M_right) {
            _header._M_right->_M_parent == &__h._header ? 
                _header._M_right->_M_parent = &_header : 0;
        }
        _header.__height = __h._header.__height;
        _header._M_parent = &_header;
        _size = __h._size;
 
        __h.reset();
    }
 
    void reset() noexcept {
        _header._M_parent = &_header;
        _header._M_left = nullptr;
        _header._M_right = nullptr;
        _header.__height = 0;
        _size = 0;
    }
};
 
template<typename T>
struct _Avl_tree_node : public _Avl_tree_node_base {
    T _M_value;
 
    _Avl_tree_node()
            : _Avl_tree_node_base() {}
    _Avl_tree_node(const T& d) 
            : _Avl_tree_node_base()
            , _M_value(d) {}
 
    _Avl_tree_node(T&& d) : _M_value(std::move(d)) {}
 
    template<typename... Args>
    _Avl_tree_node(Args&&... __args) : _M_value(std::forward<Args>(__args)...) {}
 
    T* ptr() {return std::addressof(_M_value);}
 
    const T* ptr() const {return std::addressof(_M_value);}
};
 
struct _Avl_tree_helper {
    typedef std::size_t size_type;
 
    static void fix(_Avl_tree_node_base* p) noexcept {
        if(!p) {return;}
 
        size_type h1 = p->_M_right ? p->_M_right->__height : 0;
        size_type h2 = p->_M_left ? p->_M_left->__height : 0;
        p->__height = (h1 > h2 ? h1 : h2) + 1;
    }
 
    static int balance_factor(_Avl_tree_node_base* a) noexcept {
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
 
    static _Avl_tree_node_base* right_rotate(_Avl_tree_node_base* p) noexcept {
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
 
    static _Avl_tree_node_base* left_rotate(_Avl_tree_node_base* q) noexcept {
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
 
    static _Avl_tree_node_base* balance(_Avl_tree_node_base* p) noexcept {
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
 
    static _Avl_tree_node_base* minimum(_Avl_tree_node_base* p) noexcept {
        if(p->_M_left) { return minimum(p->_M_left); }
        return balance(p);
    }
 
    static _Avl_tree_node_base* delete_minimum(_Avl_tree_node_base* p) noexcept {
        if(!p->_M_left) { return p->_M_right; }
        p->_M_left = delete_minimum(p->_M_left);
        return balance(p);
    }
 
    static _Avl_tree_node_base* maximum(_Avl_tree_node_base* p) noexcept {
        if(p->_M_right) { return maximum(p->_M_right); }
        else { return balance(p); }
    }
 
    static _Avl_tree_node_base* delete_maximum(_Avl_tree_node_base* p) noexcept {
        if (!p->_M_right) { return p->_M_left; }
        p->_M_right = delete_maximum(p->_M_right);
        return balance(p);
    }
};
 
template<typename T>
struct _Avl_iterator_base {
    typedef _Avl_tree_node_base::_Base_ptr  _Base_ptr;
    typedef _Avl_tree_node<T>*              _Tree_ptr;
    typedef _Avl_tree_helper                tree_functions; 
 
    _Base_ptr _M_iterator;
 
    _Avl_iterator_base() noexcept 
            : _M_iterator() {}
    _Avl_iterator_base(_Base_ptr __p) noexcept 
            : _M_iterator(__p) {}
 
    _Base_ptr parent_node() noexcept {return _M_iterator->_M_parent;}
 
    void incr() noexcept {
        if (!_M_iterator || _M_iterator == _M_iterator->_M_parent) { return; }
        if (_M_iterator->_M_right) { _M_iterator = tree_functions::minimum(_M_iterator->_M_right); 
        } else {
            _Base_ptr tmp = _M_iterator;
            bool check = true;
            while(_M_iterator->_M_parent && _M_iterator->_M_parent->_M_left != _M_iterator) {
                if (_M_iterator->_M_parent) { _M_iterator = _M_iterator->_M_parent; }
                else { check = false; break; }
            }
            if(check) { _M_iterator = _M_iterator->_M_parent; }
            else { _M_iterator = tmp; }
        }
    }
 
    void decr() noexcept {
        if (!_M_iterator || _M_iterator == _M_iterator->_M_parent) { return; }
        if (_M_iterator->_M_left) {
            _M_iterator = tree_functions::maximum(_M_iterator->_M_left);
        }
        else {
            _Base_ptr tmp = _M_iterator;
            bool check = true;
            while (_M_iterator->_M_parent && _M_iterator->_M_parent->_M_right != _M_iterator) {
                if (_M_iterator->_M_parent) { _M_iterator = _M_iterator->_M_parent; }
                else { check = false; break; }
            }
            if (check) { _M_iterator = _M_iterator->_M_parent; }
            else { _M_iterator = tmp; }
        }
    }
 
    friend bool operator==(const _Avl_iterator_base& __x, const _Avl_iterator_base& __y) noexcept {
        return __x._M_iterator == __y._M_iterator;
    }
 
    friend bool operator!=(const _Avl_iterator_base& __x, const _Avl_iterator_base& __y) noexcept {
        return __x._M_iterator != __y._M_iterator;
    }
};
 
template<typename T>
struct _Avl_iterator {
private:
 
    void incr() noexcept {
        if (!_M_iterator || _M_iterator == _M_iterator->_M_parent) { return; }
        if (_M_iterator->_M_right) { _M_iterator = tree_functions::minimum(_M_iterator->_M_right); 
        } else {
            _Base_ptr tmp = _M_iterator;
            bool check = true;
            while(_M_iterator->_M_parent && _M_iterator->_M_parent->_M_left != _M_iterator) {
                if (_M_iterator->_M_parent) { _M_iterator = _M_iterator->_M_parent; }
                else { check = false; break; }
            }
            if(check) { _M_iterator = _M_iterator->_M_parent; }
            else { _M_iterator = tmp; }
        }
    }
 
    void decr() noexcept {
        if (!_M_iterator) { return; }
        if (_M_iterator->_M_left) {
            _M_iterator = tree_functions::maximum(_M_iterator->_M_left);
        }
        else {
            _Base_ptr tmp = _M_iterator;
            bool check = true;
            while (_M_iterator->_M_parent && _M_iterator->_M_parent->_M_right != _M_iterator) {
                if (_M_iterator->_M_parent) { _M_iterator = _M_iterator->_M_parent; }
                else { check = false; break; }
            }
            if (check) { _M_iterator = _M_iterator->_M_parent; }
            else { _M_iterator = tmp; }
        }
    }
 
public:
    typedef _Avl_tree_node_base::_Base_ptr  _Base_ptr;
    typedef _Avl_tree_node<T>*              _Tree_ptr;
    typedef _Avl_tree_helper                tree_functions; 
 
    _Base_ptr _M_iterator;
 
    _Base_ptr parent_node() noexcept {return _M_iterator->_M_parent;}
 
    typedef T val_type;
    typedef T& reference;
    typedef T* pointer;
 
    typedef _Avl_iterator<T>      iterator;
 
    _Avl_iterator() noexcept 
            : _M_iterator() {}
 
    _Avl_iterator(_Base_ptr __x) noexcept 
            : _M_iterator(__x) {}
 
    iterator& right() noexcept {
        if(this->_M_iterator) {this->_M_iterator = this->_M_iterator->_M_right;}
        return *this;
    }
 
    iterator& left() noexcept {
        if(this->_M_iterator) {this->_M_iterator = this->_M_iterator->_M_left;}
        return *this;
    }
 
    iterator& parent() noexcept {
        if(this->_M_iterator) {this->_M_iterator = this->_M_iterator->_M_parent;}
        return *this;
    }
 
    reference operator*() noexcept {return *static_cast<_Tree_ptr>(this->_M_iterator)->ptr();}
 
    pointer operator->() noexcept {return static_cast<_Tree_ptr>(this->_M_iterator)->ptr();}
 
    iterator& operator++() noexcept {
        this->incr();
        return *this;
    }
 
    iterator& operator--() noexcept {
        this->decr();
        return *this;
    }
 
    iterator operator++(int) noexcept {
        iterator __temp = *this;
        this->incr();
        return __temp;
    }
 
    iterator operator--(int) noexcept {
        iterator __temp = *this;
        this->decr();
        return __temp;
    }
 
    friend bool operator==(const iterator& __x, const iterator& __y) noexcept {
        return __x._M_iterator == __y._M_iterator;
    }
 
    friend bool operator!=(const iterator& __x, const iterator& __y) noexcept {
        return __x._M_iterator != __y._M_iterator;
    }
};
 
template<typename T>
struct _Avl_const_iterator {
private:
 
    void incr() noexcept {
        if (!_M_iterator || _M_iterator == _M_iterator->_M_parent) { return; }
        if (_M_iterator->_M_right) { _M_iterator = tree_functions::minimum(_M_iterator->_M_right); 
        } else {
            _Base_ptr tmp = _M_iterator;
            bool check = true;
            while(_M_iterator->_M_parent && _M_iterator->_M_parent->_M_left != _M_iterator) {
                if (_M_iterator->_M_parent) { _M_iterator = _M_iterator->_M_parent; }
                else { check = false; break; }
            }
            if(check) { _M_iterator = _M_iterator->_M_parent; }
            else { _M_iterator = tmp; }
        }
    }
 
    void decr() noexcept {
        if (!_M_iterator || _M_iterator == _M_iterator->_M_parent) { return; }
        if (_M_iterator->_M_left) {
            _M_iterator = tree_functions::maximum(_M_iterator->_M_left);
        }
        else {
            _Base_ptr tmp = _M_iterator;
            bool check = true;
            while (_M_iterator->_M_parent && _M_iterator->_M_parent->_M_right != _M_iterator) {
                if (_M_iterator->_M_parent) { _M_iterator = _M_iterator->_M_parent; }
                else { check = false; break; }
            }
            if (check) { _M_iterator = _M_iterator->_M_parent; }
            else { _M_iterator = tmp; }
        }
    }
 
public:
 
    typedef const _Avl_tree_node_base*      _Base_ptr;
    typedef const _Avl_tree_node<T>*        _Tree_ptr;
    typedef _Avl_tree_helper                tree_functions; 
 
    _Base_ptr _M_iterator;
 
    _Base_ptr parent_node() noexcept {return _M_iterator->_M_parent;}
 
    typedef T           val_type;
    typedef const T&    reference;
    typedef const T*    pointer;
 
    typedef _Avl_const_iterator<T>    const_iterator;
 
    _Avl_const_iterator() noexcept 
            : _M_iterator() {}
 
    _Avl_const_iterator(const _Base_ptr __x) noexcept 
            : _M_iterator(__x) {}
 
    _Avl_const_iterator(const _Avl_iterator<T>& __it) noexcept
            : _M_iterator(__it._M_iterator) {}
 
    const_iterator& right() noexcept {
        if(this->_M_iterator) {this->_M_iterator = this->_M_iterator->_M_right;}
        return *this;
    }
 
    const_iterator& left() noexcept {
        if(this->_M_iterator) {this->_M_iterator = this->_M_iterator->_M_left;}
        return *this;
    }
 
    const_iterator& parent() noexcept {
        if(this->_M_iterator) {this->_M_iterator = this->_M_iterator->_M_parent;}
        return *this;
    }
 
    reference operator*() const noexcept {return *static_cast<_Tree_ptr>(this->_M_iterator)->ptr();}
 
    pointer operator->() const noexcept {return static_cast<_Tree_ptr>(this->_M_iterator)->ptr();}
 
    const_iterator& operator++() noexcept {
        this->incr();
        return *this;
    }
 
    const_iterator& operator--() noexcept {
        this->decr();
        return *this;
    }
 
    const_iterator operator++(int) noexcept {
        const_iterator __temp = *this;
        this->incr();
        return __temp;
    }
 
    const_iterator operator--(int) noexcept {
        const_iterator __temp = *this;
        this->decr();
        return __temp;
    }
 
    friend bool operator==(const const_iterator& __x, const const_iterator& __y) noexcept {
        return __x._M_iterator == __y._M_iterator;
    }
 
    friend bool operator!=(const const_iterator& __x, const const_iterator& __y) noexcept {
        return __x._M_iterator != __y._M_iterator;
    }
};
 
template<typename _Comparator>
struct _Key_comparator {
    _Comparator _key_comp;
 
    _Key_comparator() noexcept : _key_comp() {}
 
    _Key_comparator(_Comparator& __c) : _key_comp(__c) {}
 
    _Key_comparator(const _Key_comparator& __c) = default;
 
    _Key_comparator(_Key_comparator&& __c) noexcept : _key_comp(__c._key_comp) {}
};
 
template<typename T, typename _Comp, typename _Alloc>
class Tree {
public:
    typedef _Avl_iterator<T>       iterator;
    typedef _Avl_const_iterator<T> const_iterator;
    typedef std::size_t            size_type;
    typedef Tree<T, _Comp, _Alloc>  _tree;
 
    typedef typename std::allocator_traits<_Alloc>::template 
        rebind_alloc<_Avl_tree_node<T>> _Node_alloc;
    typedef std::allocator_traits<_Node_alloc> _Alloc_traits;
 
    using tree_functions = _Avl_tree_helper;
 
    typedef _Avl_tree_node_base     _Base_node_type;
    typedef _Avl_tree_node<T>       _Node_type;
    typedef _Base_node_type*        _Base_node_ptr;
    typedef _Node_type*             _Node_ptr;
    typedef _Alloc                  allocator_type;
 
    _Node_alloc& _M_get_Node_allocator() noexcept {
        return _M_impl;
    }
 
    const _Node_alloc& _M_get_Node_allocator() const noexcept {
        return _M_impl;
    }
 
    allocator_type get_allocator() const noexcept {
        return allocator_type(_M_get_Node_allocator());
    }
 
    struct _Avl_tree_impl : public _Node_alloc, public _Key_comparator<_Comp>, public _Avl_tree_header {
        using key_compare = _Key_comparator<_Comp>;
 
        _Avl_tree_impl() noexcept : _Node_alloc() {}
 
        _Avl_tree_impl(const _Avl_tree_impl& __x) : _Node_alloc(__x)
                , key_compare(__x._key_comp), _Avl_tree_header() {}
 
        _Avl_tree_impl(_Avl_tree_impl&& __x) : _Avl_tree_header(std::move(__x)) {}
 
        _Avl_tree_impl(_Node_alloc&& __a) : _Node_alloc(std::move(__a)) {}
 
        _Avl_tree_impl(_Avl_tree_impl&& __x, _Node_alloc&& __a) : _Node_alloc(std::move(__a))
                , key_compare(std::move(__x)), _Avl_tree_header(std::move(__x)) {}
 
        _Avl_tree_impl(const _Comp& __c, _Node_alloc&& __a) : _Node_alloc(std::move(__a))
                , key_compare(std::move(__c)) {}  
    };
 
    _Avl_tree_impl _M_impl;
 
private:
 
    template<typename U>
    bool _M_compare(const U& __x, const U& __y) const noexcept {
        return _M_impl._key_comp(__x, __y);
    }
 
    _Base_node_ptr insert(_Base_node_ptr p, const T& __x) {
        if(!p) {
            _Node_ptr __new_node;
            try {
                __new_node = _Alloc_traits::allocate(_M_get_Node_allocator(), 1);
                ::new(__new_node) T(__x);
                _Alloc_traits::construct(_M_get_Node_allocator(), __new_node, __x);
            } catch(...) {
                _Alloc_traits::destroy(_M_get_Node_allocator(), __new_node);
                _Alloc_traits::deallocate(_M_get_Node_allocator(), __new_node, 1);
                throw;
            }
 
            if(!_M_impl._header._M_right || _M_compare(__x.first,
                    static_cast<_Node_ptr>(_M_impl._header._M_right)->_M_value.first))  {
                _M_impl._header._M_right = __new_node;
            }
 
            ++_M_impl._size;
 
            return __new_node;
        }
        else if(_M_compare(__x.first,
                    static_cast<_Node_ptr>(p)->_M_value.first)) {
            _Base_node_ptr child = insert(p->_M_left, __x);
            p->_M_left = child;
            child->_M_parent = p;
        }
        else if(_M_compare(static_cast<_Node_ptr>(p)->_M_value.first,
                    __x.first)) {
            _Base_node_ptr child = insert(p->_M_right, __x);
            p->_M_right = child;
            child->_M_parent = p;
        }
        return tree_functions::balance(p);
    }
 
    _Base_node_ptr remove(_Base_node_ptr pos, const T& __k) {
        if(!pos) {return pos;}
 
        if(_M_compare(static_cast<_Node_ptr>(pos)->_M_value.first, __k.first)) {
 
            _Base_node_ptr child = remove(pos->_M_right, __k);
            pos->_M_right = child;
            if(child) { child->_M_parent = pos; }
        }
        else if(_M_compare(__k.first, static_cast<_Node_ptr>(pos)->_M_value.first)) {
 
            _Base_node_ptr child = remove(pos->_M_left, __k);
            pos->_M_left = child;
            if(child) { child->_M_parent = pos; }
        }
        else {
            _Base_node_ptr left = pos->_M_left;
            _Base_node_ptr right = pos->_M_right;
 
            if(pos == _M_impl._header._M_right) {
                if(left) {_M_impl._header._M_right = left;}
                else {_M_impl._header._M_right = pos->_M_parent;}
            }
 
            _Alloc_traits::destroy(_M_get_Node_allocator(), static_cast<_Node_ptr>(pos));
            _Alloc_traits::deallocate(_M_get_Node_allocator(), static_cast<_Node_ptr>(pos), 1);
 
            if(!right) { --_M_impl._size; return left; }
            if(!left) { --_M_impl._size; return right; }
            _Base_node_ptr m = tree_functions::minimum(right);
            m->_M_right = tree_functions::delete_minimum(right);
            if(m->_M_right) { m->_M_right->_M_parent = m; }
            m->_M_left = left;
            if(left) { left->_M_parent = m; }
            --_M_impl._size;
            return tree_functions::balance(m);
        }
        return tree_functions::balance(pos);
    }
 
public:
 
    iterator find(const T& __k) {
        _Base_node_ptr _finder = _M_impl._header._M_left;
 
        while(_finder) {
            if(_M_compare(static_cast<_Node_ptr>(_finder)->_M_value.first, __k.first)) {
                _finder = _finder->_M_right;
            } else if(_M_compare(__k.first, static_cast<_Node_ptr>(_finder)->_M_value.first)) {
                _finder = _finder->_M_left;
            }
            else {return iterator(_finder);}
        }
        return end();
    }
 
    const_iterator find(const T& __k) const {
        _Base_node_ptr _finder = _M_impl._header._M_left;
 
        while(_finder) {
            if(_M_compare(static_cast<_Node_ptr>(_finder)->_M_value.first, __k.first)) {
                _finder = _finder->_M_right;
            } else if(_M_compare(__k.first, static_cast<_Node_ptr>(_finder)->_M_value.first)) {
                _finder = _finder->_M_left;
            }
            else {return const_iterator(_finder);}
        }
        return cend();
    }
 
    _Base_node_ptr _M_root() noexcept {
        return _M_impl._header._M_left;
    }
 
public:
    Tree() noexcept : _M_impl() {}
 
    Tree(const _Comp& __c, const allocator_type& __a = allocator_type()) 
            : _M_impl(__c, _Node_alloc(__a)) {}
 
    Tree(const Tree& __r) : _M_impl() {
        auto __i = __r.begin();
        while(__i != __r.end()) {
            insert(*__i);
            ++__i;
        }
    }
 
    Tree(const allocator_type& __a) : _M_impl(_Node_alloc(__a)) {}
 
    Tree(Tree&& __t) : _M_impl(std::move(__t._M_impl)) {}
 
    ~Tree() { clear(); }
 
    void clear() noexcept {
        remove_all_nodes(_M_impl._header._M_left);
        _M_impl.reset();
    }
 
    void remove_all_nodes(_Base_node_ptr __p) {
        if(!__p) {return;}
        remove_all_nodes(__p->_M_left);
        remove_all_nodes(__p->_M_right);
 
        _Alloc_traits::destroy(_M_get_Node_allocator(), __p);
        _Alloc_traits::deallocate(_M_get_Node_allocator(), static_cast<_Node_ptr>(__p), 1);
    }
 
    Tree& operator=(const Tree& __t) {
        clear();
 
        auto __i = __t.begin();
        while(__i != __t.end()) {
            insert(*__i);
            ++__i;
        }        
 
        return *this;
    }
 
    Tree operator=(Tree&& __t) {
        clear();
 
        _M_impl._header._M_right = __t._M_impl._header._M_right;
        _M_impl._header._M_left = __t._M_impl._header._M_left;
        _M_impl._header._M_left ? _M_impl._header._M_left->_M_parent = &_M_impl._header : _M_impl._header._M_left;
        _M_impl._size = __t._M_impl._size;
        _M_impl._header.__height == __t._M_impl._header.__height;
 
        __t._M_impl.reset();
    }
 
    size_type size() const noexcept { return _M_impl._size; }
 
    void insert(const T& __x) {
       _M_impl._header._M_left = insert(_M_impl._header._M_left, __x);
    }
 
    template<typename... Args>
    _Base_node_ptr emplace(Args&&... __args) {
        _Base_node_ptr _finder = _M_root();
        _Base_node_ptr _previous = nullptr;
 
        _Node_ptr __node;
        try {
            __node = _Alloc_traits::allocate(_M_get_Node_allocator(), 1);
            ::new(__node) T(std::forward<Args>(__args)...);
            _Alloc_traits::construct(_M_get_Node_allocator(), __node, std::forward<Args>(__args)...);
        } catch(...) {
            _Alloc_traits::destroy(_M_get_Node_allocator(), __node);
            _Alloc_traits::deallocate(_M_get_Node_allocator(), __node, 1);
            throw;
        }
 
        while(_finder) {
            if(_M_compare(static_cast<_Node_ptr>(_finder)->_M_value.first,
                    __node->_M_value.first)) {
                _previous = _finder;
                _finder = _finder->_M_right;
            } else if(_M_compare(__node->_M_value.first,
                    static_cast<_Node_ptr>(_finder)->_M_value.first)) {
                _previous = _finder;
                _finder = _finder->_M_left;
            } else {
                return _finder;
            }
        }
 
        ++_M_impl._size;
 
        if(_previous) {
            if(_M_compare(__node->_M_value.first,
                    static_cast<_Node_ptr>(_previous)->_M_value.first)) {
 
                _previous->_M_left = static_cast<_Base_node_ptr>(__node);
            } else if(_M_compare(static_cast<_Node_ptr>(_previous)->_M_value.first,
                    __node->_M_value.first)) {
 
                _previous->_M_right = static_cast<_Base_node_ptr>(__node);
            }
        }
 
        __node->_M_parent = _previous;
        _finder = __node;
        if(!_M_impl._header._M_right ||
                _M_compare(static_cast<_Node_ptr>(__node)->_M_value.first
                        , static_cast<_Node_ptr>(_M_impl._header._M_right)->_M_value.first)) {
            _M_impl._header._M_right = __node;
        }
 
        if(!_M_impl._header._M_left) {
            _finder = __node;
            _M_impl._header._M_left = __node;
            _M_impl._header._M_parent = &_M_impl._header;
            return _finder;
        }
 
        _Base_node_ptr __road_to_root = __node->_M_parent;
 
        while(_M_impl._header._M_left != __road_to_root) {
            __road_to_root = tree_functions::balance(__road_to_root);
            _Base_node_ptr __temp = __road_to_root;
 
            __road_to_root = __temp->_M_parent;
            if(__temp) {
                if(_M_compare(static_cast<_Node_ptr>(__temp)->_M_value.first
                        , static_cast<_Node_ptr>(__road_to_root)->_M_value.first)) {
 
                    __road_to_root->_M_left = __temp;
                } else if(_M_compare(static_cast<_Node_ptr>(__road_to_root)->_M_value.first
                        , static_cast<_Node_ptr>(__temp)->_M_value.first)) {
 
                    __road_to_root->_M_right = __temp;
                }
            }
 
        }
 
        _M_impl._header._M_left = tree_functions::balance(__road_to_root);
        _M_impl._header._M_left->_M_parent = &_M_impl._header;
 
        return _finder;
    }
 
    bool remove(const T& __x) {
        size_type previous = _M_impl._size;
        _M_impl._header._M_left = remove(_M_impl._header._M_left, __x);
        if(_M_impl._header._M_left) {
            _M_impl._header._M_left->_M_parent = &_M_impl._header;
        }
        return previous > _M_impl._size;
    }
 
public:
 
    iterator end() noexcept {
        return iterator(&_M_impl._header);
    }
 
    const_iterator end() const noexcept {
        return const_iterator(&_M_impl._header);
    }
 
    iterator begin() noexcept {
        return iterator(_M_impl._header._M_right);
    }
 
    const_iterator begin() const noexcept {
        return const_iterator(_M_impl._header._M_right);
    }
 
    const_iterator cend() const noexcept {
        return const_iterator(&_M_impl._header);
    }
 
    const_iterator cbegin() const noexcept {
        return const_iterator(_M_impl._header._M_right);
    }
 
    bool empty() const noexcept {
        return _M_impl._size == 0;
    }
};
 
template<typename T1,
    typename T2,
    typename _Compare = std::less<T1>,
    typename _Alloc = std::allocator<std::pair<const T1, T2>>>
class map {
public:
    typedef std::pair<const T1, T2>                             value_type;
    typedef std::size_t                                         size_type;
    typedef _Alloc                                              allocator_type;
 
private:
    typedef typename std::allocator_traits<_Alloc>::template 
            rebind_alloc<value_type>                            _Pair_alloc_type;
    typedef map<T1, T2, _Compare, _Pair_alloc_type>             _Map;
    typedef Tree<value_type, _Compare, _Pair_alloc_type>        _Tree_type;
 
    _Tree_type _M_t;
 
public:
    typedef typename _Tree_type::iterator       iterator;
    typedef typename _Tree_type::const_iterator const_iterator;
 
public:
    map() = default;
 
    map(const _Compare& __c, const allocator_type& __a = allocator_type()) 
            : _M_t(__c, _Pair_alloc_type(__a)) {}
 
    map(const map&) = default; 
 
    map(map&&) = default;
 
    explicit map(const allocator_type& __a) : _M_t(_Pair_alloc_type(__a)) {}
 
    map& operator=(const map&) = default;
 
    map& operator=(map&&) = default;
 
    allocator_type get_allocator() const noexcept {return allocator_type(_M_t.get_allocator());}
 
    _Tree_type get_M_t() const noexcept { return _M_t; }
 
    size_type size() const noexcept { return _M_t.size(); }
 
    void insert(const T1& _index, const T2& _x) {
        _M_t.insert(std::make_pair(_index, _x));
    }
 
    template<typename... Args>    
    iterator emplace(Args&&... __args) {
        return _M_t.emplace(std::forward<Args>(__args)...);
    }
 
    iterator insert(const std::pair<T1, T2>& __p) {
        return _M_t.emplace(__p);
    }
 
    iterator insert(std::pair<T1, T2>&& __p) {
        return _M_t.emplace(std::move(__p));
    } 
 
    bool erase(const T1& key) noexcept {
        return _M_t.remove(value_type(key, T2()));
    }
 
    template<typename... Args>
    T2& operator[](Args&&... keys) {
        iterator __i = _M_t.emplace(std::forward<Args>(keys)..., T2());
        return (*__i).second; 
    }
 
    void clear() noexcept {_M_t.clear();}
 
    bool empty() const noexcept {return _M_t.empty();}
 
    iterator find(const T1& key) noexcept {return _M_t.find(value_type(key, T2()));} 
 
    const_iterator find(const T1& key) const noexcept {return _M_t.find(value_type(key, T2()));} 
 
    iterator begin() noexcept {
        return _M_t.begin();
    }
 
    const_iterator begin() const noexcept {
        return _M_t.begin();
    }
 
    iterator end() noexcept {
        return _M_t.end();
    }
 
    const_iterator end() const noexcept {
        return _M_t.end();
    }
 
    const_iterator cbegin() const noexcept {
        return _M_t.cbegin();
    }
 
    const_iterator cend() const noexcept {
        return _M_t.cend();
    }
 
    auto root() {
        return _M_t._M_impl._header._M_left;
    }
};