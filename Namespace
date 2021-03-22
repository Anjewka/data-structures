#include <iostream>

namespace stl
{
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
    class Node
    {
    protected:
        T data; int height;
        Node<T>* right, *left, *parent;

    public:
        Node<T>() {data = T(), height = 1, right = left = parent = nullptr;}
        Node<T>(T d) {data = d, height = 1, right = left = parent = nullptr;}

        virtual T& getData() {T& d = data; return d;}
        virtual int getHeight() {return height;}
        virtual Node<T>* getRight() {return right;}
        virtual Node<T>* getLeft() {return left;} 
        virtual Node<T>* getParent() {return parent;} 
        virtual void setData(T d) {data = d;}
        virtual void setHeight(int h) {height = h;}
        virtual void setRight(Node<T>* r) {right = r ? r : nullptr;}
        virtual void setLeft(Node<T>* l) {left = l ? l : nullptr;}
        virtual void setParent(Node<T>* p) {parent = p ? p : nullptr;}
    };

    template<class T1, class T2>
    class pair
    {
    public:
        T1 first;
        T2 second;

        pair() {first = T1(); second = T2();}
        pair(T1 f, T2 s) {first = f; second = s;}

        void operator=(pair<T1, T2> p)
        {
            first = p.first; second = p.second;
        }

        friend void print(pair<T1, T2> p)
        {
            std::cout << p.first << " " << p.second;
        }

        friend bool operator<(pair<T1, T2> p1, pair<T1, T2> p2)
        {
            return p1.first < p2.first;
        }

        friend bool operator>(pair<T1, T2> p1, pair<T1, T2> p2)
        {
            return p1.first > p2.first;
        }

        friend bool operator==(pair<T1, T2> p1, pair<T1, T2> p2)
        {
            return p1.first == p2.first;
        }
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
            else return;
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

    template<class type>
    class list
    {
    private:
        Element<type> *first = nullptr;
        Element<type> *last = nullptr;
        
        int sz = 0;

    public:
        list<type>() {}
        list<type>(int size) 
        {
            sz = size;
            begin = new Element<type>();
            Element<type>* curr = first;
            while(--size)
            {
                curr->getNext() = new Element<type>();
                curr = curr->getNext();
            }
            last = curr;
        }
        list<type>(int size, type data) 
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
            list<type> element;
            Element<type> *curr;
            
        public:  
            Element<type>* getCurr() {return curr;}
            
            iterator() {curr = element.first;}
            iterator(Element<type>* p) {curr = p;}
            void operator++() { curr = curr->getNext();}
            void operator--() { curr = curr->getPrevios();}
            type &operator*() { return curr->getData(); }
            bool operator==(iterator p) { return curr == p.getCurr(); }
            bool operator!=(iterator p) { return curr != p.getCurr(); }
        };
    
        virtual iterator begin() {return iterator(first);}
    
        virtual iterator end() {return iterator(last);}
    };

    template<class T>
    class Avltree
    {
    protected:
        
        void fix(Node<T>* p)
        {
            if(p)
            {
                int h1 = p->getRight() ? p->getRight()->getHeight() : 0; int h2 = p->getLeft() ? p->getLeft()->getHeight() : 0;
                p->setHeight((h1 > h2 ? h1 : h2) + 1);        
            }
        }

        int balanceFactor(Node<T>* a)
        {
            if(a->getRight() && a->getLeft()) {return a->getRight()->getHeight() - a->getLeft()->getHeight();}
            else if(a->getRight()) {return a->getRight()->getHeight();}
            else if(a->getLeft()) {return -(a->getLeft()->getHeight());}
            else {return 0;} 
        }

        Node<T>* rightRotate(Node<T>* p)
        {
            Node<T>* q = p->getLeft();
            p->setLeft(q->getRight());
            q->setRight(p);
            q->setParent(p->getParent());
            p->setParent(q);
            if(p->getLeft()) {p->getLeft()->setParent(p);}
            fix(p);
            fix(q);
            return q;
        }

        Node<T>* leftRotate(Node<T>* q)
        {
            Node<T>* p = q->getRight();
            q->setRight(p->getLeft());
            p->setLeft(q);
            p->setParent(q->getParent());
            q->setParent(p);
            if(q->getRight()) {q->getRight()->setParent(q);}
            fix(p); 
            fix(q);
            return p;
        }

        Node<T>* balance(Node<T>* p)
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
        Avltree<T>() {}

        virtual Node<T>* insert(Node<T>* p, T data)
        {
            if(!p) {return new Node<T>(data);}
            else if(data < p->getData()) 
            {
                Node<T>* child = insert(p->getLeft(), data); 
                p->setLeft(child); 
                child->setParent(p); 
            }
            else if(data > p->getData())
            {
                Node<T>* child = insert(p->getRight(), data); 
                p->setRight(child); 
                child->setParent(p); 
            }
            return balance(p);
        }

