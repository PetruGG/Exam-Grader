/// stud. Theodor-Pierre Moroianu
/// Universitatea Bucuresti

#include <bits/stdc++.h>
using namespace std;

typedef long long i64;
const i64 inf = 2LL * 1000000000 * 1000000000;

vector <i64> calc_dp(int nr)
{
    // dp[i] = in cate moduri pot imparti suma i
    // dp[0] = 1
    // dp[i] = sum(dp[<i])
    // -> dp[k] = (2 ** k) - 1 | k > 0

    vector <i64> dp(nr + 1);
    dp[0] = dp[1] = 1;

    for (int i = 2; i <= nr; i++)
        dp[i] = min(inf, 2 * dp[i - 1]);

    return dp;
}

vector <int> partition(int n, i64 ord)
{
    auto dp = calc_dp(n);
    vector <int> ans;

    while (n > 0) {
        assert(dp[n] >= ord);

        int act = 1; // cate scot
        while (dp[n - act] < ord)
            ord -= dp[n - act], act++;

        /// trebuie sa scot act
        assert(act <= n);
        ans.push_back(act);
        n -= act;
    }

    assert(ord == 1 && n == 0);

    return ans;
}

i64 order(int n, vector <int> partition)
{
    auto dp = calc_dp(n);
    i64 ord = 1;

    for (auto i : partition) {
        for (int j = 1; j < i; j++)
            assert((ord += dp[n - j]) < inf);
        assert((n -= i) >= 0);
    }

    assert(n == 0);

    return ord;
}

int main()
{
    ifstream in("partit.in");
    ofstream out("partit.out");

    int cerinta, n;
    in >> cerinta >> n;

    assert((cerinta == 1 || cerinta == 2) && (n >= 1 && n <= 10000));

    if (cerinta == 1) {
        i64 ord;
        in >> ord;

        assert(1 <= ord && ord < inf);

        auto ans = partition(n, ord);
        for (auto i : ans)
            out << i << ' ';
        out << '\n';
    }
    else {
        vector <int> part;
        int x;
        while (in >> x)
            part.push_back(x);

        i64 ord = order(n, part);
        out << ord << '\n';
    }

    in.close();
    out.close();

    return 0;
}
