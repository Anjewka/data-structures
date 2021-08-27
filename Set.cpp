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

    friend ostream& operator<<(ostream& output, Node<T> *root) 
    {
        if (!root) { return output; }
        return output << root->getLeft() << root->getData() << " " << root->getRight();
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
        _size--;
    }

    virtual void clear()
    {
        while (root) { root = Avltree<T>::deleteMin(root); _size = 0; }
    }

    virtual Node<T>* getRoot() { return root; }

    virtual void setRoot(Node<T>* r) { root = r; }
};

template<class T>
class set : public Tree<T>
{
protected:
    Tree<T> _set;

public:
    set(int s) { while (s--) { _set.insert(T()); } }
    set() {}

    class iterator : public Avltree<T>
    {
    private:
        Node<T>* curr;

    public:

        iterator() { curr = nullptr; }
        iterator(Node<T>* p)
        {
            curr = p;
        }

        Node<T>* getCurr() { return curr; }

        void operator++()
        {
            if (curr->getRight()) { curr = Avltree<T>::getMinimum(curr->getRight()); }
            else
            {
                Node<T>* tmp = curr;
                bool check = true;
                while (curr->getParent() && curr->getParent()->getLeft() != curr)
                {
                    if (curr->getParent()) { curr = curr->getParent(); }
                    else { check = false; break; }
                }
                if (check) { curr = curr->getParent(); }
                else { curr = tmp; }
            }
        }

        void operator--()
        {
            if (curr->getLeft())
            {
                curr = Avltree<T>::getMaximum(curr->getLeft());
            }
            else
            {
                Node<T>* tmp = curr;
                bool check = true;
                while (curr->getParent() && curr->getParent()->getRight() != curr)
                {
                    if (curr->getParent()) { curr = curr->getParent(); }
                    else { check = false; break; }
                }
                if (check) { curr = curr->getParent(); }
                else { curr = tmp; }
            }
        }

        T& operator*() { return curr->getData(); }

        bool operator==(iterator p) { return curr == p.getCurr(); }
        bool operator!=(iterator p) { return curr != p.getCurr(); }
    };

    virtual iterator begin() { return iterator(Avltree<T>::getMinimum(_set.getRoot())); }

    virtual iterator end() { return iterator(Avltree<T>::getMaximum(_set.getRoot())); }

    Tree<T> getTree() { return _set; }

    virtual void clear() { _set.clear(); }

    virtual void insert(const T& data)
    {
        _set.insert(data);
    }

    virtual void erase(set<T>::iterator i)
    {
        _set.remove(i.getCurr());
    }

    virtual unsigned int size() { return _set.size(); }

    friend ostream& operator<<(ostream& output, set<T> p) { return output << p.getTree(); }
};
