/// prof. Ovidiu Rosca 
/// Colegiul National ”Dragos Vodă” Sighetu Marmatiei
#include <fstream>

using namespace std;

ifstream fin("partit.in");
ofstream fout("partit.out");
int cerinta, n, p, lim, a[10001];
unsigned long long int k, kn, m;

int main () {
  fin >> cerinta >> n;
  if (cerinta == 1) {
    for (int i = n; i >= 63; i--)
      fout << "1 "; // In prima parte pot fi multi de 1.
    fin >> k;
    if (n >= 62)
      lim = 62;
    else
      lim = n;
    kn = 2 * (k - 1); // Ca sa avem un bit == 0 la sfarsit.
    m = (1LL << (lim - 1)); // masca cu 1 bit.
    for (int i = lim, p = 1; i >= 1; i--, m >>= 1) {
      if ((m & kn) != 0)
        p++; // Mai facem un pas, cautand bit == 0.
      else {
        fout << p << ' '; p = 1;
      }
    }
    fout << endl;
  }
  else { // cerinta 2
    int i, b = -1, lim;
    bool mare = false;
    for (i = 1; fin >> a[i]; i++) {
      if (a[i] > 1 and not mare) {
        lim = i; mare = true; // Primul loc unde avem ceva > 1.
      }
    }
    i--;
    k = (1LL << (n - lim + 1)) - 1; // multi biti de 1
    for (int j = i; j >= lim; j--) {
      b += a[j]; k -= (1LL << b); // Resetam biti.
    }
    fout << k + 1; // Numaram de la 0.
  }
  return 0;
}
