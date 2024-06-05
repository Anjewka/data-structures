#include <bits/stdc++.h>

struct _Hash_node_base {
    _Hash_node_base* _M_nxt;

    _Hash_node_base() noexcept : _M_nxt(nullptr) {}
    _Hash_node_base(_Hash_node_base* next) noexcept : _M_nxt(next) {}
};

template<typename _Value>
struct _Hash_node_val {
    typedef _Value value_type;
    typedef std::size_t hash_type;

    _Value _M_storage;
    hash_type _M_hash;

    value_type* ptr() noexcept {return std::addressof(_M_storage);}
};

template<typename _Value>
struct _Hash_node 
        : _Hash_node_base
        , _Hash_node_val<_Value> {
    
    using hash_type = typename _Hash_node_val<_Value>::hash_type;

    _Hash_node* _M_next() const noexcept {return static_cast<_Hash_node*>(_M_nxt);}

    _Hash_node() noexcept {}

    _Hash_node(const _Value& __v, hash_type __h = 0) noexcept : _Hash_node_val<_Value>{__v, __h} {}
};

template<typename _Value>
struct _Node_iterator_base {
    using _node_type = _Hash_node<_Value>;

    _node_type* _M_iterator;

    _Node_iterator_base() : _M_iterator() {}
    _Node_iterator_base(_node_type* __p) noexcept : _M_iterator(__p) {}

    void incr() noexcept {_M_iterator = _M_iterator->_M_next();}

    friend bool operator==(const _Node_iterator_base& __x, const _Node_iterator_base& __y) noexcept {
        return __x._M_iterator == __y._M_iterator;
    }

    friend bool operator!=(const _Node_iterator_base& __x, const _Node_iterator_base& __y) noexcept {
        return __x._M_iterator != __y._M_iterator;
    }
};

template<typename T>
struct _Node_iterator : public _Node_iterator_base<T> {
private:
    using _base_type = _Node_iterator_base<T>;
    using _node_type = typename _base_type::_node_type;

public:
    typedef T value_type;
    typedef _Node_iterator<T> iterator;

    using pointer = T*;
    using reference = T&;

    _Node_iterator() = default;

    _Node_iterator(_node_type* __p) noexcept : _base_type(__p) {}

    reference operator*() noexcept {return this->_M_iterator->_M_storage;}

    pointer operator->() noexcept {return this->_M_iterator->ptr();}
    
    _Node_iterator& operator++() noexcept {
        this->incr();
        return *this;
    }

    _Node_iterator operator++(int) noexcept {
        _Node_iterator __tmp(*this);
        this->incr();
        return __tmp;
    }

    _node_type* _M_node() noexcept {return this->_M_iterator;}
};

template<typename T> 
struct _Const_node_iterator : public _Node_iterator_base<T> {
private:
    using _base_type = _Node_iterator_base<T>;
    using _node_type = typename _base_type::_node_type;

public:
    typedef T value_type;
    typedef _Const_node_iterator<T> const_iterator;

    using pointer = const T*;
    using reference = const T&;

    _Const_node_iterator() = default;

    _Const_node_iterator(_node_type* __p) noexcept : _base_type(__p) {}

    reference operator*() const noexcept {return this->_M_iterator->_M_storage;}

    pointer operator->() const noexcept {return this->_M_iterator->ptr();}
    
    _Const_node_iterator& operator++() noexcept {
        this->incr();
        return *this;
    }

    _Const_node_iterator operator++(int) noexcept {
        _Node_iterator __tmp(*this);
        this->incr();
        return __tmp;
    }

    _node_type* _M_node() noexcept {return this->_M_iterator;}
};

template<typename T>
struct _Hashtable_hash : private T {
    _Hashtable_hash() = default;

    const T& _get_const() const {return hasher;}
    T& _get() {return hasher;}
private:
    T hasher{};
};

