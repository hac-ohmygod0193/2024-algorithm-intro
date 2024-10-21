#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
vector<int> merge(vector<int> left,vector<int> right){
    vector<int> result;
    int i=0,j=0;
    while(i<left.size() and j<right.size()){
        if(left[i]<=right[j]){
            result.push_back(left[i]);
            i+=1;
        }
        else{
            result.push_back(right[j]);
            j+=1;
        }
    }
    while(i<left.size()){
        result.push_back(left[i]);
        i+=1;
    }
    while(j<right.size()){
        result.push_back(right[j]);
        j+=1;
    }
    return result;
}
vector<int> merge_sort(vector<int> v){
    if(v.size()<=1) return v;
    int mid = v.size()/2;
    vector<int> left;
    vector<int> right;
    for(int i=0;i<mid;i++) left.push_back(v[i]);
    for(int i=mid;i<v.size();i++) right.push_back(v[i]);
    vector<int> sorted_left = merge_sort(left);
    vector<int> sorted_right = merge_sort(right);
    return merge(sorted_left,sorted_right);
}

int Sorting(vector<int> tokens, int power) {
    int score=0;
    int n = tokens.size();
    int a=0, b=n-1;
    bool stop=true;
    while(a<=b){
        stop=true;
        if(power>=tokens[a]){
            score+=1;
            power-=tokens[a];
            tokens[a]=-1;
            a+=1;
            stop=false;
        }
        else if(score>0 and tokens[a]<tokens[b]){
            power+=(tokens[b]-tokens[a]);
            a+=1;
            b-=1;
            stop=false;
        }
        if(stop) break;
    }
    return score;
}
int main(){
    int power;
    cin>>power;
    vector<int> tokens;
    int token;
    while (cin>>token)
    {
        if(token==-1) break;
        tokens.push_back(token);
    }
    cout<<Sorting(merge_sort(tokens),power)<<endl;
    return 0;
}