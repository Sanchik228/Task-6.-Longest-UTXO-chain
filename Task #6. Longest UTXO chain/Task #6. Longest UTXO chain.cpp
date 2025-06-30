#include <iostream>
#include <string>
#include <vector>

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
        outputs{ out2, out2 } {}
    std::string getInputID() const {
        return input_txID + ":" + std::to_string(input_or_output_id);
    }
};

int main() {
    std::cout << "Hello World!\n";
}