/// Autor Lukacs Sandor
#include <bits/stdc++.h>
#define M 1000000007

using namespace std;

ifstream in("tairos.in");
ofstream out("tairos.out");

int n, d;
int a[101][101];//listele de adiacenta
int viz[101];//viz[i] = 1, daca nodul i a fost vizitat, 0 in caz contrat
int h;//inaltimea arborelui initial
long long f[100000], nf[100000];//f[i] - cate frunze exista la distanta i, nf[i] - cate nefrunze exista la distanta i
int dmin = 101;//nivelul minim pe care se gaseste o frunza in arborele initial.
long long sol[100002], solf[100002], solnf[100002];//sol[i] - cate noduri sunt la distanta i, solf[i] - cate frunze sunt la distanta i, solnf[i] - cate nefrunze sunt la dist i

void dfs(int k, int niv){
    viz[k] = 1;//marchez ca vizitat
    if(h < niv) h = niv;//actualizez inaltimea arborelui
    f[niv] = f[niv] + 1;//consider ca nodul k e frunza
    bool ok = false;
    for(int i = 1; i <= a[k][0]; i++){
        if(viz[a[k][i]] == 0){//daca exista noduri nevizitate
            ok = true;//retin ca nu e frunza
            dfs(a[k][i], niv + 1);
        }
    }
    if(ok == true){//daca a avut descendenti
        f[niv] = f[niv] - 1;//nu e frunza
        nf[niv] = nf[niv] + 1;//il trecem la nefrunze
    }
    else//daca e frunza
        if(niv < dmin) dmin = niv;//actualizez nivelul minim

}

int main()
{
    in >> n >> d;
    for(int i = 1; i < n; i++){
        int x, y;
        in >> x >> y;
        a[x][0] = a[x][0] + 1;
        a[x][a[x][0]] = y;//adaug muchia (x, y)

        a[y][0] = a[y][0] + 1;
        a[y][a[y][0]] = x;//adaug muchia (y, x)
    }

    dfs(1, 0);

    /*out << h << "\n" << dmin << "\n";
    for(int i = 0; i <= h; i++) out << "Nivel " << i <<", numar frunze: " << f[i] << ", numar nefrunze: " << nf[i] << "\n";*/

    if(dmin == h and f[h] == 1 and nf[h] == 0){//daca avem un arbore lant
        out << 1;
        return 0;
    }

    if(h == 1){//daca avem un arbore stea fr^d
        long long ans = 1;
        for(int i = 1; i <= d; i++)
            ans = (ans * f[h]) % M;
        out << ans;
        return 0;
    }
    if(dmin == h){//daca toate frunzele sunt la acelasi nivel
        for(int i = 0; i <= h; i++) sol[i] = f[i] + nf[i];
        long long p = f[h];
        for(int i = h + 1; i <= d; i++) sol[i] = (p * sol[i - h]) % M;

        out << sol[d];
        return 0;
    }


        for(int i = 0; i <= h; i++) {
                solf[i] = f[i]; solnf[i] = nf[i];
        }

        for(int i = dmin; i <= d; i++){ //construiesc arborele cu radacina in frunzele de la distanta i
            for(int j = 1; j <= h; j++){
                solf[i + j] = (solf[i + j] + (solf[i] * f[j]) % M) % M;//daca in arborele initial aveam frunze la distanta j in noul arbore voi avea frunze la dist i + j
                solnf[i + j] = (solnf[i + j] + (solf[i] * nf[j]) % M) % M;//nefrunzele
            }
            solnf[i] = (solnf[i] + solf[i]) % M;
            solf[i] = 0;
        }

        out << (solf[d] + solnf[d]) % M;

    return 0;
}
