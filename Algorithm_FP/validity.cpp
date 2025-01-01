#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <cassert>
#include <iomanip>
using namespace std;

class Validator {
private:
    int maxCapacity;
    vector<int> memories;         // agent記憶體需求
    vector<vector<int>> teams;    // team包含的agents
    
    // 輸出結果
    int reportedCost;            // 程式回報的總cost
    int serverCount;             // 使用的server數量
    vector<int> assignments;      // agent分配結果

    // 驗證基本格式
    bool validateBasicFormat(const string& outputFile) {
        ifstream outF(outputFile);
        if (!outF) {
            cout << "錯誤：無法開啟輸出檔案" << endl;
            return false;
        }

        // 讀取cost
        if (!(outF >> reportedCost)) {
            cout << "錯誤：無法讀取cost值" << endl;
            return false;
        }

        // 讀取server數量
        if (!(outF >> serverCount)) {
            cout << "錯誤：無法讀取server數量" << endl;
            return false;
        }

        // 檢查server數量的合理性
        if (serverCount < 2) {
            cout << "錯誤：server數量必須至少為2，目前為 " << serverCount << endl;
            return false;
        }

        // 讀取assignments
        assignments.resize(memories.size());
        for (size_t i = 0; i < memories.size(); i++) {
            if (!(outF >> assignments[i])) {
                cout << "錯誤：agent分配資訊不完整" << endl;
                return false;
            }
            if (assignments[i] < 0 || assignments[i] >= serverCount) {
                cout << "錯誤：server ID " << assignments[i] << " 超出範圍 [0, " 
                     << serverCount - 1 << "]" << endl;
                return false;
            }
        }

        // 確認沒有多餘的資料
        int temp;
        if (outF >> temp) {
            cout << "錯誤：輸出檔案包含多餘的資料" << endl;
            return false;
        }

        return true;
    }

    // 驗證記憶體限制
    bool validateMemoryConstraints() {
        vector<int> serverLoads(serverCount, 0);
        vector<vector<int>> serverAgents(serverCount);  // 記錄每個server上的agents
        
        for (size_t i = 0; i < memories.size(); i++) {
            int server = assignments[i];
            serverLoads[server] += memories[i];
            serverAgents[server].push_back(i);
            
            if (serverLoads[server] > maxCapacity) {
                cout << "錯誤：Server " << server << " 的記憶體使用 (" 
                     << serverLoads[server] << ") 超過限制 (" << maxCapacity << ")" << endl;
                cout << "分配到此server的agents: ";
                for (int agent : serverAgents[server]) {
                    cout << agent << "(" << memories[agent] << ") ";
                }
                cout << endl;
                return false;
            }
        }
        
        // 印出每個server的記憶體使用情況
        cout << "\n記憶體使用情況：" << endl;
        cout << setw(10) << "Server" << setw(15) << "使用量" << setw(15) 
             << "使用率" << setw(20) << "Agents數量" << endl;
        cout << string(60, '-') << endl;
        
        for (int i = 0; i < serverCount; i++) {
            double usage = serverLoads[i] * 100.0 / maxCapacity;
            cout << setw(10) << i 
                 << setw(15) << serverLoads[i] 
                 << setw(14) << fixed << setprecision(1) << usage << "%"
                 << setw(20) << serverAgents[i].size() << endl;
        }
        
        return true;
    }

    // 計算真實的cost並驗證team分布
    bool validateTeamDistribution() {
        int totalCost = 0;
        cout << "\nTeam分布情況：" << endl;
        cout << setw(8) << "Team" << setw(15) << "跨越servers" 
             << setw(12) << "Cost" << setw(20) << "Server分布" << endl;
        cout << string(55, '-') << endl;
        
        for (size_t i = 0; i < teams.size(); i++) {
            unordered_set<int> usedServers;
            for (int agentId : teams[i]) {
                usedServers.insert(assignments[agentId]);
            }
            
            int serversSpanned = usedServers.size();
            int teamCost = (serversSpanned - 1) * (serversSpanned - 1);
            totalCost += teamCost;
            
            // 顯示每個team的分布情況
            cout << setw(8) << i 
                 << setw(15) << serversSpanned 
                 << setw(12) << teamCost 
                 << setw(20);
            
            cout << "{";
            bool first = true;
            for (int server : usedServers) {
                if (!first) cout << ",";
                cout << server;
                first = false;
            }
            cout << "}" << endl;
        }
        
        cout << "\n總Cost: " << totalCost << endl;
        if (totalCost != reportedCost) {
            cout << "錯誤：報告的cost (" << reportedCost << ") 與實際計算的cost (" 
                 << totalCost << ") 不符" << endl;
            return false;
        }
        
        return true;
    }

public:
    bool loadInput(const string& inputFile) {
        ifstream inF(inputFile);
        if (!inF) {
            cout << "錯誤：無法開啟輸入檔案" << endl;
            return false;
        }

        // 讀取最大容量
        inF >> maxCapacity;

        string line;
        getline(inF, line);  // 讀取剩餘的換行符

        // 讀取.agent段落
        while (getline(inF, line)) {
            if (line == ".agent") break;
        }

        // 讀取agent數量和記憶體需求
        int n;
        inF >> n;
        memories.resize(n);
        for (int i = 0; i < n; i++) {
            inF >> memories[i];
        }

        // 讀取.team段落
        while (getline(inF, line)) {
            if (line == ".team") break;
        }

        // 讀取team資訊
        int m;
        inF >> m;
        teams.resize(m);
        for (int i = 0; i < m; i++) {
            int teamSize;
            inF >> teamSize;
            teams[i].resize(teamSize);
            for (int j = 0; j < teamSize; j++) {
                inF >> teams[i][j];
            }
        }

        return true;
    }

    bool validate(const string& outputFile) {
        cout << "\n開始驗證輸出結果..." << endl;
        cout << "===================" << endl;
        
        // 檢查基本格式
        if (!validateBasicFormat(outputFile)) {
            return false;
        }

        // 檢查記憶體限制
        if (!validateMemoryConstraints()) {
            return false;
        }

        // 檢查team分布和cost計算
        if (!validateTeamDistribution()) {
            return false;
        }
        
        // 印出驗證通過的資訊
        cout << "\n驗證結果：通過 ✓" << endl;
        cout << "- 總cost：" << reportedCost << endl;
        cout << "- 使用 " << serverCount << " 個servers" << endl;
        cout << "- 所有記憶體限制和team分布規則都符合要求" << endl;
        
        return true;
    }
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "用法: " << argv[0] << " <input_file> <output_file>" << endl;
        return 1;
    }

    Validator validator;
    
    // 載入輸入檔案
    if (!validator.loadInput(argv[1])) {
        return 1;
    }

    // 驗證輸出結果
    bool isValid = validator.validate(argv[2]);
    
    cout << "\n最終結論：" << (isValid ? "合格 ✓" : "不合格 ✗") << endl;

    return isValid ? 0 : 1;
}