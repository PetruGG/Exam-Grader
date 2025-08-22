/// stud. Bogdan Ciobanu
/// Universitatea din Bucuresti


#include <stdio.h>
#include <string.h>

#define kMaxN 10000
#define kMaxM 10000
#define kQueueSize (1 << 21)
#define kMaxSkipSize 10
#define kMaxNumSkips 10

int initial_edge_time[kMaxM], edge_nodes[2 * kMaxM];
int edge_time[2 * kMaxM], edge_xor[2 * kMaxM];
int count_edges[kMaxN + 1];

int q[kQueueSize];
int d[kMaxN][kMaxSkipSize + 1][kMaxNumSkips + 1];
char in_queue[kMaxN][kMaxSkipSize + 1][kMaxNumSkips + 1];

int num_nodes, skip_time, mx_edges_skipped, skip_limit;

void read_graph() {
    FILE* f = fopen("ateleport.in", "r");
    int num_edges;
    fscanf(f, "%d%d%d%d%d", &num_nodes, &num_edges, &skip_time, &mx_edges_skipped, &skip_limit);
    for (int i = 0; i < num_edges; ++i) {
        int u, v, t;
        fscanf(f, "%d%d%d", &u, &v, &t); --u; --v;
        edge_nodes[i << 1 | 0] = u;
        edge_nodes[i << 1 | 1] = v;
        initial_edge_time[i] = t;

        ++count_edges[u];
        ++count_edges[v];
    }
    for (int i = 1; i <= num_nodes; ++i) {
        count_edges[i] += count_edges[i - 1];
    }
    for (int i = 0; i < 2 * num_edges; ++i) {
        int index = --count_edges[edge_nodes[i]];
        edge_time[index] = initial_edge_time[i >> 1];
        edge_xor[index] = edge_nodes[i] ^ edge_nodes[i ^ 1];
    }
    fclose(f);
}

int update_if_less(int d1, int* d2) {
    if (*d2 == -1 || d1 < *d2) {
        *d2 = d1;
        return 1;
    }
    return 0;
}

int pack(int u, int skip_size, int used_skips) {
    return u << 8 | skip_size << 4 | used_skips;
}

void unpack(int qelement, int* u, int* skip_size, int* used_skips) {
    *used_skips = qelement & 15;
    *skip_size = (qelement >> 4) & 15;
    *u = qelement >> 8;
}

void maybe_enqueue(int* tail, int u, int skip_size, int used_skips) {
    if (!in_queue[u][skip_size][used_skips]) {
        in_queue[u][skip_size][used_skips] = 1;
        q[(*tail)++ & (kQueueSize - 1)] = pack(u, skip_size, used_skips);
    }
}

int solve() {
    int head = 0, tail = 1;
    int answer = -1;
    memset(d, -1, sizeof d);
    d[0][0][0] = 0;
    while (head != tail) {
        int u, skip_size, used_skips;
        unpack(q[head++ & (kQueueSize - 1)], &u, &skip_size, &used_skips);
        in_queue[u][skip_size][used_skips] = 0;
        if (u == num_nodes - 1) {
            update_if_less(d[u][skip_size][used_skips], &answer);
        }

        for (int i = count_edges[u]; i != count_edges[u + 1]; ++i) {
            int v = u ^ edge_xor[i];
            int t = edge_time[i];
            if (update_if_less(d[u][skip_size][used_skips] + t, &d[v][0][used_skips])) {
                maybe_enqueue(&tail, v, 0, used_skips);
            }
            if (used_skips < skip_limit
                && update_if_less(d[u][skip_size][used_skips] + skip_time, &d[v][1][used_skips + 1])) {
                maybe_enqueue(&tail, v, 1, used_skips + 1);
            }
            if (skip_size > 0 && skip_size < mx_edges_skipped
                && update_if_less(d[u][skip_size][used_skips], &d[v][skip_size + 1][used_skips])) {
                maybe_enqueue(&tail, v, skip_size + 1, used_skips);
            }
        }
    }
    return answer;
}

void print(int solution) {
    FILE* f = fopen("ateleport.out", "w");
    fprintf(f, "%d\n", solution);
    fclose(f);
}

int main() {
    read_graph();
    print(solve());
    return 0;
}
