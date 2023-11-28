#include<bits/stdc++.h>
using namespace std;

int n, m;
vector<int> adj[100005];
int degree[100005], saturation[100005], colors[100005];

int DSatur() {
    
    memset(degree, 0, sizeof(degree));
    for(int i = 1; i <= m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
        degree[u]++;
        degree[v]++;
    }

    
    memset(colors, -1, sizeof(colors));

    
    set<int> used_colors;

    
    for(int i = 1; i <= n; i++) 
        saturation[i] = 0;
    for(int i = 1; i <= n; i++) {
        for(auto v : adj[i]) {
            if(i != v) {
                saturation[i]++;
            }
        }
    }

    while (find(colors+1, colors+n+1, -1) != colors+n+1) {
        int max_degree = -1;
        int vertex = -1;


        for(int i = 1; i <= n; i++) {
            if(colors[i] == -1 && degree[i] > max_degree) {
                max_degree = degree[i];
                vertex = i;
            } else if (colors[i] == -1 && degree[i] == max_degree && saturation[i] > saturation[vertex]) {
                vertex = i;
            }
        }
        set<int> available_colors;
        for(auto neighbor : adj[vertex]) {
            if(colors[neighbor] != -1) {
                available_colors.insert(colors[neighbor]);
            }
        }

        int color = 0;
        for(int i = 1; i <= n; i++) {
            if(available_colors.find(i) == available_colors.end()) {
                color = i;
                break;
            }
        }
        colors[vertex] = color;
        used_colors.insert(color);

        for(auto neighbor : adj[vertex]) {
            degree[neighbor]--;
            saturation[neighbor]++;
        }

        degree[vertex] = -1;
    }

    return used_colors.size();
}

int main() {
    cin >> n >> m;
    int num_colors = DSatur();


cout << num_colors << endl;
for(int i = 1; i <= n; i++) {
cout << colors[i] << endl;
}
return 0;
}

