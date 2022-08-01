#include <vector>
#include <utility>
#include <iostream>
#include <string>
#include <list>
#include <numeric>
using namespace std;

template<class T1, class T2>
struct Node
{
    T1 key;
    T2 value;
    bool exist;
    typename list<pair<T1, T2>>::iterator iterator;

    Node<T1, T2>() {key = T1(), value = T2(); exist = false;}

    Node<T1, T2>(const T1& _key, const T2& _value) 
    {
        key = _key;
        value = _value;
        exist = true;
    }

    Node<T1, T2>(const T1& _key, const T2& _value, const typename list<pair<T1, T2>>::iterator& _iterator) 
    {
        key = _key;
        value = _value;
        exist = true;
        iterator = _iterator;
    }
};

template<class T1, class T2>
class unordered_map
{
protected:
    vector<vector<Node<T1, T2>>> map;
    size_t _size;
    size_t _capacity;
    list<pair<T1, T2>> iterator_list;

public:
    unordered_map<T1, T2>() {_size = 0; _capacity = 1;}

    vector<vector<Node<T1, T2>>>& get_vector() {return map;}

    list<pair<T1, T2>>& get_list() {return iterator_list;}

    void fill()
    {
        size_t index;
        _capacity *= 2;
        vector<vector<Node<T1, T2>>> copy(_capacity);
        
        for(size_t i = 0; i<map.size() ; ++i)
        {
            for(size_t j = 0; j<map[i].size() ; ++j)
            {
                index = hash<T1>{}(map[i][j].key) % _capacity;

                copy[index].push_back(Node<T1, T2>(map[i][j]));
            }
        }

        map = copy;
    }

    void erase(const T1& key)
    {
        size_t index = hash<T1>{}(key) % _capacity;
    
        for(auto i = map[index].begin(); i != map[index].end() ; ++i)
        {
            if(key == i->key) {iterator_list.erase(i->iterator); map[index].erase(i); _size--; return ;}
        }
    }

    T2& operator[](const T1& key)
    {
        size_t index = hash<T1>{}(key) % _capacity;
        for(auto & i : map[index]) 
        {
            if(key == i.key) {return i.value;}
        }

        map[index].push_back(Node<T1, T2>(key, T2()));
        return (--map[index].end())->value;
    }

    const size_t& size() {return _size;}

    void print()
    {
        for(auto & i : iterator_list) {cout << i.first << " ";}
    }

    class iterator
    {
    public:
        typename list<pair<T1, T2>>::iterator it;
        T1 key;
        T2 value;

        iterator() {it = begin(); key = unordered_map<T1, T2>::key; value = unordered_map<T1, T2>::value;}
        iterator(const typename unordered_map<T1, T2>::iterator& _it) {it = _it; key = _it.key; value = _it.value;}
        iterator(const typename list<pair<T1, T2>>::iterator& _it) {it = _it; key = it->first; value = it->second;}

        void operator=(const typename unordered_map<T1, T2>::iterator& _it) {it = it; key = _it.key; value = _it.value;}

        bool operator==(const typename unordered_map<T1, T2>::iterator& _it) {return (it == _it.it);}

        bool operator!=(const typename unordered_map<T1, T2>::iterator& _it) {return (it != _it.it);}
        
        iterator operator++() 
        {
            ++it;
            key = it->first; value = it->second;
            return iterator(it);
        }

        iterator operator--() 
        {
            --it;
            key = it->first; value = it->second;
            return iterator(it);        
        }
    };

    iterator begin() {return iterator(iterator_list.begin());}

    iterator end() {return iterator(iterator_list.end());}

    iterator find(const pair<T1, T2>& value)
    {
        size_t index = hash<T1>{}(value.first) % _capacity;
        for(auto & i : map[index]) 
        {
            if(value.first == i.key && value.second == i.value) {return iterator(i.iterator);}
        }
         
        return iterator(iterator_list.end());
    }

    void insert(const pair<T1, T2>& value)
    {
        if(!_size)
        {
            iterator_list.push_back(value);
            map.push_back({Node<T1, T2>(value.first, value.second, --iterator_list.end())});
            _size++;
            return ;
        }

        if(_capacity == _size) {fill();}

        size_t index = hash<T1>{}(value.first) % _capacity;
        iterator_list.push_back(value);

        for(auto & i : map[index]) {if(i.key == value.first && i.value == value.second) {return;}}

        map[index].push_back(Node<T1, T2>(value.first, value.second, --iterator_list.end()));
        _size++;
    }

    void insert(const T1& key, const T2& value) {insert(make_pair(key, value));}

    void clear() {iterator_list.clear(); map.clear(); _size = 0; _capacity = 1;}

    unordered_map<T1, T2>& operator=(const unordered_map<T1, T2>& m)
    {
        map = m.get_vector();
        iterator_list = m.get_list();
        _size = m.size();
        _capacity = m.capacity();
    }

    const size_t& capacity() {return _capacity;}
};
