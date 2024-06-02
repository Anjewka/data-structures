#include <bits/stdc++.h>

#define _BUF_SIZE 512
std::size_t node_size(std::size_t s) {
    return (s < _BUF_SIZE ? std::size_t(_BUF_SIZE / s) : std::size_t(1));
}

template<typename T, typename _Reference, typename _Pointer>
struct _Deque_iterator {

    typedef _Deque_iterator<T, T&, T*>              iterator;
    typedef _Deque_iterator<T, const T&, const T*>  const_iterator;
    typedef T*                    _Type_ptr;
    typedef T**                   _Map_ptr;
    using difference_type   = std::ptrdiff_t;

public:

    typedef _Pointer         pointer;
    typedef _Reference       reference;
    typedef std::size_t      size_type;
    typedef T                value_type;

    _Map_ptr node;
    _Type_ptr first;
    _Type_ptr last;
    _Type_ptr cur;

    _Deque_iterator() noexcept
      : node(), first(), last(), cur() { }

    void _M_set_node(_Map_ptr __new_node) {
	    node = __new_node;
        first = *__new_node;
	    last = first + difference_type(node_size(sizeof(T)));
    }

    _Deque_iterator& operator++() {
        ++cur;

        if (cur == last) {
            _M_set_node(node + 1);
            cur = first;
        }
        return *this;
    }

    _Deque_iterator& operator--() {
        if (cur == first) {
            _M_set_node(node - 1);
            cur = last;
        }
        --cur;
        return *this;
    }

    _Deque_iterator operator+=(size_type __n) {
        if(__n <= last - cur) {
            cur += __n;
            return *this;
        }
        __n -= last - cur; 
        size_type number_node = __n / node_size(sizeof(T)) + 1;
        _M_set_node(node + number_node);
        cur = first + __n % node_size(sizeof(T));

        return *this;
    }

    _Deque_iterator operator-=(size_type __n) {
        if(__n <= cur - first) {
            cur -= __n;
            return *this;
        }
        __n -= cur - first; 
        size_type number_node = __n / node_size(sizeof(T)) + 1;
        _M_set_node(node - number_node);
        cur = last - __n % node_size(sizeof(T));

        return *this;
    }

    _Deque_iterator operator+(size_type __n) {
        _Deque_iterator __temp = *this;
        __temp += __n;
        return __temp;
    }

    reference operator*() { return *cur; }

    pointer operator->() { return cur; }

    friend bool operator==(const _Deque_iterator<T, _Reference, _Pointer> __l
            , const _Deque_iterator<T, _Reference, _Pointer> __r) {
        return __l.cur == __r.cur; 
    }

    friend bool operator!=(const _Deque_iterator<T, _Reference, _Pointer> __l
            , const _Deque_iterator<T, _Reference, _Pointer> __r) {
        return __l.cur != __r.cur; 
    }

    friend iterator operator+(const _Deque_iterator<T, _Reference, _Pointer>& __r, difference_type __n) {
        _Deque_iterator<T, _Reference, _Pointer> temp = __r;
        temp += __n;
        return temp;
    }

};

template<typename T, typename _Alloc>
class Deque_base {
public:
    typedef T*           _ptr;
    typedef const T*     _const_ptr;
    typedef std::size_t  size_type;
    typedef _Deque_iterator<T, T&, T*>             iterator;
    typedef _Deque_iterator<T, const T&, const T*> const_iterator;
    typedef typename iterator::_Map_ptr _Map_ptr;

    typename _Alloc::template rebind<T>::other _allocator;
    typedef typename _Alloc::template rebind<T>::other _alloc_type;

    typename _Alloc::template rebind<_ptr>::other _map_allocator;
    typedef typename _Alloc::template rebind<_ptr>::other _map_alloc_type;

    using _Alloc_traits = std::allocator_traits<_alloc_type>;
    using _Malloc_traits = std::allocator_traits<_map_alloc_type>;

    struct _Impl {
        _Map_ptr _M_map;
        size_type _size;
        iterator beg;
        iterator _end;

        _Impl() noexcept : _M_map(), _size(), beg(), _end() {}

        _Impl(_Impl&& __impl) : _M_map(__impl._M_map)
                    , _size(__impl._size)
                    , beg(__impl.beg)
                    , _end(__impl._end) {
            __impl._M_map = nullptr;
            __impl._size = 0;
            __impl.beg = iterator();
            __impl._end = iterator();
        }

