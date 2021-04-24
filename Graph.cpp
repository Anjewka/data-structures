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
