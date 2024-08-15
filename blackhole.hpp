#include <iostream>
#include <vector>
#include <map>
#include <climits>
#include <cassert>
#include "minimax.hpp"
#include "Value_int.hpp"
#include "HValue_int.hpp"

int n;
std::vector<std::vector<int>> board_adj;

class Move : public BaseMove<Move> {
  public:
	int p;

	Move() { Move(0); }

	Move(int p) {
		this->p = p;
	}

	bool operator==(const Move& m) const {
		return this->p == m.p;
	}

	static Move parse_move(std::string s);
	std::string to_string() const {
        return std::to_string(p + 1);
	}
};

Move Move::parse_move(std::string s) { // throws error if syntax of move is wrong
    int a;
	try {
        a = stoi(s);
    } catch (std::exception e) {
        throw std::runtime_error("Move should be integer between 1 and " + std::to_string(n));
    }

    return a - 1;
}

template<class Value, class HValue, class Move>
class State : public BaseState<State, Value, HValue, Move> {
  public:
	bool player_to_move;
    int num_to_place;
	std::vector<int> nums;
    std::vector<bool> color;
	
	State() { // starting state
		this->player_to_move = 1;
        this->num_to_place = 1;
		this->nums = std::vector<int>(2 * n + 1);
        this->color = std::vector<bool>(2 * n + 1);
	}

	State(std::vector<std::vector<int>> grid, bool player_to_move, int num_to_place) {
		this->nums = nums;
		this->player_to_move = player_to_move;
        this->num_to_place = num_to_place;
	}

	std::pair<bool, Value> get_terminal() const {
        if (num_to_place != n + 1) {
            return {false, Value(0)};
        }
        
        return {true, Value(get_heuristic_value().a)};
	}

	HValue get_heuristic_value() const {
		int score = 0;
        for (int i = 0; i < 2 * n + 1; ++i) {
            if (nums[i] == 0) {
                for (int ne : board_adj[i]) {
                    if (nums[ne]) {
                        if (color[ne]) {
                            score -= nums[ne];
                        } else {
                            score += nums[ne];
                        }
                    }
                }
            }
        }
        
        return HValue(score);
    }

	std::string to_string() const {
        std::string r = "";
        for (int i = 0; i < 2 * n + 1; ++i) {
            if (nums[i]) {
                r += std::to_string(nums[i]) + (nums[i] <= 9 ? " " : "") + " ";
            } else {
                r += "__ ";
            }
        }
        r += "\n";
        for (int i = 0; i < 2 * n + 1; ++i) {
            r += (nums[i] ? (color[i] ? "A " : "B ") : "  ");
            r += " ";
        }
        r += "\n";

        return r;
	}

	std::vector<std::pair<Move, State>> adj() const {
		std::vector<std::pair<Move, State>> r;

		for (int i = 0; i < 2 * n + 1; ++i) {
            if (nums[i]) continue;
            State new_state = *this;
            new_state.nums[i] = num_to_place;
            new_state.color[i] = player_to_move;
            if (!player_to_move) {
                new_state.num_to_place++;
            }
            new_state.player_to_move = !player_to_move;
            r.push_back({Move(i), new_state});
		}

		return r;
	}

	std::strong_ordering operator<=>(const State& s) const {
		return std::pair<std::vector<int>, std::vector<bool>>{this->nums, this->color} <=> std::pair<std::vector<int>, std::vector<bool>>{s.nums, s.color};
	}

	bool operator==(const State& s) const {
		return std::pair<std::vector<int>, std::vector<bool>>{this->nums, this->color} == std::pair<std::vector<int>, std::vector<bool>>{s.nums, s.color};
	}
};
