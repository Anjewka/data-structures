#include <bits/stdc++.h>

template<class T>
class Node
{
protected:
    T data; unsigned int height;
    std::shared_ptr<Node<T>> right, left, parent;

public:
    Node<T>() { data = T(), height = 1; }
    Node<T>(T d) { data = d, height = 1; }

    T& getData() { return data; }
    unsigned int getHeight() { return height; }
    std::shared_ptr<Node<T>> getRight() { return right; }
    std::shared_ptr<Node<T>> getLeft() { return left; }
    std::shared_ptr<Node<T>> getParent() { return parent; }
    void setData(T d) { data = d; }
    void setHeight(int h) { height = h; }
    void setRight(std::shared_ptr<Node<T>> r) { right = r ? r : nullptr; }
    void setLeft(std::shared_ptr<Node<T>> l) { left = l ? l : nullptr; }
    void setParent(std::shared_ptr<Node<T>> p) { parent = p ? p : nullptr; }
};

template<class T1, class T2>
class Pair
{
public:
    T1 first;
    T2 second;

    Pair() { first = T1(); second = T2(); }
    Pair(T1 f, T2 s) { first = f; second = s; }

    void operator=(Pair<T1, T2> p)
    {
        first = p.first; second = p.second;
    }

    friend void print(Pair<T1, T2> p)
    {
        std::cout << p.first << " " << p.second;
    }

    friend bool operator<(const Pair<T1, T2>& p1, const Pair<T1, T2>& p2)
    {
        return p1.first < p2.first;
    }

    friend bool operator>(const Pair<T1, T2>& p1, const Pair<T1, T2>& p2)
    {
        return p1.first > p2.first;
    }

    friend bool operator==(const Pair<T1, T2>& p1, const Pair<T1, T2>& p2)
    {
        return p1.first == p2.first;
    }
};

template<class T>
class Avltree
{
protected:

    void fix(std::shared_ptr<Node<T>> p)
    {
        if (p)
        {
            int h1 = p->getRight() ? p->getRight()->getHeight() : 0; int h2 = p->getLeft() ? p->getLeft()->getHeight() : 0;
            p->setHeight((h1 > h2 ? h1 : h2) + 1);
        }
    }

    int balanceFactor(std::shared_ptr<Node<T>> a)
    {
        if (a->getRight() && a->getLeft()) { return a->getRight()->getHeight() - a->getLeft()->getHeight(); }
        else if (a->getRight()) { return a->getRight()->getHeight(); }
        else if (a->getLeft()) { return -1 * (a->getLeft()->getHeight()); }
        else { return 0; }
    }

    std::shared_ptr<Node<T>> rightRotate(std::shared_ptr<Node<T>> p)
    {
        std::shared_ptr<Node<T>> q = p->getLeft();
        p->setLeft(q->getRight());
        q->setRight(p);
        q->setParent(p->getParent());
        p->setParent(q);
        if (p->getLeft()) { p->getLeft()->setParent(p); }
        fix(p);
        fix(q);
        return q;
    }

    std::shared_ptr<Node<T>> leftRotate(std::shared_ptr<Node<T>> q)
    {
        std::shared_ptr<Node<T>> p = q->getRight();
        q->setRight(p->getLeft());
        p->setLeft(q);
        p->setParent(q->getParent());
        q->setParent(p);
        if (q->getRight()) { q->getRight()->setParent(q); }
        fix(p);
        fix(q);
        return p;
    }

    std::shared_ptr<Node<T>> balance(std::shared_ptr<Node<T>> p)
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

    std::shared_ptr<Node<T>> insert(std::shared_ptr<Node<T>> p, const T& data)
    {
        if (!p) { return std::make_shared<Node<T>>(data);}
        else if (data < p->getData())
        {
            std::shared_ptr<Node<T>> child = insert(p->getLeft(), data);
            p->setLeft(child);
            child->setParent(p);
        }
        else if (data > p->getData())
        {
            std::shared_ptr<Node<T>> child = insert(p->getRight(), data);
            p->setRight(child);
            child->setParent(p);
        }
        return balance(p);
    }

    std::shared_ptr<Node<T>> getMinimum(std::shared_ptr<Node<T>> p)
    {
        if (p->getLeft()) { return getMinimum(p->getLeft()); }
        else { return balance(p); }
    }

    std::shared_ptr<Node<T>> deleteMin(std::shared_ptr<Node<T>> p)
    {
        if (!p->getLeft()) { return p->getRight(); }
        p->setLeft(deleteMin(p->getLeft()));
        return balance(p);
    }

    std::shared_ptr<Node<T>> getMaximum(std::shared_ptr<Node<T>> p)
    {
        if (p->getRight()) { return getMaximum(p->getRight()); }
        else { return balance(p); }
    }

    std::shared_ptr<Node<T>> deleteMaximum(std::shared_ptr<Node<T>> p)
    {
        if (!p->getRight()) { return p->getLeft(); }
        p->setRight(deleteMaximum(p->getRight()));
        return balance(p);
    }

