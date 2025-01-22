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
    long long id;
    long long mem;
    long long serverid=-1;
    set<long long> teamid;
};
struct merged_agent{
    long long id;
    long long mem;
    set<long long> teamid;
    
    long long size;
    set<long long> childs;
};
struct merged_pair{
    long long a;
    long long b;
    long long common_count;
    long long reduce_cost;
};
struct server{
    long long id;
    long long mem;
    set<long long> agents;
};
struct answer{
    bool hasanswer=false;
    long long k;
    long long total_cost;
    vector<long long> agent_serverids;
};
bool cmp(const agent &a, const agent &b){
    return a.mem > b.mem;
}
bool cmp2(const agent &a, const agent &b){
    return a.id < b.id;
}
bool compress_cmp1(const merged_pair &a, const merged_pair &b) {
    if(a.common_count == b.common_count){
        return a.reduce_cost > b.reduce_cost;
    }
    return a.common_count > b.common_count;
}
bool compress_cmp2(const merged_pair &a, const merged_pair &b) {
    if(a.reduce_cost == b.reduce_cost){
        return a.common_count > b.common_count;
    }
    return a.reduce_cost > b.reduce_cost;
}
long long common(const set<long long> &a, const set<long long> &b) {
    long long count = 0;
    //cout<<a.size()<<' '<<b.size()<<endl;
    for (long long x : a) {
        //cout<< "x: " << x << endl;
        if (b.find(x) != b.end()) {
            count++;
        }
    }
    return count;
}
long long reduce(const set<long long> &a, const set<long long> &b, const vector<long long> &team_size) {
    //cout<<a.size()<<' '<<b.size()<<endl;
    long long reduce_cost = 0;
    for (long long x : a) {
        //cout<< "x: " << x << endl;
        if (b.find(x) != b.end()) {
            // 4->3, 3^2-2^2=5=2*(4-1)-1
            reduce_cost += (team_size[x]-1)*2-1;
        }
    }
    return reduce_cost;

}

double st;
bool flag=false;
long long timelimit = 7000;
vector<merged_agent> compress(vector<merged_agent> &original_agents, long long memory_limit, vector<long long> &team_size, long long mode){
    long long n = original_agents.size();
    vector<merged_pair> merged_pairs;
    for(long long i=0;i<n;i++){
        for(long long j=i+1;j<n;j++){
            long long common_count = common(original_agents[i].teamid, original_agents[j].teamid);
            long long reduce_cost = reduce(original_agents[i].teamid, original_agents[j].teamid, team_size);
            long long total_memory = original_agents[i].mem + original_agents[j].mem;
            if(total_memory > memory_limit){
                continue;
            }
            merged_pair mp;
            mp.a = original_agents[i].id;
            mp.b = original_agents[j].id;
            mp.common_count = common_count;
            merged_pairs.push_back(mp);
        }
        if(((clock()-st)/CLOCKS_PER_SEC)>timelimit){
            flag = true;
            return original_agents;
        }
    }
    vector<merged_agent> merged_agents;
    if(merged_pairs.size()==0){
        return original_agents;
    }
    vector<long long > merged;
    merged.resize(original_agents.size(), 0);
    if(mode==2){
        sort(merged_pairs.begin(), merged_pairs.end(), compress_cmp2);
    }
    else{
        sort(merged_pairs.begin(), merged_pairs.end(), compress_cmp1);
    }
    long long max_count = merged_pairs[0].common_count;
    //cout<<"max_count="<<max_count<<endl;
    long long cnt = 0;
    long long len = original_agents.size();
    for(auto &mp : merged_pairs){
        if(((clock()-st)/CLOCKS_PER_SEC)>timelimit){
            flag = true;
            return original_agents;
        }
        if(merged[mp.a] || merged[mp.b]){
            continue;
        }
        if(mp.common_count < max_count/2 and cnt > len/10 and len<50000 and mode==3){
            continue;
        }
        cnt++;
        if(original_agents[mp.a].mem + original_agents[mp.b].mem > memory_limit){
            continue;
        }
        
        merged[mp.a] = 1;
        merged[mp.b] = 1;
        merged_agent ma;
        ma.id = merged_agents.size();
        ma.size = 2;
        ma.mem = original_agents[mp.a].mem + original_agents[mp.b].mem;
        ma.teamid = original_agents[mp.a].teamid;
        ma.teamid.insert(original_agents[mp.a].teamid.begin(), original_agents[mp.a].teamid.end());
        ma.teamid.insert(original_agents[mp.b].teamid.begin(), original_agents[mp.b].teamid.end());
        ma.childs.insert(original_agents[mp.a].childs.begin(), original_agents[mp.a].childs.end());
        ma.childs.insert(original_agents[mp.b].childs.begin(), original_agents[mp.b].childs.end());
        for (long long x : original_agents[mp.a].childs) {
            //cout<< "x: " << x << endl;
            if (original_agents[mp.b].childs.find(x) != original_agents[mp.b].childs.end()) {
                // 4->3, 3^2-2^2=5=2*(4-1)-1
                team_size[x]-=1;
            }
        }
        merged_agents.push_back(ma);
    }
    for(long long i=0;i<original_agents.size();i++){
        if(merged[i]){
            continue;
        }
        merged_agent ma;
        ma.id = merged_agents.size();
        ma.size = 2;
        ma.mem = original_agents[i].mem;
        ma.teamid = original_agents[i].teamid;
        ma.childs.insert(original_agents[i].childs.begin(), original_agents[i].childs.end());
        merged_agents.push_back(ma);
    }
    return merged_agents;
}

