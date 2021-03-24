#include <iostream>
#include <algorithm>
using namespace std;

template<class T>
class Element
{
protected:
	T data;
	Element<T>* next;
	Element<T>* previous;
public:
	Element() {data = T(); next = previous = nullptr;}
	Element(T d) {data = d; next = previous = nullptr;}
	~Element() {delete next; delete previous;}

	virtual T& getData() {T& d = data; return d;}
	virtual Element<T>* getNext() {return next;}
	virtual Element<T>* getPrevious() {return previous;}
	virtual void setData(T d) {data = d;}
	virtual void setNext(Element<T> *n) {next = n;}
	virtual void setPrevious(Element<T> *p) {previous = p;}
};

template<class T>
class deque
{
protected:
	Element<T>* first;
	Element<T>* last;
	int sz;

public:
	deque() {first = last = nullptr; sz = 0;}
	deque(int size) 
	{
		sz = size;
		first = new Element<T>(T());
		Element<T> *curr = first;
		while(--size)
		{
			curr->setNext(new Element<T>(T()));
			curr = curr->getNext();
		}
		last = curr;
	}
	deque(int size, T data) 
	{
		sz = size;
		first = new Element<T>(data);
		Element<T> *curr = first;
		while(--size)
		{
			curr->setNext(new Element<T>(data));
			curr = curr->getNext();
		}
		last = curr;
	}
	~deque() 
	{
		if(!empty())
		{
			while(last != first) {pop_back();}
			delete first;
		}
	}

	virtual void push_back(T data)
	{
		if(first)
		{
			Element<T> *new_elem = last;
			new_elem->setNext(new Element<T>(data));
			last = new_elem->getNext();
			last->setPrevious(new_elem);
		}
		else
		{
			first = last = new Element<T>(data);
		}
		sz++;
	}

	virtual void push_front(T data)
	{
		if(first)
		{
			Element<T> *new_elem = first;
			new_elem->setPrevious(new Element<T>(data));
			first = new_elem->getPrevious();
			first->setNext(new_elem);
			sz++;
		}
		else
		{
			first = last = new Element<T>(data);
		}
	}

	virtual void pop_back()
	{
		if(first && first->getNext())
		{
			Element<T> *curr = last;
			last = curr->getPrevious();
			last->setNext(nullptr);
			curr->setPrevious(nullptr);
			delete curr;
			sz--;
		}
		else if(first) 
		{
			first = last = nullptr;
		}
	}

	virtual void pop_front()
	{
		if(first->getNext())
		{
			Element<T>* curr = first;
			first = curr->getNext();
			first->setPrevious(nullptr);
			curr->setNext(nullptr);
			delete curr;
			sz--;
		}
		else if(first) 
		{
			first = last = nullptr;
		}
	}

	virtual T front() 
	{
		if(first) return first->getData();
		return T(0);
	}

	virtual T back() 
	{
		if(first) return last->getData();
		return T(0);
	}

	virtual void insert(int pos, T data)
	{
		if(pos > 0 && pos < size())
		{
			Element<T>* curr = first;
			while(pos--) curr = curr->getNext();
			Element<T>* new_elem = new Element<T>(data);
			new_elem->setNext(curr);
			new_elem->setPrevious(curr->getPrevious());
			new_elem->getPrevious()->setNext(new_elem);
			curr->setPrevious(new_elem);
			sz++;
		}
		else if(pos == 0) push_front(data);
		else if(pos == size()) push_back(data);
		else return;
	}

	virtual void erase(int pos)
	{
		if(pos > 0 && pos < size())
		{
			Element<T>* curr = first;
			while(pos--) {curr = curr->getNext();}
			curr->getPrevious()->setNext(curr->getNext());
			curr->getNext()->setPrevious(curr->getPrevious());
			delete curr;
			sz--;
		}
		else if(pos == 0) {pop_front();}
		else if(pos == size()) {pop_back();}
	}

	virtual int size() {return sz;}

	virtual bool empty() 
	{
		if(!first) return true;
		else return false;
	}	

    virtual void clear()
    {
        while(last)
        {
            pop_back();
        }
        last = nullptr;
        first = nullptr;
        sz = 0;
    }
	
	class iterator
    {
    private:
       	Element<T>* curr;
        
    public:  
       	Element<T>* getCurr() {if(curr) {return curr;} else {return T();}}
        
       	iterator() {curr = nullptr;}
		iterator(Element<T>* p) {curr = p;}

		void operator++() {curr = curr->getNext();}

		void operator--() {curr = curr->getPrevious();}

		T& operator*() 
        { 
			return curr->getData();
        }

		void operator=(T t) 
		{
			curr->setData(t);
		}

        void operator=(iterator i)
        {
            curr = i.getCurr();
        }

		bool operator==(iterator p) {return curr == p.getCurr();}

		bool operator!=(iterator p) {return curr != p.getCurr();}

        bool operator>(iterator i)
        {
            Element<T>* newcurr = getCurr();
            bool res = true;
            while(newcurr) 
            {
                if(newcurr == i.getCurr())
                {
                    return false;
                }
                newcurr = newcurr->getNext();
            }
            return res;
        } 

		bool operator<(iterator i)
        {
            Element<T>* newcurr = i.getCurr();
            bool res = true;
            while(newcurr) 
            {
                if(newcurr == getCurr())
                {
                    return false;
                }
                newcurr = newcurr->getNext();
            }
            return res;
        } 

        void operator+=(int n)
        {
            while(n--)
            {
                if(curr) {curr = curr->getNext();}
                else {break;}
            }
        }

        void operator-=(int n)
        {
            while(n--)
            {
                if(curr) {curr = curr->getPrevious();}
                else {break;}
            }
        }

        iterator operator+(int n)
        {
            while(n--)
            {
                if(curr) {curr = curr->getNext();}
                else {break;}
            }
            return iterator(curr);
        }

        iterator operator-(int n)
        {
            while(n--)
            {
                if(curr) {curr = curr->getPrevious();}
                else {break;}
            }
            return iterator(curr);
        }

        friend int operator-(iterator i1, iterator i2)
        {
            int ans = 0;
            if(i1 > i2) 
            {
                Element<T>* n = i2.getCurr();
                while(n != i1.getCurr()) {n = n->getNext(); ans++;}
            }
            else
            {
                Element<T>* n = i1.getCurr();
                while(n != i2.getCurr()) {n = n->getNext(); ans++;}               
                ans *= -1;
            }
            return ans;
        }
	};

	virtual iterator begin() {return iterator(first);}

	virtual iterator end() {return iterator(last);}
};
