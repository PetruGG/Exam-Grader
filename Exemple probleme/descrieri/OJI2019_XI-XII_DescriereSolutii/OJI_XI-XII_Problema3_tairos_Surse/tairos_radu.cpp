/// autor Radu Muntean
#include <fstream>
#include <assert.h>

using namespace std;

ifstream in ("tairos.in");
ofstream out ("tairos.out");

const int N_MAX = 100, D_MAX = 10000, R = 1000000007;

int N, D;
int fst[N_MAX + 1], val[2 * N_MAX + 1], nxt[2 * N_MAX + 1];
int leafs[N_MAX + 1];
int not_leafs[N_MAX + 1];

// Vom folosi vectorul sol, unde sol[i] va reprezenta numarul de foste frunze aflate la distanta i de radacina modulo R.
long long sol[N_MAX * D_MAX + 1];

void add_edge(int x, int y) {
    // "Static int" spune ca variabila 'contor' va fi initializata doar la primul
    // apel al functie 'add_edge()', iar, in rest, se va prelua valoarea lui 'contor'
    // lasata de apelul precedent al functiei noastre 'add_edge()'.
    static int contor = 0;
    val[++contor] = y;
    nxt[contor] = fst[x];
    fst[x] = contor;
}

int no_nodes_visited = 0;

void dfs(int node, int dad, int depth) {
    no_nodes_visited ++;
    bool is_leaf = true;
    for (int p = fst[node]; p != 0; p = nxt[p]) {
        if (val[p] == dad) {
            continue;
        }
        is_leaf = false;
        dfs(val[p], node, depth + 1);
    }
    if (is_leaf) {
        leafs[depth] ++;
    } else {
        not_leafs[depth] ++;
    }
}

int main() {
    int a, b;
    in >> N >> D;
    assert (N > 1 && N <= 100);
    assert (D > 0 && D <= 10000);

    for (int i = 0; i < N - 1; i++) {
        in >> a >> b;
        assert (a > 0 && a <= N);
        assert (b > 0 && b <= N);
        add_edge(a, b);
        add_edge(b, a);
    }
    // Facem un DFS ca sa gasim numarul de frunze de la fiecare nivel de adancime in arbore.
    dfs(1, 0, 0);
    assert (no_nodes_visited == N);

    // Avand o singura radacina, sol[0] = 1;
    sol[0] = 1;
    long long solution = 0;
    // TODO(Radu) scos long long.

    for (int depth = 0; depth <= D; depth++) {
        sol[depth] %= R;
        if (D - depth < N) {
            solution += sol[depth] * not_leafs[D - depth];
            solution %= R;
        }
        for (int forward = 1; forward <= N; forward++) {
            sol[depth + forward] += sol[depth] * leafs[forward];
            // TODO(Radu) pus modulo si aici la calibratul testelor.
        }
    }

    out << solution;
    return 0;
}
