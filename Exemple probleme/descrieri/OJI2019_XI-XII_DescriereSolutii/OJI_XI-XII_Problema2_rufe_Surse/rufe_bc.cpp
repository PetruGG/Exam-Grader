// autor Ciobanu Bogdan
#include <stdio.h>
#include <algorithm>
#include <memory>
using namespace std;

long long n, m, x, y, k;

long long unwrap(long long a) {
    return a > 0LL ? a : 0LL;    
}

long long sum(long long from, long long to, long long i0) {
    if (from > to) return 0LL;
    return (to - from + 2 * i0) * (to - from + 1) / 2;
}

long long count_corner(long long x, long long y, long long d) {
    return sum(unwrap(x + d - n), min(d, m - y), unwrap(n - x - d) + 1)
         + unwrap(m - y - d) * (n - x + 1);
}

long long count_greater_equal(long long d) {
    if (d == 0) return n * m;
    
    return count_corner(x, y, d) + count_corner(n - x + 1, m - y + 1, d)
         + count_corner(n - x + 1, y, d) + count_corner(x, m - y + 1, d)
         - unwrap(x - d) - unwrap(y - d) - unwrap(n - (x + d - 1)) - unwrap(m - (y + d - 1));
}

int main() {
    unique_ptr<FILE, decltype(&fclose)> f(fopen("rufe.in", "r"), &fclose);
    fscanf(f.get(), "%lld%lld%lld%lld%lld", &n, &m, &x, &y, &k);
    
    long long l = -1, r = n + m - 1;
    while (r - l > 1LL) {
        long long p = (l + r) / 2;
        ((count_greater_equal(p) < k) ? r : l) = p;
    }
    
    f.reset(fopen("rufe.out", "w"));
    fprintf(f.get(), "%lld\n", l);
    return 0;
}
