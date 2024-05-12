#include<bits/stdc++.h>
using namespace std;
#define fr(i,a,b) for(int i=a;i<=b;++i)
#define rf(i,a,b) for(int i=a;i>=b;--i)
typedef long long ll;
template<typename T>
inline void read(T &X){
    X=0; bool fh = 0; char c=getchar();
    while(!isdigit(c)) fh |= (c=='-'), c=getchar();
    while(isdigit(c)) X = (X<<1) + (X<<3) + (c^48), c = getchar();
    X = (fh? -X : X);
}
int main(){
    #ifdef OJ
    freopen("input.txt","r",stdin);
    #endif
    remove("disk"), remove("Recycle"),remove("block"),remove("map"),  remove("root");
    return 0;
}