struct _Range_hashing {
    std::size_t operator()(std::size_t __n, std::size_t __d) const noexcept {
        return __n % __d;
    }
};

template<typename _Key, 
    typename _Value, 
    typename _Alloc, 
    typename _Hash,
    typename _Equal,
    typename _Ranagehash = _Range_hashing>
class _Hashtable : private _Hashtable_hash<_Hash> {
public:
    typedef std::pair<_Key, _Value> pair_type;

    using _node_type = _Hash_node<pair_type>;
    using _node_base_ptr = _Hash_node_base*;
    using _node_ptr = _Hash_node<pair_type>*;
    using _node_base = _Hash_node_base;
    using _node_value_type = _Hash_node_val<pair_type>;
    using _buckets_ptr = _node_ptr*;
    using iterator = typename _Node_iterator<pair_type>::iterator;
    using const_iterator = typename _Const_node_iterator<pair_type>::const_iterator;
    using __hash = _Hashtable_hash<_Hash>;
    using __equal = _Equal;

    typename _Alloc::template rebind<_node_ptr>::other __buckets_alloc;
    typename _Alloc::template rebind<_node_type>::other __node_alloc;
    typedef typename _Alloc::template rebind<_node_ptr>::other __buckets_alloc_type;
    typedef typename _Alloc::template rebind<_node_type>::other __node_alloc_type;
    using _buckets_alloc_traits = std::allocator_traits<__buckets_alloc_type>;
    using _node_alloc_traits = std::allocator_traits<__node_alloc_type>;

    typedef std::size_t _hash_code;
    typedef std::size_t size_type;

    _node_ptr _M_single_bucket = nullptr;
    __equal __eq;

    _buckets_ptr _M_buckets = &_M_single_bucket;
    size_type _M_bucket_count = 1;
    _node_base _M_before_begin;
    size_type element_count = 0;

private:
    _hash_code _M_hash_code(const _Key& __k) const {
        return _M_hash()(__k);
    }

    _node_ptr _M_begin() const {return static_cast<_node_ptr>(_M_before_begin._M_nxt); }

    const _Hash&  _M_hash() const { return __hash::_get_const(); }

    size_type _M_bucket_index(size_type _code, size_type _b) const noexcept {
        return _Ranagehash{}(_code, _b);
    }

    void insert_bucket_begin(size_type __x, _node_ptr __node) {
        if(_M_buckets[__x]) {
            _node_ptr temp = _M_buckets[__x];
            __node->_M_nxt = temp->_M_nxt;
            _M_buckets[__x]->_M_nxt = __node;
        } else {
            __node->_M_nxt = _M_before_begin._M_nxt;
            _M_before_begin._M_nxt = __node;

            if(__node->_M_nxt) {
                _M_buckets[
                    _M_bucket_index(
                        _M_hash_code(__node->_M_next()->_M_storage.first), _M_bucket_count
                    )
                ] = __node;
            }
            _M_buckets[__x] = static_cast<_node_ptr>(&_M_before_begin);
        }
    }

    iterator _M_insert_unique_node(size_type __bkt, size_type __code, _node_ptr __node, size_type __ic = 1) {
        size_t __rehash_count = 0;

        if(!element_count || __ic + element_count > _M_bucket_count) {
            __rehash_count = std::max((__ic + element_count) * 2, static_cast<size_type>(13));
        }

        if(__rehash_count) {
            try {
                rehash(__rehash_count);
            } catch(...) {
                clear();
                throw ("method rehash");
            }
            __bkt = _M_bucket_index(__code, _M_bucket_count);
        }

        ++element_count;
        insert_bucket_begin(__bkt, __node);
        __node->_M_hash = __bkt;
        return iterator(__node);
    }