long long cost(const map<long long , set<long long > > &team_servers) {
    long long total_cost = 0;
    for (const auto &entry : team_servers) {
        long long num_servers = entry.second.size();
        if (num_servers >0) {
            //cout<< "team id: " << entry.first << " num_servers: " << num_servers << endl;
            total_cost += (num_servers - 1) * (num_servers - 1);
        }
    }

    return total_cost;
}



void solve(answer &ans, vector<agent> agents, vector<merged_agent> clean_agents, long long memory_limit, vector<long long> team_size, long long mode){
    long long k = clean_agents.size();
    long long total_cost = 0;
    map<long long , set<long long > > team_servers;
    while(true){
        if(((clock()-st)/CLOCKS_PER_SEC)>timelimit){
            flag = true;
            break;
        }
        vector<merged_agent> merged_agents = compress(clean_agents, memory_limit, team_size, mode);
        clean_agents = merged_agents;
        if(merged_agents.size()==k){
            for(long long i=0;i<merged_agents.size();i++){
                long long serverid = merged_agents[i].id;
                for(auto it=merged_agents[i].childs.begin();it!=merged_agents[i].childs.end();it++){
                    agents[*it].serverid = serverid;
                    for(long long x : agents[*it].teamid){
                        team_servers[x].insert(serverid);
                    }
                }
            }
            total_cost = cost(team_servers);
            //cout<<"----------------"<<endl;
            //cout<<"mode="<<mode<<endl;
            //cout<<"total_cost="<<total_cost<<endl;
            //cout<<"k="<<k<<endl;
            //cout<<"----------------"<<endl;
            if(ans.hasanswer==false){
                ans.hasanswer = true;
                ans.k = k;
                ans.total_cost = total_cost;
                for(long long i=0;i<agents.size();i++){
                    ans.agent_serverids.push_back(agents[i].serverid);
                }
            }
            else if(ans.total_cost>total_cost){
                if(ans.k==2 and k>2) break;
                ans.k = k;
                ans.total_cost = total_cost;
                for(long long i=0;i<agents.size();i++){
                    ans.agent_serverids[i] = agents[i].serverid;
                }
            }
            break;
        }
        k = merged_agents.size();
    }
    
}
void greedy(answer &ans, vector<agent> agents, long long memory_limit){
    long long k = 1;
    vector<server> servers;
    server srv;
    srv.id = 0;
    srv.mem = 0;
    servers.push_back(srv);
    long long n = agents.size();
    map<long long , set<long long > > team_servers;
    long long i = 0;
    for(i=0;i<n;i++){
        bool f = true;
        for(long long j=0;j<k;j++){
            if(agents[i].mem + servers[j].mem <= memory_limit){
                servers[j].mem += agents[i].mem;
                servers[j].agents.insert(agents[i].id);
                agents[i].serverid = j;
                for (long long teamId : agents[i].teamid) {
                    team_servers[teamId].insert(j);
                }
                f = false;
                break;
            }
            
        }
        if(f){
            server s;
            s.id = k;
            s.mem = agents[i].mem;
            s.agents.insert(agents[i].id);
            agents[i].serverid = k;
            for (long long teamId : agents[i].teamid) {
                team_servers[teamId].insert(k);
            }
            servers.push_back(s);
            k++;
        }
    }
    long long total_cost = cost(team_servers);
    //cout<<"----------------"<<endl;
    //cout<<"mode=greedy"<<endl;
    //cout<<"total_cost="<<total_cost<<endl;
    //cout<<"k="<<servers.size()<<endl;
    //cout<<"----------------"<<endl;
    if(ans.hasanswer==false){
        ans.hasanswer = true;
        ans.k = servers.size();
        ans.total_cost = total_cost;
        for(long long i=0;i<agents.size();i++){
            ans.agent_serverids.push_back(agents[i].serverid);
        }
    }
    else if(ans.total_cost>total_cost){
        if(ans.k==2 and servers.size()>2) return;
        ans.k = servers.size();
        ans.total_cost = total_cost;
        for(long long i=0;i<agents.size();i++){
            ans.agent_serverids[i] = agents[i].serverid;
        }
    }
}

