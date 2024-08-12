#include <iostream>
#include <concepts>
#include <vector>
#include <map>

class Base_Value {
  public:
	Value();
	bool operator<=>(const Base_Value& v) const;
};

class Base_HValue { // heuristic value
  public:
	Base_HValue();
	bool operator<(const Base_HValue& v) const;
	bool operator==(const Base_HValue& v) const;
	bool operator<=(const Base_HValue& v) const { return *this < v || *this == v; }
};

class Base_Move {
  public:
	Base_Move();
};

template<class Value, class HValue, class Move>
class Base_State {
  public:
	State();
	bool turn; // player to move
	bool is_terminal() const;
	Value get_terminal_value() const;
	HValue get_heuristic_value() const;
	std::vector<State> adj(); // return all states reachable in one turn
	bool operator<=>(const State& s) const; // comparator for maps
};

template<typename State>
concept StateConcept = requires(State s) {
    { s.is_terminal() } -> std::convertible_to<bool>;
    { s.get_terminal_value() } -> std::convertible_to<Value>;
    { s.get_heuristic_value() } -> std::convertible_to<HValue>;
    { s.adj() } -> std::convertible_to<std::vector<State>>;
    { s.operator<(std::declval<State>()) } -> std::convertible_to<bool>;
    { s.operator==(std::declval<State>()) } -> std::convertible_to<bool>;
};

template <class Value, class HValue, class State, class Move>
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