    std::pair<bool, iterator> emplace(const pair_type& __i) {
        size_type __code = _M_hash_code(__i.first);
        size_type __bkt = _M_bucket_index(__code, _M_bucket_count);

        _node_ptr __node = static_cast<_node_ptr>(_M_buckets[__bkt]);

        if(__node) {
            __node = __node->_M_next();
        }

        while(__node && __node->_M_hash == __bkt) {
            if(__eq(__node->_M_storage.first, __i.first)) {
                return {false, iterator(__node)};
            }
            __node = __node->_M_next();
        }

        try {
            __node = _node_alloc_traits::allocate(__node_alloc, 1);
            ::new(__node) pair_type(__i);
            _node_alloc_traits::construct(__node_alloc, __node, __i);
        } catch(...) {
            _node_alloc_traits::deallocate(__node_alloc, __node, 1);
            throw("can't allocate memmory");
        }
        __node->_M_hash = __bkt;

        return {true, _M_insert_unique_node(__bkt, __code, __node)};
    }

public:
    _Hashtable() = default;

    _Hashtable(std::initializer_list<pair_type> _list) {
        for(auto & i : _list) {
            emplace(std::make_pair(i.first, i.second));
        }
    }

    _Hashtable(const _Hashtable& __h) {
        _M_buckets = _buckets_alloc_traits::allocate(__buckets_alloc, __h._M_bucket_count);
        for(std::size_t i = 0; i<__h._M_bucket_count; ++i) {
            _buckets_alloc_traits::construct(__buckets_alloc, _M_buckets + i, nullptr);
            _M_buckets[i] = nullptr;
        }

        _node_ptr __p = __h._M_begin();
        _M_before_begin._M_nxt = nullptr;
        size_type __bbegin_bkt = 0;

        while (__p) {
            _node_ptr __next = __p->_M_next();
            size_type __bkt = _M_bucket_index(_M_hash_code(__p->_M_storage.first), __h._M_bucket_count);
            __p->_M_hash = __bkt;
            if (!_M_buckets[__bkt]) {
                __p->_M_nxt = _M_before_begin._M_nxt;
                _M_before_begin._M_nxt = __p;
                _M_buckets[__bkt] = static_cast<_node_ptr>(&_M_before_begin);
                _M_buckets[__bbegin_bkt] = __p;
                __bbegin_bkt = __bkt;
            }
            else {
                __p->_M_nxt = _M_buckets[__bkt]->_M_nxt;
                _M_buckets[__bkt]->_M_nxt = __p;
            }
            __p = __next;
        }
    }

    ~_Hashtable() {
        clear();
    }

public:

    std::pair<bool, iterator> insert(const _Key& __k, const _Value& __v) {
        return emplace(std::make_pair(__k, __v));
    }

    _Value& operator[](const _Key& __k) {
        pair_type __p = {__k, _Value()};
        iterator __i = emplace(__p).second;
        return __i->second;
    }
    
    iterator find(const _Key& __k) {
        size_type __code = _M_hash_code(__k);
        size_type __bkt = _M_bucket_index(__code, _M_bucket_count);

        _node_ptr __node = static_cast<_node_ptr>(_M_buckets[__bkt]);

        if(__node) {
            __node = __node->_M_next();
        }

        while(__node && __node->_M_hash == __bkt) {
            if(__node->_M_storage.first == __k) {
                return iterator(__node);
            }
            __node = __node->_M_next();
        }
        return end();
    }

    const_iterator find(const _Key& __k) const {
        size_type __code = _M_hash_code(__k);
        size_type __bkt = _M_bucket_index(__code, _M_bucket_count);

        _node_ptr __node = static_cast<_node_ptr>(_M_buckets[__bkt]);

        if(__node) {
            __node = __node->_M_next();
        }

        while(__node && __node->_M_hash == __bkt) {
            if(__node->_M_storage.first == __k) {
                return const_iterator(__node);
            }
            __node = __node->_M_next();
        }
        return cend();
    }

