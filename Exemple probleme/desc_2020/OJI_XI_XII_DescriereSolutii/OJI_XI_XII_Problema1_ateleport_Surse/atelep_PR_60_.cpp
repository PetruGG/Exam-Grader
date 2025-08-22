/// prof. Ionel-Vasile Pit-Rada
/// Colegiul National Traian din Drobeta Turnu Severin

///K==0, 60p
#include <fstream>
using namespace std;
ifstream  fin("ateleport.in");
ofstream fout("ateleport.out");
int N,M,P,L,K,k,nh,i,j,x,y,t;
int start[10002],d[10002],viz[10002];
struct legatura{
    int vecin,cost,leg;
}V[20002];
struct nodh{
    int varf,cost;
}H[20002];
void urcare(int q){
    while(q>=2 && H[q/2].cost>H[q].cost){
        nodh x=H[q/2]; H[q/2]=H[q]; H[q]=x;
        q=q/2;
    }
}
void coborare(int q){
    while(2*q<=nh){
        int r=2*q;
        if(r+1<=nh && H[r].cost>H[r+1].cost)r++;
        if(H[q].cost>H[r].cost){
            nodh x=H[q]; H[q]=H[r]; H[r]=x;
            q=r;
        }
        else break;
    }
}
int main()
{
    fin>>N>>M>>P>>L>>K;
    k=0;
    for(i=1;i<=M;i++){
        fin>>x>>y>>t;
        k++; V[k]={y,t,start[x]}; start[x]=k;
        k++; V[k]={x,t,start[y]}; start[y]=k;
    }
    if(K==0){
        for(i=1;i<=N;i++)d[i]=1000000000;
        nh=0;
        for(i=start[1];i;i=V[i].leg){
            nh++; H[nh]={V[i].vecin,V[i].cost};
            urcare(nh);
        }
        viz[1]=1; d[1]=0;
        while(viz[N]==0){
            while(viz[H[1].varf]==1){
                H[1]=H[nh]; nh--; coborare(1);
            }
            j=H[1].varf;
            viz[j]=1; d[j]=H[1].cost;
            for(i=start[j];i;i=V[i].leg){
                k=V[i].vecin;
                if(viz[k]==0 && d[k]>d[j]+V[i].cost){
                    d[k]=d[j]+V[i].cost;
                    nh++; H[nh]={k,d[k]}; urcare(nh);
                }
            }
        }
        fout<<d[N];
    }
    fout.close(); fin.close();
    return 0;
}