        _Impl(const _Impl& __impl) = default;
    };

    _Impl _M_impl;

    void _M_initialize_map(size_type __n) {
        size_type nodes_count = (__n / node_size(sizeof(T)) + 1); 

        _M_impl._size = std::max(static_cast<size_type>(8), size_type(nodes_count + 2));
        _M_impl._M_map = allocate_map(_M_impl._size);

        _Map_ptr __nstart = (_M_impl._M_map + (_M_impl._size - nodes_count) / 2);
        _Map_ptr __n_end = __nstart + nodes_count;

        try {
            _M_create_nodes(__nstart, __n_end);
        } catch(...) {
            _Malloc_traits::deallocate(_map_allocator, _M_impl._M_map, _M_impl._size);
            _M_impl._M_map = _Map_ptr();
            _M_impl._size = 0;
            throw ;
        }

        _M_impl.beg._M_set_node(__nstart);
        _M_impl._end._M_set_node(__n_end - 1);
        _M_impl.beg.cur = _M_impl.beg.first;
        _M_impl._end.cur = (_M_impl._end.first + __n % node_size(sizeof(T)));
    }

    void _M_destroy_nodes(_Map_ptr __nstart, _Map_ptr __n_end) {
        for(_Map_ptr __n = __nstart; __n < __n_end; ++__n) {
            _Alloc_traits::deallocate(_allocator, *__n, node_size(sizeof(T)));
        }
    }

    void _M_create_nodes(_Map_ptr __nstart, _Map_ptr __n_end) {
        _Map_ptr __cur;
        try {
	        for (__cur = __nstart; __cur < __n_end; ++__cur) {
                *__cur = _Alloc_traits::allocate(_allocator,  node_size(sizeof(T)));
            }
	    } catch (...) {
            _M_destroy_nodes(__nstart, __cur);
            throw ;
        }
    }

    _Map_ptr allocate_map(size_type __s) {
        return _Malloc_traits::allocate(_map_allocator, __s);
    }

    void deallocate_map(_Map_ptr __p, size_type __s) {
        _Malloc_traits::deallocate(_map_allocator, __p, __s);
    }

    void reallocate(size_type __add, bool __add_at_front) {
        _Map_ptr new_map;
        size_type prev_size = _M_impl._end.node - _M_impl.beg.node + 1;
        size_type new_size = __add + prev_size + 1;

        new_map = allocate_map(new_size);

        if(!__add_at_front) {__add = 0;}
        std::copy(_M_impl.beg.node, _M_impl._end.node + 1, new_map + __add);

        deallocate_map(_M_impl._M_map, prev_size);
        _M_impl._M_map = new_map;
        _M_impl._end.iterator::_M_set_node(new_map + prev_size + __add - 1);
        _M_impl.beg.iterator::_M_set_node(new_map + __add);

        _M_impl._size = new_size;
    }

    template<typename... Args>
    void emplace_back(Args&&... _args) {
        if(_M_impl._end.cur != _M_impl._end.last) {
            _Alloc_traits::construct(_allocator
                        , _M_impl._end.cur
                        , std::forward<Args>(_args)...);
            ++_M_impl._end.cur;
            return;
        }

        reserve_at_back();

        _Map_ptr after_end = _M_impl._end.node + 1;
        *after_end = _Alloc_traits::allocate(_allocator, node_size(sizeof(T)));

        try {
            _Alloc_traits::construct(_allocator
                        , _M_impl._end.cur
                        , std::forward<Args>(_args)...);
            _M_impl._end._M_set_node(after_end);
            _M_impl._end.cur = _M_impl._end.first;
        } catch(...) {
            --_M_impl._end;
            _Alloc_traits::deallocate(_allocator, *(_M_impl._end.node + 1),
            node_size(sizeof(T)));            
            throw;
        }
    }

