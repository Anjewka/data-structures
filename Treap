#include <iostream>
using namespace std;

template<class T>
class Node
{
protected:
	Node *l, *r;
	int size, Sum, Max, Num;
	int y;
	T data;

public:
	Node(T n, int v)
	{
		size = 0;
		Sum = Max = Num = v;
		data = n;
		y = rand();
		l = r = nullptr;
	}

	Node()
	{
		Sum = Max = size = Num = 0;
		l = r = nullptr;
		data = T(0);
		y = rand();
	}

	virtual Node*& getRight() {return r;}

	virtual Node*& getLeft() {return l;}

	virtual void setLeft(Node* N) {N ? l = N : l = nullptr;}

	virtual void setRight(Node* N) {N ? r = N : r = nullptr;}

	virtual T getData() {return data;}

	virtual int getY() {return y;}

	virtual void setSize(int new_size) {Sum = new_size;}

	virtual int getSum() {return Sum;}

	virtual int getMax() {return Max;}

	virtual void setMax(int new_max) {Max = new_max;}

	virtual void setSum(int new_sum) {Sum = new_sum;}

	virtual int getNum() {return Num;}

	friend int getSize(Node* N) 
	{
		return (N ? N->size : 0);
	}

	friend void setSize(Node* N, int k) 
	{
		N ? N->size = k : 0;
	}

	friend void upd(Node* N)
	{
		if(!N) return ;
		else 
		{
			N->size = getSize(N->getLeft()) + getSize(N->getRight()) + 1;
			if(N->getLeft() && N->getRight()) N->setSum(N->getLeft()->getSum() + N->getRight()->getSum() + N->getNum()) ;
			else return;
			if(N->getLeft() && N->getRight()) N->setMax(max(N->Max, max(N->getLeft()->getMax(), N->getRight()->getMax())));
			else return;
		}
	}

	friend Node* findIn(Node* a, int index)
	{
		if(!a) return nullptr;
		if(getSize(a) == index) return a;
		else
		{
			return getSize(a) < index ? findIn(a->getRight(), index - getSize(a->getLeft()) - 1) : findIn(a->getLeft(), index);
		}
	}
};


template<class T>
class treap : public Node<T> 
{
protected:
	Node<T> *root;
public:
	treap() : Node<T>() { root = nullptr; }

	treap(T n, int v) : Node<T>(n, v) {}

	virtual Node<T>* getRoot() {return root;}

	virtual Node<T>* merge(Node<T>* a, Node<T>* b)
	{
		if(!a) return b;
		if(!b) return a;
		if(a->getY() > b->getY())
		{
			a->setRight(merge(a->getRight(), b));
			upd(a);
			return a;
		}
		else
		{
			b->setLeft(merge(a, b->getLeft()));
			upd(b);
			return b;
		}
	}

	virtual void split(Node<T>* a, int index, Node<T> *&l, Node<T> *&r)
	{
		if(!a) return void(l = r = a);
		else if(index > getSize(a->getLeft()) + 1)
		{
			split(a->getRight(), index - getSize(a->getLeft()) - 1, a->getRight(), r);
			l = a;	
		} 	
		else
		{
			split(a->getLeft(), index, l, a->getLeft());
			r = a;
		} 
		upd(l), upd(r);
	}	

	virtual void insert(T k, int index, int v)
	{
		Node<T> *r;
		split(root, index, root, r);
		root = merge(root, new Node(k, v));
		root = merge(root, r);
	}

	friend void print(treap a)
	{
		Print(a.root);
	}

	T operator [](int index)
	{
		return findIn(root, index) ? findIn(root, index)->getData() : 0;
	}

	virtual void delete_item(int index)
	{
		if(!root) return;
		if(getSize(root) == index)
		{
			root = merge(root->getLeft(), root->getRight());
		} else
		{
			getSize(root) < index ? delete_item(index - getSize(root->getLeft()) - 1) : delete_item(index);
		}	
	}

	virtual int segmentSum(int a, int b)
	{
		Node<T>* t1,* t2,* t3,* t4;
		split(root, a, t1, t2);
		split(t2, ++b, t3, t4);
	    int res = t3->getSum();
	    t2 = merge(t3, t4);
	    root = merge(t1, t2);
	    return res;
	}

	virtual int segmentMax(int a, int b)
	{
		Node<T>* t1,* t2,* t3,* t4;
		split(root, a, t1, t2);
		split(t2, ++b, t3, t4);
	    int res = t3->getMax();
	    t2 = merge(t3, t4);
	    root = merge(t1, t2);
	    return res;
	}
};

template<class T>
void Print(Node<T>* a)
{
	if(!a) return;
	else
	{
		Print(a->getLeft());
		cout << a->getData() << " ";
		Print(a->getRight());
	} 
}
