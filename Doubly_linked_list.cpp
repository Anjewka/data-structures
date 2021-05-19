#include <iostream>

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

	virtual T& getData() {T& d = data; return d;}
	virtual Element<T>* getNext() {return next;}
	virtual Element<T>* getPrevious() {return previous;}
	virtual void setData(T d) {data = d;}
	virtual void setNext(Element<T> *n) {next = n;}
	virtual void setPrevious(Element<T> *p) {previous = p;}
};

template<class T>
class list
{
private:
	Element<T> *first = nullptr;
	Element<T> *last = nullptr;
	
	int sz = 0;

public:
	list<T>() {}
	list<T>(int size) 
	{
		sz = size;
		begin = new Element<T>();
		Element<T>* curr = first;
		while(--size)
		{
			curr->getNext() = new Element<T>();
			curr = curr->getNext();
		}
		last = curr;
	}
	list<T>(int size, T data) 
	{
		sz = size;
		first = new Element<T>(data);
		Element<T>* curr = first;
		while(--size)
		{
			curr->setNext(new Element<T>(data));
			curr = curr->getNext();
		}
		last = curr;
	}
	~list<T>()
	{
		while(last != first) {pop_back();}
		delete last;
	}

	virtual void push_back(T data)
	{
		if(first)
		{
			Element<T> *curr = last;
			curr->setNext(new Element<T>(data));
			last = curr->getNext();	
			last->setPrevious(curr);
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
			Element<T> *curr = new Element<T>(data);
			curr->setNext(first);
			first = curr;
			curr->getNext()->setPrevious(curr);
		}
		else
		{
			first = new Element<T>(data);
			last = first;
		}
		sz++;
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

	virtual void insert(int pos, T data)
	{
		if(pos > 0 && pos < size())
		{
			Element<T>* curr = first;
			while(pos--) {curr = curr->getNext();}
			Element<T>* new_elem = new Element<T>(data);
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
			Element<T>* curr = first;
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
		list<T> element;
		Element<T> *curr;
		
	public:  
		Element<T>* getCurr() {return curr;}
		
		iterator() {curr = element.first;}
		iterator(Element<T>* p) {curr = p;}
		void operator++() { curr = curr->getNext();}
		void operator--() { curr = curr->getPrevios();}
		T &operator*() { return curr->getData(); }
		bool operator==(iterator p) { return curr == p.getCurr(); }
		bool operator!=(iterator p) { return curr != p.getCurr(); }
	};

	virtual iterator begin() {return iterator(first);}

	virtual iterator end() {return iterator(last);}
};
