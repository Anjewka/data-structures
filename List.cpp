#include <iostream>
using namespace std;

template<class T>
class Element
{
protected:
	T data;
	Element<T>* next;
	Element<T>* previous;
public:
	Element() { data = T(); next = previous = nullptr; }
	Element(const T& data) { this->data = data; next = previous = nullptr; }

	virtual T& getData() { return data; }
	virtual Element<T>* getNext() { return next; }
	virtual Element<T>* getPrevious() { return previous; }
	virtual void setData(T d) { data = d; }
	virtual void setNext(Element<T>* n) { next = n; }
	virtual void setPrevious(Element<T>* p) { previous = p; }
};

template<class T>
class list
{
private:
	Element<T>* first = nullptr;
	Element<T>* last = nullptr;

	size_t _size = 0;

public:
	list<T>() {}
	list<T>(size_t size)
	{
		_size = size;
		first = new Element<T>();
		Element<T>* curr = first;
		while (--size)
		{
			curr->getNext(new Element<T>());
			curr = curr->getNext();
		}
		last = curr;
	}
	list<T>(size_t size, const T& data)
	{
		_size = size;
		first = new Element<T>(data);
		Element<T>* curr = first;
		while (--size)
		{
			curr->setNext(new Element<T>(data));
			curr = curr->getNext();
		}
		last = curr;
	}

	class iterator
	{
	private:
		list<T> element;
		Element<T>* curr;

	public:
		Element<T>* getCurr() { return curr; }

		iterator() { curr = element.first; }

		iterator(Element<T>* p) { curr = p; }

		void operator++() { if (curr->getNext()) { curr = curr->getNext(); } }

		iterator operator+(int number)
		{
			while (number && curr->getNext()) { number--; curr = curr->getNext(); }
			if (number >= 1 && curr->getNext()) { curr = curr->getNext(); }
			return iterator(curr);
		}
		iterator operator-(int number)
		{
			while (number && curr->getPrevious()) { number--; curr = curr->getPrevious(); }
			if (number >= 1 && curr->getPrevious()) { curr = curr->getPrevious(); }
			return iterator(curr);
		}
		void operator--() { if (curr->getPrevious()) { curr = curr->getPrevios(); } }

		void operator=(iterator p) { curr = p.getCurr(); }

		T& operator*() 
		{
			T data = T();
			T& output = data;
			if(curr) return curr->getData(); 
			return output;
		}

		bool operator==(iterator p) { return curr == p.getCurr(); }

		bool operator!=(iterator p) { return curr != p.getCurr(); }
	};

	iterator begin() { return iterator(first); }

	iterator end() { return iterator(last); }

	void push_back(const T& data)
	{
		if (first)
		{
			Element<T>* curr = last;
			curr->setNext(new Element<T>(data));
			last = curr->getNext();
			last->setPrevious(curr);
		}
		else
		{
			first = last = new Element<T>(data);
		}
		_size++;
	}

	void push_front(const T& data)
	{
		if (first)
		{
			Element<T>* curr = new Element<T>(data);
			curr->setNext(first);
			first = curr;
			curr->getNext()->setPrevious(curr);
		}
		else
		{
			first = new Element<T>(data);
			last = first;
		}
		_size++;
	}


	void pop_back()
	{
		if (!empty())
		{
			if (first == last) { first = last = nullptr; }
			else
			{
				Element<T>* curr = last;
				last = curr->getPrevious();
				if (last) { last->setNext(nullptr); }
				if (curr) { curr->setPrevious(nullptr); }
				delete curr;
				--_size;
			}
		}
	}

	void pop_front()
	{
		if (first->getNext())
		{
			Element<T>* curr = first;
			first = curr->getNext();
			if (first) { first->setPrevious(nullptr); }
			if (curr) { curr->setNext(nullptr); }
			delete curr;
			_size--;
		}
		else if (first)
		{
			first = last = nullptr;
		}
	}

	void erase(list<T>::iterator pos)
	{
		Element<T>* curr = pos.getCurr();
		if (curr->getPrevious() && curr->getNext())
		{
			curr->getPrevious()->setNext(curr->getNext());
			curr->getNext()->setPrevious(curr->getPrevious());
		}
		else if (curr->getNext()) { curr->getNext()->setPrevious(nullptr); first = curr->getNext(); }
		else if (curr->getPrevious()) { curr->getPrevious()->setNext(nullptr); last = curr->getPrevious(); }
		curr->setNext(nullptr);
		curr->setPrevious(nullptr);
		delete curr;
		_size--;
	}

	void insert(list<T>::iterator pos, const T& data)
	{
		Element<T>* curr = pos.getCurr();
		Element<T>* new_elem = new Element<T>(data);
		new_elem->setPrevious(curr->getPrevious());
		new_elem->setNext(curr);
		if (curr->getPrevious()) { curr->getPrevious()->setNext(new_elem); }
		curr->setPrevious(new_elem);
		if (first == curr) { first = new_elem; }
		_size++;
	}

	size_t size() { return _size; }

	bool empty()
	{
		if (first) return false;
		return true;
	}
};
