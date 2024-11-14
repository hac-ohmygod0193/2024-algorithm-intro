#include<iostream>
#include<sstream>
#include<vector>
using namespace std;
signed main(){
    stringstream ss;
    string s;
    getline(cin,s);
    ss<<s;
    int n;
    vector<int> v;
    int dp[1002];
    while(ss>>n){
        v.push_back(n);
    }
    if(v.size()<=2){
        cout<<min(v[0],v[1])<<endl;
        return 0;
    }
    dp[0] = 0;
    dp[1] = 0;
    dp[2] = dp[1]+v[1];
    for(int i=3;i<v.size();i++){
        dp[i] = min(dp[i-1]+v[i-1],dp[i-3]+v[i-3]);
    }
    int ans = min(dp[v.size()-1]+v[v.size()-1],min(dp[v.size()-2]+v[v.size()-2], dp[v.size()-3]+v[v.size()-3]));
    cout<<ans<<endl;
}