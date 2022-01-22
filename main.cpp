#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <fstream>
#include <math.h>
#include "include/utils.h"

typedef unsigned short element_type;

enum State: element_type {
    kEmpty = 0,
    kObstacle = 1
    kStart = 2
    kFinish = 3
};

inline std::string cell_string(const State& state) {
    switch(state) {
    case ::kEmpty:
        return "0   ";
    case ::kObstacle:
        return "⛰️  ";
    case ::kStart:
        return "🚦  ";
    case ::kFinish:
        return "🏁  ";
    }
    return "";
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
    return parse_board(ifs);
}

inline void print_board(const board_type& board) {
    for (auto& row: board) {
        std::stringstream line;
        for (auto& i: row) {
            line << cell_string(i);
        }
        LOG(line.str().c_str());
    }
}

inline bool is_node_empty(const board_type& board, int x, int y) {
    if (board.size() <= y) return false;
    const auto& row = board.at(y);
    if (row.size() <= x) return false;
    return row.at(x) == ::kEmpty;
}

struct Pos {
    int x, y;
    Pos(int x, int y): x(x), y(y) {}
    bool operator==(const Pos& other) const { return x == other.x && y == other.y; }
};

struct Node {
    Pos pos;
    int g;
    int f;

    bool operator==(const Node& other) const { return pos == other.pos && g == other.g && f == other.f; }
};

inline constexpr int heuristic_function(const Pos& pos, const Pos& goal) {
    return std::abs(goal.x - pos.x) + std::abs(goal.y - pos.y);
}

inline std::vector<Node> expand_node(const board_type& board, const Node& node, const Pos& end) {
    std::vector<Node> nodes;
    int new_g_value = node.g + 1;
    auto& pos = node.pos;
    Node nodes_to_text[4] = {
      {{  pos.x, pos.y-1}, new_g_value},
      {{  pos.x, pos.y+1}, new_g_value},
      {{pos.x-1,   pos.y}, new_g_value},
      {{pos.x+1,   pos.y}, new_g_value}
    };
    for (auto& n: nodes_to_text) {
        n.f = n.g + heuristic_function(n.pos, end);
        if (is_node_empty(board, n.pos.x, n.pos.y)) {
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

inline void print_board(const board_type& board, const std::vector<Node> road) {
    for (int i =0; i < board.size(); i++) {
        auto row = board[i];
        std::stringstream line;
        for (int j =0; j < row.size(); j++) {
            auto element = row[j];
            auto pos = Pos(j,i);
            auto last = std::find_if(road.begin(),
                                     road.end(),
                                     [&](auto node) {
                return node.pos == pos;
            });
            if (last != road.end()) {
                line << "🚗  ";
            } else {
                line << cell_string(element);
            }
        }
        LOG(line.str().c_str());
    }
}

inline void search_board(const board_type& board,const Pos& start, const Pos& end) {
    std::vector<Pos> expansion{start};
    std::vector<Node> close;
    std::vector<Node> open{ {start, 0} };

    while (open.size() > 0) {
        std::sort(open.begin(), open.end(), [](auto first, auto second) { return first.f < second.f; });
        auto node = open.at(0);
        open.erase(open.begin());
        close.push_back(std::move(node));
        auto new_expansion = expand_node(board, node, end);
        std::erase_if(new_expansion, [&](auto& new_node){
            return std::find(expansion.begin(), expansion.end(), new_node.pos) != expansion.end();
        });
        if (new_expansion.size() != 0) {
            for (auto& node: new_expansion) {
                expansion.push_back(node.pos);
            }
            auto last = std::find_if(new_expansion.begin(),
                                     new_expansion.end(),
                                     [&end](auto node) {
                                     return node.pos == end;
                    });
            if (last != new_expansion.end()) {
                close.push_back(*last);
                break;
            } else {
                open.insert(open.end(), new_expansion.begin(), new_expansion.end());
            }
        }
    }

    print_board(board, close);
    print_road(close);
    print_road(expansion);
}

int main(int argc, const char** argv) {
    board_type board = read_board("../../board_data.txt");
    print_board(board);
    search_board(board, {0,0}, {5, 4});
}