    void rehash(size_type __count) {
        _buckets_ptr __copy_buckets = _buckets_alloc_traits::allocate(__buckets_alloc, __count);
        for(std::size_t i = 0; i<__count; ++i) {
            _buckets_alloc_traits::construct(__buckets_alloc, __copy_buckets + i, nullptr);
        }
        for(size_t i = 0; i<__count ; ++i) {__copy_buckets[i] = nullptr;}
        _node_ptr __p = _M_begin();
        _M_before_begin._M_nxt = nullptr;
        size_type __bbegin_bkt = 0;

        while (__p) {
            _node_ptr __next = __p->_M_next();
            size_type __bkt = _M_bucket_index(_M_hash_code(__p->_M_storage.first), __count);
            __p->_M_hash = __bkt;
            if (!__copy_buckets[__bkt]) {
                __p->_M_nxt = _M_before_begin._M_nxt;
                _M_before_begin._M_nxt = __p;
                __copy_buckets[__bkt] = static_cast<_node_ptr>(&_M_before_begin);
                __copy_buckets[__bbegin_bkt] = __p;
                __bbegin_bkt = __bkt;
            }
            else {
                __p->_M_nxt = __copy_buckets[__bkt]->_M_nxt;
                __copy_buckets[__bkt]->_M_nxt = __p;
            }
            __p = __next;
        }

        for(size_t i = 0; i<_M_bucket_count ; ++i) {
            _buckets_alloc_traits::destroy(__buckets_alloc,_M_buckets + i);
        }
        if(element_count > 1) {
            _buckets_alloc_traits::deallocate(__buckets_alloc, _M_buckets,_M_bucket_count);
        }
        _M_bucket_count = __count;
        _M_buckets = __copy_buckets;
    }

    _Value& at(const _Key& __k) {
        iterator __f = find(__k);
        if(__f == end()) {throw std::out_of_range ("method \"at\"");;}
        return __f->second;
    }

    const _Value& at(const _Key& __k) const {
        iterator __f = find(__k);
        if(__f == end()) {throw std::out_of_range ("method \"at\"");;}
        return __f->second;   
    }

    void erase(const _Key& __k) {
        _hash_code __code = _M_hash_code(__k);
        _hash_code __bkt = _M_bucket_index(__code, _M_bucket_count);

        _node_ptr __node = _M_buckets[__bkt];
        if(!__node || !__node->_M_nxt) {return ;}
        _node_ptr temp = __node->_M_next();

        while(temp && temp->_M_hash == __bkt) {
            if(temp->_M_storage.first == __k) {
                __node->_M_nxt = temp->_M_nxt;
                if(temp->_M_nxt) {
                    _M_buckets[temp->_M_next()->_M_hash] = __node;
                } else if(_M_buckets[__bkt] == temp) {
                    _M_buckets[__bkt] = nullptr;
                }

                if(temp) {
                    _node_alloc_traits::destroy(__node_alloc, temp);
                    _node_alloc_traits::deallocate(__node_alloc, temp, 1);
                }
                --element_count;

                if(!element_count) {
                    _buckets_alloc_traits::deallocate(__buckets_alloc, _M_buckets,_M_bucket_count);
                    _M_single_bucket = nullptr;
                    _M_buckets = &_M_single_bucket;
                    _M_bucket_count = 1;
                    _M_before_begin;
                }
                return;
            }
            temp = temp->_M_next();
            __node = __node->_M_next();
        }
    }

