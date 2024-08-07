#include <bits/stdc++.h>

template<typename T, typename _Pointer, typename _Reference>
struct _Vector_iterator {
private:
    typedef _Vector_iterator<T, T*, T&>                 iterator;
    typedef _Vector_iterator<T, const T*, const T&>     const_iterator; 
    typedef _Vector_iterator<T, _Pointer, _Reference>   this_iterator;
public:
    typedef _Pointer                                    pointer;
    typedef _Reference                                  reference;
    typedef std::size_t                                 size_type;
    typedef T                                           value_type;

    using difference_type = std::ptrdiff_t;
    typedef std::random_access_iterator_tag	iterator_category;
   
    pointer _M_current;

    _Vector_iterator() noexcept : _M_current() {}

    _Vector_iterator(pointer __p) noexcept : _M_current(__p) {}

    _Vector_iterator(const iterator& __i) noexcept : _M_current(__i._M_current) {}

    _Vector_iterator(const const_iterator& __i) noexcept
                : _M_current(__i._M_current) {}

    reference operator*() const noexcept {return *_M_current;}

    pointer operator->() const noexcept {return _M_current;}

    this_iterator& operator++() noexcept {
        ++_M_current;
        return *this;
    }

    this_iterator& operator++(int) noexcept {
        auto __temp = _M_current;
        ++_M_current;
        return __temp;
    }

    this_iterator& operator--() noexcept {
        return --_M_current;
        return *this;
    }

    this_iterator& operator--(int) noexcept {
        auto __temp = _M_current;
        --_M_current;
        return __temp;
    }

    reference operator[](difference_type __n) const noexcept {
        return _M_current[__n];
    }

    this_iterator& operator+=(difference_type __n) noexcept {
        _M_current += __n;
        return *this;
    }

    this_iterator operator+(difference_type __n) noexcept {
        return this_iterator(_M_current + __n);
    }

    this_iterator& operator-=(difference_type __n) noexcept {
        _M_current -= __n;
        return *this;
    }

    this_iterator operator-(difference_type __n) noexcept {
        return this_iterator(_M_current - __n);
    }

    friend bool operator==(const this_iterator __l, const this_iterator __r) {
        return __l._M_current == __r._M_current; 
    }

    friend bool operator!=(const this_iterator __l, const this_iterator __r) {
        return __l._M_current != __r._M_current; 
    }

    friend bool operator<(const this_iterator& __x, const this_iterator& __y) noexcept {
	    return (__x._M_current < __y._M_current);
    }

    friend bool operator>(const this_iterator& __x, const this_iterator& __y) noexcept {
	    return (__x._M_current > __y._M_current);
    }

    friend bool operator<=(const this_iterator& __x, const this_iterator& __y) noexcept {
	    return (__x._M_current <= __y._M_current);
    }

    friend bool operator>=(const this_iterator& __x, const this_iterator& __y) noexcept {
	    return (__x._M_current >= __y._M_current);
    }
};

template<typename T, typename _Alloc>
struct _Vector_base {

    typedef typename _Alloc::template rebind<T>::other alloc_type;
    typedef typename std::allocator_traits<_Alloc>::pointer pointer;

    typedef _Vector_iterator<T, T*, T&>             iterator;
    typedef _Vector_iterator<T, const T*, const T&> const_iterator;

    struct _Vector_impl_data {
        pointer start;
        pointer finish;
        pointer capacity;

        _Vector_impl_data() noexcept : start(), finish(), capacity() {}

        _Vector_impl_data(_Vector_impl_data&& __x) noexcept : start(__x.start)
                , finish(__x.finish), capacity(__x.capacity) {
            __x.start = pointer();
            __x.finish = pointer();
            __x.capacity = pointer();
        }
    };

    struct _Vector_impl : public _Vector_impl_data, public alloc_type {
        _Vector_impl() noexcept : alloc_type() {}

        _Vector_impl(alloc_type const& __i) noexcept : alloc_type(__i) {}

        _Vector_impl(_Vector_impl&& __i) noexcept : _Vector_impl_data(std::move(__i))
                , alloc_type(std::move(__i)) {}

        _Vector_impl(alloc_type&& __i) noexcept : alloc_type(std::move(__i)) {}
    };

public:
    _Vector_impl _M_impl;

