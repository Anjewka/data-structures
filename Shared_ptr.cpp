#include <bits/stdc++.h>

template<typename T>
class shared_ptr;

template<typename T, bool = std::is_array<T>::value, bool = std::is_void<T>::value>
class __shared_ptr_access {
public:
    using element_type = T;

    element_type& operator*() const noexcept {
        return *_M_get();
    }

    element_type* operator->() const noexcept {
        return _M_get();
    }

private:
    element_type* _M_get() const noexcept {
        return static_cast<const shared_ptr<T>*>(this)->get();
    }
};

template<typename T>
class __shared_ptr_access<T, false, true> {
public:
    using element_type = T;

    element_type* operator->() const noexcept {
        auto __ptr = static_cast<const shared_ptr<T>*>(this)->get();
        return __ptr;
    }
};

template<typename T>
class __shared_ptr_access<T, true, false> {
public:
    using element_type = T;
    using difference_type = std::ptrdiff_t;

    element_type& operator[](difference_type __i) const noexcept {
	    return _M_get()[__i];
    }

private:
    element_type* _M_get() const noexcept { 
        return static_cast<const shared_ptr<T[]>*>(this)->get();
    }
};

class count_base {
private:
    std::size_t _M_count;

public:
    count_base() noexcept : _M_count(1) {}

    template<typename U>
    count_base(U __p) {}

    virtual void dispose() noexcept = 0;

    virtual void destroy() noexcept {delete this;}

    bool _M_add_ref_lock_nothrow() noexcept;

    void add_ref() noexcept {
        ++_M_count;
    }

    void release() noexcept {
        if(!(--_M_count)) {
            dispose();
            destroy();
        }
    }

    std::size_t get_count() {
        return _M_count;
    }
};

template<typename T, bool _Array = false>
class counter_ptr : public count_base {
public:
    counter_ptr(T __p) noexcept : p(__p) {}

    void dispose() noexcept override {
        if(_Array) {
            delete[] p;
        } else {
            delete p;
        }
    }

    void destroy() noexcept override {
        delete this;
    }

    counter_ptr(const counter_ptr&) = delete;

    counter_ptr& operator=(const counter_ptr&) = delete;

private:
    T p;
};

template<bool _Array>
class __shared_count {
private:
    count_base* _M_pi;

public:

    __shared_count() noexcept : _M_pi() {}
    
    __shared_count(const __shared_count& __p) noexcept
            : _M_pi(__p._M_pi) {
        _M_pi->add_ref();
    }

    template<typename U>
    __shared_count(U __p) : _M_pi(0) {
        try {
            _M_pi = new counter_ptr<U, _Array>(__p);
	    }
	    catch(...) {
            delete __p;
	        throw;
	    }
	}

    void swap(__shared_count& __s) noexcept {
	    count_base* __tmp = __s._M_pi;
	    __s._M_pi = _M_pi;
	    _M_pi = __tmp;
    }

    __shared_count& operator=(const __shared_count& __p) {
        count_base* new_ptr = __p._M_pi;
        if(new_ptr != _M_pi) {
            if (new_ptr) {
                new_ptr->add_ref();
            }
            if (_M_pi) {
                _M_pi->release();
            }
            _M_pi = new_ptr;
        }
        return *this;
    }

    std::size_t get_count() {
        return _M_pi ? _M_pi->get_count() : 0;
    }

    ~__shared_count() {
        if(_M_pi != nullptr) {
            _M_pi->release();
        }
    }
};

template<typename T, bool _Array>
struct shared_ptr_base {
    using element_type = typename std::remove_extent<T>::type;

    shared_ptr_base() noexcept : _M_ptr(0), _M_count() {}

    template <typename U>
    shared_ptr_base(U* __p) : _M_ptr(__p), _M_count(__p) {}

    shared_ptr_base(shared_ptr_base&& __p) noexcept
            : _M_ptr(__p._M_ptr), _M_count() {
        _M_count.swap(__p._M_count);
        __p._M_ptr = nullptr;
    }

    shared_ptr_base(const shared_ptr_base&) noexcept = default;

    shared_ptr_base& operator=(const shared_ptr_base&) noexcept = default;

    shared_ptr_base& operator=(shared_ptr_base& __p) noexcept {
        
    }

    ~shared_ptr_base() = default;

    element_type* get() const noexcept {
        return _M_ptr;
    }

    std::size_t get_count() {
        return _M_count.get_count();
    }

protected:
    element_type* _M_ptr;
    __shared_count<_Array> _M_count;
};

template<typename T>
class shared_ptr : public __shared_ptr_access<T, false, false>
        , public shared_ptr_base<T, false> {
public:

    shared_ptr() : shared_ptr_base<T, false>() {}

    template <typename U>
    shared_ptr(U* __p) : shared_ptr_base<T, false>(__p) {}

    shared_ptr(shared_ptr&& __ptr) 
            : shared_ptr_base<T, false>(std::move(__ptr)) {} 

    shared_ptr(const shared_ptr&) noexcept = default;

    shared_ptr& operator=(const shared_ptr&) noexcept = default;

    ~shared_ptr() = default;
};

template<typename T>
class shared_ptr<T[]> : public __shared_ptr_access<T, true, false>
        , public shared_ptr_base<T, true> {
public:

    shared_ptr() : shared_ptr_base<T, true>() {}

    template <typename U>
    shared_ptr(U* __p) : shared_ptr_base<T, true>(__p) {}

    shared_ptr(shared_ptr&& __ptr) 
            : shared_ptr_base<T, true>(std::move(__ptr)) {} 

    shared_ptr(const shared_ptr&) noexcept = default;

    shared_ptr& operator=(const shared_ptr&) noexcept = default;

    ~shared_ptr() = default; 
};

template<typename T, typename... Args>
inline shared_ptr<std::_NonArray<T>> make_shared(Args&&... __args) {
    return shared_ptr<T>(new T(std::forward<Args>(__args)...));
}