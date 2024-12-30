#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
struct agent{
    int id;
    int mem;
    int serverid=-1;
    vector<int> teamid;
};
struct server{
    int id;
    int mem;
    set<int> agents;
};
bool cmp(const agent &a, const agent &b){
    return a.mem < b.mem;
}
bool cmp2(const agent &a, const agent &b){
    return a.id < b.id;
}
bool cmp3(const agent &a, const agent &b){
    return a.teamid.size() < b.teamid.size();
}
vector<agent> agents; // store every agent go to which server
vector<int> servers; // store every server's memory
int memory_limit;
int n,m ; // n = number of agents, m = number of teams
vector< map<int,int> > team_servers; // record agent number in each server in each team
vector<int> team_num_servers; // record number of different servers in each team
int min_cost = -1;
int best_k = 0;
vector<int> answer;
int total_cost = 0;
bool flag = false;
//long long Iter=0;
//long long maxIter = 1e8L*10; //1 min
double st;
void Writeans(){
    sort(agents.begin(),agents.end(),cmp2);
    ofstream outputFile("./output.txt");
    if (!outputFile) {
        cerr << "Unable to open file";
        exit(1);
    }
    outputFile << min_cost << ' ' << best_k << endl;
    for(int i=0;i<n;i++){
        outputFile << answer[i] << ' ';
    }
    outputFile << endl;
    outputFile.close();
    exit(0);
}
void dfs(int id){
    //Iter++;
    //cout<<"Iter: "<<Iter<<endl;
    if(((clock()-st)/CLOCKS_PER_SEC)>300){
        cout<<"Time out"<<endl;
        Writeans();
        return;
    }
    //cout<<"id: "<<id<<endl; 
    if(id==n){
        if(min_cost==-1 or total_cost<min_cost){
            cout<<"----------------------"<<endl;
            cout<<"cost: "<<total_cost<<endl;
            cout<<"server number: "<<servers.size()<<endl;
            cout<<"----------------------"<<endl;
            min_cost = total_cost;
            best_k = servers.size();
            cout<<"min cost: "<<min_cost<<' '<<best_k<<endl;
            
            for(int i=0;i<n;i++){
                answer[i] = agents[i].serverid;
                //cout<<i<<' '<<answer[i]<<endl;
            }
            /*
            for(int i=0;i<team_servers.size();i++){
                cout<<"team "<<i<<endl;
                cout<<"num_servers: "<<team_num_servers[i]<<endl;
                for(auto it=team_servers[i].begin();it!=team_servers[i].end();it++){
                    cout<<it->first<<' '<<it->second<<endl;
                }
            }
            */
            
        }
    }
    else{
        if(total_cost>=min_cost and min_cost!=-1)
            return;
        for(int i=0;i<servers.size();i++){
            int serverid = i;
            if(servers[i]>agents[id].mem){
                servers[i] -= agents[id].mem;
                agents[id].serverid = i;
                for(int j=0;j<agents[id].teamid.size();j++){
                    int teamid = agents[id].teamid[j];
                    if(team_servers[teamid][serverid]==0){
                        // num: 3->4, cost: (3*3-2*2)=5=(2*2+1)
                        total_cost += max((team_num_servers[teamid]-1)*2+1,0);
                        team_num_servers[teamid]+=1;
                    }
                    team_servers[teamid][serverid]+=1;
                }
                dfs(id+1);
                servers[i] += agents[id].mem;
                for(int j=0;j<agents[id].teamid.size();j++){
                    int teamid = agents[id].teamid[j];
                    if(team_servers[teamid][serverid]==1){ // if only one agent in this server
                        total_cost -= max((team_num_servers[teamid]-1)*2-1,0); 
                        // num: 4->3, cost: -(3*3-2*2)=-5=-(3*2-1)
                        team_num_servers[teamid]-=1;
                    }
                    team_servers[teamid][serverid]-=1;
                }
                agents[id].serverid = -1;
            }
        }
        if(servers.size()<best_k*1.1){
            servers.push_back(memory_limit-agents[id].mem);
            int serverid = servers.size()-1;
            agents[id].serverid = serverid;
            for(int j=0;j<agents[id].teamid.size();j++){
                int teamid = agents[id].teamid[j];
                if(team_servers[teamid][serverid]==0){
                    total_cost += max((team_num_servers[teamid]-1)*2+1,0);
                    team_num_servers[teamid]+=1;
                }
                team_servers[teamid][serverid]+=1;
            }
            dfs(id+1);
        }
    }
}
signed main(){
    ifstream inputFile("./input.txt");
    if (!inputFile) {
        cerr << "Unable to open file";
        exit(1);
    }
    st=clock();
    inputFile >> memory_limit;
    
    string s;
    inputFile >> s;
    
    inputFile >> n;
    for (int i = 0; i < n; ++i) {
        agent a;
        a.id = i;
        inputFile >> a.mem;
        agents.push_back(a);
        answer.push_back(-1);
    }
    
    inputFile >> s;
    if (s == ".team") {
        inputFile >> m;
        for (int i = 0; i < m; ++i) {
            int teamSize;
            inputFile >> teamSize;
            for (int j = 0; j < teamSize; ++j) {
                int agentId;
                inputFile >> agentId;
                agents[agentId].teamid.push_back(i);
            }
        }
    }
    team_servers.resize(m);
    team_num_servers.resize(m,0);
    best_k = n;
    sort(agents.begin(),agents.end(),cmp3);
    dfs(0);
    inputFile.close();
}