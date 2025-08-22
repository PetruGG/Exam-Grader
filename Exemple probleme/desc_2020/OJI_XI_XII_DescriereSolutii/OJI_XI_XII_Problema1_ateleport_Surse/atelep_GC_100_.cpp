/// stud. George Chichirim
/// Universitatea din Oxford

#include <bits/stdc++.h>

using namespace std;

const int MaxN = 10010;
const int inf = 1e9 + 10;

struct edge {
    int node, cost;
};
vector<edge> g[MaxN];
struct inHeap {
    int node, k, l, cost;
    bool operator <(const inHeap& aux) const {
        return cost > aux.cost;
    }
};
priority_queue<inHeap> q;
int din[MaxN][11][11];
int N, M, P, L, K;

void update(int nod, int k, int l, int cost) {
    if (din[nod][k][l] > cost) {
        din[nod][k][l] = cost;
        q.push({nod, k, l, cost});
    }
}

void dijkstra() {
    for (int i = 1; i <= N; i++)
        for (int k = 0; k <= K; k++)
            for (int l = 0; l <= L; l++)
                din[i][k][l] = inf;
    din[1][0][0] = 0;
    q.push({1, 0, 0, 0});
    while (!q.empty()) {
        auto aux = q.top();
        q.pop();
        int node = aux.node;
        int k = aux.k;
        int l = aux.l;
        int cost = aux.cost;
        if (din[node][k][l] != cost) continue;
        if (k < K) {
            int cost1 = cost;
            if (l > 0) cost1 += P;
            update(node, k + 1, 0, cost1);
        }
        for (auto e : g[node]) {
            int node1 = e.node;
            if (l < L){
                update(node1, k, l + 1, cost);
            }
            if (l == 0) {
                int cost1 = cost + e.cost;
                update(node1, k, 0, cost1);
            }
        }
    }
}

int main() {
    ifstream fin("ateleport.in");
    ofstream fout("ateleport.out");
    fin >> N >> M >> P >> L >> K;
    assert(1 <= N && N <= 10000);
    assert(1 <= M && M <= 10000);
    assert(1 <= P && P <= 100000);
    assert(0 <= L && L <= 10);
    assert(0 <= K && K <= 10);
    map<pair<int, int>, int> vaz;
    for (int i = 1; i <= M; i++) {
        int x, y, t;
        fin >> x >> y >> t;
        assert(1 <= x && x <= N);
        assert(1 <= y && y <= N);
        assert(x != y);
        assert(1 <= t  && t <= 100000);
        assert(vaz.find({x, y}) == vaz.end());
        vaz[{x, y}] = vaz[{y, x}] = 1;
        g[x].push_back({y, t});
        g[y].push_back({x, t});
    }
    dijkstra();
    fout << din[N][K][0] << "\n";
    return 0;
}
