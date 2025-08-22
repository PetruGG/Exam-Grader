/// prof. Pit-Rada Ionel Vasile
/// Colegiul National Traian din Drobeta-Turnu Severin
#include <fstream>
using namespace std;
ifstream  fin("partit.in");
ofstream fout("partit.out");
int c,n,a;
long long k,p;
int g(int n, long long k){
    if(n>=64)return 1;
    if(((long long)1<<(n-1))>=k)return 1;
    return 0;
}
int main()
{
    fin>>c>>n;
    if(c==1){
        fin>>k;
        long long d;
        int i;
        while(k){
            while(g(n-1,k)==1){
                fout<<"1 ";n--;
            }
            if(k==((long long)1<<(n-1))){
                i=n; k=0;
            }
            else{
                if(k==((long long)1<<(n-1))-1){
                    i=n-1; k=0;
                }
                else{
                    d=((long long)1<<(n-2)); i=1;
                    while(k>=d && d){
                        k=k-d; i++; d=d/2;
                    }
                }
            }
            fout<<i<<" ";
            n=n-i;
        }
        while(n){fout<<"1 ";n--;}
    }
    else{
        k=0;
        while(fin>>a){
            long long d=((long long)1<<(n-2));
            n=n-a;
            while(a>1){
                k=k+d; d=d/2; a--;
            }
        }
        fout<<k+1;
    }
    fout.close(); fin.close();
    return 0;
}
