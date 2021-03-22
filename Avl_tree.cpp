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

class Avltree : public Node
{
protected:
    
	void fix(Node* p)
	{
        	if(p)
		{
			int h1 = p->getRight() ? p->getRight()->getHeight() : 0; int h2 = p->getLeft() ? p->getLeft()->getHeight() : 0;
		    	p->setHeight((h1 > h2 ? h1 : h2) + 1);        
		}
	}

	int balanceFactor(Node* a)
	{
		if(a->getRight() && a->getLeft()) return a->getRight()->getHeight() - a->getLeft()->getHeight();
		else if(a->getRight()) return a->getRight()->getHeight();
		else if(a->getLeft()) return -(a->getLeft()->getHeight());
		else return 0; 
    	}

	Node* rightRotate(Node *p)
	{
		Node* q = p->getLeft();
		p->setLeft(q->getRight());
		q->setRight(p);
		fix(p);
		fix(q);
		return q;
	}

	Node* leftRotate(Node *q)
	{
		Node* p = q->getRight();
		q->setRight(p->getLeft());
		p->setLeft(q);
		fix(p); 
		fix(q);
		return p;
	}

	Node* balance(Node* p)
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
	Avltree() : Node() {}

	virtual Node* insert(Node* p, int data)
	{
		if(!p) return new Node(data);
		if(data < p->getData()) p->setLeft(insert(p->getLeft(), data));
		else p->setRight(insert(p->getRight(), data));
		return balance(p);
	}

	virtual Node* findMin(Node* p)
	{
		if(p->getLeft()) return findMin(p->getLeft());
		else return balance(p);
	}

	virtual Node* deleteMin(Node* p)
	{
		if(!p->getLeft()) return p->getRight();
		p->setLeft(deleteMin(p->getLeft()));
		return balance(p);
	}

	virtual Node* remove(Node* p, int data, int& sz)
	{
		if(!p) {return nullptr; sz--;}
		if(data < p->getData()) {p->setLeft(remove(p->getLeft(), data, sz));}
		else if(data > p->getData()) {p->setRight(remove(p->getRight(), data, sz));}
		else
		{
			Node* left = p->getLeft(); Node* right = p->getRight();
			if(!right) return left;
			Node* m = findMin(right);
			m->setRight(deleteMin(m));
			m->setLeft(left);
            		sz--;
			return balance(m);
		}
		return balance(p);
	}
};

class Tree : public Avltree
{
protected:
    	Node* root;
    	int sz;
    
public:
	Tree() {root = nullptr; sz = 0;}
	Tree(int size) 
    	{
        	if(size >= 0)
        	{
            		sz = size;
            		if(sz > 0) root = new Node(0);
            		while(--size) {root = Avltree::insert(root, 0);}
        	}
        	else {sz = 0; root = nullptr;}
    	}
    	Tree(int size, int data) 
    	{
        	if(size >= 0)
        	{
            		sz = size;
            		if(sz > 0) root = new Node(data);
            		while(--size) {root = Avltree::insert(root, data);}
        	}
        	else {sz = 0; root = nullptr;}
    	}
    
    	virtual int size() {return sz;}
	
	virtual void insert(int data)
    	{
        	root = Avltree::insert(root, data);
        	sz++;
    	}
    
    	virtual void remove(int data)
    	{
        	root = Avltree::remove(root, data, sz);
    	}

	virtual Node* getRoot() {return root;}

	friend ostream &operator<<(ostream & output, Tree a)
	{
		return output << a.getRoot();
	}
};
