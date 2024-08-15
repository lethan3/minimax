#include "blackhole.hpp"

typedef State<Value, HValue, Move> StateT;
int main() {
    n = 7;
    board_adj.resize(2 * n + 1);

    int k = 5;
    std::vector<std::vector<int>> tri(k);
    int curr = 0;
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j <= i; ++j) {
            tri[i].push_back(curr);
            curr++;
        }
    }

    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < (int)tri[i].size() - 1; ++j) {
            board_adj[tri[i][j]].push_back(tri[i][j + 1]);
            board_adj[tri[i][j + 1]].push_back(tri[i][j]);
        }
    }

    for (int i = 0; i < k - 1; ++i) {
        for (int j = 0; j < (int)tri[i].size(); ++j) {
            board_adj[tri[i][j]].push_back(tri[i + 1][j]);
            board_adj[tri[i + 1][j]].push_back(tri[i][j]);
            board_adj[tri[i][j]].push_back(tri[i + 1][j + 1]);
            board_adj[tri[i + 1][j + 1]].push_back(tri[i][j]);
        }
    }

    // for (int i = 0; i < 2 * n; ++i) {
    //     board_adj[i].push_back(i + 1);
    //     board_adj[i + 1].push_back(i);
    // }

    Minimax<State, Value, HValue, Move> minimax;
    StateT initstate;

    minimax.run(initstate);
    // minimax.minimax(initstate, Value::VALUE_MIN, Value::VALUE_MAX);

    // StateT cstate = initstate;
    // while (!cstate.get_terminal().first) {
    //     for (std::string s : cstate.display()) {
    //         std::cout << s << "\n";
    //     }
    //     std::cout << "Value: " << minimax.state_values[cstate].to_string() << "\n";
    //     std::cout << "\n";
    //     std::cout << (minimax.best_move[cstate].first.to_string()) << "\n";

    //     cstate = minimax.best_move[cstate].second;
    // }

    // for (std::string s : cstate.display()) {
    //     std::cout << s << "\n";
    // }
    // std::cout << "Value: " << minimax.state_values[cstate].to_string() << "\n";
}