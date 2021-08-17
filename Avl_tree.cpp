#include <iostream>
using namespace std;

class Node
{
protected:
	int data; int height;
	Node* right, *left;

public:
	Node() {data = 0, height = 1, right = left = nullptr;}
	Node(int d) {data = d, height = 1, right = left = nullptr;}

	virtual int getData() {return data;}
	virtual int getHeight() {return height;}
	virtual Node* getRight() {return right;}
	virtual Node* getLeft() {return left;} 
	virtual void setData(int d) {data = d;}
	virtual void setHeight(int h) {height = h;}
	virtual void setRight(Node* r) {right = r;}
	virtual void setLeft(Node* l) {left = l;}

	friend ostream &operator<<(ostream &output, Node* a)
	{
		if(!a) return output;
		else return output << a->getLeft() << a->getData() << " " << a->getRight();
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
