#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

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

int main() {
    std::string filename = "transactions.csv";
    std::vector<Transaction> txs = readFromCSV(filename);
    std::cout << "Readed transactions: " << txs.size() << std::endl;
    return 0;
}