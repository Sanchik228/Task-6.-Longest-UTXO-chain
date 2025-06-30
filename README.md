# Task #6. Longest UTXO chain

This project implements an algorithm for finding the longest UTXO chain in a simplified Bitcoin-like transaction dataset.  
It operates on transactions with exactly one input and two outputs, and reconstructs how coins flow through the network.

Task Description:
Bitcoin is a UTXO-based system (Unspent Transaction Output). 
This means that coins are tracked by their transaction outputs rather than by account balances.  
The goal is to:
- Load transactions from a CSV file
- Filter only valid transactions: one input and two outputs
- Build a UTXO dependency graph
- Find the longest chain of transactions where each one spends the output of the previous
- Output this chain and its length

How to start:
1. Enter the necessary data into the file “transactions.csv” in the following format: txid, input txid: output index, output1 id, output2 id. 
Or use the data that is already in this file.
2. Compile and run the proram.

Algorithm Explanation:
The UTXO graph is a directed acyclic graph (DAG).
To find the longest chain:
We build a dependency graph: each transaction points to its "children"
Apply DFS with memoization to compute the longest path starting from each node
Track the actual path using a next[] array
Time complexity: O(N + E)
Where N — number of transactions, E — number of edges (dependencies)