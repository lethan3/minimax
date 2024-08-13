#include "tictactoe.hpp"

typedef State<Value, HValue, Move> StateT;
int main() {
    n = m = w = 3;
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