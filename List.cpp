#include <bits/stdc++.h>

struct _List_node_base {
	_List_node_base* _M_prev;
	_List_node_base* _M_next;
};

template<typename T>
struct _List_node : _List_node_base {
	T _M_value;

	_List_node(const T& __x) : _M_value(__x) {}

	T* ptr() {return std::addressof(_M_value);}

    const T* ptr() const {return std::addressof(_M_value);}
};

template<typename T>
struct _List_iterator {
	_List_node_base* _M_iterator;

	typedef _List_node_base*   _Base_ptr;
	typedef _List_iterator     iterator;
	typedef _List_node<T>*     _Node_ptr;

	typedef T*                 pointer;
	typedef T&                 reference;
	typedef T                  value_type;

	_List_iterator() noexcept : _M_iterator() {}
	_List_iterator(_Base_ptr __p) noexcept : _M_iterator(__p) {}

	_Base_ptr get_base_ptr() noexcept {return _M_iterator;}

	pointer operator->() {return static_cast<_Node_ptr>(_M_iterator)->ptr();}

	reference operator*() {return *static_cast<_Node_ptr>(_M_iterator)->ptr();}

	iterator operator++() noexcept {
		_M_iterator = _M_iterator->_M_next;
		return *this;
	}

	iterator operator++(int) noexcept {
		iterator __temp = _M_iterator;
		_M_iterator = _M_iterator->_M_next;
		return __temp;
	}

	iterator operator--() noexcept {
		_M_iterator = _M_iterator->_M_prev;
		return *this;
	}

	iterator operator--(int) noexcept {
		iterator __temp = _M_iterator;
		_M_iterator = _M_iterator->_M_prev;
		return __temp;
	}

	friend bool operator==(const iterator& __l, const iterator& __r) noexcept {
		return __l._M_iterator == __r._M_iterator;
	}

	friend bool operator!=(const iterator& __l, const iterator& __r) noexcept {
		return __l._M_iterator != __r._M_iterator;
	}
};

template<typename T>
struct _List_const_iterator {
	const _List_node_base* _M_iterator;

	typedef const _List_node_base*         _Base_ptr;
	typedef const _List_const_iterator     const_iterator;
	typedef const _List_node<T>*           _Node_ptr;

	typedef const T*                       pointer;
	typedef const T&                       reference;
	typedef T                        value_type;

	_List_const_iterator() noexcept : _M_iterator() {}

	_List_const_iterator(_Base_ptr __p) noexcept : _M_iterator(__p) {}

	_List_const_iterator(const _List_iterator<T>& __i) noexcept : _M_iterator(__i._M_iterator) {}
 
	_Base_ptr get_base_ptr() const noexcept {return _M_iterator;}

	pointer operator->() const {return static_cast<_Node_ptr>(_M_iterator)->ptr();}

	reference operator*() const {return *static_cast<_Node_ptr>(_M_iterator)->ptr();}

	const_iterator operator++() noexcept {
		_M_iterator = _M_iterator->_M_next;
		return *this;
	}

	const_iterator operator++(int) noexcept {
		const_iterator __temp = _M_iterator;
		_M_iterator = _M_iterator->_M_next;
		return __temp;
	}

	const_iterator operator--() noexcept {
		_M_iterator = _M_iterator->_M_prev;
		return *this;
	}

	const_iterator operator--(int) noexcept {
		const_iterator __temp = _M_iterator;
		_M_iterator = _M_iterator->_M_prev;
		return __temp;
	}

	friend bool operator==(const const_iterator& __l, const const_iterator& __r) noexcept {
		return __l.get_base_ptr() == __r.get_base_ptr();
	}

	friend bool operator!=(const const_iterator& __l, const const_iterator& __r) noexcept {
		return __l.get_base_ptr() != __r.get_base_ptr();
	}
};

template<typename T, typename _Alloc>
class _List_base {
public:
	typedef _List_node_base         _Base_node;
	typedef _List_node_base*        _Base_node_ptr;
	typedef _List_node<T>           _Node;
	typedef _List_node<T>*          _Node_ptr;
	typedef std::size_t             size_type;
	using iterator = typename _List_iterator<T>::iterator;
	using const_iterator = typename _List_const_iterator<T>::const_iterator;

	struct _Node_header : _Base_node {
		std::size_t __count = 0;
	};

	_Node_header _M_l;
	size_type    _size;
	_List_base() : _M_l{&_M_l, &_M_l}, _size() {}
};

