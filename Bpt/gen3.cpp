#include <algorithm>
#include<bits/stdc++.h>
using namespace std;
int T=1;
int a[100010];
void data1(){
    cout<<T<<endl;
    for(int i=1;i<=T;i++)
        a[i]=i;
    random_shuffle(a+1,a+T+1);
    for(int i=1;i<=T;i++)
        cout<<"insert "<<i<<" "<<i<<endl;
}//only insert to check structure
void data2(){
    cout<<T<<endl;
    // for(int i=1;i<=T;i++)
    //     cout<<"insert "<<i<<" "<<i<<endl;
    // for(int i=1;i<=T;i++)
    //     cout<<"find "<<i<<endl;
    // for(int i=1;i<=T;i++)
    //     cout<<"insert "<<i<<" "<<i+T<<endl;
    for(int i=1;i<=T;i++)
        cout<<"find "<<i<<endl;
}//regularly insert and find
map<pair<int,int>,bool>vis;
void data3(){
    cout<<100*T<<endl;
    for(int i=1;i<=100*T;i++){
        int op=rand()%2;
        if(i<=50)
            op=1;
        if(op){
            int ind=rand()%100+1,val=rand()%100+1;
            while(vis[make_pair(ind,val)])
                ind=rand()%100+1,val=rand()%100+1;
            vis[make_pair(ind,val)]=1;
            cout<<"insert "<<ind<<" "<<val<<endl;
        }
        else{
            int ind=rand()%100+1;
            cout<<"find "<<ind<<endl;
        }
    }
}//randomly insert and find
void data4(){
    cout<<T<<endl;
    for(int i=1;i<=T;i++)
        cout<<"delete "<<i<<" "<<i<<endl;
}//only insert and delete to check the structure
void data7(){
    cout<<4*T<<endl;
    for(int i=1;i<=2*T;i++)
        cout<<"insert "<<2*T-i+1<<" "<<2*T-i+1<<endl;
    for(int i=1;i<=2*T;i++)
        cout<<"delete "<<2*T-i+1<<" "<<2*T-i+1<<endl;
}
void data5(){
    cout<<7*T<<endl;
    for(int i=1;i<=2*T;i++)
        cout<<"insert "<<i<<" "<<i<<endl;
    for(int i=1;i<=2*T;i++)
        cout<<"find "<<i<<endl;
    for(int i=T+1;i<=2*T;i++)
        cout<<"delete "<<i<<" "<<i<<endl;
    for(int i=1;i<=2*T;i++)
        cout<<"find "<<i<<endl;
}//regularly insert,delete,find
void data6(){
    cout<<100*T<<endl;
    for(int i=1;i<=100*T;i++){
        int op=rand()%4;
        if(i<=50)
            op=1;
        if(op==1){
            int ind=rand()%100+1,val=rand()%100+1;
            while(vis[make_pair(ind,val)])
                ind=rand()%100+1,val=rand()%100+1;
            vis[make_pair(ind,val)]=1;
            cout<<"insert "<<ind<<" "<<val<<endl;
        }
        if(op==2){
            int ind=rand()%100+1;
            cout<<"find "<<ind<<endl;
        }
        if(op==3){//delete existed element
            int ind=rand()%100+1,val=rand()%100+1;
            while(!vis[make_pair(ind,val)])
                ind=rand()%100+1,val=rand()%100+1;
            vis[make_pair(ind,val)]=0;
            cout<<"delete "<<ind<<" "<<val<<endl;
        }
        if(op==0){//possibly delete non-existed element
            int ind=rand()%100+1,val=rand()%100+1;
            vis[make_pair(ind,val)]=0;
            cout<<"delete "<<ind<<" "<<val<<endl;
        }
    }
}//randomly insert,delete,find

int main(){
    //srand(time(0));
    freopen("input.txt","w",stdout);
     data4();return 0;
    int a[1005];
    for(int i=0;i<=100; ++i)
        a[i] = i;
    std::random_shuffle(a,a+101);
    std::cout << T << std::endl;
    for(int i = 0;i <= 100;++i)
        std::cout<<a[i]<<std::endl;
    return 0;
}