    _Vector_base() = default;

    _Vector_base(_Vector_base&&) = default;

    _Vector_base(const _Vector_base& __x) noexcept : _M_impl(__x) {}

    _Vector_base(std::size_t count) : _M_impl() {create_storage(count);}

    _Vector_base(std::size_t count, const _Alloc& __a) noexcept 
                : _M_impl(__a) {
        create_storage(count);
    }

    ~_Vector_base() {
        _M_deallocate(_M_impl.start, _M_impl.capacity - _M_impl.start);
    }

    alloc_type& _M_get_Tp_allocator() noexcept {return this->_M_impl;}

    const alloc_type& _M_get_Tp_allocator() const noexcept {return this->_M_impl;}

    _Alloc get_allocator() const noexcept {return _Alloc(_M_get_Tp_allocator());}

    pointer _M_allocate(std::size_t count) {
        typedef std::allocator_traits<alloc_type> _Tp;
        return count ? _Tp::allocate(_M_impl, count) : pointer();  
    }

    void _M_deallocate(pointer __p, std::size_t count) {
        typedef std::allocator_traits<alloc_type> _Tp;
        count ? _Tp::deallocate(_M_impl, __p, count) : void();
    }

    void create_storage(std::size_t count) {
        _M_impl.start = _M_allocate(count);
        _M_impl.finish = _M_impl.start;
        _M_impl.capacity = _M_impl.start + count;
    } 
};

template<typename T, typename _Alloc = std::allocator<T>>
class vector : public _Vector_base<T, _Alloc> {
    using _Base = _Vector_base<T, _Alloc>;
    using alloc_type = typename _Base::alloc_type;
    using _Alloc_traits = std::allocator_traits<alloc_type>;

public:
    typedef T                                       value_type;
    typedef typename _Base::pointer                 pointer;
    typedef typename _Alloc_traits::const_pointer   const_pointer;
    typedef value_type&                             reference;
    typedef const value_type&                       const_reference;
    typedef std::size_t                             size_type;
    typedef typename _Base::iterator                iterator;
    typedef typename _Base::const_iterator          const_iterator;

    vector() = default;

    vector(const _Alloc& __a) noexcept : _Base(__a) {}

    vector(size_type count, const _Alloc& __a = _Alloc()) : _Base(count) {
        this->_M_impl.finish = fill_def_initialize(this->_M_impl.start
                , count, this->_M_get_Tp_allocator());
    }

    vector(size_type count, const value_type& __x, const _Alloc& __a = _Alloc()) 
                : _Base(count) {
        this->_M_impl.finish = fill_initialize(this->_M_impl.start
                    , count, __x, this->_M_get_Tp_allocator());
    }

    vector(const vector& __v) : _Base(__v.size()
                , _Alloc_traits::select_on_container_copy_construction(
                    __v._M_get_Tp_allocator())) {
        this->_M_impl.finish = uninitialized_copy(__v.begin(), __v.end()
                , this->_M_impl.start
                , this->_M_get_Tp_allocator());
    }

    vector(vector&& __v) = default;

    vector(std::initializer_list<value_type> __l, const _Alloc& __a = _Alloc())
                : _Base(__a) {
        initialize_range(__l.begin(), __l.end());
    }

    size_type size() const {return this->_M_impl.finish - this->_M_impl.start;}

    size_type capacity() const {return this->_M_impl.capacity - this->_M_impl.start;}

private:

    template<typename _ForwardIterator>
    _ForwardIterator fill_initialize(_ForwardIterator first, size_type count
                , const T& __x, _Alloc& __a) {
        typedef std::allocator_traits<_Alloc> __traits;
        auto __cur = first;
        try {
            for(; count > 0; --count) {
                __traits::construct(__a, std::__addressof(*__cur), __x);
                ++__cur;
            }
            return __cur;
        } catch(...) {
            for(auto __p = this->_M_impl.start; __p != __cur; ++__p) {
                __traits::destroy(__a, std::__addressof(*__p));
            }
            throw;
        }
    }