    iterator erase(iterator __i) {
        if(!__i._M_node()) { return __i; }

        _hash_code __bkt = (__i._M_node())->_M_hash;
        const _Key& __k = __i._M_node()->_M_storage.first;

        _node_ptr __node = _M_buckets[__bkt];
        if(!__node || !__node->_M_nxt) {return end();}
        _node_ptr temp = __node->_M_next();

        while(temp && temp->_M_hash == __bkt) {
            if(temp->_M_storage.first == __k) {
                __node->_M_nxt = temp->_M_nxt;

                if(temp->_M_nxt) {
                    _M_buckets[temp->_M_next()->_M_hash] = __node;
                } else if(_M_buckets[__bkt] == temp) {
                    _M_buckets[__bkt] = nullptr;
                }

                if(temp) {
                    _node_alloc_traits::destroy(__node_alloc, temp);
                    _node_alloc_traits::deallocate(__node_alloc, temp, 1);
                }
                --element_count;

                if(!element_count) {
                    _buckets_alloc_traits::deallocate(__buckets_alloc, _M_buckets,_M_bucket_count);
                    _M_single_bucket = nullptr;
                    _M_buckets = &_M_single_bucket;
                    _M_bucket_count = 1;
                    _M_before_begin;
                    return end();
                }
                if(__node) {return __node->_M_next();}
                return end();
            }
            temp = temp->_M_next();
            __node = __node->_M_next();
        }

        return end();
    }

    void clear() noexcept {
        if(!_M_before_begin._M_nxt) {return;}

        _node_ptr __p = _M_begin();

        while(__p) {
            _node_ptr temp = __p->_M_next();

            _node_alloc_traits::destroy(__node_alloc, __p);
            _node_alloc_traits::deallocate(__node_alloc, __p, 1);
            __p = temp;
        }

        _buckets_alloc_traits::deallocate(__buckets_alloc, _M_buckets,_M_bucket_count);
        element_count = 0;
        _M_before_begin._M_nxt = nullptr;
        _M_single_bucket = nullptr;
        _M_buckets = &_M_single_bucket;
        _M_bucket_count = 1;
        element_count = 0;
    }

    size_type size() const noexcept {return element_count;}   

    iterator begin() noexcept {return iterator(_M_begin());}

    iterator end() noexcept {return iterator(nullptr);}

    const_iterator cbegin() const noexcept {return const_iterator(_M_begin());}

    const_iterator cend() const noexcept {return const_iterator(nullptr);}
};

template<typename _Key,
    typename _Value,
    typename _Alloc = std::allocator<std::pair<const _Key, _Value>>, 
    typename _Hash = std::hash<_Key>,
    typename _Equal = std::equal_to<_Key>>
class unordered_map {

    typedef _Hashtable<const _Key, _Value, _Alloc, _Hash, _Equal> _hashtable;
    _hashtable _M_h;

public:
    typedef typename _hashtable::const_iterator const_iterator;
    typedef typename _hashtable::iterator       iterator;
    typedef typename _hashtable::size_type      size_type;
    typedef typename _hashtable::pair_type      pair_type;

    unordered_map() = default;

    unordered_map(std::initializer_list<pair_type> _list) : _M_h(_list) {}

    unordered_map(const unordered_map& __m) = default;

    unordered_map(unordered_map&& __m) : _M_h(std::move(__m)) {}

    std::pair<bool, iterator> insert(const _Key __k, const _Value __v) {return _M_h.insert(__k, __v);}

    _Value& operator[](const _Key& __k) {
        return _M_h[__k];
    }

    iterator find(const _Key& __k) {
        return _M_h.find(__k); 
    }

    const_iterator find(const _Key& __k) const {
        return _M_h.find(__k); 
    }

    _Value& at(const _Key& __k) {
        return _M_h.at(__k);
    }

    const _Value& at(const _Key& __k) const {
        return _M_h.at(__k);
    }

    void erase(const _Key& __k) {
        _M_h.erase(__k);
    }

    iterator erase(const iterator& __i) {
        return _M_h.erase(__i);
    }

    size_type size() const noexcept {return _M_h.size();}

    void rehash(size_type __count) {_M_h.rehash(__count);}

    void clear() noexcept {
        _M_h.clear();
    }

    iterator begin() noexcept {return _M_h.begin();}

    iterator end() noexcept {return _M_h.end();}

    const_iterator cbegin() const noexcept {return _M_h.cbegin();}

    const_iterator cend() const noexcept {return _M_h.cend();}
};