int main(int argc, char* argv[]){
    st = clock();
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return 1;
    }
    ifstream inputFile(argv[1]);
    if (!inputFile) {
        cerr << "Unable to open input file" << endl;
        return 1;
    }
    ofstream outputFile(argv[2]);
    if (!outputFile) {
        cerr << "Unable to open output file" << endl;
        return 1;
    }
    if (!inputFile) {
        cerr << "Unable to open file";
        exit(1);
    }

    long long memory_limit;
    inputFile >> memory_limit;
    
    string s;
    inputFile >> s;
    vector<agent> agents;
    vector<long long > best_agents;
    vector<merged_pair> merged_pairs;
    vector<merged_agent> original_agents;
    long long n;
    inputFile >> n;
    for (long long i = 0; i < n; ++i) {
        agent a;
        a.id = i;
        inputFile >> a.mem;
        agents.push_back(a);
        merged_agent ma;
        ma.id = i;
        ma.size = 1;
        ma.mem = a.mem;
        ma.childs.insert(i);
        original_agents.push_back(ma);
    }
    inputFile >> s;
    vector<long long > original_team_size;
    if (s == ".team") {
        long long m;
        inputFile >> m;
        for (long long i = 0; i < m; ++i) {
            long long teamSize;
            inputFile >> teamSize;
            original_team_size.push_back(teamSize);
            for (long long j = 0; j < teamSize; ++j) {
                long long agentId;
                inputFile >> agentId;
                agents[agentId].teamid.insert(i);
                original_agents[agentId].teamid.insert(i);
            }
        }
    }
    long long best_k, best_total_cost;
    long long k=n;
    long long total_cost = 0;
    long long mode;
    answer ans;
    greedy(ans,agents,memory_limit);
    for(long long mode=1;mode<=3;mode++){
        if(flag) break;
        solve(ans, agents, original_agents, memory_limit, original_team_size, mode);
    }
    
    if (!outputFile) {
        cerr << "Unable to open file";
        exit(1);
    }
    outputFile<<ans.total_cost<<endl;
    outputFile<<ans.k<<endl;
    for(long long i=0;i<ans.agent_serverids.size();i++){
        outputFile<<ans.agent_serverids[i]<<endl;
    }
    outputFile.close();
    inputFile.close();
    //cout<<"time="<<((clock()-st)/CLOCKS_PER_SEC)/60<<endl;
}
