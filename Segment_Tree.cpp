#include <iostream>

template<class T>
class Tree
{
private:
	vector<T> array;
	int size;

public:
	Tree<T>() { size = 0; }

	Tree<T>(vector<T>& a)
	{
		array.resize(4 * a.size());
		size = a.size() - 1;
		make_tree(a, 1, 0, size);
	}

	void make_tree(vector<T>& a, int curr, int l, int r)
	{
		if (curr == 1) { array.clear(); array.resize(4 * a.size()); size = a.size(); }
		if (l == r) {array[curr] = a[l];}
		else
		{
			int m = (l + r) / 2;
			make_tree(a, 2 * curr, l, m);
			make_tree(a, 2 * curr + 1, m + 1, r);
			array[curr] = array[2 * curr] + array[2 * curr + 1];
		}
	}

	T segment_sum(int l, int r)
	{
		return segment_sum(1, 0, size - 1, l, r);
	}

	T segment_sum(int curr, int l1, int r1, int l, int r)
	{
		if (l > r || l < 0 || r > size - 1) { return T(); }
		if(l1 == l && r1 == r) {return array[curr];}

		int m = (l1 + r1) / 2;
		return segment_sum(2 * curr, l1, m, l, min(m, r)) + segment_sum(2 * curr + 1, m + 1, r1, max(m + 1, l), r);
	}
};
