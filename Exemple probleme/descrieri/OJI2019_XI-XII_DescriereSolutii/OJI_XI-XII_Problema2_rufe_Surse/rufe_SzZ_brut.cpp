/// Autor Szabo Zoltan
#include <bits/stdc++.h>

using namespace std;
ifstream fin("rufe.in");
ofstream fout("rufe.out");
long long distanta(long long n, long long m, long long a, long long b, long long k)
{
    if (m*n==k) return 0;
    long long d,sus,st,jos,dr,l1,l2;
    sus=a-1;
    st=b-1;
    jos=n-a;
    dr=m-b;
    long long contor=0;
    long long lmax=max(sus+st, max(st+jos, max(jos+dr, dr+sus)));
    for (d=lmax; d>0; d--)
        for (l1=d; l1>0; l1--)
        {
            l2=d-l1;
            if (a+l1<=n and b+l2<=m)
            {
                contor++;
                if (contor==k)
                    return d;
            }
            if (b+l1<=m and a-l2>0)
            {
                contor++;
                if (contor==k)
                    return d;
            }
            if (a-l1>0 and b-l2>0)
            {
                contor++;
                if (contor==k)
                    return d;
            }
            if (b-l1>0 and a+l2<=n)
            {
                contor++;
                if (contor==k)
                    return d;
            }
        }
}
int main()
{
    long long n,m,t,a,b,k;
    fin>>n>>m>>a>>b>>k;
    fout<<distanta(n,m,a,b,k)<<"\n";
    return 0;
}
