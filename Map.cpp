#include <iostream>
using namespace std;

template<class T>
class Node
{
protected:
    T data; int height;
    Node<T>* right, * left, * parent;

public:
    Node<T>() { data = T(), height = 1, right = left = parent = nullptr; }
    Node<T>(T d) { data = d, height = 1, right = left = parent = nullptr; }

    virtual T& getData() { return data; }
    virtual int getHeight() { return height; }
    virtual Node<T>* getRight() { return right; }
    virtual Node<T>* getLeft() { return left; }
    virtual Node<T>* getParent() { return parent; }
    virtual void setData(T d) { data = d; }
    virtual void setHeight(int h) { height = h; }
    virtual void setRight(Node<T>* r) { right = r ? r : nullptr; }
    virtual void setLeft(Node<T>* l) { left = l ? l : nullptr; }
    virtual void setParent(Node<T>* p) { parent = p ? p : nullptr; }
};

template<class T1, class T2>
class pair
{
public:
    T1 first;
    T2 second;

    pair() { first = T1(); second = T2(); }
    pair(T1 f, T2 s) { first = f; second = s; }

    void operator=(pair<T1, T2> p)
    {
        first = p.first; second = p.second;
    }

    friend void print(pair<T1, T2> p)
    {
        std::cout << p.first << " " << p.second;
    }

    friend bool operator<(pair<T1, T2> p1, pair<T1, T2> p2)
    {
        return p1.first < p2.first;
    }

    friend bool operator>(pair<T1, T2> p1, pair<T1, T2> p2)
    {
        return p1.first > p2.first;
    }

    friend bool operator==(pair<T1, T2> p1, pair<T1, T2> p2)
    {
        return p1.first == p2.first;
    }
};

template<class T>
class Avltree
{
protected:

    void fix(Node<T>* p)
    {
        if (p)
        {
            int h1 = p->getRight() ? p->getRight()->getHeight() : 0; int h2 = p->getLeft() ? p->getLeft()->getHeight() : 0;
            p->setHeight((h1 > h2 ? h1 : h2) + 1);
        }
    }

    int balanceFactor(Node<T>* a)
    {
        if (a->getRight() && a->getLeft()) { return a->getRight()->getHeight() - a->getLeft()->getHeight(); }
        else if (a->getRight()) { return a->getRight()->getHeight(); }
        else if (a->getLeft()) { return -1 * (a->getLeft()->getHeight()); }
        else { return 0; }
    }

    Node<T>* rightRotate(Node<T>* p)
    {
        Node<T>* q = p->getLeft();
        p->setLeft(q->getRight());
        q->setRight(p);
        q->setParent(p->getParent());
        p->setParent(q);
        if (p->getLeft()) { p->getLeft()->setParent(p); }
        fix(p);
        fix(q);
        return q;
    }

    Node<T>* leftRotate(Node<T>* q)
    {
        Node<T>* p = q->getRight();
        q->setRight(p->getLeft());
        p->setLeft(q);
        p->setParent(q->getParent());
        q->setParent(p);
        if (q->getRight()) { q->getRight()->setParent(q); }
        fix(p);
        fix(q);
        return p;
    }

    Node<T>* balance(Node<T>* p)
    {
        fix(p);
        if (balanceFactor(p) == 2)
        {
            if (balanceFactor(p->getRight()) < 0)
            {
                p->setRight(rightRotate(p->getRight()));
            }
            return leftRotate(p);
        }

        if (balanceFactor(p) == -2)
        {
            if (balanceFactor(p->getLeft()) > 0)
            {
                p->setLeft(leftRotate(p->getLeft()));
            }
            return rightRotate(p);
        }
        return p;
    }

public:
    Avltree<T>() {}

    virtual Node<T>* insert(Node<T>* p, const T& data)
    {
        if (!p) { return new Node<T>(data); }
        else if (data < p->getData())
        {
            Node<T>* child = insert(p->getLeft(), data);
            p->setLeft(child);
            child->setParent(p);
        }
        else if (data > p->getData())
        {
            Node<T>* child = insert(p->getRight(), data);
            p->setRight(child);
            child->setParent(p);
        }
        return balance(p);
    }

    virtual Node<T>* getMinimum(Node<T>* p)
    {
        if (p->getLeft()) { return getMinimum(p->getLeft()); }
        else { return balance(p); }
    }

    virtual Node<T>* deleteMin(Node<T>* p)
    {
        if (!p->getLeft()) { return p->getRight(); }
        p->setLeft(deleteMin(p->getLeft()));
        return balance(p);
    }

    virtual Node<T>* getMaximum(Node<T>* p)
    {
        if (p->getRight()) { return getMaximum(p->getRight()); }
        else { return balance(p); }
    }

    virtual Node<T>* deleteMaximum(Node<T>* p)
    {
        if (!p->getRight()) { return p->getLeft(); }
        p->setRight(deleteMaximum(p->getRight()));
        return balance(p);
    }

