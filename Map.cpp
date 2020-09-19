#include <iostream>
#include <string>
using namespace std;

template<class type_index, class type_data>
class Node
{
protected:
	type_data data; int height;
    	type_index index;
	Node<type_index, type_data>* right, *left;

public:
	Node<type_index, type_data>() {data = type_data(), index = type_index(), height = 1, right = left = nullptr;}
    	Node<type_index, type_data>(type_index i, type_data d) {data = d, index = i, height = 1, right = left = nullptr;}
	Node<type_index, type_data>(type_data d) {data = d, index = type_index(), height = 1, right = left = nullptr;}

    
	virtual type_data getData() {return data;}
    	virtual type_index getIndex() {return index;}
	virtual int getHeight() {return height;}
	virtual Node<type_index, type_data>* getRight() {return right;}
	virtual Node<type_index, type_data>* getLeft() {return left;} 
	virtual void setData(type_data d) {data = d;}
	virtual void setIndex(type_index i) {index = i;}
	virtual void setHeight(int h) {height = h;}
	virtual void setRight(Node<type_index, type_data>* r) {right = r;}
	virtual void setLeft(Node<type_index, type_data>* l) {left = l;}    

	friend ostream &operator<<(ostream &output, Node<type_index, type_data>* a)
	{
		if(!a) return output;
		else return output << a->getLeft() << a->getData() << " " << a->getRight();
    	}
};

template<class type_index, class type_data>
class Avltree : public Node<type_index, type_data>
{
protected:
	void fix(Node<type_index, type_data>* p)
	{
        if(p)
		{
		    int h1 = p->getRight() ? p->getRight()->getHeight() : 0; int h2 = p->getLeft() ? p->getLeft()->getHeight() : 0;
		    p->setHeight((h1 > h2 ? h1 : h2) + 1);        
		}
	}

	int balanceFactor(Node<type_index, type_data>* a)
	{
		if(a->getRight() && a->getLeft()) return a->getRight()->getHeight() - a->getLeft()->getHeight();
		else if(a->getRight()) return a->getRight()->getHeight();
		else if(a->getLeft()) return -(a->getLeft()->getHeight());
		else return 0; 
    	}

	Node<type_index, type_data>* rightRotate(Node<type_index, type_data> *p)
	{
		Node<type_index, type_data>* q = p->getLeft();
		p->setLeft(q->getRight());
		q->setRight(p);
		fix(p);
		fix(q);
		return q;
	}

	Node<type_index, type_data>* leftRotate(Node<type_index, type_data> *q)
	{
		Node<type_index, type_data>* p = q->getRight();
		q->setRight(p->getLeft());
		p->setLeft(q);
		fix(p); 
		fix(q);
		return p;
	}

	Node<type_index, type_data>* balance(Node<type_index, type_data>* p)
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
			if(balanceFactor(p->getLeft()) > 0)
			{
				p->setLeft(leftRotate(p->getLeft()));
		    	}
			return rightRotate(p);
		}
		return p;
	}

public:
	Avltree<type_index, type_data>() : Node<type_index, type_data>() {}

	virtual Node<type_index, type_data>* insert(Node<type_index, type_data>* p, type_index index, type_data data)
	{
		if(!p) return new Node<type_index, type_data>(index, data);
		if(index < p->getIndex()) p->setLeft(insert(p->getLeft(), index, data));
		else p->setRight(insert(p->getRight(), index, data));
		return balance(p);
	}

	virtual Node<type_index, type_data>* findMin(Node<type_index, type_data>* p)
	{
		if(p->getLeft()) return findMin(p->getLeft());
		else return p;
	}
	
	virtual Node<type_index, type_data>* findMax(Node<type_index, type_data>* p)
	{
		if(p->getRight()) return findMax(p->getRight());
		else return p;
	}

	virtual Node<type_index, type_data>* deleteMin(Node<type_index, type_data>* p)
	{
		if(!p->getLeft()) return p->getRight();
		p->setLeft(deleteMin(p->getLeft()));
		return balance(p);
	}

	virtual Node<type_index, type_data>* remove(Node<type_index, type_data>* p, type_index index)
	{
		if(!p) return nullptr;
		if(index < p->getIndex()) p->setLeft(remove(p->getLeft(), index));
		else if(index > p->getIndex()) p->setRight(remove(p->getRight(), index));
		else
		{
			Node<type_index, type_data>* left = p->getLeft(); Node<type_index, type_data>* right = p->getRight();
			if(!right) return left;
			Node<type_index, type_data>* m = findMin(right);
			m->setRight(deleteMin(m));
			m->setLeft(left);
			return balance(m);
		}
		return balance(p);
	}
	
	
};

template<class type_index, class type_data>
class Tree : public Avltree<type_index, type_data>
{
protected:
    Node<type_index, type_data>* root;
    
    int size;
    
public:
	Tree<type_index, type_data>() {root = nullptr; size = 0;}
	
    	virtual Node<type_index, type_data>* getRoot() {return root;}
	virtual int Size() {return size;}
	virtual void setSize(int sz) {size = sz;}
	virtual void setRoot(Node<type_index, type_data>* r) {root = r;}
	
	virtual void insert(type_index index, type_data data)
	{
		root = Avltree<type_index, type_data>::insert(root, index, data);
		setSize(Size() + 1);
	}
    
	virtual void remove(type_index index)
	{
		root = Avltree<type_index, type_data>::remove(root, index);
		setSize(Size() - 1);
	}
    
	type_data operator[](type_index index)
	{
		Node<type_index, type_data>*curr = getRoot();
		while(curr->getIndex() < index && curr->getRight() || curr->getIndex() > index && curr->getLeft())
		{
		    if(curr->getIndex() < index) curr = curr->getRight();
		    else curr = curr->getLeft();
		}
		if(curr->getIndex() == index) {return curr->getData();}
		else if(curr->getRight() && curr->getRight()->getIndex() == index) {return curr->getRight()->getData();}
		else if(curr->getLeft() && curr->getLeft()->getIndex() == index) {return curr->getLeft()->getData();}
		else return type_data();
	}
    
	virtual void clear()
	{
		root = nullptr; 
	}

	friend ostream &operator<<(ostream & output, Tree<type_index, type_data> a)
	{
	      return output << a.getRoot();
	}
};

template<class type_index, class type_data>
class map : Tree<type_index, type_data>
{
protected:
    Tree<type_index, type_data> Map;
    
public:
    map(int size) { while(size--) {Map.insert(type_index(size), type_data());} }
    map() {}
    
    virtual int Size() {return Map.Size();}
    
    virtual void insert(type_index index, type_data data)
    {
        Map.insert(index, data);
    }
    
    virtual void erase(type_index index)
    {
        Map.remove(index);
    }
    
    virtual void clear()
    {
        Map.clear(); 
    }
    
    type_data operator[](type_index index)
    {
        return Map[index];
    }
    
    friend ostream &operator<<(ostream &output, map p) {return output << p.Map;}
};
