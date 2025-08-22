/// Autor Szabo Zoltan
#include <iostream>
#include <fstream>
using namespace std;
ifstream fin("tairos.in");
ofstream fout("tairos.out");
const long long R=1000000007;
int N, D, mat[105][105], tata[105], lungime[105],sterile[105], frunze[105],nrfrunze[100],distanta[105], nfr, maxadanc;
bool viz[105], steril[105];   /// un nod este steril daca  nu e frunza
long long nrsol[10005];
void dfs(int nod, int adancime, int &maxadanc)
{
    if (maxadanc<adancime) maxadanc=adancime;       /// calculam adancimea maxima al arborelui
    lungime[nod]=adancime;
    for (int i=1;i<=mat[nod][0];i++)
        if(!viz[mat[nod][i]])          /// daca un nod nu a fost vizitat, atunci este fiul nodului curent
        {
            steril[nod]=true;          /// nodul curent nu poate fi frunza daca are un descendent
            viz[mat[nod][i]]=true;
            tata[mat[nod][i]]=nod;
            dfs(mat[nod][i], adancime+1,maxadanc);
        }
}

void citire(int &N, int &D, int tata[105])
{
    int x,y;
    fin>>N>>D;
    for(int i=1; i<=N-1; i++)
    {
        fin>>x>>y;
        mat[x][0]++;mat[x][mat[x][0]]=y;         /// lista de adiacenta
        mat[y][0]++;mat[y][mat[y][0]]=x;
    }
    viz[1]=true;                     /// se stie ca nodul 1 este radacina
    dfs(1,0,maxadanc);               /// dfs porneste cu nivel 0, radacina 1 si va calcula adancimea maxima a arborelui
}
int main()
{

    citire(N,D,tata);
    for(int i=1;i<=N; i++)
        if (steril[i])
            sterile[lungime[i]]++;  /// creste numarul nodurilor sterile de o anumita distanta
        else
            frunze[lungime[i]]++;   /// creste numarul frunzelor de o anumita distanta

    nfr=0;
    for(int i=1;i<=N;i++)
        if(frunze[i])  /// daca exista frunze la distanta i de radacina
        {
            nfr++;
            nrfrunze[nfr]=frunze[i];   /// numarul frunzelor la
            distanta[nfr]=i;           /// distanta data
        }

    nrsol[0]=1;
    for (int i=1;i<=maxadanc;i++)
        for (int j=1; j<=nfr; j++)
            if (distanta[j]<=i)
                nrsol[i]=(nrsol[i]+(nrsol[i-distanta[j]]*nrfrunze[j])%R)%R;        /// construiesc valorile initiale pana la maxadanc

    for (int i=maxadanc+1; i<=D; i++)
        for (int j=1; j<=nfr; j++)
            nrsol[i]=(nrsol[i]+(nrsol[i-distanta[j]]*nrfrunze[j])%R)%R;        /// construiesc valorile initiale pana la maxadanc

    for (int i=1;i<=N;i++) {
    	if (D - i >= 0)
        	nrsol[D]=(nrsol[D]+(nrsol[D-i]*sterile[i])%R)%R;            /// copletam cu nodurile sterile
    }

    fout<<nrsol[D];
    return 0;
}
