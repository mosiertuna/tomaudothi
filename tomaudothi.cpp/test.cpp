#include<bits/stdc++.h>
using namespace std;

int n, m;
vector<int> adj[100005];
int degree[100005], colors[100005];

bool cmp(int a, int b) {
    return degree[a] > degree[b];
}

int GreedyColoring() {

    memset(degree, 0, sizeof(degree));
    for(int i = 1; i <= m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
        degree[u]++;
        degree[v]++;
    }

    // Sắp xếp các đỉnh theo thứ tự giảm dần của bậc
    vector<int> vertices(n);
    for(int i = 1; i <= n; i++) {
        vertices[i-1] = i;
    }
    sort(vertices.begin(), vertices.end(), cmp);

    // Duyệt qua các đỉnh theo thứ tự đã sắp xếp
    int num_colors = 0;
    memset(colors, -1, sizeof(colors));
    for(int i = 0; i < n; i++) {
        int v = vertices[i];

        // Xác định màu sẽ được sử dụng cho đỉnh này
        unordered_set<int> used_colors;
        for(auto neighbor : adj[v]) {
            if(colors[neighbor] != -1) {
                used_colors.insert(colors[neighbor]);
            }
        }
        int color = 1;
        while(used_colors.count(color)) {
            color++;
        }

        // Lưu màu đã chọn và tăng số lượng màu cần tô
        colors[v] = color;
        num_colors = max(num_colors, color);
    }
    return num_colors;
}

int main(){
    cin >> n >> m;
    int num_colors = GreedyColoring();

    cout << num_colors << endl;
    for(int i = 1; i <= n; i++) {
        cout << colors[i] << endl;
    }
    return 0;
}
