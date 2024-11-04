#include <bits/stdc++.h>
 
#define _BUF_SIZE 512
std::size_t node_size(std::size_t s) {
    return (s < _BUF_SIZE ? std::size_t(_BUF_SIZE / s) : std::size_t(1));
}
 
template<typename T, typename _Reference, typename _Pointer>
struct _Deque_iterator {
 
    typedef _Deque_iterator<T, T&, T*>              iterator;
    typedef _Deque_iterator<T, const T&, const T*>  const_iterator;
    typedef T* _Type_ptr;
    typedef T** _Map_ptr;
    typedef _Deque_iterator<T, _Reference, _Pointer> this_class;
 
public:
 
    typedef _Pointer         pointer;
    typedef _Reference       reference;
    typedef std::size_t      size_type;
    typedef T                value_type;
    using difference_type = std::ptrdiff_t;
    typedef std::random_access_iterator_tag	iterator_category;
 
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
 
    this_class& operator++() {
        ++cur;
 
        if (cur == last) {
            _M_set_node(node + 1);
            cur = first;
        }
        return *this;
    }
 
    this_class& operator--() {
        if (cur == first) {
            _M_set_node(node - 1);
            cur = last;
        }
        --cur;
        return *this;
    }
 
    this_class& operator+=(difference_type __n) noexcept {
        const difference_type __offset = __n + (cur - first);
        if (__offset >= 0 && __offset < difference_type(node_size(sizeof(T))))
            cur += __n;
        else
        {
            const difference_type __node_offset =
                __offset > 0 ? __offset / difference_type(node_size(sizeof(T)))
                : -difference_type((-__offset - 1)
                    / node_size(sizeof(T))) - 1;
            _M_set_node(node + __node_offset);
            cur = first + (__offset - __node_offset
                * difference_type(node_size(sizeof(T))));
        }
        return *this;
    }
 
    this_class operator-=(size_type __n) {
        if (__n <= cur - first) {
            cur -= __n;
            return *this;
        }
        __n -= cur - first;
        size_type number_node = __n / node_size(sizeof(T)) + 1;
        _M_set_node(node - number_node);
        cur = last - __n % node_size(sizeof(T));
 
        return *this;
    }
 
    this_class operator+(size_type __n) {
        this_class __temp = *this;
        __temp += __n;
        return __temp;
    }
 
    reference operator*() { return *cur; }
 
    pointer operator->() { return cur; }
 
    friend bool operator==(const this_class __l, const this_class __r) {
        return __l.cur == __r.cur;
    }
 
    friend bool operator!=(const this_class __l, const this_class __r) {
        return __l.cur != __r.cur;
    }
 
    friend this_class operator+(const this_class& __r, difference_type __n) {
        this_class temp = __r;
        temp += __n;
        return temp;
    }
 
    friend this_class operator-(const this_class& __r, difference_type __n) {
        this_class temp = __r;
        temp -= __n;
        return temp;
    }
 
    friend difference_type operator-(const this_class& __x, const this_class& __y) noexcept {
        return difference_type(node_size(sizeof(T)))
            * (__x.node - __y.node - bool(__x.node))
            + (__x.cur - __x.first)
            + (__y.last - __y.cur);
    }
 
    friend bool operator<(const this_class& __x, const this_class& __y) noexcept {
        return (__x.node == __y.node)
            ? (__x.cur < __y.cur) : (__x.node < __y.node);
    }
};
 
template<typename T, typename _Alloc>
class Deque_base {
public:
    typedef T* _ptr;
    typedef const T* _const_ptr;
    typedef std::size_t  size_type;
    typedef _Deque_iterator<T, T&, T*>             iterator;
    typedef _Deque_iterator<T, const T&, const T*> const_iterator;
    typedef typename iterator::_Map_ptr _Map_ptr;
 
    typedef typename std::allocator_traits<_Alloc>::template rebind_alloc<T> _T_alloc_type;
    typedef std::allocator_traits<_T_alloc_type> _Alloc_traits;
 
    typedef typename _Alloc_traits::template rebind_alloc<_ptr> _Map_alloc_type;
    typedef std::allocator_traits<_Map_alloc_type> _Map_alloc_traits;
 
    using allocator_type = _Alloc;
 
public:
 
    allocator_type get_allocator() const noexcept {
        return allocator_type(_M_get_Tp_allocator());
    }
 
    _Map_alloc_type _M_get_map_allocator() const noexcept {
        return _Map_alloc_type(_M_get_Tp_allocator());
    }
 
