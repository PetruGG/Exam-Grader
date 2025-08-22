/// prof. Ionel-Vasile Pit-Rada
/// Colegiul National Traian din Drobeta Turnu Severin

///Dijkstra
#include <fstream>
using namespace std;
ifstream  fin("ateleport.in");
ofstream fout("ateleport.out");
int N,M,P,L,K,k,i,x,y,z,c,t,nh,x1,y1,z1,c1;
int start[10002],d[10002][11][11],poz[10002][11][11];
char viz[10002][11][11];
struct legatura{
    int vecin,cost,leg;
}V[20002];
struct nodh{
    int v,k,l,c;
}H[1000002];
void urcare(int q){
    while(q>=2 && H[q/2].c>H[q].c){
        poz[H[q].v][H[q].k][H[q].l]=q/2;
        poz[H[q/2].v][H[q/2].k][H[q/2].l]=q;
        nodh x=H[q/2]; H[q/2]=H[q]; H[q]=x;
        q=q/2;
    }
}
void coborare(int q){
    while(2*q<=nh){
        int r=2*q;
        if(r+1<=nh && H[r].c>H[r+1].c)r++;
        if(H[q].c>H[r].c){
            poz[H[q].v][H[q].k][H[q].l]=r;
            poz[H[r].v][H[r].k][H[r].l]=q;
            nodh x=H[q]; H[q]=H[r]; H[r]=x;
            q=r;
        }
        else break;
    }
}
int main(){
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
    nh=1; H[1]={1,0,0,0};
    poz[1][0][0]=1;
    viz[1][0][0]=1;
    do{
        x=H[1].v;y=H[1].k;z=H[1].l;c=d[x][y][z];
        viz[x][y][z]=0;

        H[1]=H[nh];
        poz[H[nh].v][H[nh].k][H[nh].l]=1;
        nh--;
        coborare(1);
        for(int i=start[x];i;i=V[i].leg){
            ///fara teleportare
            x1=V[i].vecin;c1=V[i].cost;
            if(z==0){y1=y;z1=0;}
            else {y1=y+1;z1=0;}
            if(c+c1<d[x1][y1][z1]){
                d[x1][y1][z1]=c+c1;
                if(viz[x1][y1][z1]==0){
                    nh++; H[nh]={x1,y1,z1,c+c1};
                    poz[x1][y1][z1]=nh;
                    urcare(nh);
                    viz[x1][y1][z1]=1;
                }
                else{
                    H[poz[x1][y1][z1]].c=c+c1;
                    urcare(poz[x1][y1][z1]);
                }
            }
            if(K>0){
                ///cu teleportare
                if(y==K)continue;
                if(z==0)c1=P;
                else c1=0;
                y1=y;z1=z+1;
                if(z1==L){
                    z1=0;
                    y1=y+1;
                }
                if(c+c1<d[x1][y1][z1]){
                    d[x1][y1][z1]=c+c1;
                    if(viz[x1][y1][z1]==0){
                        nh++; H[nh]={x1,y1,z1,c+c1};
                        poz[x1][y1][z1]=nh;
                        urcare(nh);
                        viz[x1][y1][z1]=1;
                    }
                    else{
                        H[poz[x1][y1][z1]].c=c+c1;
                        urcare(poz[x1][y1][z1]);
                    }
                }
            }
        }
    }
    while(nh>0);
    int vmin=d[N][K][0];
    for(y=0;y<=K-1;y++){
        for(z=0;z<=L-1;z++){
            vmin=min(vmin,d[N][y][z]);
        }
    }
    fout<<vmin<<"\n";
    fout.close(); fin.close();
    return 0;
}
