#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <chrono>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <stdexcept>
#include <math.h>
#include "include/utils.h"
#include "include/DoubleDispatch.h"

typedef unsigned short element_type;

enum class State: element_type {
    kEmpty = 0,
    kObstacle = 1,
    kStart = 2,
    kFinish = 3,
    kExpanded = 4
};

struct Pos {
    int x, y;
    Pos(int x, int y) : x(x), y(y) {}
    bool operator==(const Pos& other) const { return x == other.x && y == other.y; }
};

inline std::wstring cell_string(const State& state) {
    switch(state) {
    case State::kEmpty:
        return L"0   ";
    case State::kObstacle:
        return L"\xe2\x9b\xb0\xef\xb8\x8f   "; // ⛰️
    case State::kStart:
        return L"\xf0\x9f\x9a\xa6  "; // 🚦
    case State::kFinish:
        return L"\xf0\x9f\x8f\x81  "; // 🏁
    case State::kExpanded:
        return L"\xf0\x9f\x9a\x97  "; // 🚗
    }
    return L"";
}


typedef std::vector<std::vector<State>> board_type;

inline std::vector<State> parse_line(const std::string& line) {
    std::istringstream lss(line);
    std::vector<State> row;
    std::string temp;
    while (std::getline(lss, temp, ',')) {
        row.push_back((State)stoi(temp));
    }
    return row;
}

inline board_type parse_board(std::istream& board) {
    board_type b;
    std::string line;
    while (std::getline(board, line)) {
        b.push_back(parse_line(line));
    }
    return b;
}

inline board_type read_board(const std::string& board_file) {
    std::ifstream ifs(board_file);
    if (!ifs.good()) {
        ifs = std::ifstream(std::string("../") + board_file);
        if (!ifs.good()) {
            throw std::invalid_argument("File not found!");
        }
    }
    return parse_board(ifs);
}

inline struct Pos get_board_node(const board_type& board, const State& state) {
    for (int y = 0; y < board.size(); y++) {
        for (int x = 0; x < board[y].size(); x++) {
            if (board[y][x] == state) {
                return { x,y };
            }
        }
    }
    return {0, 0};
}

inline struct Pos get_board_start(const board_type& board) {
    return get_board_node(board, State::kStart);
}

inline struct Pos get_board_end(const board_type& board) {
    return get_board_node(board, State::kFinish);
}

inline void print_board(const board_type& board) {
    for (auto& row: board) {
        std::wstringstream line;
        for (auto& i: row) {
            line << cell_string(i);
        }
        LOG(line.str());
    }
}

inline bool is_node_valid(const board_type& board, int x, int y) {
    if (board.size() <= y) return false;
    const auto& row = board.at(y);
    if (row.size() <= x) return false;
    auto state = row.at(x);
    return state != State::kObstacle && state != State::kExpanded;
}


struct Node {
    Pos pos;
    int g;
    int f;

    bool operator==(const Node& other) const { return pos == other.pos && g == other.g && f == other.f; }
};

inline int heuristic_function(const Pos& pos, const Pos& goal) {
    return std::abs(goal.x - pos.x) + std::abs(goal.y - pos.y);
}

inline std::vector<Node> expand_node(board_type& board, const Node& node, const Pos& end) {
    std::vector<Node> nodes;
    int new_g_value = node.g + 1;
    auto& pos = node.pos;
    board[pos.y][pos.x] = State::kExpanded;
    Node nodes_to_text[4] = {
      {{  pos.x, pos.y-1}, new_g_value},
      {{  pos.x, pos.y+1}, new_g_value},
      {{pos.x-1,   pos.y}, new_g_value},
      {{pos.x+1,   pos.y}, new_g_value}
    };

    for (auto& n: nodes_to_text) {
        n.f = n.g + heuristic_function(n.pos, end);
        if (is_node_valid(board, n.pos.x, n.pos.y)) {
            nodes.push_back(std::move(n));
        }
    }
    return nodes;
}

inline void print_road(const std::vector<Pos> road) {
    std::stringstream s;
    for (auto& node: road) {
        s << "(" << node.x << ", " << node.y << "), ";
    }
    LOG(s.str().c_str());
}

inline void print_road(const std::vector<Node> road) {
    std::stringstream s;
    for (auto& node: road) {
        s << "(" << node.pos.x << ", " << node.pos.y << ", " << node.g << ", " << node.f << "), ";
    }
    LOG(s.str().c_str());
}

inline void search_board(board_type& board,const Pos& start, const Pos& end) {
    std::vector<Node> open{ {start, 0} };

    while (open.size() > 0) {
        std::sort(open.begin(), open.end(), [](auto first, auto second) { return first.f < second.f; });
        auto node = open.at(0);
        open.erase(open.begin());
        auto new_expansion = expand_node(board, node, end);

        if (new_expansion.size() != 0) {
            auto last = std::find_if(new_expansion.begin(),
                                     new_expansion.end(),
                                     [&end](auto node) {
                                     return node.pos == end;
                    });
            if (last == new_expansion.end()) {
                open.insert(open.end(), new_expansion.begin(), new_expansion.end());
            } else {
                break;
            }
        }
    }

    print_board(board);
}

int main(int argc, const char** argv) {
    {
        Interactor p;
        const auto objects = std::vector<Object*>{ new FirstClassObject, new SecondClassObject };
        for (auto animal : objects) {
            p.interactWith(animal);
            delete animal;
        }
    }

#ifdef _MSVC_LANG
    std::filesystem::current_path("../");
    LOG(std::filesystem::current_path());
#endif
    board_type board = read_board("board_data.txt");
    print_board(board);
    const auto& start = get_board_start(board);
    const auto& end = get_board_end(board);
    search_board(board, start, end);
}
