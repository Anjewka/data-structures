#include <bits/stdc++.h>
using namespace std;

class Graph
{
private:
    vector<vector<int>> G;

public:
    Graph() {}
    Graph(vector<vector<int>> G) {this->G = G;}

    vector<int> bfs(int s, int e)
    {
        deque<int> d;
        vector<bool> used(G.size(), false);
        vector<int> p(G.size());
        d.push_back(e);
        while(!d.empty())
        {
            int v = d.back();
            d.pop_back();
            used[v] = true;
            for(int i = 0; i<G.size(); ++i)
            {
                if(!used[i] && G[v][i] > 0)
                {
                    d.push_back(i);
                    p[v] = i;
                    break;
                }
            }
        }

        vector<int> w;
        w.push_back(e);
        int i = e;
        while(i != s)
        {
            w.push_back(p[i]);
            i = p[i];
        }
        reverse(w.begin(), w.end());
        return w;
    }

    vector<int> dfs(vector<pair<int, int>> &g)
    {
        vector<int> w;
        vector<int> res;
        vector<bool> used(false, g.size());
        deque<int> d;
        while(!d.empty())
        {
            int v = d.top();
            d.pop_front();
            used[v] = true;
            for(int i = 0; i<g.size() ; ++i)
            {
                if(g[i].first == v && !used[g[i].second]) {d.push_back(g[i].second); w.push_back(g[i].first);}
                if(g[i].second == v && !used[g[i].first]) {d.push_back(g[i].first); w.push_back(g[i].second);}
            }
        }

        if(!w.empty())
        {
            res.push_back(0);
            
        }
        return w;
    }

    vector<pair<int, int>> min_span_tree()
    {
        vector<pair<int, int>> res;
        vector<bool> used(G.size(), false);
        int count = 1;
        used[0] = true;

        while(count != G.size())
        {
            count ++;
            int x = 0, y = 0, min = 1000;
            for(int i = 0; i<G.size() ; ++i)
            {
                for(int j = 0; j<G[i].size() ; ++j)
                {
                    if(G[i][j] && !used[j] && G[i][j] < min)
                    {
                        min = G[i][j]; x = i; y = j;
                    }
                }
            }
            res.push_back(make_pair(x, y)); used[y] = true;
        }
        return res;
    }

    vector<int> min_way(int i)
    {
        vector<int> way(G.size(), 1000);
        if(i >= G.size()) {return way;}

        vector<int> p(G.size());
        way[i] = 0;
        deque<int> q; 
        q.push_back(i);
        while(!q.empty())
        {
            int i = q.front();
            q.pop_front();
            for(int j = 0; j<G[i].size() ; ++j)
            {
                if(G[i][j] && G[i][j] + way[i] < way[j]) {way[j] = way[i] + G[i][j]; q.push_back(j); p[j] = i;}
            }
        }

        return way;
    }
};

int main()
{
    vector<vector<int>> g;

    g = 
    {
    { 0, 6, 2, 1, 9, 1, 8, 1, 4, 8, 6, 1, 3 },
    { 6, 0, 2, 5, 1, 9, 9, 8, 1, 7, 9, 1, 1 },
    { 2, 2, 0, 4, 2, 2, 5, 3, 4, 6, 0, 3, 0 },
    { 1, 5, 4, 0, 1, 2, 4, 9, 4, 8, 8, 0, 9 },
    { 9, 1, 2, 1, 0, 3, 5, 4, 4, 4, 5, 4, 8 },
    { 1, 9, 2, 2, 3, 0, 2, 5, 1, 6, 9, 5, 8 },
    { 8, 9, 5, 4, 5, 2, 0, 7, 9, 3, 5, 9, 6 },
    { 1, 8, 3, 9, 4, 5, 7, 0, 5, 2, 0, 9, 2 },
    { 4, 1, 4, 4, 4, 1, 9, 5, 0, 6, 9, 2, 9 },
    { 8, 7, 6, 8, 4, 6, 3, 2, 6, 0, 9, 5, 4 },
    { 6, 9, 0, 8, 5, 9, 5, 0, 9, 9, 0, 5, 1 },
    { 1, 1, 3, 0, 4, 5, 9, 9, 2, 5, 5, 0, 0 },
    { 3, 1, 0, 9, 8, 8, 6, 2, 9, 4, 1, 0, 0 },
    };

    Graph G(g);

    vector<pair<int, int>> min_span = G.min_span_tree();
    for(vector<pair<int, int>>::iterator i = min_span.begin() ; i != min_span.end() ; ++i)
    {
        cout << i->first << "------>" << i->second << '\n';
    }

    cout << '\n' << '\n';
    vector<int> w = G.dfs(min_span);
    for(auto & i : w) {cout << i << " ";}
}