    template<typename... Args>
    void emplace_front(Args&&... _args) {
        if(_M_impl.beg.cur != _M_impl.beg.first) {
            _Alloc_traits::construct(_allocator,
                        _M_impl.beg.cur - 1,
                        std::forward<Args>(_args)...);
            --_M_impl.beg.cur;
            return;
        }

        reserve_at_front();

        _Map_ptr before_begin = _M_impl.beg.node - 1;
        *before_begin = _Alloc_traits::allocate(_allocator, node_size(sizeof(T)));

        try {
            _M_impl.beg._M_set_node(before_begin);
            _M_impl.beg.cur = _M_impl.beg.last - 1;
            _Alloc_traits::construct(_allocator
                        , _M_impl.beg.cur
                        , std::forward<Args>(_args)...);
        } catch(...) {
            ++_M_impl.beg;
            _Alloc_traits::deallocate(_allocator, *(_M_impl.beg.node - 1),
            node_size(sizeof(T)));
            throw;
        }
    }

    void push_back(const T& __x) {  
        emplace_back(__x);
    }

    void push_front(const T& __x) {
        emplace_front(__x);
    }

    void push_back(T&& __x) {
        emplace_back(std::move(__x));
    }

    void push_front(T&& __x) {
        emplace_front(std::move(__x));
    }

    void reserve_at_front(std::size_t __add = 1)  {
	    if(__add > size_t(_M_impl.beg.node - _M_impl._M_map)) {
            reallocate(__add, true);
        }
    }

    void reserve_at_back(std::size_t __add = 1) {
        if(__add + 1 > _M_impl._size - (_M_impl._end.node - _M_impl._M_map)) {
            reallocate(__add, false);
        }
    }

    void pop_back() noexcept {
        if(_M_impl._end.cur == _M_impl._end.first) {
            _Alloc_traits::deallocate(_allocator, _M_impl._end.first, node_size(sizeof(T)));
            _Alloc_traits::destroy(_allocator, _M_impl._end.cur);
            _M_impl._end._M_set_node(_M_impl._end.node - 1);
            _M_impl._end.cur = _M_impl._end.last - 1;
            return;
        }

        auto last = _M_impl._end.cur;
        --_M_impl._end.cur;
        _Alloc_traits::destroy(_allocator, last);
    }

    void pop_front() noexcept {
        if(_M_impl.beg.cur == _M_impl.beg.last) {
            _Alloc_traits::deallocate(_allocator, _M_impl.beg.first, node_size(sizeof(T)));
            _Alloc_traits::destroy(_allocator, _M_impl.beg.cur);
            _M_impl.beg._M_set_node(_M_impl.beg.node + 1);
            _M_impl.beg.cur = _M_impl.beg.first + 1;
            return;
        }

        auto _begin = _M_impl.beg.cur;
        ++_M_impl.beg.cur;
        _Alloc_traits::destroy(_allocator, _begin);
    }

    Deque_base() : _M_impl() {_M_initialize_map(0);}

    Deque_base(size_type __c) : _M_impl() {_M_initialize_map(__c);}

    Deque_base(const Deque_base& __d) : _M_impl() {
        _M_initialize_map(0);
        auto i = __d._M_impl.beg;
        while(i != __d._M_impl._end) {
            push_back(*i);
            ++i;
        }
    }

    Deque_base(Deque_base&& __d) : _M_impl(std::move(__d._M_impl)) {__d._M_initialize_map(0);}
};

template<typename T, typename _Alloc = std::allocator<T>>
class deque : public Deque_base<T, _Alloc> {
public:
    typedef Deque_base<T, _Alloc> _base;

    using _base::_ptr;
    using _base::_const_ptr;

    typedef typename _base::size_type size_type;
    typedef typename _base::iterator  iterator;
    typedef typename _base::iterator  const_iterator;

    deque() = default;

    deque(size_type __c) : Deque_base<T, _Alloc>(__c) {}

    deque(deque&& __d) : _base(std::move(__d)) {}

    deque(const deque& __d) = default;

    void push_front(const T& __x) {
        _base::push_front(__x);
    }

    void push_back(const T& __x) {
        _base::push_back(__x);
    }

    T& operator[](std::size_t __i) {
        return *(begin() + __i);
    }

    typename _base::_Impl& get_M() {return this->_M_impl;}

    void pop_back() {
        _base::pop_back();
    }

    iterator begin() {return this->_M_impl.beg;}

    iterator end() {return this->_M_impl._end;}

    const_iterator cbegin() {return this->_M_impl.beg;}

    const_iterator cend() {return this->_M_impl._end;}
};