    std::shared_ptr<Node<T>> remove(std::shared_ptr<Node<T>> p, std::shared_ptr<Node<T>> data, unsigned int& size)
    {
        if (!p) { return nullptr; size--; }
        if (data->getData() < p->getData())
        {
            std::shared_ptr<Node<T>> child = remove(p->getLeft(), data, size);
            p->setLeft(child);
            if (child) { child->setParent(p); }
        }
        else if (data->getData() > p->getData())
        {
            std::shared_ptr<Node<T>> child = remove(p->getRight(), data, size);
            p->setRight(child);
            if (child) { child->setParent(p); }
        }
        else
        {
            std::shared_ptr<Node<T>> left = p->getLeft(); std::shared_ptr<Node<T>> right = p->getRight();
            if (!right) { return left; }
            std::shared_ptr<Node<T>> m = getMinimum(right);
            m->setRight(deleteMin(right));
            if (m->getRight()) { m->getRight()->setParent(m); }
            m->setLeft(left);
            if (left) { left->setParent(m); }
            return balance(m);
        }
        return balance(p);
    }
};

template<class T>
class Tree : public Avltree<T>
{
protected:
    std::shared_ptr<Node<T>> root;
    unsigned int _size;

public:
    Tree() { root = nullptr; _size = 0; }
    Tree(unsigned int _size)
    {
        this->_size = _size;
        root = std::make_shared<Node<T>>(T());
        while (--_size) { root = Avltree<T>::insert(root, T()); }
    }

    std::shared_ptr<Node<T>> operator[](const T& index)
    {
        std::shared_ptr<Node<T>> curr = getRoot();

        while (curr->getData() < index && curr->getRight() || curr->getData() > index && curr->getLeft())
        {
            if (curr->getData() < index) curr = curr->getRight();
            else curr = curr->getLeft();
        }

        if (curr->getData() == index) { return curr; }
        else if (curr->getRight() && curr->getRight()->getData().first == index.first) { return curr->getRight(); }
        else if (curr->getLeft() && curr->getLeft()->getData().first == index.first) { return curr->getLeft(); }
        else
        {
            return nullptr;
        }
    }

    unsigned int size() { return _size; }

    void insert(const T& data)
    {
        root = Avltree<T>::insert(root, data);
        _size++;
    }

    void remove(std::shared_ptr<Node<T>> data)
    {
        root = Avltree<T>::remove(root, data, _size);
        _size--;
    }

    void clear()
    {
        while (root) { root = Avltree<T>::deleteMin(root); }
    }

    std::shared_ptr<Node<T>> getRoot() { return root; }

    void setRoot(std::shared_ptr<Node<T>> r) { root = r; }
};

template<class T1, class T2>
class map : public Avltree<Pair<T1, T2>>
{
protected:
    Tree<Pair<T1, T2>> _map;

public:
    map() {}

    class iterator : public Avltree<Pair<T1, T2>>
    {
    private:
        std::shared_ptr<Node<Pair<T1, T2>>> curr;

    public:
        T1 first; T2 second;

        iterator() { curr = nullptr; first = T1(); second = T2(); }
        iterator(std::shared_ptr<Node<Pair<T1, T2>>> p)
        {
            curr = p;
            first = curr ? curr->getData().first : T1();
            second = curr ? curr->getData().second : T2();
        }

        std::shared_ptr<Node<Pair<T1, T2>>> getCurr() { return curr; }

        void operator++()
        {
            if (!curr) { return; }
            if (curr->getRight()) { curr = Avltree<Pair<T1, T2>>::getMinimum(curr->getRight()); }
            else
            {
                std::shared_ptr<Node<Pair<T1, T2>>> tmp = curr;
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

        friend iterator operator+(iterator i, unsigned int number)
        {
            iterator i1 = i;
            while(number)
            {
                ++i1;
                number--;
            }
            return i1;
        }

        void operator--()
        {
            if (curr->getLeft())
            {
                curr = Avltree<Pair<T1, T2>>::getMaximum(curr->getLeft());
            }
            else
            {
                std::shared_ptr<Node<Pair<T1, T2>>> tmp = curr;
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

    iterator begin() { return iterator(Avltree<Pair<T1, T2>>::getMinimum(_map.getRoot())); }

    iterator end() { return iterator(Avltree<Pair<T1, T2>>::getMaximum(_map.getRoot())); }

    T2 max() { return Avltree<Pair<T1, T2>>::getMaximum(_map.getRoot())->getData().first; }

    Tree<Pair<T1, T2>> get_map() { return _map; }

    unsigned int size() { return _map.size(); }

    void insert(const T1& index, const T2& data)
    {
        Pair<T1, T2> p(index, data);
        _map.insert(p);
    }

    void erase(iterator i)
    {
        if(!i.getCurr()) {return;}
        _map.remove(i.getCurr());
    }

    void clear()
    {
        _map.clear();
    }

    T2& operator[](const T1& index)
    {
        Pair<T1, T2> p(index, T2());

        if(_map.size() && _map[p] != nullptr) {return _map[p]->getData().second;}
        _map.insert(p);
        return _map[p]->getData().second;
    }

    void operator=(const map<T1, T2>& m)
    {
        _map = m._map;
    }
};

template<class T>
void print(std::shared_ptr<T> p)
{
    if (!p) { return void(); }
    // print(p->getLeft());
    std::cout << p->getLeft()->getData().second << " ";
    // print(p->getRight());
}

int main()
{
    map<int, int> p;
    p.insert(1, 1);
    p.insert(2, 2);

    p.insert(3, 3);
    p.insert(4, 4);
    p.insert(5, 5);
    p.insert(6, 6);
    p.insert(7, 7);
    p.insert(8, 8);

    map<int, int>::iterator i = p.begin();
    while(i != p.end())
    {
        std::cout << i.first << " " << i.second << '\n';
        ++i;
    }
    // std::shared_ptr<Node<int>> p = std::make_shared<Node<int>>(new Node<int>(5));
    // std::cout << a->getRight();
    // map<int, int> m;
    // m.insert(1, 1);
}