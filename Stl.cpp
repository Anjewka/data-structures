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
        Element() { data = T(); next = previous = nullptr; }
        Element(T d) { data = d; next = previous = nullptr; }

        virtual T& getData() { T& d = data; return d; }
        virtual Element<T>* getNext() { return next; }
        virtual Element<T>* getPrevious() { return previous; }
        virtual void setData(T d) { data = d; }
        virtual void setNext(Element<T>* n) { next = n; }
        virtual void setPrevious(Element<T>* p) { previous = p; }
    };

    template<class T>
    class Node
    {
    protected:
        T data; int height;
        Node<T>* right, * left, * parent;

    public:
        Node<T>() { data = T(), height = 1, right = left = parent = nullptr; }
        Node<T>(T d) { data = d, height = 1, right = left = parent = nullptr; }

        virtual T& getData() { T& d = data; return d; }
        virtual int getHeight() { return height; }
        virtual Node<T>* getRight() { return right; }
        virtual Node<T>* getLeft() { return left; }
        virtual Node<T>* getParent() { return parent; }
        virtual void setData(T d) { data = d; }
        virtual void setHeight(int h) { height = h; }
        virtual void setRight(Node<T>* r) { right = r ? r : nullptr; }
        virtual void setLeft(Node<T>* l) { left = l ? l : nullptr; }
        virtual void setParent(Node<T>* p) { parent = p ? p : nullptr; }
    };

    template<class T1, class T2>
    class pair
    {
    public:
        T1 first;
        T2 second;

        pair() { first = T1(); second = T2(); }
        pair(T1 f, T2 s) { first = f; second = s; }

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
            Element<T>* getCurr() { return curr; }

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
            if (curr) { new_elem->setPrevious(curr->getPrevious()); }
            new_elem->setNext(curr);
            if (curr && curr->getPrevious()) { curr->getPrevious()->setNext(new_elem); }
            if (curr) { curr->setPrevious(new_elem); }
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

        friend std::ostream& operator<<(std::ostream& output, deque a)
        {
            if (a.empty()) return output;
            Element<T>* curr = a.first;
            while (curr != a.last) { output << curr->getData() << " "; curr = curr->getNext(); }
            output << curr->getData();
            return output;
        }
    };

    template<class T>
    class list
    {
    private:
        Element<T>* first = nullptr;
        Element<T>* last = nullptr;

        int sz = 0;

    public:
        list<T>() {}
        list<T>(int size)
        {
            sz = size;
            first = new Element<T>();
            Element<T>* curr = first;
            while (--size)
            {
                curr->getNext(new Element<T>());
                curr = curr->getNext();
            }
            last = curr;
        }
        list<T>(int size, T data)
        {
            sz = size;
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
            T& operator*() { return curr->getData(); }
            bool operator==(iterator p) { return curr == p.getCurr(); }
            bool operator!=(iterator p) { return curr != p.getCurr(); }
        };

        virtual iterator begin() { return iterator(first); }

        virtual iterator end() { return iterator(last); }

        virtual void push_back(T data)
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
            sz++;
        }

        virtual void push_front(T data)
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
            sz++;
        }


        virtual void pop_back()
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
                    --sz;
                }
            }
        }

        virtual void pop_front()
        {
            if (first->getNext())
            {
                Element<T>* curr = first;
                first = curr->getNext();
                if (first) { first->setPrevious(nullptr); }
                if (curr) { curr->setNext(nullptr); }
                delete curr;
                sz--;
            }
            else if (first)
            {
                first = last = nullptr;
            }
        }

        virtual void erase(list<T>::iterator pos)
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

        virtual void insert(list<T>::iterator pos, T data)
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

        virtual int size() { return sz; }

        virtual bool empty()
        {
            if (first) return false;
            return true;
        }
    };

    template<class T>
    class Avltree
    {
    protected:

        void fix(Node<T>* p)
        {
            if (p)
            {
                int h1 = p->getRight() ? p->getRight()->getHeight() : 0; int h2 = p->getLeft() ? p->getLeft()->getHeight() : 0;
                p->setHeight((h1 > h2 ? h1 : h2) + 1);
            }
        }

        int balanceFactor(Node<T>* a)
        {
            if (a->getRight() && a->getLeft()) { return a->getRight()->getHeight() - a->getLeft()->getHeight(); }
            else if (a->getRight()) { return a->getRight()->getHeight(); }
            else if (a->getLeft()) { return -1 * (a->getLeft()->getHeight()); }
            else { return 0; }
        }

        Node<T>* rightRotate(Node<T>* p)
        {
            Node<T>* q = p->getLeft();
            p->setLeft(q->getRight());
            q->setRight(p);
            q->setParent(p->getParent());
            p->setParent(q);
            if (p->getLeft()) { p->getLeft()->setParent(p); }
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
            if (q->getRight()) { q->getRight()->setParent(q); }
            fix(p);
            fix(q);
            return p;
        }

        Node<T>* balance(Node<T>* p)
        {
            fix(p);
            if (balanceFactor(p) == 2)
            {
                if (balanceFactor(p->getRight()) < 0)
                {
                    p->setRight(rightRotate(p->getRight()));
                }
                return leftRotate(p);
            }

            if (balanceFactor(p) == -2)
            {
                if (balanceFactor(p->getLeft()) > 0)
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
            if (!p) { return new Node<T>(data); }
            else if (data < p->getData())
            {
                Node<T>* child = insert(p->getLeft(), data);
                p->setLeft(child);
                child->setParent(p);
            }
            else if (data > p->getData())
            {
                Node<T>* child = insert(p->getRight(), data);
                p->setRight(child);
                child->setParent(p);
            }
            return balance(p);
        }

        virtual Node<T>* getMinimum(Node<T>* p)
        {
            if (p->getLeft()) { return getMinimum(p->getLeft()); }
            else { return balance(p); }
        }

        virtual Node<T>* deleteMin(Node<T>* p)
        {
            if (!p->getLeft()) { return p->getRight(); }
            p->setLeft(deleteMin(p->getLeft()));
            return balance(p);
        }

        virtual Node<T>* getMaximum(Node<T>* p)
        {
            if (p->getRight()) { return getMaximum(p->getRight()); }
            else { return balance(p); }
        }

        virtual Node<T>* deleteMaximum(Node<T>* p)
        {
            if (!p->getRight()) { return p->getLeft(); }
            p->setRight(deleteMaximum(p->getRight()));
            return balance(p);
        }

        virtual Node<T>* remove(Node<T>* p, Node<T>* data, int& sz)
        {
            if (!p) { return nullptr; sz--; }
            if (data->getData() < p->getData())
            {
                Node<T>* child = remove(p->getLeft(), data, sz);
                p->setLeft(child);
                if (child) { child->setParent(p); }
            }
            else if (data->getData() > p->getData())
            {
                Node<T>* child = remove(p->getRight(), data, sz);
                p->setRight(child);
                if (child) { child->setParent(p); }
            }
            else
            {
                Node<T>* left = p->getLeft(); Node<T>* right = p->getRight();
                if (!right) { return left; }
                Node<T>* m = getMinimum(right);
                m->setRight(deleteMin(right));
                if (m->getRight()) { m->getRight()->setParent(m); }
                m->setLeft(left);
                if (left) { left->setParent(m); }
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
        Tree() { root = nullptr; sz = 0; }
        Tree(int size)
        {
            if (size >= 0)
            {
                sz = size;
                if (sz > 0) root = new Node<T>(T());
                while (--size) { root = Avltree<T>::insert(root, T()); }
            }
            else { sz = 0; root = nullptr; }
        }
        Tree(int size, T data)
        {
            if (size >= 0)
            {
                sz = size;
                if (sz > 0) root = new Node<T>(data);
                while (--size) { root = Avltree<T>::insert(root, data); }
            }
            else { sz = 0; root = nullptr; }
        }

        T& operator[](T index)
        {
            Node<T>* curr = getRoot();

            while (curr->getData() < index && curr->getRight() || curr->getData() > index && curr->getLeft())
            {
                if (curr->getData() < index) curr = curr->getRight();
                else curr = curr->getLeft();
            }

            if (curr->getData() == index) { return curr->getData(); }
            else if (curr->getRight() && curr->getRight()->getData() == index) { return curr->getRight()->getData(); }
            else if (curr->getLeft() && curr->getLeft()->getData() == index) { return curr->getLeft()->getData(); }
            else
            {
                T d = T();
                T& data = d;
                return data;
            }
        }

        virtual int size() { return sz; }

        virtual void insert(T data)
        {
            root = Avltree<T>::insert(root, data);
            root = Avltree<T>::balance(root);
            sz++;
        }

        virtual void remove(Node<T>* data)
        {
            root = Avltree<T>::remove(root, data, sz);
            root = Avltree<T>::balance(root);
        }

        virtual void clear()
        {
            while (root) { root = Avltree<T>::deleteMin(root); }
        }

        virtual Node<T>* getRoot() { return root; }

        virtual void setRoot(Node<T>* r) { root = r; }
    };

    template<class T>
    class set : public Tree<T>
    {
    protected:
        Tree<T> Set;

    public:
        set(int s) { while (s--) { Set.insert(T()); } }
        set() {}

        class iterator : public Avltree<T>
        {
        private:
            Node<T>* curr;

        public:

            iterator() { curr = nullptr; }
            iterator(Node<T>* p)
            {
                curr = p;
            }

            Node<T>* getCurr() { return curr; }

            void operator++()
            {
                if (curr->getRight()) { curr = Avltree<T>::getMinimum(curr->getRight()); }
                else
                {
                    Node<T>* tmp = curr;
                    bool check = true;
                    while (curr->getParent() && curr->getParent()->getLeft() != curr)
                    {
                        if (curr->getParent()) { curr = curr->getParent(); }
                        else { check = false; break; }
                    }
                    if (check) { curr = curr->getParent(); }
                    else { curr = tmp; }
                }
            }

            void operator--()
            {
                if (curr->getLeft())
                {
                    curr = Avltree<T>::getMaximum(curr->getLeft());
                }
                else
                {
                    Node<T>* tmp = curr;
                    bool check = true;
                    while (curr->getParent() && curr->getParent()->getRight() != curr)
                    {
                        if (curr->getParent()) { curr = curr->getParent(); }
                        else { check = false; break; }
                    }
                    if (check) { curr = curr->getParent(); }
                    else { curr = tmp; }
                }
            }

            T& operator*() { return curr->getData(); }

            bool operator==(iterator p) { return curr == p.getCurr(); }
            bool operator!=(iterator p) { return curr != p.getCurr(); }
        };

        virtual iterator begin() { return iterator(Avltree<T>::getMinimum(Set.getRoot())); }

        virtual iterator end() { return iterator(Avltree<T>::getMaximum(Set.getRoot())); }

        Tree<T> getTree() { return Set; }

        virtual void insert(T data)
        {
            Set.insert(data);
        }

        virtual void erase(set<T>::iterator i)
        {
            Set.remove(i.getCurr());
        }

        virtual void clear()
        {
            Set.clear();
        }

        friend std::ostream& operator<<(std::ostream& output, set p) { return output << p.Set; }
    };

    template<class T1, class T2>
    class map : public Avltree<pair<T1, T2>>
    {
    protected:
        Tree<pair<T1, T2>> Map;

    public:
        map() {}

        class iterator : public Avltree<pair<T1, T2>>
        {
        private:
            Node<pair<T1, T2>>* curr;

        public:
            T1 first; T2 second;

            iterator() { curr = nullptr; first = T1(); second = T2(); }
            iterator(Node<pair<T1, T1>>* p)
            {
                curr = p;
                first = curr ? curr->getData().first : T1();
                second = curr ? curr->getData().second : T2();
            }

            Node<pair<T1, T1>>* getCurr() { return curr; }

            void operator++()
            {
                if (!curr) { return; }
                if (curr->getRight()) { curr = Avltree<pair<T1, T1>>::getMinimum(curr->getRight()); }
                else
                {
                    Node<pair<T1, T1>>* tmp = curr;
                    bool check = true;
                    while (curr->getParent() && curr->getParent()->getLeft() != curr)
                    {
                        if (curr->getParent()) { curr = curr->getParent(); }
                        else { check = false; break; }
                    }
                    if (check) { curr = curr->getParent(); }
                    else { curr = tmp; }
                }
                first = curr ? curr->getData().first : T1();
                second = curr ? curr->getData().second : T2();
            }

            void operator--()
            {
                if (curr->getLeft())
                {
                    curr = Avltree<pair<T1, T1>>::getMaximum(curr->getLeft());
                }
                else
                {
                    Node<pair<T1, T1>>* tmp = curr;
                    bool check = true;
                    while (curr->getParent() && curr->getParent()->getRight() != curr)
                    {
                        if (curr->getParent()) { curr = curr->getParent(); }
                        else { check = false; break; }
                    }
                    if (check) { curr = curr->getParent(); }
                    else { curr = tmp; }
                }
                first = curr ? curr->getData().first : T1();
                second = curr ? curr->getData().second : T2();
            }

            bool operator==(iterator p) { return curr == p.getCurr(); }
            bool operator!=(iterator p) { return curr != p.getCurr(); }
        };

        virtual iterator begin() { return iterator(Avltree<pair<T1, T1>>::getMinimum(Map.getRoot())); }

        virtual iterator end() { return iterator(Avltree<pair<T1, T1>>::getMaximum(Map.getRoot())); }

        virtual Tree<pair<T1, T2>> getMap() { return Map; }

        virtual int size() { return Map.size(); }

        virtual void insert(T1 index, T2 data)
        {
            pair<T1, T2> p(index, data);
            Map.insert(p);
        }

        virtual void erase(map<T1, T2>::iterator i)
        {
            Map.remove(i.getCurr());
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
    };

    template<class T>
    class heap
    {
    private:
        T* array;
        int length;
        int size;

    public:
        heap<T>() { array = new T[100]; length = 0; size = 100; }

        heap<T>(int size)
        {
            if (size > 0) { array = new T[size]; }
            else { array = new T[100]; }
            length = 0;
            this->size = size;
        }

        int getParentIndex(int index)
        {
            if (index < 0 || index > length) return 0;
            return (index - 1) / 2;
        }

        int getLeftChildIndex(int index)
        {
            if (index < 0 || index > length) return 0;
            return (2 * index + 1);
        }

        int getRightChildIndex(int index)
        {
            if (index < 0 || index > length) return 0;
            return (2 * index + 2);
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

            if (left >= length) { return; }

            if (array[left] > array[right]) { mi = left; }
            if (right < length && array[right] > array[mi]) { mi = right; }

            if (mi != i)
            {
                swap(array[mi], array[i]);
                SiftDown(mi);
            }
        }

        void SiftUp(int i)
        {
            int p = getParentIndex(i);

            if (i > length || i == 0) { return; }

            if (array[i] > array[p])
            {
                swap(array[i], array[p]);
            }
            SiftUp(p);
        }

        T getMaximum()
        {
            return length > 0 ? array[0] : T();
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

        void erase(T element)
        {
            int i = 0;
            while (i <= length)
            {
                if (array[i] == element) { break; }
                ++i;
            }

            if (empty() || i == length) { return; }
            if (i >= 0 && i < length) { swap(array[i], array[length - 1]); }
            array[length - 1] = 0;
            --length;
            SiftDown(0);
        }

        T ExtractMaximum()
        {
            T max = getMaximum();
            swap(array[0], array[length - 1]);
            array[length - 1] = T();
            length--;
            SiftDown(0);
            return max;
        }

        void print()
        {
            for (int i = 0; i < length; ++i) { std::cout << array[i] << " "; }
        }
    };
}
