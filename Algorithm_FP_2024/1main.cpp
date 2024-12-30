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
int cost(const map<int, set<int> > &team_servers) {
    int total_cost = 0;
    for (const auto &entry : team_servers) {
        int num_servers = entry.second.size();
        if (num_servers > 1) {
            //cout<< "team id: " << entry.first << " num_servers: " << num_servers << endl;
            total_cost += (num_servers - 1) * (num_servers - 1);
        }
    }

    return total_cost;
}
signed main(){
    ifstream inputFile("./input.txt");
    if (!inputFile) {
        cerr << "Unable to open file";
        exit(1);
    }

    int memort_limit;
    inputFile >> memort_limit;
    
    string s;
    inputFile >> s;
    vector<agent> agents;
    int n;
    inputFile >> n;
    for (int i = 0; i < n; ++i) {
        agent a;
        a.id = i;
        inputFile >> a.mem;
        agents.push_back(a);
    }
    inputFile >> s;
    if (s == ".team") {
        int m;
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
    sort(agents.begin(), agents.end(), cmp3);
    int k = 1;
    vector<server> servers;
    server srv;
    srv.id = 0;
    srv.mem = 0;
    servers.push_back(srv);

    map<int, set<int> > team_servers;
    int i = 0;
    for(i=0;i<n;i++){
        bool flag = true;
        for(int j=0;j<k;j++){
            if(agents[i].mem + servers[j].mem <= memort_limit){
                servers[j].mem += agents[i].mem;
                servers[j].agents.insert(agents[i].id);
                agents[i].serverid = j;
                for (int teamId : agents[i].teamid) {
                    team_servers[teamId].insert(j);
                }
                flag = false;
                break;
            }
            
        }
        if(flag){
            server s;
            s.id = k;
            s.mem = agents[i].mem;
            s.agents.insert(agents[i].id);
            agents[i].serverid = k;
            for (int teamId : agents[i].teamid) {
                team_servers[teamId].insert(k);
            }
            servers.push_back(s);
            k++;
        }
    }


    cout << cost(team_servers) << endl;
    cout << servers.size() << endl;
    for(int i=0;i<n;i++){
        //cout << agents[i].serverid << endl;
    }
    inputFile.close();
}