    template<typename _ForwardIterator>
    _ForwardIterator fill_def_initialize(_ForwardIterator first
                , size_type count, _Alloc& __a) {
        typedef std::allocator_traits<_Alloc> __traits;
        auto __cur = first;
        try {
            for(; count > 0; --count) {
                __traits::construct(__a, std::__addressof(*__cur));
                ++__cur;
            }
            return __cur;
        } catch(...) {
            for(auto __p = this->_M_impl.start; __p != __cur; ++__p) {
                __traits::destroy(__a, std::__addressof(*__p));
            }
            throw;
        }
    }

    template<typename _ForwardIterator, typename _InputIterator>
    _ForwardIterator uninitialized_copy(_InputIterator first, _InputIterator last
                , _ForwardIterator iter, _Alloc& __a) {
        typedef std::allocator_traits<_Alloc> __traits;
        auto __curr = iter;
        try {
            for(; first != last; ++first, ++__curr) {
                __traits::construct(__a, std::__addressof(*__curr), *first);
            }
            return __curr;
        } catch(...) {
            for(auto __p = iter; __p != __curr; ++__p) {
                __traits::destroy(__a, std::__addressof(*__p));
            }
            throw;
        }
    }

    template<typename _Inputiterator>
    void initialize_range(_Inputiterator first, _Inputiterator last) {
        try {
            for(; first != last ; ++first) {
                emplace_back(*first);
            }
        } catch(...) {
            clear();
            throw;
        }
    }

public:
    iterator begin() noexcept {return iterator(this->_M_impl.start);}

    const_iterator begin() const noexcept {
        return const_iterator(this->_M_impl.start);
    }

    iterator end() noexcept {return iterator(this->_M_impl.finish);}

    const_iterator end() const noexcept {
        return const_iterator(this->_M_impl.finish);
    }

    const_iterator cbegin() const noexcept {
        return const_iterator(this->_M_impl.start);
    }

    const_iterator cend() const noexcept {
        return const_iterator(this->_M_impl.finish);
    }

    void reserve(size_type __n) {
        if(__n < size() || __n < capacity() && __n > size()) {return;}

        auto __q = this->_M_allocate(__n);

        auto __end = uninitialized_copy(this->_M_impl.start, this->_M_impl.finish, __q
                    , this->_M_get_Tp_allocator());
        
        this->_M_deallocate(this->_M_impl.start
                    , this->_M_impl.capacity - this->_M_impl.start);
        this->_M_impl.start = __q;
        this->_M_impl.finish = __end;
        this->_M_impl.capacity = __q + __n;
    }

    void resize(size_type __n) {
        if(__n <= capacity() && __n > size()) {
            this->_M_impl.finish = fill_def_initialize(this->_M_impl.finish
                    , __n - size(), this->_M_get_Tp_allocator());
        } else if(__n <= capacity() && __n < size()) {
            while(--__n) {
                pop_back();
            }
        } else if(__n > capacity()) {
            reserve(__n);
            this->_M_impl.finish = fill_def_initialize(this->_M_impl.finish
                    , __n - size(), this->_M_get_Tp_allocator());
        }
    }

    void shrink_to_fit() {
        if(this->_M_impl.capacity == this->_M_impl.finish) {return;}

        this->_M_deallocate(this->_M_impl.finish
                    , size_type(this->_M_impl.capacity - this->_M_impl.finish));
        this->_M_impl.capacity = this->_M_impl.finish;
    }

    void push_back(const value_type& __x) noexcept {
        if(size() == capacity()) {
            reserve(std::max(capacity() * 2, capacity() + 1));
        }

        _Alloc_traits::construct(this->_M_impl, this->_M_impl.finish, __x);
        ++this->_M_impl.finish;
    }

    void push_back(value_type&& __x) {
        emplace_back(std::move(__x));
    }

    template<typename ... _Args>
    void emplace_back(_Args&& ... __args) {
        if(size() == capacity()) {
            reserve(std::max(capacity() * 2, capacity() + 1));
        }

        _Alloc_traits::construct(this->_M_impl, this->_M_impl.finish
                    , std::forward<_Args>(__args)...);
        ++this->_M_impl.finish;
    }

    void pop_back() {
        if(size() > 0) {
            --this->_M_impl.finish;
            _Alloc_traits::destroy(this->_M_impl, this->_M_impl.finish);
        }
    }

    void clear() noexcept {return;}
};