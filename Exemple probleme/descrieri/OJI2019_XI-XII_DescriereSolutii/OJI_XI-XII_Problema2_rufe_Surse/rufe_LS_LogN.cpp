/// autor Lukacs Sandor
#include <bits/stdc++.h>

using namespace std;

ifstream in("rufe.in");
ofstream out("rufe.out");

long long n, m, k, a, b;

long long pozitii_libere(long long a, long long b, long long d){//numarul de pozitii libere daca ultimul tricou s-a asezat la d
    long long total = 2 * d * (d - 1)+1;//numarul total de pozitii aflate la distantele 1, 2, ... d - 1, nu toate se gasesc pe uscator
    long long poz;
    //scadem ce este in plus in sus
    if(d > a) {
        poz = d - a;//triunghi isoscel cu inaltimea poz
        total = total - poz * (poz + 1) + poz;//de 2 ori suma gauss pana la poz, din care scadem poz, deoarece apare de 2 ori
    }
    //in jos
    if(d > n - a + 1){
        poz = d - (n - a + 1);
        total = total - poz * (poz + 1) + poz;
    }
    //stanga
    if(d > b){
        poz = d - b;
        total = total - poz * (poz + 1) + poz;
    }
    //dreapta
    if(d > m - b + 1){
        poz = d - (m - b + 1);
        total = total - poz * (poz + 1) + poz;
    }
    //adaugam ceea ce am scazut de 2 ori
    //stanga - sus
    if(d > a + b - 1){
        poz = d - (a + b - 1) - 1;
        total = total + poz * (poz + 1) / 2;
    }
    //dreapta - sus
    if(d > a +  m - b){
        poz = d - (a + m - b) - 1;
        total = total + poz * (poz + 1) / 2;
    }
    //stanga - jos
    if(d > n - a + b){
        poz = d - (n - a + b) - 1;
        total = total + poz * (poz + 1) / 2;
    }
    //dreapta jos
    if(d > n - a + m - b + 1){
        poz = d - (n - a + m - b + 1) - 1;
        total = total + poz * (poz + 1) / 2;
    }

    return total;
}

int main()
{
    in >> n >> m >> a >> b >> k;
    if(k == 1) out << max(max(a - 1 + b - 1, a - 1 + m - b), max(n - a + b - 1, n - a + m - b));
    else if(k >= n * m - 4) out << 1;
    else{
        long long l = 1, r = n + m, mid, sol;
        bool stop = false;
        while(l <= r and stop == false){
            mid = l + (r - l) / 2;//distanta la care se aseaza ultimul tricou
            if(n * m - pozitii_libere(a, b, mid) >= k){//daca a pus la uscat mai mult de k tricouri cand ultimul tricou e la dist mid
                if(n * m - pozitii_libere(a, b, mid + 1) < k or mid == r){//daca ultimul tricou e cu o unitate mai departe de sosete, dar asa nu incap k tricouri
                    sol = mid; stop = true;
                }
                else l = mid + 1;//punem tricouri la distanta mai mare
            }
            else r = mid - 1;//tricouri mai aproape de sosete
        }
        out << sol;
    }
    return 0;
}
