#include <iostream>
#include <string>
using namespace std;

class Node
{
protected:
	int data; unsigned int height;
	Node* right, *left, *parent;

public:
	Node() {data = 0, height = 1, right = left = parent = nullptr;}
	Node(int d) {data = d, height = 1, right = left = parent = nullptr;}
    virtual ~Node() {delete right, left;}

	int& getData() {int& d = data; return d;}
	unsigned int getHeight() {return height;}
	Node* getRight() {return right;}
	Node* getLeft() {return left;} 
    Node* getParent() {return parent;} 
	void setData(const int& d) {data = d;}
	void setHeight(const unsigned int& h) {height = h;}
	void setRight(Node* r) {right = r ? r : nullptr;}
	void setLeft(Node* l) {left = l ? l : nullptr;}
    void setParent(Node* p) {parent = p ? p : nullptr;}

	friend ostream &operator<<(ostream &output, Node* p)
	{
        if(!p) {return output;}
        else
        {
            return output << p->getLeft() << p->getData() << " " << p->getRight();
        }    
	}
};

class Avltree
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
    	if(a->getRight() && a->getLeft()) {return a->getRight()->getHeight() - a->getLeft()->getHeight();}
        else if(a->getRight()) {return a->getRight()->getHeight();}
        else if(a->getLeft()) {return -(a->getLeft()->getHeight());}
        else {return 0;} 
    }

	Node* rightRotate(Node *p)
	{
		Node* q = p->getLeft();
		p->setLeft(q->getRight());
        q->setRight(p);
        q->setParent(p->getParent());
        p->setParent(q);
        if(p->getLeft()) {p->getLeft()->setParent(p);}
		fix(p);
		fix(q);
		return q;
	}

	Node* leftRotate(Node *q)
	{
		Node* p = q->getRight();
        q->setRight(p->getLeft());
        p->setLeft(q);
        p->setParent(q->getParent());
        q->setParent(p);
        if(q->getRight()) {q->getRight()->setParent(q);}
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
			if(balanceFactor(p->getLeft()) > 0)
            {
                p->setLeft(leftRotate(p->getLeft()));
            }
			return rightRotate(p);
		}
		return p;
	}

public:
	Avltree(){}

	Node* insert(Node* p, const int& data)
	{
		if(!p) {return new Node(data);}
		else if(data < p->getData()) 
        {
            Node* child = insert(p->getLeft(), data); 
            p->setLeft(child); 
            child->setParent(p); 
        }
		else 
        {
            Node* child = insert(p->getRight(), data); 
            p->setRight(child); 
            child->setParent(p); 
        }
		return balance(p);
	}

	Node* getMinimum(Node* p)
	{
		if(p->getLeft()) {return getMinimum(p->getLeft());}
		else {return balance(p);}
	}   

    Node* getMaximum(Node* p)
    {
        if(p->getRight()) {return getMaximum(p->getRight());}
        else {return balance(p);}
    }

	Node* deleteMin(Node* p)
	{
		if(!p->getLeft()) {return p->getRight();}
		p->setLeft(deleteMin(p->getLeft()));
		return balance(p);
	}

	Node* remove(Node* p, const int& data, int& sz)
	{
		if(!p) {return nullptr; sz--;}
		if(data < p->getData()) 
		{
			Node* child = remove(p->getLeft(), data, sz);
			p->setLeft(child);
			if(child) {child->setParent(p);}
		}
		else if(data > p->getData()) 
		{
			Node* child = remove(p->getRight(), data, sz);
			p->setRight(child);
			if(child) {child->setParent(p);}
		}
		else
		{
			Node* left = p->getLeft(); Node* right = p->getRight();
			if(!right) {return left;}
			Node* m = getMinimum(right);
			m->setRight(deleteMin(right));
			if(m->getRight()) {m->getRight()->setParent(m);}
			m->setLeft(left);
			if(left) {left->setParent(m);}
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
        root = Avltree::balance(root);
        sz++;
    }
    
    virtual void remove(int data)
    {
        root = Avltree::remove(root, data, sz);
		root = Avltree::balance(root);
    }

	virtual Node* getRoot() {return root;}

    virtual void setRoot(Node* r) {root = r;}

    class iterator : public Avltree
    {
    private:
        Node* curr;

    public:
        iterator() {curr = nullptr;}
        iterator(Node* p) {curr = p;}

        Node* getCurr() {return curr;}
        
        void operator++() 
        {
            if(curr->getRight()) {curr = Avltree::getMinimum(curr->getRight());}
            else 
            {
                Node* tmp = curr;
                bool check = true;
                while(curr->getParent() && curr->getParent()->getLeft() != curr)
                {
                    if(curr->getParent()) {curr = curr->getParent();}
                    else {check = false; break;}
                }
                if(check) {curr = curr->getParent();}
                else {curr = tmp;}
            }            
        }

        void operator--() 
        {
            if(curr->getLeft()) {curr = Avltree::getMaximum(curr->getLeft());}
            else
            {
                Node* tmp = curr;
                bool check = true;
                while(curr->getParent() && curr->getParent()->getRight() != curr)
                {
                    if(curr->getParent()) {curr = curr->getParent();}
                    else {check = false; break;}
                }
                if(check) {curr = curr->getParent();}
                else {curr = tmp;}
            }            
        }

        int operator*() {return curr->getData(); }
        bool operator==(iterator p) {return curr == p.getCurr();}
        bool operator!=(iterator p) {return curr != p.getCurr();}
    };

    virtual iterator begin() {return iterator(Avltree::getMinimum(getRoot()));}
   
    virtual iterator end() {return iterator(Avltree::getMaximum(getRoot()));}

	friend ostream &operator<<(ostream& output, Tree a)
	{
		return output << a.getRoot();
	}   
};

int main()
{
    Tree a;

    a.insert(1);
    a.insert(2);
    a.insert(3);
    a.insert(5);
    a.insert(4);
    a.insert(6);

	cout << a;
}