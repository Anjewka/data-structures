#include <iostream>
using namespace std;

template<class type>
class Node
{
protected:
	type data; int height;
	Node<type>* right, *left;

public:
	Node<type>() {data = type(), height = 1, right = left = nullptr;}
	Node<type>(type d) {data = d, height = 1, right = left = nullptr;}

	virtual type getData() {return data;}
	virtual int getHeight() {return height;}
	virtual Node* getRight() {return right;}
	virtual Node* getLeft() {return left;} 
	virtual void setData(type d) {data = d;}
	virtual void setHeight(int h) {height = h;}
	virtual void setRight(Node* r) {right = r;}
	virtual void setLeft(Node* l) {left = l;}

	friend ostream &operator<<(ostream &output, Node<type>* a)
	{
		if(!a) return output;
		else return output << a->getLeft() << a->getData() << " " << a->getRight();
	}
};

template<class type>
class Avltree : public Node<type>
{
protected:
    
	void fix(Node<type>* p)
	{
		if(p)
		{
		    int h1 = p->getRight() ? p->getRight()->getHeight() : 0; int h2 = p->getLeft() ? p->getLeft()->getHeight() : 0;
		    p->setHeight((h1 > h2 ? h1 : h2) + 1);        
		}
	}

	int balanceFactor(Node<type>* a)
	{
		if(a->getRight() && a->getLeft()) return a->getRight()->getHeight() - a->getLeft()->getHeight();
		else if(a->getRight()) return a->getRight()->getHeight();
		else if(a->getLeft()) return -(a->getLeft()->getHeight());
		else return 0; 
	}

	Node<type>* rightRotate(Node<type> *p)
	{
		Node<type>* q = p->getLeft();
		p->setLeft(q->getRight());
		q->setRight(p);
		fix(p);
		fix(q);
		return q;
	}

	Node<type>* leftRotate(Node<type> *q)
	{
		Node<type>* p = q->getRight();
		q->setRight(p->getLeft());
		p->setLeft(q);
		fix(p); 
		fix(q);
		return p;
	}

	Node<type>* balance(Node<type>* p)
	{
		fix(p);
		if(balanceFactor(p) == 2)
		{
			if(balanceFactor(p->getRight()) < 0) 
			{
				p->setRight(rightRotate(p->getRight()));
			}
			return leftRotate(p);
		}

		if(balanceFactor(p) == -2)
		{
			if(p && p->getLeft() && balanceFactor(p->getLeft()) > 0)
			{
				p->setLeft(leftRotate(p->getLeft()));
			}
			return rightRotate(p);
		}
		return p;
	}

public:
	Avltree<type>() : Node<type>() {}

	virtual Node<type>* insert(Node<type>* p, type data)
	{
		if(!p) return new Node<type>(data);
		if(data < p->getData()) p->setLeft(insert(p->getLeft(), data));
		else p->setRight(insert(p->getRight(), data));
		return balance(p);
	}

	virtual Node<type>* findMin(Node<type>* p)
	{
		if(p->getLeft()) return findMin(p->getLeft());
		else return balance(p);
	}

	virtual Node<type>* deleteMin(Node<type>* p)
	{
		if(!p->getLeft()) return p->getRight();
		p->setLeft(deleteMin(p->getLeft()));
		return balance(p);
	}

	virtual Node<type>* remove(Node<type>* p, type data)
	{
		if(!p) return nullptr;
		if(data < p->getData()) p->setLeft(remove(p->getLeft(), data));
		else if(data > p->getData()) p->setRight(remove(p->getRight(), data));
		else
		{
			Node<type>* left = p->getLeft(); Node<type>* right = p->getRight();
			if(!right) return left;
			Node<type>* m = findMin(right);
			m->setRight(deleteMin(m));
			m->setLeft(left);
			return balance(m);
		}
		return balance(p);
	}
};

template<class type>
class Tree : public Avltree<type>
{
protected:
	Node<type>* root;
    
    	int size;
    
public:
    	Tree<type>() {root = nullptr;}
	
	virtual int Size() {return size;}
	virtual Node<type>* getRoot() {return root;}
	virtual void setSize(int sz) {size = sz;} 

	virtual void insert(type data)
	{
		root = Avltree<type>::insert(root, data);
	}

	virtual void remove(type data)
	{
		root = Avltree<type>::remove(root, data);
	}

	virtual void clear()
	{
		root = nullptr;
	}

	friend ostream &operator<<(ostream & output, Tree<type> a)
	{
		return output << a.getRoot();
	}
};

template<class type>
class set : public Tree<type>
{
protected:
	Tree<type> Set;

public:
	set(int s) { while(s--) {Set.insert(type());} }
	set() {}

	virtual void insert(type data)
	{
		Set.insert(data);
	}

	virtual void erase(type data)
	{
		Set.remove(data);
	}

	virtual void clear()
	{
		Set.clear(); 
	}
	
	friend ostream &operator<<(ostream &output, set p) {return output << p.Set;}
};
