/// student Alexandru Turdean  
/// Universitatea Tehnica din Cluj-Napoca

#include<fstream>
#include<vector>

using namespace std;
ifstream fin("ateleport.in");
ofstream fout("ateleport.out");

struct muchie{
    int nod;
    int timp;
};

struct element{
    int nod;
    int timp;
    bool operator<(const element &x) const
    {
        return timp < x.timp;
    }
};

struct min_heap{ /// pentru usurinta se poate folosi priority_queue
    element v[1000001];
    int size = 0;

    element top()
    {
        return v[1];
    }

    void push(element el)
    {
        size++;
        v[size] = el;
        int pos = size;
        while(pos != 1)
            if(v[pos] < v[pos/2])
                swap(v[pos/2],v[pos]), pos/=2;
            else
                break;
    }

    void pop()
    {
        if(size == 0)
            return;
        swap(v[1],v[size]);
        size--;
        int pos = 1;
        while(pos*2+1 <= size)
        {

            if(v[pos*2] < v[pos] && v[pos*2] < v[pos*2+1])
                swap(v[pos*2], v[pos]), pos = pos*2;
            else if(v[pos*2+1] < v[pos])
                swap(v[pos*2+1], v[pos]), pos = pos*2+1;
            else
                break;
        }
        if(pos*2 == size && v[pos*2] < v[pos])
            swap(v[pos*2], v[pos]);
    }

};

int n,m,p,l,k;
int timp_minim[10001]; /// timp_minim[x] = timpul minim pentru a ajunge in nodul x (nu ne putem teleporta)
vector<muchie> muchii[10001];
min_heap myHeap;

int main()
{
    fin >> n >> m >> p >> l >> k;
    for(int i = 1; i <= m; i++)
    {
        int x,y,t;
        fin >> x >> y >> t;
        muchii[x].push_back(muchie{y,t});
        muchii[y].push_back(muchie{x,t});
    }

    for(int i = 1; i <= n; i++)
        timp_minim[i] = 2000000000;

    timp_minim[1] = 0;
    myHeap.push(element{1,0});
    while(myHeap.size > 0)
    {
        element x;
        x = myHeap.top();
        myHeap.pop();

        if(timp_minim[x.nod] < x.timp)
            continue; /// am ajuns intr-un nod in care am mai fost cu un timp mai mic

        if(x.nod == n)
        {
            fout << x.timp;
            return 0;
        }

        for(int i = 0; i < muchii[x.nod].size(); i++)
        {
            element y;
            y.nod = muchii[x.nod][i].nod;
            y.timp = x.timp + muchii[x.nod][i].timp;
            if(timp_minim[y.nod] > y.timp)
            {
                timp_minim[y.nod] = y.timp;
                myHeap.push(y);
            }
        }
    }
    return 0;
}
