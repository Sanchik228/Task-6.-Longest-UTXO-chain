#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>

struct Transaction {
    std::string txID;
    std::string input_txID;
    int input_or_output_id;
    std::vector<std::string> outputs;

    Transaction(const std::string& txID_,
        const std::string& input_txID_,
        int input_or_output_id_,
        const std::string out1,
        const std::string out2)
        : txID(txID_), input_txID(input_txID_), input_or_output_id(input_or_output_id_),
        outputs{ out1, out2 } {}
    std::string getInputID() const {
        return input_txID + ":" + std::to_string(input_or_output_id);
    }
};

std::vector<Transaction> readFromCSV(const std::string& f) {
    std::vector<Transaction> transactios;
    std::ifstream file(f);

    if (!file.is_open()) {
        std::cerr << "File could not be opened: " << f << "\n";
        return transactios;
    }
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string txID, input_field, out1, out2;

        if (!std::getline(ss, txID, ',')) continue;
        if (!std::getline(ss, input_field, ',')) continue;
        if (!std::getline(ss, out1, ',')) continue;
        if (!std::getline(ss, out2, ',')) continue;

        size_t colon_pos = input_field.find(':');
        if (colon_pos == std::string::npos) continue;

        std::string input_txID = input_field.substr(0, colon_pos);
        int input_output_ID = std::stoi(input_field.substr(colon_pos + 1));

        transactios.emplace_back(txID, input_txID, input_output_ID, out1, out2);
    }
    return transactios;
}

int dfs(int node,
    const std::vector<std::vector<int>>& children,
    std::vector<int>& memo,
    std::vector<int>& next) {
    if (memo[node] != -1) return memo[node];

    int max_depth = 1;
    int best_child = -1;

    for (int child : children[node]) {
        int d = dfs(child, children, memo, next);
        if (1 + d > max_depth) {
            max_depth = 1 + d;
            best_child = child;
        }
    }

    next[node] = best_child;
    return memo[node] = max_depth;
}

int main() {
    auto txs = readFromCSV("transactions.csv");
    int n = txs.size();
    std::cout << "Loaded " << n << " transactions\n";

    std::unordered_map<std::string, int> utxo_ID;
    utxo_ID.reserve(n * 2);
    for (int i = 0; i < n; i++) {
        utxo_ID[txs[i].txID + ":0"] = i;
        utxo_ID[txs[i].txID + ":1"] = i;
    }

    std::vector<std::vector<int>> children(n);
    for (int j = 0; j < n; j++) {
        std::string in_id = txs[j].getInputID();
        auto it = utxo_ID.find(in_id);
        if (it != utxo_ID.end()) {
            children[it->second].push_back(j);
        }
    }

    for (int i = 0; i < n; i++) {
        std::cout << txs[i].txID << " -> ";
        for (int c : children[i])
            std::cout << txs[c].txID << " ";
        std::cout << "\n";
    }

    std::vector<int> memo(n, -1);
    std::vector<int> next(n, -1);

    int longest = 0;
    int start_index = -1;

    for (int i = 0; i < n; ++i) {
        int depth = dfs(i, children, memo, next);
        if (depth > longest) {
            longest = depth;
            start_index = i;
        }
    }

    std::cout << "\nLongest UTXO chain starts from " << txs[start_index].txID
        << " and has length " << longest << "\n";

    std::cout << "Chain: ";
    int current = start_index;
    while (current != -1) {
        std::cout << txs[current].txID;
        current = next[current];
        if (current != -1) std::cout << " -> ";
    }
    std::cout << std::endl;

    return 0;
}