    _T_alloc_type& _M_get_Tp_allocator() noexcept {
        return this->_M_impl;
    }
 
    const _T_alloc_type& _M_get_Tp_allocator() const noexcept {
        return this->_M_impl;
    }
 
    struct _Deque_data {
        _Map_ptr _M_map;
        size_type _size;
        iterator beg;
        iterator _end;
 
        _Deque_data() noexcept : _M_map(), _size(), beg(), _end() {}
 
        _Deque_data(const _Deque_data& __impl) = default;
 
        _Deque_data& operator=(const _Deque_data& __impl) = default;
 
        _Deque_data(_Deque_data&& __impl) noexcept : _Deque_data(__impl) {
            __impl = _Deque_data();
        }
    };
 
    struct _Deque_impl : public _Deque_data, public _T_alloc_type {
        _Deque_impl() : _T_alloc_type() {}
 
        _Deque_impl(const _T_alloc_type& __a) noexcept : _T_alloc_type(__a) {}
 
        _Deque_impl(_T_alloc_type&& __a) noexcept : _T_alloc_type(std::move(__a)) {}
 
        _Deque_impl(_Deque_data&& __d, _T_alloc_type&& __a) noexcept
                : _Deque_data(std::move(__d)), _T_alloc_type(std::move(__a)) {}
    };
 
    _Deque_impl _M_impl;
 
    void _M_initialize_map(size_type __n) {
        size_type nodes_count = (__n / node_size(sizeof(T)) + 1);
 
        _M_impl._size = std::max(static_cast<size_type>(8), size_type(nodes_count + 2));
        _M_impl._M_map = allocate_map(_M_impl._size);
 
        _Map_ptr __nstart = (_M_impl._M_map + (_M_impl._size - nodes_count) / 2);
        _Map_ptr __n_end = __nstart + nodes_count;
 
        try {
            _M_create_nodes(__nstart, __n_end);
        }
        catch (...) {
            deallocate_map(_M_impl._M_map, _M_impl._size);
            _M_impl._M_map = _Map_ptr();
            _M_impl._size = 0;
            throw;
        }
 
        _M_impl.beg._M_set_node(__nstart);
        _M_impl._end._M_set_node(__n_end - 1);
        _M_impl.beg.cur = _M_impl.beg.first;
        _M_impl._end.cur = (_M_impl._end.first + __n % node_size(sizeof(T)));
    }
 
    void _M_destroy_nodes(_Map_ptr __nstart, _Map_ptr __n_end) {
        for (_Map_ptr __n = __nstart; __n < __n_end; ++__n) {
            _Alloc_traits::deallocate(_M_impl, *__n, node_size(sizeof(T)));
        }
    }
 
    void _M_create_nodes(_Map_ptr __nstart, _Map_ptr __n_end) {
        _Map_ptr __cur;
        try {
            for (__cur = __nstart; __cur < __n_end; ++__cur) {
                *__cur = _Alloc_traits::allocate(_M_impl, node_size(sizeof(T)));
            }
        }
        catch (...) {
            _M_destroy_nodes(__nstart, __cur);
            throw;
        }
    }
 
    _Map_ptr allocate_map(size_type __n) {
        _Map_alloc_type map_alloc = _M_get_map_allocator();
        return _Map_alloc_traits::allocate(map_alloc, __n);
    }
 
    void deallocate_map(_Map_ptr __p, size_type __n) {
        _Map_alloc_type map_alloc = _M_get_map_allocator();
        _Map_alloc_traits::deallocate(map_alloc, __p, __n);
    }
 
    void reallocate(size_type __add, bool __add_at_front) {
        _Map_ptr new_map;
        size_type prev_size = _M_impl._end.node - _M_impl.beg.node + 1;
        size_type new_size = __add + prev_size + 1;
 
        new_map = allocate_map(new_size);
 
        if (!__add_at_front) { __add = 0; }
        std::copy(_M_impl.beg.node, _M_impl._end.node + 1, new_map + __add);
 
        deallocate_map(_M_impl._M_map, _M_impl._size);
        _M_impl._M_map = new_map;
        _M_impl._end.iterator::_M_set_node(new_map + prev_size + __add - 1);
        _M_impl.beg.iterator::_M_set_node(new_map + __add);
 
        _M_impl._size = new_size;
    }
 
    void destroy_data() {
        _M_destroy_nodes(_M_impl.beg.node, _M_impl._end.node + 1);
        deallocate_map(_M_impl._M_map, _M_impl._size);
    }
 
