#include <iostream>

template<class T>
class Node
{
protected:
    T data; int height;
    Node<T>* right, * left, * parent;

public:
    Node<T>() { data = T(), height = 1, right = left = parent = nullptr; }
    Node<T>(T d) { data = d, height = 1, right = left = parent = nullptr; }

    virtual T& getData() { T& d = data; return d; }
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

    virtual Node<T>* insert(Node<T>* p, T data)
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

    virtual Node<T>* remove(Node<T>* p, Node<T>* data, int& sz)
    {
        if (!p) { return nullptr; sz--; }
        if (data->getData() < p->getData())
        {
            Node<T>* child = remove(p->getLeft(), data, sz);
            p->setLeft(child);
            if (child) { child->setParent(p); }
        }
        else if (data->getData() > p->getData())
        {
            Node<T>* child = remove(p->getRight(), data, sz);
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
            sz--;
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
    int sz;

public:
    Tree() { root = nullptr; sz = 0; }
    Tree(int size)
    {
        if (size >= 0)
        {
            sz = size;
            if (sz > 0) root = new Node<T>(T());
            while (--size) { root = Avltree<T>::insert(root, T()); }
        }
        else { sz = 0; root = nullptr; }
    }
    Tree(int size, T data)
    {
        if (size >= 0)
        {
            sz = size;
            if (sz > 0) root = new Node<T>(data);
            while (--size) { root = Avltree<T>::insert(root, data); }
        }
        else { sz = 0; root = nullptr; }
    }

    T& operator[](T index)
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

    virtual int size() { return sz; }

    virtual void insert(T data)
    {
        root = Avltree<T>::insert(root, data);
        root = Avltree<T>::balance(root);
        sz++;
    }

    virtual void remove(Node<T>* data)
    {
        root = Avltree<T>::remove(root, data, sz);
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
    Tree<pair<T1, T2>> Map;

public:
    map() {}

        class iterator : public Avltree<pair<T1, T2>>
    {
    private:
        Node<pair<T1, T2>>* curr;

    public:
        T1 first; T2 second;

        iterator() { curr = nullptr; first = T1(); second = T2(); }
        iterator(Node<pair<T1, T1>>* p)
        {
            curr = p;
            first = curr ? curr->getData().first : T1();
            second = curr ? curr->getData().second : T2();
        }

        Node<pair<T1, T1>>* getCurr() { return curr; }

        void operator++()
        {
            if (!curr) { return; }
            if (curr->getRight()) { curr = Avltree<pair<T1, T1>>::getMinimum(curr->getRight()); }
            else
            {
                Node<pair<T1, T1>>* tmp = curr;
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
                curr = Avltree<pair<T1, T1>>::getMaximum(curr->getLeft());
            }
            else
            {
                Node<pair<T1, T1>>* tmp = curr;
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

    virtual iterator begin() { return iterator(Avltree<pair<T1, T1>>::getMinimum(Map.getRoot())); }

    virtual iterator end() { return iterator(Avltree<pair<T1, T1>>::getMaximum(Map.getRoot())); }

    virtual Tree<pair<T1, T2>> getMap() { return Map; }

    virtual int size() { return Map.size(); }

    virtual void insert(T1 index, T2 data)
    {
        pair<T1, T2> p(index, data);
        Map.insert(p);
    }

    virtual void erase(map<T1, T2>::iterator i)
    {
        Map.remove(i.getCurr());
    }

    virtual void clear()
    {
        Map.clear();
    }

    T2& operator[](T1 index)
    {
        pair<T1, T2> p(index, T2());
        Map.insert(p);
        return Map[p].second;
    }

    virtual void operator=(map<T1, T2> m)
    {
        Map = m.Map;
    }
};
