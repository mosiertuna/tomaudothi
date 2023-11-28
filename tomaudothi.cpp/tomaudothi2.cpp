#include<bits/stdc++.h>
using namespace std;

const int N = 1e5+5;
int n, m;
vector<int> A[N], C;

int B[N];

void input(){
    cin >> n >> m;
    for(int i=1; i<=m; i++){
        int u, v;
        cin >> u >> v;
        A[u].push_back(v);
        A[v].push_back(u);
    }
}

void colorGraph(){
    int k = 1;
    for(int i=1; i<=n; i++){
        if(B[i] == 0){
            B[i] = k;
            C.insert(C.end(), A[i].begin(), A[i].end());
            for(int j=2; j<=n; j++){
                if(find(C.begin(), C.end(), j) == C.end() && B[j] == 0){
                    bool can_color = true;
                    for(auto v : A[j]){
                        if(B[v] == k){
                            can_color = false;
                            break;
                        }
                    }
                    if(can_color){
                        B[j] = k;
                        C.insert(C.end(), A[j].begin(), A[j].end());
                    }
                }
            }
            C.clear();
            k++;
        }
    }
}

int main(){
    input();
    colorGraph();
    for(int i=1; i<=n; i++)
        cout << B[i] << " ";
    return 0;
}