    template<typename... Args>
    void emplace_back(Args&&... _args) {
        if (_M_impl._end.cur != _M_impl._end.last - 1) {
            _Alloc_traits::construct(_M_impl
                , _M_impl._end.cur
                , std::forward<Args>(_args)...);
            ++_M_impl._end.cur;
            return;
        }
 
        reserve_at_back();
 
        _Map_ptr after_end = _M_impl._end.node + 1;
        *after_end = _Alloc_traits::allocate(_M_impl, node_size(sizeof(T)));
 
        try {
            _Alloc_traits::construct(_M_impl
                , _M_impl._end.cur
                , std::forward<Args>(_args)...);
            _M_impl._end._M_set_node(after_end);
            _M_impl._end.cur = _M_impl._end.first;
        }
        catch (...) {
            --_M_impl._end;
            _Alloc_traits::deallocate(_M_impl, *(_M_impl._end.node + 1),
                node_size(sizeof(T)));
            throw;
        }
    }
 
    template<typename... Args>
    void emplace_front(Args&&... _args) {
        if (_M_impl.beg.cur != _M_impl.beg.first) {
            _Alloc_traits::construct(_M_impl,
                _M_impl.beg.cur - 1,
                std::forward<Args>(_args)...);
            --_M_impl.beg.cur;
            return;
        }
 
        reserve_at_front();
 
        _Map_ptr before_begin = _M_impl.beg.node - 1;
        *before_begin = _Alloc_traits::allocate(_M_impl, node_size(sizeof(T)));
 
        try {
            _M_impl.beg._M_set_node(before_begin);
            _M_impl.beg.cur = _M_impl.beg.last - 1;
            _Alloc_traits::construct(_M_impl
                , _M_impl.beg.cur
                , std::forward<Args>(_args)...);
        }
        catch (...) {
            ++_M_impl.beg;
            _Alloc_traits::deallocate(_M_impl, *(_M_impl.beg.node - 1),
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
 
    void reserve_at_front(std::size_t __add = 1) {
        if (__add > size_t(_M_impl.beg.node - _M_impl._M_map)) {
            reallocate(__add, true);
        }
    }
 
    void reserve_at_back(std::size_t __add = 1) {
        if (__add + 1 > _M_impl._size - (_M_impl._end.node - _M_impl._M_map)) {
            reallocate(__add, false);
        }
    }
 
    void pop_back() noexcept {
        if(_M_impl._end == _M_impl.beg) { return; }
 
        if(_M_impl._end.cur == _M_impl._end.first) {
            _Alloc_traits::deallocate(_M_impl, _M_impl._end.first, node_size(sizeof(T)));
            _Alloc_traits::destroy(_M_impl, _M_impl._end.cur);
            _M_impl._end._M_set_node(_M_impl._end.node - 1);
            _M_impl._end.cur = _M_impl._end.last - 1;
            return;
        }
 
        auto last = _M_impl._end.cur;
        --_M_impl._end.cur;
        _Alloc_traits::destroy(_M_impl, last);
    }
 
    void pop_front() noexcept {
        if(_M_impl._end == _M_impl.beg) { return; }
 
        if (_M_impl.beg.cur == _M_impl.beg.last) {
            _Alloc_traits::deallocate(_M_impl, _M_impl.beg.first, node_size(sizeof(T)));
            _Alloc_traits::destroy(_M_impl, _M_impl.beg.cur);
            _M_impl.beg._M_set_node(_M_impl.beg.node + 1);
            _M_impl.beg.cur = _M_impl.beg.first + 1;
            return;
        }
 
        auto _begin = _M_impl.beg.cur;
        _M_impl.beg += 1;
        _Alloc_traits::destroy(_M_impl, _begin);
    }
 
    Deque_base() : _M_impl() { _M_initialize_map(0); }
 
    Deque_base(const allocator_type& __a, size_type __c) : _M_impl(__a) { _M_initialize_map(__c); }
 
    Deque_base(const allocator_type& __a) : _M_impl(__a) {}
 
    Deque_base(Deque_base&& __d) : _M_impl(std::move(__d._M_get_Tp_allocator())) {
        __d._M_initialize_map(0);
    }
 
    Deque_base(Deque_base&& __d, const allocator_type& __a)
            : _M_impl(std::move(__d._M_impl), _Tp_alloc_type(__a)) {
        __d._M_initialize_map(0);
    }
 
    Deque_base(Deque_base&& __d, const allocator_type& __a, size_type __n)
            : _M_impl(std::move(__d._M_impl), _Tp_alloc_type(__a)) {
        if (__a == __d.get_allocator()) {
            if (__d._M_impl._M_map) {
                _M_initialize_map(0);
                std::swap(__d._M_impl, _M_impl);
            }
            else {
                _M_initialize_map(__n);
            }
        }
    }
};
 
template<typename T, typename _Alloc = std::allocator<T>>
class deque : public Deque_base<T, _Alloc> {
public:
    using _base = Deque_base<T, _Alloc>;
 
    using _base::_ptr;
    using _base::_const_ptr;
 
    typedef typename _base::size_type   size_type;
    typedef typename _base::iterator    iterator;
    typedef typename _base::iterator    const_iterator;
    using value_type = T;
 
private:
    template<typename _ForwardIterator>
    _ForwardIterator fill_initialize(_ForwardIterator first, size_type count
                , const value_type& __x, _Alloc& __a) {
        typedef std::allocator_traits<_Alloc> __traits;
        auto __cur = first;
        try {
            for(; count > 0; --count) {
                __traits::construct(__a, std::__addressof(*__cur), __x);
                ++__cur;
            }
            return __cur;
        } catch(...) {
            for(auto __p = this->_M_impl.beg; __p != __cur; ++__p) {
                __traits::destroy(__a, std::__addressof(*__p));
            }
            throw;
        }
    }
 
    template<typename _ForwardIterator>
    void initialize_range(_ForwardIterator __b, _ForwardIterator __l, _Alloc& __a) {
        size_type __size = __l - __b;
        this->_M_initialize_map(__size);
 
        typedef std::allocator_traits<_Alloc> __traits;
        iterator __i = begin();
        _ForwardIterator __forward_iterator = __b;
 
        try {
            for(; __forward_iterator != __l; ++__i, ++__forward_iterator) {
                __traits::construct(__a, std::__addressof(*(__i))
                        , *__forward_iterator);
            }
        } catch(...) {
            for(iterator __p = begin(); __p != __i; ++__p) {
                __traits::destroy(__a, std::__addressof(*(__p)));
            }
            throw;
        }        
    }
 
public:
    using allocator_type = _Alloc;
 
    deque() = default;
 
    deque(const allocator_type& __a) : _base(__a) {}
 
    deque(size_type __n, const allocator_type& __a = allocator_type()) : _base(__a, __n) {}
 
    deque(size_type __n, const value_type& __v, const allocator_type& __a = allocator_type())
            : _base(__a, __n) {
        fill_initialize(begin(), __n, __v, this->_M_get_Tp_allocator());
    }
 
    deque(deque&& __d) : _base(std::move(__d)) {}
 
    deque(std::initializer_list<value_type> __l, const allocator_type& __a = allocator_type())
            : _base(__a) {
        initialize_range(__l.begin(), __l.end(), this->_M_get_Tp_allocator());
    }
 
    deque(const deque& __d) = default;
 
    void push_front(const T& __x) {
        _base::push_front(__x);
    }
 
    void push_back(const T& __x) {
        _base::push_back(__x);
    }
 
    void push_back(T&& __x) {
        _base::push_back(std::move(__x));
    }
 
    template<typename... Args>
    void emplace_back(Args&&... __args) {
        _base::emplace_back(std::forward<Args>(__args)...);
    }
 
    template<typename... Args>
    void emplace_front(Args&&... __args) {
        _base::emplace_front(std::forward<Args>(__args)...);
    }
 
    T& operator[](std::size_t __i) {
        return *(begin() + __i);
    }
 
    typename _base::_Deque_impl& get_M() { return this->_M_impl; }
 
    void pop_back() {
        _base::pop_back();
    }
 
    void pop_front() {
        _base::pop_front();
    }
 
    void clear() noexcept {
        while(!empty()) {
            pop_back();
        }
    }
 
    bool empty() const noexcept {
        return this->_M_impl.beg == this->_M_impl._end;
    }
 
    ~deque() noexcept {
        this->destroy_data();
    }

    size_type size() const noexcept {
        return this->_M_impl._end - this->_M_impl.beg;
    }
 
    iterator begin() noexcept { return this->_M_impl.beg; }

    const_iterator begin() const noexcept { return this->_M_impl.beg; }
 
    iterator end() noexcept { return this->_M_impl._end; }

    const_iterator end() const noexcept { return this->_M_impl._end; }
 
    const_iterator cbegin() const noexcept { return this->_M_impl.beg; }
 
    const_iterator cend() const noexcept { return this->_M_impl._end; }
};