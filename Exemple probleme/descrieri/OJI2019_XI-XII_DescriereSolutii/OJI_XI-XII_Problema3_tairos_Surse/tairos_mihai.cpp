/// Autor Mihai Calancea
#include <iostream>
#include <fstream>
#include <vector>
const int MOD = 1e9 + 7;
using namespace std;

vector<int> mem;
vector<vector<int>> g;

void dfs(int, int, vector<int>&, int&, int);
int solve(int);

void dfs(int node, int target, vector<int> &seen, int &ans, int depth) {
    seen[node] = 1;
    if(depth == target) {
        ans = (ans + 1) % MOD;
        return;
    }

    bool any = false;

    for(auto v : g[node]) {
        if(seen[v])
            continue;
        any = true;
        dfs(v, target, seen, ans, depth + 1);
    }

    if(not any) {
        ans += solve(target - depth);
        ans %= MOD;
    }
}

int solve(int d) {
    if(mem[d] >= 0)
        return mem[d];
    int ans = 0;
    vector<int> seen(g.size(), 0);
    dfs(0, d, seen, ans, 0);
    return (mem[d] = ans);
}

int main() {
    ifstream cin("tairos.in");
    ofstream cout("tairos.out");

    int n; cin >> n;
    int d; cin >> d;

    g = vector<vector<int>> (n);

    for(int i = 0; i < n - 1; i += 1) {
        int a, b; cin >> a >> b;
        a -= 1, b -= 1;
        g[a].push_back(b);
        g[b].push_back(a);
    }

    mem = vector<int> (d + 1, -1);

    cout << solve(d) << "\n";
}
