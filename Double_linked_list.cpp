#include <iostream>
#include <iterator>
using namespace std;

template<class type>
class Element
{
protected:
	Element<type> *next;
	Element<type> *previous;
	type data;

public:
	Element () {next = nullptr; data = type(); previous = nullptr;}

	Element(Element<type> *n) {next = n; data = type(); previous = nullptr;}

	Element(Element<type> *n, type d) {data = d; next = n; previous = nullptr;}

	Element(type d) {next = nullptr; data = d; previous = nullptr;}

	virtual void setData(type d) {data = d;}

	virtual void setNext(Element<type> *n) {next = n;}

	virtual void setPrevious(Element<type> *pre) {previous = pre;}

	virtual type getData() {return data;}

	virtual Element<type>* getNext() {return next;}

	virtual Element<type>* getPrevious() {return previous;}
};

template<class type>
class List : public Element<type>
{
protected:
	Element<type> *first = nullptr;
	Element<type> *last = nullptr;
    
    
	int sz = 0;

public:
	List<type>() : Element<type>() {}
	List<type>(int size) 
    	{
        	sz = size;
        	begin = new Element<type>();
        	Element<type>* curr = first;
        	while(--size)
        	{
            		curr->setNext(new Element<type>());
            		curr = curr->getNext();
        	}
        	last = curr;
    	}
    	List<type>(int size, type data) 
    	{
        	sz = size;
        	first = new Element<type>(data);
        	Element<type>* curr = first;
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
			Element<type> *curr = last;
			curr->setNext(new Element<type>(data));
			last = curr->getNext();	
			last->setPrevious(curr);
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
			Element<type> *curr = new Element<type>(data);
			curr->setNext(first);
			first = curr;
			curr->getNext()->setPrevious(curr);
		}
		else
		{
			first = new Element<type>(data);
			last = first;
		}
		sz++;
	}


	virtual void pop_back() 
	{
		if(last) 
		{
			if(last->getPrevious()) {last = last->getPrevious(); last->setNext(nullptr);}
			else first = last = nullptr;
			sz--;
		}
	}

	virtual void pop_front() 
	{
		if(first) 
		{
			if(first->getNext()) {first = first->getNext(); first->setPrevious(nullptr);}
			else first = last = nullptr;
			sz--;
		}
	}

	virtual void insert(int pos, type data)
	{
		if(pos > 0 && pos < size())
		{
			Element<type>* curr = first;
			while(pos--) {curr = curr->getNext();}
			Element<type>* new_elem = new Element<type>(data);
			new_elem->setNext(curr);
			new_elem->setPrevious(curr->getPrevious());
			new_elem->getPrevious()->setNext(new_elem);
			curr->setPrevious(new_elem);
			sz++;
		}
		else if(pos == size()) push_back(data);
		else if(pos == 0) push_front(data);
	}

	virtual void erase(int pos)
	{
		if(pos > 0 && pos < size())
		{
			Element<type>* curr = first;
			while(pos--) {curr = curr->getNext();}
			curr->getPrevious()->setNext(curr->getNext());
			curr->getNext()->setPrevious(curr->getPrevious());
            curr->setPrevious(nullptr); curr->setNext(nullptr);
            delete curr;
			sz--;
		} 
		else if(pos == 0) {pop_front();}
		else if(pos == size()) {pop_back();}
	}

	virtual int size() {return sz;}

	virtual bool empty() 
	{
		if(first) return false;
		return true;
	}
	
	class iterator
    	{
    	private:
        	List<type> element;
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
	
	friend ostream &operator<<(ostream &output, List<type> a)
    	{
        	if(a.empty()) {return output;}
        	Element<type>* curr = a.first;
        	while(curr != a.last)
        	{
            	output << curr->getData() << " ";
            	curr = curr->getNext();
        	}
        	output << curr->getData();
        	return output;
   	}
};