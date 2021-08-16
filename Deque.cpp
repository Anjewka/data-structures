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
	Element() { data = T(); next = previous = nullptr; }
	Element(T d) { data = d; next = previous = nullptr; }
	~Element() { delete next; delete previous; }

	virtual T& getData() { T& d = data; return d; }
	virtual Element<T>* getNext() { return next; }
	virtual Element<T>* getPrevious() { return previous; }
	virtual void setData(T d) { data = d; }
	virtual void setNext(Element<T>* n) { next = n; }
	virtual void setPrevious(Element<T>* p) { previous = p; }
};

template<class T>
class deque
{
protected:
	Element<T>* first;
	Element<T>* last;
	unsigned int sz;

public:
	deque() { first = last = nullptr; sz = 0; }
	deque(unsigned int size)
	{
		sz = size;
		first = new Element<T>();
		Element<T>* curr = first;
		while (--size)
		{
			curr->setNext(new Element<T>());
			curr->getNext()->setPrevious(curr);
			curr = curr->getNext();
		}
		last = curr;
	}
	deque(unsigned int size, T data)
	{
		sz = size;
		first = new Element<T>(data);
		Element<T>* curr = first;
		while (--size)
		{
			curr->setNext(new Element<T>(data));
			curr->getNext()->setPrevious(curr);
			curr = curr->getNext();
		}
		last = curr;
	}
	~deque()
	{
		if (!empty())
		{
			while (last != first) { pop_back(); }
			delete first;
		}
	}

	class iterator
	{
	private:
		Element<T>* curr;

	public:
		Element<T>* getCurr() {return curr;}

		iterator() { curr = nullptr; }
		iterator(Element<T>* p) { curr = p; }

		void operator++() { curr = curr->getNext(); }

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

		void operator--() { curr = curr->getPrevious(); }

		T& operator*()
		{
			if (curr) { return curr->getData(); }
			else
			{
				T copy = T();
				T& returned = copy;
				return returned;
			}
		}

		void operator=(iterator i)
		{
			curr = i.getCurr();
		}

		bool operator==(iterator p) { return curr == p.getCurr(); }

		bool operator!=(iterator p) { return curr != p.getCurr(); }

		bool operator>(iterator i)
		{
			Element<T>* newcurr = getCurr();
			bool res = true;
			while (newcurr)
			{
				if (newcurr == i.getCurr())
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
			while (newcurr)
			{
				if (newcurr == getCurr())
				{
					return false;
				}
				newcurr = newcurr->getNext();
			}
			return res;
		}
	};

	virtual iterator begin() { return iterator(first); }

	virtual iterator end() { return iterator(last); }

	virtual void push_back(T data)
	{
		if (first)
		{
			Element<T>* new_elem = last;
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
		if (first)
		{
			Element<T>* new_elem = first;
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
		if (first && first->getNext())
		{
			Element<T>* curr = last;
			last = curr->getPrevious();
			last->setNext(nullptr);
			curr->setPrevious(nullptr);
			delete curr;
			sz--;
		}
		else if (first)
		{
			first = last = nullptr;
		}
	}

	virtual void pop_front()
	{
		if (first->getNext())
		{
			Element<T>* curr = first;
			first = curr->getNext();
			first->setPrevious(nullptr);
			curr->setNext(nullptr);
			delete curr;
			sz--;
		}
		else if (first)
		{
			first = last = nullptr;
		}
	}

	virtual T front()
	{
		if (first) return first->getData();
		return T(0);
	}

	virtual T back()
	{
		if (first) return last->getData();
		return T(0);
	}

	virtual void insert(deque<T>::iterator pos, T data)
	{
		Element<T>* curr = pos.getCurr();
		Element<T>* new_elem = new Element<T>(data);
		new_elem->setPrevious(curr->getPrevious());
		new_elem->setNext(curr);
		if (curr->getPrevious()) { curr->getPrevious()->setNext(new_elem); }
		curr->setPrevious(new_elem);
		if (first == curr) { first = new_elem; }
		sz++;
	}

	virtual void erase(deque<T>::iterator pos)
	{
		if (!empty())
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
			sz--;
		}
	}

	virtual unsigned int size() { return sz; }

	virtual bool empty()
	{
		if (!first) return true;
		else return false;
	}

	virtual void clear()
	{
		while (last)
		{
			pop_back();
		}
		last = nullptr;
		first = nullptr;
		sz = 0;
	}

	friend ostream& operator<<(ostream& output, deque a)
	{
		if (a.empty()) return output;
		Element<T>* curr = a.first;
		while (curr != a.last) { output << curr->getData() << " "; curr = curr->getNext(); }
		output << curr->getData();
		return output;
	}
};