        virtual Node<T>* getMinimum(Node<T>* p)
        {
            if(p->getLeft()) {return getMinimum(p->getLeft());}
            else {return balance(p);}
        }

        virtual Node<T>* deleteMin(Node<T>* p)
        {
            if(!p->getLeft()) {return p->getRight();}
            p->setLeft(deleteMin(p->getLeft()));
            return balance(p);
        }

        virtual Node<T>* getMaximum(Node<T>* p)
        {
            if(p->getRight()) {return getMaximum(p->getRight());}
            else {return balance(p);}
        }

        virtual Node<T>* deleteMaximum(Node<T>* p)
        {
            if(!p->getRight()) {return p->getLeft();}
            p->setRight(deleteMaximum(p->getRight()));
            return balance(p);
        }

        virtual Node<T>* remove(Node<T>* p, T data, int& sz)
        {
            if(!p) {return nullptr; sz--;}
            if(data < p->getData()) 
            {
                Node<T>* child = remove(p->getLeft(), data, sz);
                p->setLeft(child);
                if(child) {child->setParent(p);}
            }
            else if(data > p->getData()) 
            {
                Node<T>* child = remove(p->getRight(), data, sz);
                p->setRight(child);
                if(child) {child->setParent(p);}
            }
            else
            {
                return nullptr;
                Node<T>* left = p->getLeft(); Node<T>* right = p->getRight();
                if(!right) {return left;}
                Node<T>* m = getMinimum(right);
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

    template<class T>
    class Tree : public Avltree<T>
    {
    protected:
        Node<T>* root;
        int sz;
        
    public:
        Tree() {root = nullptr; sz = 0;}
        Tree(int size) 
        {
            if(size >= 0)
            {
                sz = size;
                if(sz > 0) root = new Node<T>(T());
                while(--size) {root = Avltree<T>::insert(root, T());}
            }
            else {sz = 0; root = nullptr;}
        }
        Tree(int size, T data) 
        {
            if(size >= 0)
            {
                sz = size;
                if(sz > 0) root = new Node<T>(data);
                while(--size) {root = Avltree<T>::insert(root, data);}
            }
            else {sz = 0; root = nullptr;}
        }

        T& operator[](T index)
        {
            Node<T>* curr = getRoot();

            while(curr->getData() < index && curr->getRight() || curr->getData() > index && curr->getLeft())
            {
                if(curr->getData() < index) curr = curr->getRight();
                else curr = curr->getLeft();
            }

            if(curr->getData() == index)  {return curr->getData();}
            else if(curr->getRight() && curr->getRight()->getData() == index) {return curr->getRight()->getData();}
            else if(curr->getLeft() && curr->getLeft()->getData() == index) {return curr->getLeft()->getData();}
            else 
            {
                T d = T();
                T& data = d;
                return data;          
            }
        }
        
        virtual int size() {return sz;}
        
        virtual void insert(T data)
        {
            root = Avltree<T>::insert(root, data);
            root = Avltree<T>::balance(root);
            sz++;
        }
        
        virtual void remove(T data)
        {
            root = Avltree<T>::remove(root, data, sz);
            root = Avltree<T>::balance(root);
        }

        virtual void clear()
        {
            while(root) {root = Avltree<T>::deleteMin(root);}
        }

        virtual Node<T>* getRoot() {return root;}

        virtual void setRoot(Node<T>* r) {root = r;}
    };

    template<class T>
    class set : public Avltree<T>
    {
    protected:
        Tree<T> Set;

    public:
        set(int s) { while(s--) {Set.insert(T());} }
        set() {}
        
        virtual void insert(T data)
        {
            Set.insert(data);
        }
        
        virtual void erase(T data)
        {
            Set.remove(data);
        }

        virtual void pop_back()
        {
            if(Set.getRoot()) {Set.setRoot(Avltree<T>::deleteMaximum(Set.getRoot()));}
        }

        virtual Tree<T> getSet() {return Set;}
        
        virtual void clear()
        {
            while(Set.getRoot()) {pop_back();}
        }

        class iterator : public Avltree<T>
        {
        private:
            Node<T>* curr;

        public:
            iterator() {curr = nullptr;}
            iterator(Node<T>* p) {curr = p;}

            Node<T>* getCurr() {return curr;}
            
            void operator++() 
            {
                if(curr->getRight()) {curr = Avltree<T>::getMinimum(curr->getRight());}
                else 
                {
                    Node<T>* tmp = curr;
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
                if(curr->getLeft()) {curr = Avltree<T>::getMaximum(curr->getLeft());}
                else
                {
                    Node<T>* tmp = curr;
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

            T operator*() {return curr->getData();}
            bool operator==(iterator p) {return curr == p.getCurr();}
            bool operator!=(iterator p) {return curr != p.getCurr();}
        };

        virtual iterator begin() {return iterator(Avltree<T>::getMinimum(Set.getRoot()));}
    
        virtual iterator end() {return iterator(Avltree<T>::getMaximum(Set.getRoot()));}
    };

    template<class T1, class T2>
    class map : public Avltree<pair<T1, T2>>
    {
    protected:
        Tree<pair<T1, T2>> Map;
        
    public:
        map() {}

        virtual Tree<pair<T1, T2>> getMap() {return Map;} 
        
        virtual int size() {return Map.size();}
        
        virtual void insert(T1 index, T2 data)
        {
            pair<T1, T2> p(index, data);
            Map.insert(p);
        }
        
        virtual void erase(T1 index)
        {
            pair<T1, T2> p(index, T2());
            Map.remove(p);
        }
        
        virtual void clear()
        {
            Map.clear(); 
        }
        
        T2& operator[](T1 index)
        {
            pair<T1, T2> p(index, T2());
            Map.insert(p);
            return Map[p].second;
        }

        virtual void operator=(map<T1, T2> m)
        {
            Map = m.Map;
        }

        class iterator : public Avltree<pair<T1, T2>>
        {
        private:
            Node<pair<T1, T2>>* curr;

        public:
            T1 first; T2 second;

            iterator() {curr = nullptr; first = T1(); second = T2();}
            iterator(Node<pair<T1, T1>>* p) 
            {
                curr = p; 
                first = curr ? curr->getData().first : T1();
                second = curr ? curr->getData().second : T2();
            }		

            Node<pair<T1, T1>>* getCurr() {return curr;}
            
            void operator++() 
            {
                if(curr->getRight()) {curr = Avltree<pair<T1, T1>>::getMinimum(curr->getRight());}
                else 
                {
                    Node<pair<T1, T1>>* tmp = curr;
                    bool check = true;
                    while(curr->getParent() && curr->getParent()->getLeft() != curr)
                    {
                        if(curr->getParent()) {curr = curr->getParent();}
                        else {check = false; break;}
                    }
                    if(check) {curr = curr->getParent();}
                    else {curr = tmp;}
                }
                first = curr ? curr->getData().first : T1();
                second = curr ? curr->getData().second : T2();         
            }

            void operator--() 
            {
                if(curr->getLeft()) 
                {
                    curr = Avltree<pair<T1, T1>>::getMaximum(curr->getLeft());
                }
                else
                {
                    Node<pair<T1, T1>>* tmp = curr;
                    bool check = true;
                    while(curr->getParent() && curr->getParent()->getRight() != curr)
                    {
                        if(curr->getParent()) {curr = curr->getParent();}
                        else {check = false; break;}
                    }
                    if(check) {curr = curr->getParent();}
                    else {curr = tmp;}
                }
                first = curr ? curr->getData().first : T1();
                second = curr ? curr->getData().second : T2();
            }

            bool operator==(iterator p) {return curr == p.getCurr();}
            bool operator!=(iterator p) {return curr != p.getCurr();}
        };

        virtual iterator begin() {return iterator(Avltree<pair<T1, T1>>::getMinimum(Map.getRoot()));}
    
        virtual iterator end() {return iterator(Avltree<pair<T1, T1>>::getMaximum(Map.getRoot()));}
    };

    template<class T>
    class Heap
    {
    private:
        T* array;
        int length;
        int size;

    public:
        Heap<T>() {array = new T[100]; length = 0; size = 100;}

        Heap<T>(int size) 
        {
            if(size > 0) {array = new T[size];}
            else {array = new T[100];}
            length = 0;
            this->size = size;
        }

        int getParentIndex(int index) 
        {
            if(index < 0 || index > length) return 0;
            return (index - 1) / 2;
        }

        int getLeftChildIndex(int index) 
        {
            if(index < 0 || index > length) return 0;
            return (2 * index + 1);
        }

        int getRightChildIndex(int index) 
        {
            if(index < 0 || index > length) return 0;
            return (2 * index + 2);
        }

        T getMaximum() 
        {
            return length > 0 ? array[0] : T();
        }

        bool empty() 
        {
            return length > 0 ? false : true;
        }

        void SiftDown(int i)
        {
            int left = getLeftChildIndex(i);
            int right = getRightChildIndex(i); 
            int mi = i;

            if(left >= length) {return;}

            if(array[left] > array[right]) {mi = left;}
            if(right < length && array[right] > array[mi]) {mi = right;}

            if(mi != i)
            {
                swap(array[mi], array[i]);
                SiftDown(mi);
            }
        }

        void SiftUp(int i)
        {
            int p = getParentIndex(i);

            if(i > length || i == 0) {return;}

            if(array[i] > array[p]) 
            {
                swap(array[i], array[p]);
            }
            SiftUp(p);
        }

        void insert(T data)
        {
            if (length >= size)
            {
                return;
            }

            length++; 
            int i = length - 1; 
            array[i] = data; 
            SiftUp(i);
        }

        T ExtractMax()
        {
            T m = getMaximum();
            swap(array[0], array[length - 1]);
            array[length - 1] = T();
            length--;
            SiftDown(0);
            return m;
        }
    };
}