    virtual Node<T>* remove(Node<T>* p, Node<T>* data, unsigned int& size)
    {
        if (!p) { return nullptr; size--; }
        if (data->getData() < p->getData())
        {
            Node<T>* child = remove(p->getLeft(), data, size);
            p->setLeft(child);
            if (child) { child->setParent(p); }
        }
        else if (data->getData() > p->getData())
        {
            Node<T>* child = remove(p->getRight(), data, size);
            p->setRight(child);
            if (child) { child->setParent(p); }
        }
        else
        {
            Node<T>* left = p->getLeft(); Node<T>* right = p->getRight();
            if (!right) { return left; }
            Node<T>* m = getMinimum(right);
            m->setRight(deleteMin(right));
            if (m->getRight()) { m->getRight()->setParent(m); }
            m->setLeft(left);
            if (left) { left->setParent(m); }
            size--;
            return balance(m);
        }
        return balance(p);
    }
};

template<class T>
class Tree : public Avltree<T>
{
protected:
    Node<T>* root;
    unsigned int _size;

public:
    Tree() { root = nullptr; _size = 0; }
    Tree(unsigned int _size)
    {
        this->_size = _size;
        root = new Node<T>(T());
        while (--_size) { root = Avltree<T>::insert(root, T()); }
    }
    Tree(unsigned int _size, T data)
    {
            this->_size = _size;
            root = new Node<T>(data);
            while (--_size) { root = Avltree<T>::insert(root, data); }
    }

    T& operator[](const T& index)
    {
        Node<T>* curr = getRoot();

        while (curr->getData() < index && curr->getRight() || curr->getData() > index && curr->getLeft())
        {
            if (curr->getData() < index) curr = curr->getRight();
            else curr = curr->getLeft();
        }

        if (curr->getData() == index) { return curr->getData(); }
        else if (curr->getRight() && curr->getRight()->getData() == index) { return curr->getRight()->getData(); }
        else if (curr->getLeft() && curr->getLeft()->getData() == index) { return curr->getLeft()->getData(); }
        else
        {
            T d = T();
            T& data = d;
            return data;
        }
    }

    virtual unsigned int size() { return _size; }

    virtual void insert(const T& data)
    {
        root = Avltree<T>::insert(root, data);
        root = Avltree<T>::balance(root);
        _size++;
    }

    virtual void remove(Node<T>* data)
    {
        root = Avltree<T>::remove(root, data, _size);
        root = Avltree<T>::balance(root);
    }

    virtual void clear()
    {
        while (root) { root = Avltree<T>::deleteMin(root); }
    }

    virtual Node<T>* getRoot() { return root; }

    virtual void setRoot(Node<T>* r) { root = r; }
};

template<class T1, class T2>
class map : public Avltree<pair<T1, T2>>
{
protected:
    Tree<pair<T1, T2>> _map;

public:
    map() {}

    class iterator : public Avltree<pair<T1, T2>>
    {
    private:
        Node<pair<T1, T2>>* curr;

    public:
        T1 first; T2 second;

        iterator() { curr = nullptr; first = T1(); second = T2(); }
        iterator(Node<pair<T1, T2>>* p)
        {
            curr = p;
            first = curr ? curr->getData().first : T1();
            second = curr ? curr->getData().second : T2();
        }

        Node<pair<T1, T2>>* getCurr() { return curr; }

        void operator++()
        {
            if (!curr) { return; }
            if (curr->getRight()) { curr = Avltree<pair<T1, T2>>::getMinimum(curr->getRight()); }
            else
            {
                Node<pair<T1, T2>>* tmp = curr;
                bool check = true;
                while (curr->getParent() && curr->getParent()->getLeft() != curr)
                {
                    if (curr->getParent()) { curr = curr->getParent(); }
                    else { check = false; break; }
                }
                if (check) { curr = curr->getParent(); }
                else { curr = tmp; }
            }
            first = curr ? curr->getData().first : T1();
            second = curr ? curr->getData().second : T2();
        }

        void operator--()
        {
            if (curr->getLeft())
            {
                curr = Avltree<pair<T1, T2>>::getMaximum(curr->getLeft());
            }
            else
            {
                Node<pair<T1, T2>>* tmp = curr;
                bool check = true;
                while (curr->getParent() && curr->getParent()->getRight() != curr)
                {
                    if (curr->getParent()) { curr = curr->getParent(); }
                    else { check = false; break; }
                }
                if (check) { curr = curr->getParent(); }
                else { curr = tmp; }
            }
            first = curr ? curr->getData().first : T1();
            second = curr ? curr->getData().second : T2();
        }

        bool operator==(iterator p) { return curr == p.getCurr(); }
        bool operator!=(iterator p) { return curr != p.getCurr(); }
    };

    virtual iterator begin() { return iterator(Avltree<pair<T1, T2>>::getMinimum(_map.getRoot())); }

    virtual iterator end() { return iterator(Avltree<pair<T1, T2>>::getMaximum(_map.getRoot())); }

    virtual Tree<pair<T1, T2>> get_map() { return _map; }

    virtual unsigned int size() { return _map.size(); }

    virtual void insert(const T1& index, const T2& data)
    {
        pair<T1, T2> p(index, data);
        _map.insert(p);
    }

    virtual void erase(iterator i)
    {
        _map.remove(i.getCurr());
    }

    virtual void clear()
    {
        _map.clear();
    }

    T2& operator[](const T1& index)
    {
        pair<T1, T2> p(index, T2());
        _map.insert(p);
        return _map[p].second;
    }

    virtual void operator=(map<T1, T2> m)
    {
        _map = m._map;
    }
};
