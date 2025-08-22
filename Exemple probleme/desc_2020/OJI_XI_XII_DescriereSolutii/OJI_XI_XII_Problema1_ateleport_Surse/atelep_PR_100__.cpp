/// prof. Ionel-Vasile Pit-Rada
/// Colegiul National Traian din Drobeta Turnu Severin

///Bellman-Ford cu coada
#include <fstream>
using namespace std;
ifstream  fin("ateleport.in");
ofstream fout("ateleport.out");
int N,M,P,L,K,k,i,x,y,z,c,t,x1,y1,z1,c1,pr,ul,nr;
int start[10002],d[10002][11][11];
char viz[10002][11][11];
struct legatura{
    int vecin,cost,leg;
}V[20002];
struct nodh{
    int v,k,l;
}H[1000002];///coada

int main()
{
    fin>>N>>M>>P>>L>>K;
    k=0;
    for(i=1;i<=M;i++){
        fin>>x>>y>>t;
        k++; V[k]={y,t,start[x]}; start[x]=k;
        k++; V[k]={x,t,start[y]}; start[y]=k;
    }
    for(x=1;x<=N;x++){
        for(y=0;y<=K;y++){
            for(z=0;z<=L;z++){
                d[x][y][z]=1000000000;
            }
        }
    }
    d[1][0][0]=0;

    H[1]={1,0,0};pr=1;ul=1;nr=1;
    viz[1][0][0]=1;
    do{
        x=H[pr].v;y=H[pr].k;z=H[pr].l;
        c=d[x][y][z];
        nr--;
        pr++;if(pr>1000000)pr=1;
        viz[x][y][z]=0;

        for(int i=start[x];i;i=V[i].leg){
            x1=V[i].vecin;c1=V[i].cost;

            ///fara teleportare
            if(z==0){y1=y;z1=0;}
            else {y1=y+1;z1=0;}
            if(c+c1<d[x1][y1][z1]){
                d[x1][y1][z1]=c+c1;
                if(viz[x1][y1][z1]==0){
                    nr++;
                    ul++; if(ul>1000000)ul=1;
                    H[ul]={x1,y1,z1};
                    viz[x1][y1][z1]=1;
                }
            }
            if(K>0){
                ///cu teleportare
                if(y==K)continue;
                if(z==0)c1=P;
                else c1=0;
                y1=y;z1=z+1;
                if(z1==L){
                    z1=0;y1=y+1;
                }
                if(c+c1<d[x1][y1][z1]){
                    d[x1][y1][z1]=c+c1;
                    if(viz[x1][y1][z1]==0){
                        nr++;
                        ul++; if(ul>1000000)ul=1;
                        H[ul]={x1,y1,z1};
                        viz[x1][y1][z1]=1;
                    }
                }
            }
        }

    }
    while(nr>0);
    int vmin=d[N][K][0];
    for(y=0;y<=K;y++){
        for(z=0;z<=L;z++){
            vmin=min(vmin,d[N][y][z]);
        }
    }
    fout<<vmin<<"\n";
    fout.close(); fin.close();
    return 0;
}
