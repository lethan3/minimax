#include <iostream>
#include <vector>
#include <map>

class Value;
static const Value VALUE_MIN, VALUE_MAX;
bool Value::operator<(const Value& v) const;
bool Value::operator==(const Value& v) const;
bool Value::operator<=(const Value& v) const { return *this < v || *this == v; }

class HValue { // heuristic value
  public:
	HValue();
	bool operator<(const HValue& v) const;
	bool operator==(const HValue& v) const;
	bool operator<=(const HValue& v) const { return *this < v || *this == v; }
};

class State {
  public:
	State();
	bool turn; // player to move
	bool is_terminal() const;
	Value get_terminal_value() const;
	HValue get_heuristic_value() const;
	std::vector<State> adj(); // return all states reachable in one turn
	bool operator<(const State& s) const; // comparator for maps
	bool operator==(const State& s) const;
};

class Minimax {
  public:
	std::map<State, Value> state_values;
	std::map<State, State> best_move;
	std::map<State, bool> processed;

	Value minimax(State state, Value alpha, Value beta, bool maximizing_player) {
		if (processed[state]) {
			return state_values[state];
		}
		if (state.is_terminal()) {
			return state.get_terminal_value();
		}
		if (maximizing_player) {
			Value value = Value::VALUE_MIN;
			for (State ne : state.adj()) {
				value = std::max(value, minimax(ne, alpha, beta, false));
				if (beta <= value) {
					break;
				}
				alpha = std::max(alpha, value);
			}
			return value;
		} else {
			Value value = Value::VALUE_MAX;
			for (State ne : state.adj()) {
				value = std::max(value, minimax(ne, alpha, beta, false));
				if (value <= alpha) {
					break;
				}
				beta = std::min(beta, value);
			}
			return value;
		}
	}
};