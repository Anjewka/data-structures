#include <bits/stdc++.h>
using namespace std;

template<class T>
class heap
{
private:
    T* array;
    unsigned int length;
    unsigned int size;

public:
    heap<T>() { array = new T[100]; length = 0; size = 100; }

    heap<T>(unsigned int size)
    {
        if (size > 0) { array = new T[size]; }
        else { array = new T[100]; }
        length = 0;
        this->size = size;
    }

    size_t&  getParentIndex(const size_t& index)
    {
        size_t out;
        if (index < 0 || index > length) { out = 0; }
        else { out = (index - 1) / 2; }
        return out;
    }

    size_t& getLeftChildIndex(const size_t& index)
    {
        size_t out;
        if (index < 0 || index > length) { out = 0; }
        else { out = (2 * index + 1); }
        return out;
    }

    size_t& getRightChildIndex(const size_t& index)
    {
        size_t out;
        if (index < 0 || index > length) { out = 0; }
        else { out = (2 * index + 2); }
        return out;
    }

    bool empty()
    {
        return length > 0 ? false : true;
    }

    void SiftDown(size_t i)
    {
        unsigned int left = getLeftChildIndex(i);
        unsigned int right = getRightChildIndex(i);
        unsigned int mi = i;

        if (left >= length) { return; }

        if (array[left] > array[right]) { mi = left; }
        if (right < length && array[right] > array[mi]) { mi = right; }

        if (mi != i)
        {
            swap(array[mi], array[i]);
            SiftDown(mi);
        }
    }

    void SiftUp(size_t i)
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

    void insert(const T& data)
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

    void erase(const T& element)
    {
        size_t i = 0;
        while (i <= length)
        {
            if (array[i] == element) { break; }
            ++i;
        }

        if (empty() || i == length) { return; }
        if (i < length) { swap(array[i], array[length - 1]); }
        array[length - 1] = 0;
        --length;
        SiftDown(0);
    }

    T& ExtractMaximum()
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
        for (size_t i = 0; i < length; ++i) { cout << array[i] << " "; }
    }
};
