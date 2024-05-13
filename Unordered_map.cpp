#include <bits/stdc++.h>

struct _Hash_node_base {
    _Hash_node_base* _M_nxt;

    _Hash_node_base() : _M_nxt(nullptr) {}
    _Hash_node_base(_Hash_node_base* next) : _M_nxt(next) {}

    virtual ~_Hash_node_base() {_M_nxt = nullptr;}
};

template<typename _Value>
struct _Hash_node_val {
    typedef _Value value_type;

    _Value _M_storage;
    size_t _M_hash;

    value_type* ptr() {return std::addressof(_M_storage);}

    _Hash_node_val(const _Value& __v) : _M_storage(__v), _M_hash(0) {}

    _Hash_node_val(const _Value& __v, size_t __h) : _M_storage(__v), _M_hash(__h) {}

    _Hash_node_val() = default;
};

template<typename _Value>
struct _Hash_node 
        : _Hash_node_base
        , _Hash_node_val<_Value> {
    _Hash_node* _M_next() {return static_cast<_Hash_node*>(_M_nxt);}

    _Hash_node* next() {return static_cast<_Hash_node*>(_M_nxt);}

    _Hash_node(const _Value& __v) : _Hash_node_val<_Value>(__v) {}

    _Hash_node(const _Value& __v, size_t __h) : _Hash_node_val<_Value>::_M_storage(__v) {_Hash_node_val<_Value>::_M_hash = __h;}

    _Hash_node() = default;
};

template<typename _Value>
struct _Node_iterator_base {
    using _node_type = _Hash_node<_Value>;

    _node_type* _M_iterator;

    _Node_iterator_base() : _M_iterator() {}
    _Node_iterator_base(_node_type* __p) : _M_iterator(__p) {}

    void incr() {_M_iterator = _M_iterator->_M_next();}

    friend bool operator==(const _Node_iterator_base& __x, const _Node_iterator_base& __y) {
        return __x._M_iterator == __y._M_iterator;
    }

    friend bool operator!=(const _Node_iterator_base& __x, const _Node_iterator_base& __y) {
        return __x._M_iterator != __y._M_iterator;
    }
};

template<typename _Value>
struct _Node_iterator : public _Node_iterator_base<_Value> {
private:
    using _base_type = _Node_iterator_base<_Value>;
    using _node_type = typename _base_type::_node_type;

public:
    typedef _Value value_type;
    typedef _Node_iterator<_Value> iterator;

    using pointer = _Value*;
    using reference = _Value&;

    _Node_iterator() = default;

    _Node_iterator(_node_type* __p) : _base_type(__p) {}

    reference operator*() {return this->_M_iterator->_M_storage;}

    pointer operator->() {return this->_M_iterator->ptr();}
    
    _Node_iterator& operator++() {
        this->incr();
        return *this;
    }

    _Node_iterator operator++(int) {
        _Node_iterator __tmp(*this);
        this->incr();
        return __tmp;
    }

    _node_type* _M_node() {return this->_M_iterator;}
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
    std::size_t operator()(std::size_t __n, std::size_t __d) {
        return __n % __d;
    }
};

template<typename _Key, 
    typename _Value, 
    typename _Alloc, 
    typename _Hash, 
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
    using __hash = _Hashtable_hash<_Hash>;

    typename _Alloc::template rebind<_node_ptr>::other __buckets_alloc;
    typename _Alloc::template rebind<_node_type>::other __node_alloc;
    typedef typename _Alloc::template rebind<_node_ptr>::other __buckets_alloc_type;
    typedef typename _Alloc::template rebind<_node_type>::other __node_alloc_type;
    using _buckets_alloc_traits = std::allocator_traits<__buckets_alloc_type>;
    using _node_alloc_traits = std::allocator_traits<__node_alloc_type>;

    typedef std::size_t _hash_code;
    typedef std::size_t size_type;

    _node_ptr _M_single_bucket = nullptr;

