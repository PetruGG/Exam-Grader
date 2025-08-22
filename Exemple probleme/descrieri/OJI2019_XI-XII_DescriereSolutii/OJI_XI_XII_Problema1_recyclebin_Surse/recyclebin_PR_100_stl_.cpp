/// prof. Ionel-Vasile Pit-Rada
/// Colegiul National Traian Drobeta Turnu Severin

// O(N^2 * log(N))
#include <vector>
#include <fstream>
using namespace std;

int main() {
    ifstream            f("recyclebin.in");
    ofstream            g("recyclebin.out");
    int                 n; f >> n;
    vector<int>         a(n + 1);
    vector<vector<int>> best(n + 1, vector<int>(n));
    
    for (int i = 1; i <= n; i++) {
        f >> a[i]; best[i][0] = a[i];
    }
    
    int ans = a[1];
    for (int i = 2; i <= n; i++) {
        for (int mask = 0; mask < i; mask++) {
            best[i][mask] = max(best[i][mask], a[i] + best[i - 1][mask]);
            for (int avail = mask, bit = 0; avail; avail ^= bit) {
                bit = avail & -avail;
                best[i][mask] = max(best[i][mask], a[i] + best[i - bit - 1][mask ^ bit]);
                best[i][mask] = max(best[i][mask], best[i - bit][mask ^ bit]);
            }
            ans = max(ans, best[i][mask]);
        }
    }
    g << ans;
    
    return 0;
}
