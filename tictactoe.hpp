#include <iostream>
#include <vector>
#include <map>
#include <climits>
#include <cassert>
#include "minimax.hpp"
#include "Value_int.hpp"
#include "HValue_int.hpp"

int n = 4, m = 4, w = 3;

class Move : public BaseMove<Move> {
  public:
	int row, col;

	Move() { Move(0, 0); }

	Move(int row, int col) {
		this->row = row;
		this->col = col;
	}

	static Move parse_move(std::string s);
	std::string to_string() const {
		return (char)('A' + col) + std::to_string(row + 1);
	}
};

Move Move::parse_move(std::string s) { // throws error if syntax of move is wrong
	for (int i = 0; i < (int)s.size(); ++i) {
		if (i == 0) assert('A' <= s[i] && s[i] <= 'Z' || 'a' <= s[i] && s[i] <= 'z');
		else assert('0' <= s[i] && s[i] <= '9');
	}
	return Move(stoi(s.substr(1)) - 1, 'A' <= s[0] && s[0] <= 'Z' ? s[0] - 'A' : s[0] - 'a');
}

template<class Value, class HValue, class Move>
class State : public BaseState<State, Value, HValue, Move> {
  public:
	bool player_to_move;
	std::vector<std::vector<int>> grid;
	
	State() { // starting state
		this->grid = std::vector<std::vector<int>>(n, std::vector<int>(m));
		this->player_to_move = 1;
	}

	State(std::vector<std::vector<int>> grid, bool player_to_move) {
		this->grid = grid;
		this->player_to_move = player_to_move;
	}

	bool is_full() const {
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				if (!grid[i][j]) return false;
			}
		}
		
		return true;
	}

	bool in_range(int r, int c) const {
		return std::min(r, c) >= 0 && r < n && c < m;
	}

	std::pair<bool, Value> get_terminal() const {
		int dir[4][2] = {{0, 1}, {1, 1}, {1, 0}, {1, -1}};

		for (int d = 0; d < 4; ++d) {
			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < m; ++j) {
					bool good = true;
					for (int k = 0; k < w; ++k) {
						int nr = i + k * dir[d][0], nc = j + k * dir[d][1];
						if (!in_range(nr, nc)) {
							good = false;
							break;
						}
						if (!grid[nr][nc] || grid[nr][nc] != grid[i][j]) {
							good = false;
							break;
						}
					}

					if (good) {
						return {true, Value(grid[i][j] == 1 ? 1 : -1)};
					}
				}
			}
		}

		if (is_full()) {
			return {true, Value(0)};
		}

		return {false, Value(0)};
	}

	HValue get_heuristic_value() const {
		return HValue(0);
	}

	std::vector<std::string> display() const {
		std::vector<std::vector<char>> v(4 * n - 1, std::vector<char>(4 * m - 1));
		for (int i = 0; i < (int)v.size(); ++i) {
			for (int j = 0; j < (int)v[0].size(); ++j) {
				if (i % 4 == 3 && j % 4 == 3) {
					v[i][j] = '+';
				} else if (i % 4 == 3) {
					v[i][j] = '-';
				} else if (j % 4 == 3) {
					v[i][j] = '|';
				} else {
					v[i][j] = ' ';
				}
			}
		}

		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				v[4 * i + 1][4 * j + 1] = (grid[i][j] ? (grid[i][j] == 1 ? 'X' : 'O') : ' ');
			}
		}

		std::vector<std::string> r(v.size());

		for (int i = 0; i < (int)v.size(); ++i) {
			r[i] = std::string(v[i].begin(), v[i].end());
		}
		
		return r;
	}

	std::vector<std::pair<Move, State>> adj() const {
		std::vector<std::pair<Move, State>> r;
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				if (grid[i][j]) continue;
				State new_state = *this;
				new_state.grid[i][j] = player_to_move ? 1 : 2;
				new_state.player_to_move = !player_to_move;
				r.push_back({Move(i, j), new_state});
			}
		}

		return r;
	}

	std::strong_ordering operator<=>(const State& s) const {
		return std::pair<std::vector<std::vector<int>>, bool>{this->grid, this->player_to_move} 
			<=> std::pair<std::vector<std::vector<int>>, bool>{s.grid, s.player_to_move};
	}
};