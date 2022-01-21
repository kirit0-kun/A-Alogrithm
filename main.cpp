#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <sstream>
#include <fstream>
#include "include/utils.h"

typedef unsigned short element_type;

enum State: element_type {
    kEmpty = 0,
    kObstacle = 1
};

inline std::string cellString(const State& state) {
    switch(state) {
    case ::kEmpty:
        return "0  ";
    case ::kObstacle:
        return "⛰️ ";
    }
    return "";
}

typedef std::vector<std::vector<State>> board_type;

inline std::vector<State> parseLine(const std::string& line) {
    std::istringstream lss(line);
    std::vector<State> row;
    std::string temp;
    while (std::getline(lss, temp, ',')) {
        row.push_back((State)stoi(temp));
    }
    return row;
}

inline board_type parseBoard(std::istream& board) {
    board_type b;
    std::string line;
    while (std::getline(board, line)) {
        b.push_back(parseLine(line));
    }
    return b;
}

inline board_type readBoard(const std::string& board_file) {
    std::ifstream ifs(board_file);
    return parseBoard(ifs);
}

inline void printBoard(const board_type& board) {
    for (auto& row: board) {
        std::stringstream line;
        for (auto& i: row) {
            line << cellString(i);
        }
        LOG(line.str().c_str());
    }
}

int main(int argc, const char** argv) {
    board_type board = readBoard("../../board_data.txt");
    printBoard(board);
}
