#include <iostream>
using namespace std;

template<class type>
class Element
{
protected:
	type data;
	Element<type> *next;
	Element<type> *previous;
public:
	Element() {data = type(); next = previous = nullptr;}
	Element(type d) {data = d; next = previous = nullptr;}

	virtual type getData() {return data;}
	virtual Element<type> *getNext() {return next;}
	virtual Element<type> *getPrevious() {return previous;}
	virtual void setData(type d) {data = d;}
	virtual void setNext(Element<type> *n) {next = n;}
	virtual void setPrevious(Element<type> *p) {previous = p;}
};

template<class type>
class Deque : public Element<type>
{
protected:
	Element<type> *first;
	Element<type> *last;
	int sz = 0;
public:
	Deque() : Element<type>() {first = last = nullptr;}
	Deque(int size) 
	{
		sz = size;
		first = new Element<type>(type());
		Element<type> *curr = first;
		while(--size)
		{
			curr->setNext(new Element<type>(type()));
			curr = curr->getNext();
		}
		last = curr;
	}
	Deque(int size, type data) 
	{
		sz = size;
		first = new Element<type>(data);
		Element<type> *curr = first;
		while(--size)
		{
			curr->setNext(new Element<type>(data));
			curr = curr->getNext();
		}
		last = curr;
	}

	virtual void push_back(type data)
	{
		if(first)
		{
			Element<type> *new_elem = last;
			new_elem->setNext(new Element<type>(data));
			last = new_elem->getNext();
			last->setPrevious(new_elem);
		}
		else
		{
			first = last = new Element<type>(data);
		}
		sz++;
	}

	virtual void push_front(type data)
	{
		if(first)
		{
			Element<type> *new_elem = first;
			new_elem->setPrevious(new Element<type>(data));
			first = new_elem->getPrevious();
			first->setNext(new_elem);
			sz++;
		}
		else
		{
			first = last = new Element<type>(data);
		}
	}

	virtual void pop_back()
	{
		if(first)
		{
			last = last->getPrevious();
			sz--;
		}
	}

	virtual void pop_front()
	{
		if(first)
		{
			first = first->getNext();
			sz--;
		}
	}

	virtual type front() 

	{
		if(first) return first->getData();
		return type(0);
	}

	virtual type back() 
	{
		if(first) return last->getData();
		return type(0);
	}

	virtual void insert(int pos, type data)
	{
		if(pos > 0 && pos < size())
		{
			Element<type>* curr = first;
			while(pos--) curr = curr->getNext();
			Element<type>* new_elem = new Element<type>(data);
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
			Element<type>* curr = first;
			while(pos--) {curr = curr->getNext();}
			curr->getPrevious()->setNext(curr->getNext());
			curr->getNext()->setPrevious(curr->getPrevious());
			delete curr;
			sz--;
		}
		else if(pos == 0) {pop_front();}
		else if(pos == size()) {pop_back();}
		else return;
	}

	virtual int size() {return sz;}

	virtual bool empty() 
	{
		if(!first) return true;
		else return false;
	}	
	
	class iterator
    	{
    	private:
        	Deque<type> element;
        	Element<type> *curr;
        
    	public:  
        	Element<type>* getCurr() {return curr;}
        
        	iterator() {curr = element.first;}
		iterator(Element<type>* p) {curr = p;}
		void operator++() { curr = curr->getNext();}
		void operator--() { curr = curr->getPrevios();}
		type operator*() { return curr->getData(); }
		bool operator==(iterator p) { return curr == p.getCurr(); }
		bool operator!=(iterator p) { return curr != p.getCurr(); }
	};

	virtual iterator begin() {return iterator(first);}

	virtual iterator end() {return iterator(last);}

	friend ostream &operator<<(ostream &output, Deque a)
	{
		if(a.empty()) return output;
		Element<type>* curr = a.first;
		while(curr != a.last) {output << curr->getData() << " "; curr = curr->getNext();}
		output << curr->getData();
		return output;
	}
};