private:
    _buckets_ptr _M_buckets = &_M_single_bucket;
    size_type _M_bucket_count = 1;
    _node_base _M_before_begin;
    size_type element_count = 0;

    _hash_code _M_hash_code(const _Key& __k) const {
        return _M_hash()(__k);
    }

    _node_ptr _M_begin() {return static_cast<_node_ptr>(_M_before_begin._M_nxt); }

    const _Hash&  _M_hash() const { return __hash::_get_const(); }

    size_type _M_bucket_index(size_type _code, size_type _b) {
        return _Ranagehash{}(_code, _b);
    }

    void insert_bucket_begin(size_type __x, _node_ptr __node) {
        if(_M_buckets[__x]) {
            __node->_M_nxt = _M_buckets[__x]->_M_nxt;
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

    void rehash(size_type __count) {
        _buckets_ptr __copy_buckets = _buckets_alloc_traits::allocate(__buckets_alloc, __count);
        _node_ptr __p = _M_begin();
        _M_before_begin = nullptr;
        size_type __bbegin_bkt = 0;

        while (__p)
        {
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

    iterator _M_insert_unique_node(size_type __bkt, size_type __code, _node_ptr __node, size_type __ic = 1) {
        size_t __rehash_count = 0;

        if(!element_count || __ic + element_count > _M_bucket_count) {
            __rehash_count = std::max((__ic + element_count) * 2, static_cast<size_type>(13));
        }

        if(__rehash_count) {
            rehash(__rehash_count);
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
            if(__node->_M_storage.first == __i.first) {
                return {false, iterator(__node)};
            }
            __node = __node->_M_next();
        }

        __node = _node_alloc_traits::allocate(__node_alloc, 1);
        ::new(__node) pair_type(__i);
        _node_alloc_traits::construct(__node_alloc, __node, __i);
        __node->_M_hash = __bkt;

        return {true, _M_insert_unique_node(__bkt, __code, __node)};
    }

public:
    _Hashtable() = default;

    _Hashtable(std::initializer_list<pair_type> _list) {
        rehash(_list.size());

        for(auto & i : _list) {
            emplace(std::make_pair(i.first, i.second));
        }
    }

    std::pair<bool, iterator> insert(const _Key& __k, const _Value& __v) {
        return emplace(std::make_pair(__k, __v));
    }

    _Value& operator[](const _Key& __k) {
        pair_type __p = {__k, _Value()};
        iterator __i = emplace(__p).second;
        return __i->second;
    }
    
    iterator find(const _Key& __k) const {
        _hash_code __code = _M_hash_code(__k);
        _hash_code __bkt = _M_bucket_index(__code, _M_bucket_count);

        _node_ptr __node = _M_buckets[__bkt];

        while(__node && __node->_M_hash == __bkt) {
            if(__node->_M_storage.first == __k) {
                return iterator(__node);
            }
            __node = __node->_M_next();
        }
        return end();
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
                }
                temp->_M_nxt = nullptr;
                if(temp) {
                    _node_alloc_traits::destroy(__node_alloc, temp);
                    _node_alloc_traits::deallocate(__node_alloc, temp, 1);
                }
                --element_count;
                return;
            }
            temp = temp->_M_next();
            __node = __node->_M_next();
        }
    }

    iterator erase(iterator __i) {
        if(!__i._M_node()) {return __i;}
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
                }
                temp->_M_nxt = nullptr;
                if(temp) {
                    _node_alloc_traits::destroy(__node_alloc, temp);
                    _node_alloc_traits::deallocate(__node_alloc, temp, 1);
                }
                --element_count;
                return iterator(__node);
            }
            temp = temp->_M_next();
            __node = __node->_M_next();
        }
        return end();
    }

    size_type size() const {return element_count;}   

    iterator begin() {return iterator(_M_begin());}

    iterator end() {return iterator(nullptr);}

    void print() {
        _node_ptr p = _M_begin();
        while(p) {
            std::cout << p->_M_hash << " " << p->_M_storage.second << "\n";
            p = p->_M_next();
        }
    }
};

template<typename _Key,
    typename _Value,
    typename _Alloc = std::allocator<std::pair<const _Key, _Value>>, 
    typename _Hash = std::hash<_Key>>
class unordered_map {

    typedef _Hashtable<const _Key, _Value, _Alloc, _Hash> _hashtable;
    _hashtable _M_h;

public:
    typedef typename _hashtable::iterator     iterator;
    typedef typename _hashtable::size_type    size_type;
    typedef typename _hashtable::pair_type    pair_type;

    unordered_map() = default;

    unordered_map(std::initializer_list<pair_type> _list) : _M_h(_list) {}

    std::pair<bool, iterator> insert(const _Key __k, const _Value __v) {return _M_h.insert(__k, __v);}

    _Value& operator[](const _Key& __k) {
        return _M_h[__k];
    }

    iterator find(const _Key& __k) const {
        return _M_h.find(__k); 
    }

    void erase(const _Key& __k) {
        _M_h.erase(__k);
    }

    iterator erase(const iterator& __i) {
        return _M_h.erase(__i);
    }

    size_type size() const {return _M_h.size();}

    void rehash() {_M_h.rehash();}

    iterator begin() {return _M_h.begin();}

    iterator end() {return _M_h.end();}

    void print() {_M_h.print();}
};
