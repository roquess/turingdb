#include <chrono>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include "prefix-tree.h"
#include "benchmark.h"
#include "utils.h"

using HashingStringIndexer = std::unordered_map<std::string, std::vector<NodeID>>;

void dumpTree(StringApproximatorIndex* trie, const std::string& inPath) {
    std::fstream infile(inPath);
    std::string input;

    while (std::getline(infile, input)) {
        if (input == "Inputs" || input.empty()) continue;
        auto words = preprocess(input);
        for (const auto& word : words) trie->insert(word);
    }
    infile.close();
}

void dumpHash(HashingStringIndexer& hash, const std::string& inPath) {
    std::fstream infile(inPath);
    std::string input;

    NodeID id{0};
    while (std::getline(infile, input)) {
        if (input == "Inputs" || input.empty()) continue;
        auto words = preprocess(input);
        for (const auto& word : words) {
            hash[word].push_back(id);
        }
        id++;
    }
    infile.close();
}

QueryResults* queryHash(HashingStringIndexer& hash, const std::string& queryPath) {
    QueryResults* res = new QueryResults;
    
    std::fstream infile(queryPath);
    std::string line;
    while (std::getline(infile, line)) {
        if (line == "Query" || line.empty()) continue;
    
        if (line.front() == '"' && line.back() == '"') {
            line = line.substr(1, line.length() - 2);
        }
        std::vector<std::string> words = preprocess(line);
        for (const auto& word : words) {
            auto it = hash.find(word);
            if (it != std::end(hash)) res->finds += hash[word].size();
            else res->misses++;
        }
    }
    return res;
}

QueryResults* queryTree(StringApproximatorIndex* trie, const std::string& queryPath) {
    QueryResults* res = new QueryResults;
    
    std::fstream infile(queryPath);
    std::string line;
    while (std::getline(infile, line)) {
        if (line == "Query" || line.empty()) continue;
    
        if (line.front() == '"' && line.back() == '"') {
            line = line.substr(1, line.length() - 2);
        }
        std::vector<std::string> words = preprocess(line);
        for (const auto& word : words){
            using Trie = StringApproximatorIndex;
            auto it = trie->find(word);
            if (it._result == Trie::FOUND) {
                size_t numOwners = it._nodePtr->_owners->size();
                res->finds += numOwners;
            }
            else if (it._result == Trie::FOUND_PREFIX) res->partial_finds++;
            else res->misses++;
        }
    }
    infile.close();
    return res;
}

// Awfully slow brute force approach
QueryResults* naiveQuery(const std::string& inputPath, const std::string& queryPath) {
    std::fstream qFile(queryPath);
    std::fstream iFile(inputPath);

    QueryResults* res = new QueryResults;

    std::string qLine;
    while (std::getline(qFile, qLine)) {
        if (qLine == "Query" || qLine.empty()) continue;
    
        if (qLine.front() == '"' && qLine.back() == '"') {
            qLine = qLine.substr(1, qLine.length() - 2);
        }
        std::vector<std::string> queries = preprocess(qLine);
        std::string iLine;
        for (const auto& q : queries) {
            iFile.clear();
            iFile.seekg(0, std::ios::beg);
            bool found{false};
            while (std::getline(iFile, iLine)) {
                if (iLine == "Inputs" || iLine.empty()) continue;
                auto tokens = preprocess(iLine);
                auto it = std::find(std::begin(tokens), std::end(tokens), q);
                if (it != std::end(tokens)) {
                    res->finds++;
                    found |= true;
                }
            }
            if (!found) res->misses++;
        }
    }
    iFile.close();
    qFile.close();
    return res;
}

void benchmarkPrefix(const std::string& inputsPath, const std::string& queryPath) {
    using std::chrono::high_resolution_clock;

    auto tsIndexPrior = high_resolution_clock::now();
    auto trie = new StringApproximatorIndex();
    dumpTree(trie, inputsPath);
    auto tsIndexPost = high_resolution_clock::now();

    std::chrono::duration<double,std::milli> timeIndexCreation =
        tsIndexPost - tsIndexPrior;
    std::cout << "Prefix Tree Index Creation: " << timeIndexCreation.count() << "ms" << std::endl;

    auto tsQueryPrior = high_resolution_clock::now();
    QueryResults* results = queryTree(trie, queryPath);
    auto tsQueryPost = high_resolution_clock::now();

    std::chrono::duration<double,std::milli> timeQuery =
        tsQueryPost - tsQueryPrior;
    std::cout << "Prefix Tree Query: " << timeQuery.count() << "ms" << std::endl;
    std::cout << "with: " << std::endl;
    std::cout << results->finds << " complete matches" << std::endl;
    std::cout << results->partial_finds << " partial matches" << std::endl;
    std::cout << results->misses << " misses" << std::endl;
}


void benchmarkNaive(const std::string& inputsPath, const std::string& queryPath) {
    using std::chrono::high_resolution_clock;
    auto tsQueryPrior = high_resolution_clock::now();
    QueryResults* results = naiveQuery(inputsPath, queryPath);
    auto tsQueryPost = high_resolution_clock::now();

    std::chrono::duration<double,std::milli> timeQuery =
        tsQueryPost - tsQueryPrior;
    std::cout << "Naive Brute Force Query: " << timeQuery.count() << "ms" << std::endl;
    std::cout << "with: " << std::endl;
    std::cout << results->finds << " complete matches" << std::endl;
    std::cout << results->partial_finds << " partial matches" << std::endl;
    std::cout << results->misses << " misses" << std::endl;
}

void benchmarkHash(const std::string& inputsPath, const std::string& queryPath) {
    using std::chrono::high_resolution_clock;

    auto tsIndexPrior = high_resolution_clock::now();
    HashingStringIndexer index{};
    dumpHash(index, inputsPath);
    auto tsIndexPost = high_resolution_clock::now();

    std::chrono::duration<double,std::milli> timeIndexCreation =
        tsIndexPost - tsIndexPrior;
    std::cout << "Hashing Index Creation: " << timeIndexCreation.count() << "ms" << std::endl;

    auto tsQueryPrior = high_resolution_clock::now();
    QueryResults* results = queryHash(index, queryPath);
    auto tsQueryPost = high_resolution_clock::now();

    std::chrono::duration<double,std::milli> timeQuery =
        tsQueryPost - tsQueryPrior;
    std::cout << "Hashing index Query: " << timeQuery.count() << "ms" << std::endl;
    std::cout << "with: " << std::endl;
    std::cout << results->finds << " complete matches" << std::endl;
    std::cout << results->partial_finds << " partial matches" << std::endl;
    std::cout << results->misses << " misses" << std::endl;
}
