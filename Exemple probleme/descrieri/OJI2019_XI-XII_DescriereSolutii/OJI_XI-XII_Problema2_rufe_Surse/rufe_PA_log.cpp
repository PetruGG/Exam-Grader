/// Autor Adrian Panaete
#include <bits/stdc++.h>

using namespace std;
ifstream f("rufe.in");
ofstream g("rufe.out");
int64_t lo,hi,mi,r,R,a[4],b[4];
void read(),solve();
int64_t drep(int64_t A,int64_t B,int64_t D)
{
    return D<=A?D*(D+1)/2:D<=B?A*(A+1)/2+(D-A)*A:D>=A+B?A*B:A*B-(A+B-D)*(A+B-D-1)/2;
}

int main()
{
    read();solve();
    return 0;
}
void read()
{
    int64_t n,m,i,j,k;
    f>>n>>m>>i>>j>>k;
    hi=n+m+1,R=m*n-k-1;
    a[0]=i;b[0]=j-1;a[1]=n-i;b[1]=j;a[2]=i-1;b[2]=m-j+1;a[3]=n-i+1;b[3]=m-j;
    for(int p=0;p<4;p++)if(a[p]>b[p])swap(a[p],b[p]);

}
void solve()
{
    while(hi-lo>1)
    {
        mi=(lo+hi)/2,r=R;
        for(int p=0;p<4;p++)r-=drep(a[p],b[p],mi);
        if(r>=0)
            lo=mi;
        else
            hi=mi;
    }
    g<<hi;
}
