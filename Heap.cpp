#include <bits/stdc++.h>
using namespace std;

template<class T>
class Heap
{
private:
    T* array;
    int length;
    int size;

public:
    Heap<T>() { array = new T[100]; length = 0; size = 100; }

    Heap<T>(int size)
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
        for (int i = 0; i < length; ++i) { cout << array[i] << " "; }
    }
};