template<typename T, typename _Alloc = std::allocator<T>>
class list : public _List_base<T, _Alloc> {
public:
	typedef _List_base<T, _Alloc>     list_base;
	typedef list<T, _Alloc>           list_type;

	using _Base_node = typename list_base::_Base_node;
	using _Base_node_ptr = typename list_base::_Base_node_ptr;
	using _Node = typename list_base::_Node;
	using _Node_ptr = typename list_base::_Node_ptr;
	using size_type = typename list_base::size_type;
	using iterator = typename list_base::iterator;
	using const_iterator = typename list_base::const_iterator;


	typename _Alloc::template rebind<_Node>::other _allocator;
    typedef typename _Alloc::template rebind<_Node>::other _alloc_type;
    using _Alloc_traits = std::allocator_traits<_alloc_type>;

	template<typename ... Args>
	_Node_ptr create(Args&& ... _args) {
		try {
			_Node_ptr __node = _Alloc_traits::allocate(_allocator, 1);
			_Alloc_traits::construct(_allocator, __node, std::forward<Args>(_args)...);
			return __node;
		} catch(...) {
			throw ;
		}
	}

	void destroy_node(_Node_ptr __node) noexcept {
		_Alloc_traits::destroy(_allocator, __node);
		_Alloc_traits::deallocate(_allocator, __node, 1);
	}

	template<typename ... Args>
	void emplace_back(Args&& ... _args) {
		emplace(const_iterator(this->_M_l._M_prev), std::forward<Args>(_args)...);
	}

	template<typename ... Args>
	void emplace_front(Args&& ... _args) {
		emplace(const_iterator(&this->_M_l), std::forward<Args>(_args)...);
	}
 
	template<typename ... Args>
	void emplace(const_iterator pos, Args&& ... _args) {
		_Node_ptr __node = create(std::forward<Args>(_args)...);
		_Base_node_ptr __before = const_cast<_Base_node_ptr>(pos.get_base_ptr());
		++this->_size;
		__node->_M_prev = __before;
		_Base_node_ptr __temp = __before->_M_next;
		__before->_M_next = __node;
		__node->_M_next = __temp;
		__temp->_M_prev = __node;
	}

	void push_back(const T& __x) {
		emplace_back(__x);
	}

	void push_back(T&& __x) {
		emplace_back(std::move(__x));
	}

	void push_front(const T& __x) {
		emplace_front(__x);
	}

	void push_front(T&& __x) {
		emplace_front(std::move(__x));
	}

	void insert(const_iterator pos, const T& __x) {
		emplace(pos, __x);
	}

	void insert(const_iterator pos, T&& __x) {
		emplace(pos, std::move(__x));
	}

	void erase(const_iterator pos) noexcept {
		_Base_node_ptr __it = const_cast<_Base_node_ptr>(pos.get_base_ptr());
		if(!__it || pos == cend()) {return;}
		_Base_node_ptr __before = __it->_M_prev;
		_Base_node_ptr __next = __it->_M_next;
		if(__before) {__before->_M_next = __next;}
		if(__next) {__next->_M_prev = __before;}
		--this->_size;
		destroy_node(static_cast<_Node_ptr>(__it));
	}

	void pop_front() {
		erase(const_iterator(this->_M_l._M_next));
	}

	void pop_back() {
		erase(const_iterator(this->_M_l._M_prev));
	}

	void clear() {
		_Base_node_ptr __p = this->_M_l._M_next;
		while(__p != &this->_M_l) {
			_Node* __tmp = static_cast<_Node*>(__p);
			__p = __tmp->_M_next;
			--this->_size;;
			destroy_node(__tmp);
		}
	}

	size_type size() const noexcept {return this->_size;}

public:
	list() : list_base() {}

	list(std::initializer_list<T> __l) : list_base() {
		for(auto & i : __l) {
			emplace_back(i);
		}
	}

	list(const list_type& __l) : list_base() {
		for(auto & i : __l) {
			emplace_back(i);
		}
	}

	list_type& operator=(const list_type& __l) {
		clear();

		auto i = __l.cbegin();
		while(i != __l.cend()) {
			emplace_back(*i);
			++i;
		}
		return *this;
	}

	list_type& operator=(std::initializer_list<T> __l) {
		clear();

		for(auto & i : __l) {
			emplace_back(i);
		}
		return *this;
	}

	iterator end() noexcept {return iterator(&this->_M_l);}

	iterator begin() noexcept {return iterator(this->_M_l._M_next);}

	const_iterator cend() const noexcept {return const_iterator(&this->_M_l);}

	const_iterator cbegin() const noexcept {return const_iterator(this->_M_l._M_next);}
};