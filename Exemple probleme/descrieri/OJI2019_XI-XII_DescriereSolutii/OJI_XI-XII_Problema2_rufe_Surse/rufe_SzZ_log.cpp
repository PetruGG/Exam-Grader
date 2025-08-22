/// autor Szabo Zoltan
#include <bits/stdc++.h>
using namespace std;
ifstream fin("rufe.in");
ofstream fout("rufe.out");
long long distanta(long long n, long long m, long long a, long long b, long long k)
{
    if (m*n==k) return 0;
    long long v;
    long long stanga;
    long long dreapta;
    long long mij,d,sus,st,jos,dr,k1;
    sus=a-1;
    st=b-1;
    jos=n-a;
    dr=m-b;
    stanga=1;    // limita stanga pt cautare binara
    dreapta=max(sus+st, max(st+jos, max(jos+dr, dr+sus))); // limita dreapta pt cautatre binara
    while (stanga<=dreapta)
    {
        mij=(stanga+dreapta)/2;
        d=mij-1;
        k1= 2*(d+1)*d; /// calculez numarul tricourilor la distanta <=d
  /// metoda includerii si excluderii
        if (d>sus) {v=d-sus;k1=k1-v*v;};  /// elimin locurile de deasupra dreptunghiului
        if (d>st) {v=d-st;k1=k1-v*v;};  /// elimin locurile din stanga dreptunghiului
        if (d>jos) {v=d-jos;k1=k1-v*v;}; /// elimin locurile de sub dreptunghi
        if (d>dr) {v=d-dr;k1=k1-v*v;}; /// elimin locurile din dreapta dreptunghiului
        if (d>st+sus+1) {v=d-st-sus-1;k1=k1+v*(v+1)/2;} /// adaug locurile eliminate de doua ori
        if (d>st+jos+1) {v=d-st-jos-1;k1=k1+v*(v+1)/2;}
        if (d>dr+jos+1) {v=d-dr-jos-1;k1=k1+v*(v+1)/2;}
        if (d>dr+sus+1) {v=d-dr-sus-1;k1=k1+v*(v+1)/2;}
        k1=m*n-k1-1; /// transform k1 in numarul tricourilor la distanta >d
        if (k1<k)
            dreapta=mij-1;
        else
            stanga=mij+1;
    }
    return dreapta;
}
int main()
{
    long long n,m,a,b,k;
    fin>>n>>m>>a>>b>>k;
    fout<<distanta(n,m,a,b,k)<<"\n";
    return 0;
}
